#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <print>
#include <random>
#include <typeinfo>
#include <stdexcept>

#include <cstdlib>

#include "../draw2d/surface.hpp"
#include "../draw2d/surface-ex.hpp"
#include "../draw2d/draw-ex.hpp"
#include "../draw2d/image.hpp"

#include "../support/error.hpp"
#include "../support/context.hpp"
#include "../support/runconfig.hpp"

#include "../vmlib/vec2.hpp"

//blit sandbox following the other sandboxes
namespace
{
	constexpr char const* kWindowTitle = "COMP3811-Sandbox: Blit";

	void glfw_callback_error_( int, char const* );
	void glfw_callback_key_( GLFWwindow*, int, int, int, int );

	struct GLFWCleanupHelper
	{
		~GLFWCleanupHelper();
	};
	struct GLFWWindowDeleter
	{
		~GLFWWindowDeleter();
		GLFWwindow* window;
	};
}

int main( int aArgc, char* aArgv[] ) try
{
	// Parse command line arguments
	RuntimeConfig const config = parse_command_line( aArgc, aArgv );

	// Initialize GLFW
	if( GLFW_TRUE != glfwInit() )
	{
		char const* msg = nullptr;
		int ecode = glfwGetError( &msg );
		throw Error( "glfwInit() failed with '{}' ({})", msg, ecode );
	}

	// Ensure that we call glfwTerminate() at the end of the program.
	GLFWCleanupHelper cleanupHelper;

	// Configure GLFW and create window
	glfwSetErrorCallback( &glfw_callback_error_ );

	glfwWindowHint( GLFW_SRGB_CAPABLE, GLFW_TRUE );
	glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );

#	if !defined(__APPLE__)
	// Most platforms will support OpenGL 4.3
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
#	else // defined(__APPLE__)
	// Apple has at most OpenGL 4.1, so don't ask for something newer.
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
#	endif // ~ __APPLE__
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

#	if !defined(NDEBUG)
	// When building in debug mode, request an OpenGL debug context. This
	// enables additional debugging features. However, this can carry extra
	// overheads. We therefore do not do this for release builds.
	glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE );
#	endif // ~ !NDEBUG

	GLFWwindow* window = glfwCreateWindow(
		int(config.initialWindowWidth),
		int(config.initialWindowHeight),
		kWindowTitle,
		nullptr, nullptr
	);

	if( !window )
	{
		char const* msg = nullptr;
		int ecode = glfwGetError( &msg );
		throw Error( "glfwCreateWindow() failed with '{}' ({})", msg, ecode );
	}

	GLFWWindowDeleter windowDeleter{ window };

	// Set up event handling
	std::size_t testId = 1;
	glfwSetWindowUserPointer( window, &testId );

	glfwSetKeyCallback( window, &glfw_callback_key_ );

	// Set up drawing stuff
	glfwMakeContextCurrent( window );
	glfwSwapInterval( 1 ); // V-Sync is on.

	// Get actual framebuffer size.
	int iwidth, iheight;
	glfwGetFramebufferSize( window, &iwidth, &iheight );

	float wscale = 1.f, hscale = 1.f;
#	if defined(__APPLE__)
	glfwGetWindowContentScale( window, &wscale, &hscale );
#	endif

	assert( iwidth >= 0 && iheight >= 0 );
	auto fbwidth = std::uint32_t(iwidth / wscale) >> config.framebufferScaleShift;
	auto fbheight = std::uint32_t(iheight / hscale) >> config.framebufferScaleShift;

	Context context( fbwidth, fbheight );
	SurfaceEx surface( fbwidth, fbheight );

	glViewport( 0, 0, iwidth, iheight );

	// Load test image
	auto testImage = load_image( "assets/earth.png" );
	if( !testImage )
	{
		std::print( stderr, "Warning: Could not load assets/earth.png\n" );
		std::print( stderr, "Some tests may not work correctly.\n" );
	}

	// Main loop
	while( !glfwWindowShouldClose( window ) )
	{
		// Let GLFW process events
		glfwWaitEvents();
		
		// Check if window was resized.
		{
			int nwidth, nheight;
			glfwGetFramebufferSize( window, &nwidth, &nheight );

			if( 0 == nwidth || 0 == nheight )
			{
				// Window minimized? Pause until it is unminimized.
				do
				{
					glfwWaitEvents();
					glfwGetFramebufferSize( window, &nwidth, &nheight );
				} while( 0 == nwidth || 0 == nheight );
			}

			if( iwidth != nwidth || iheight != nheight )
			{
				iwidth = nwidth;
				iheight = nheight;

				float ws = 1.f, hs = 1.f;
#				if defined(__APPLE__)
				glfwGetWindowContentScale( window, &ws, &hs );
#				endif

				glViewport( 0, 0, iwidth, iheight );

				assert( iwidth >= 0 && iheight >= 0 );
				fbwidth = std::uint32_t(iwidth / ws) >> config.framebufferScaleShift;
				fbheight = std::uint32_t(iheight / hs) >> config.framebufferScaleShift;

				// Resize things
				context.resize( fbwidth, fbheight );
				surface = SurfaceEx( fbwidth, fbheight );
			}
		}

		// Draw scene
		surface.clear();

		if( testImage )
		{
			switch( testId )
			{
				case 1: {
					// Test 1: blit_ex_solid - center of screen
					blit_ex_solid( surface, *testImage, 
						{ float(fbwidth/2 - testImage->get_width()/2), 
						  float(fbheight/2 - testImage->get_height()/2) } );
					std::print( "Test 1: blit_ex_solid at center\n" );
				} break;
				
				case 2: {
					// Test 2: blit_ex_solid - top-left corner
					blit_ex_solid( surface, *testImage, { 0.f, 0.f } );
					std::print( "Test 2: blit_ex_solid at top-left\n" );
				} break;
				
				case 3: {
					// Test 3: blit_ex_solid - partially off-screen (top-left)
					blit_ex_solid( surface, *testImage, { -100.f, -100.f } );
					std::print( "Test 3: blit_ex_solid partially off-screen (top-left)\n" );
				} break;
				
				case 4: {
					// Test 4: blit_ex_solid - partially off-screen (bottom-right)
					blit_ex_solid( surface, *testImage, 
						{ float(fbwidth - testImage->get_width()/2), 
						  float(fbheight - testImage->get_height()/2) } );
					std::print( "Test 4: blit_ex_solid partially off-screen (bottom-right)\n" );
				} break;
				
				case 5: {
					// Test 5: blit_ex_memcpy - center of screen
					blit_ex_memcpy( surface, *testImage, 
						{ float(fbwidth/2 - testImage->get_width()/2), 
						  float(fbheight/2 - testImage->get_height()/2) } );
					std::print( "Test 5: blit_ex_memcpy at center\n" );
				} break;
				
				case 6: {
					// Test 6: blit_ex_memcpy - top-left corner
					blit_ex_memcpy( surface, *testImage, { 0.f, 0.f } );
					std::print( "Test 6: blit_ex_memcpy at top-left\n" );
				} break;
				
				case 7: {
					// Test 7: blit_ex_memcpy - partially off-screen (top-left)
					blit_ex_memcpy( surface, *testImage, { -100.f, -100.f } );
					std::print( "Test 7: blit_ex_memcpy partially off-screen (top-left)\n" );
				} break;
				
				case 8: {
					// Test 8: blit_ex_memcpy - partially off-screen (bottom-right)
					blit_ex_memcpy( surface, *testImage, 
						{ float(fbwidth - testImage->get_width()/2), 
						  float(fbheight - testImage->get_height()/2) } );
					std::print( "Test 8: blit_ex_memcpy partially off-screen (bottom-right)\n" );
				} break;
				
				case 9: {
					// Test 9: Compare both functions side by side
					blit_ex_solid( surface, *testImage, { 50.f, 50.f } );
					blit_ex_memcpy( surface, *testImage, 
						{ float(fbwidth/2 + 50.f), 50.f } );
					std::print( "Test 9: Both functions side by side (left: solid, right: memcpy)\n" );
				} break;
				
				case 0: {
					// Test 0: Multiple blits to test for correctness
					blit_ex_solid( surface, *testImage, { 10.f, 10.f } );
					blit_ex_solid( surface, *testImage, { 200.f, 10.f } );
					blit_ex_solid( surface, *testImage, { 10.f, 200.f } );
					blit_ex_solid( surface, *testImage, { 200.f, 200.f } );
					std::print( "Test 0: Multiple blit_ex_solid calls\n" );
				} break;
			}
		}
		else
		{
			std::print( "Error: No test image loaded. Please ensure assets/earth.png exists.\n" );
		}
		
		context.draw( surface );

		// Display results
		glfwSwapBuffers( window );
	}

	return 0;
}
catch( std::exception const& eErr )
{
	std::print( stderr, "Top-level Exception ({}):\n", typeid(eErr).name() );
	std::print( stderr, "{}\n", eErr.what() );
	std::print( stderr, "Bye.\n" );
	return 1;
}


namespace
{
	void glfw_callback_error_( int aErrNum, char const* aErrDesc )
	{
		std::print( stderr, "GLFW error: {} ({})\n", aErrDesc, aErrNum );
	}

	void glfw_callback_key_( GLFWwindow* aWindow, int aKey, int, int aAction, int )
	{
		if( GLFW_KEY_ESCAPE == aKey && GLFW_PRESS == aAction )
		{
			glfwSetWindowShouldClose( aWindow, GLFW_TRUE );
			return;
		}

		auto* testid = static_cast<std::size_t*>(glfwGetWindowUserPointer( aWindow ));
		assert( testid );

		if( GLFW_PRESS == aAction )
		{
			std::size_t select = 0;
			switch( aKey )
			{
				case GLFW_KEY_1: select = 1; break;
				case GLFW_KEY_2: select = 2; break;
				case GLFW_KEY_3: select = 3; break;
				case GLFW_KEY_4: select = 4; break;
				case GLFW_KEY_5: select = 5; break;
				case GLFW_KEY_6: select = 6; break;
				case GLFW_KEY_7: select = 7; break;
				case GLFW_KEY_8: select = 8; break;
				case GLFW_KEY_9: select = 9; break;
				case GLFW_KEY_0: select = 0; break;
				default: select = *testid; break;
			}

			if( select != *testid )
			{
				*testid = select;
				std::print( "Selected test {}\n", select );
			}
		}
	}
}

namespace
{
	GLFWCleanupHelper::~GLFWCleanupHelper()
	{
		glfwTerminate();
	}

	GLFWWindowDeleter::~GLFWWindowDeleter()
	{
		if( window )
			glfwDestroyWindow( window );
	}
}

