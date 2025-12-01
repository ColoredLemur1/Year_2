#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <print>
#include <numbers>
#include <typeinfo>
#include <stdexcept>

#include <cstdlib>

#include "defaults.hpp"

#include "../support/error.hpp"
#include "../support/program.hpp"
#include "../support/checkpoint.hpp"
#include "../support/debug_output.hpp"

#include "../vmlib/vec2.hpp"
#include "../vmlib/mat22.hpp"

namespace
{
	constexpr char const* kWindowTitle = "COMP3811 - GL Triangle";

	struct State_
	{
		ShaderProgram* prog;
	};

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

int main() try
{

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

	//glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

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
		1280,
		720,
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
	State_ state{};

	glfwSetWindowUserPointer( window, &state );

	glfwSetKeyCallback( window, &glfw_callback_key_ );

	// Set up drawing stuff
	glfwMakeContextCurrent( window );
	glfwSwapInterval( 1 ); // V-Sync is on.

	// Initialize GLAD
	// This will load the OpenGL API. We mustn't make any OpenGL calls before this!
	if( !gladLoadGLLoader( (GLADloadproc)&glfwGetProcAddress ) )
		throw Error( "gladLoadGLLoader() failed - cannot load GL API!" );

	std::print( "RENDERER {}\n", (char const*)glGetString( GL_RENDERER ) );
	std::print( "VENDOR {}\n", (char const*)glGetString( GL_VENDOR ) );
	std::print( "VERSION {}\n", (char const*)glGetString( GL_VERSION ) );
	std::print( "SHADING_LANGUAGE_VERSION {}\n", (char const*)glGetString( GL_SHADING_LANGUAGE_VERSION ) );

	// Ddebug output
#	if !defined(NDEBUG)
	setup_gl_debug_output();
#	endif // ~ !NDEBUG

	// Global GL state
	OGL_CHECKPOINT_ALWAYS();

	// TODO: global GL setup goes here
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_CULL_FACE);

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);


	OGL_CHECKPOINT_ALWAYS();

	// Get actual framebuffer size.
	// This can be different from the window size, as standard window
	// decorations (title bar, borders, ...) may be included in the window size
	// but not be part of the drawable surface area.
	int iwidth, iheight;
	glfwGetFramebufferSize( window, &iwidth, &iheight );

	glViewport( 0, 0, iwidth, iheight );

	// Load shader program
	ShaderProgram prog( {
		{ GL_VERTEX_SHADER, "assets/ex2/passthrough.vert" },
		{ GL_FRAGMENT_SHADER, "assets/ex2/modulate.frag" }
	} );

	state.prog = &prog;

	// Create vertex buffers and VAO
	//TODO: create VBOs and VAO
	static float const kPositions[] = {
		0.f, 0.8f,
		-0.7f, -0.8f,
		0.7f, -0.8f,

		-0.9f, 0.3f,
		-0.9f, -0.3f,
		-0.6f, 0.0f,

		0.6f, 0.0f,
		0.9f, -0.3f,
		0.9f, 0.3f,
	};

	GLuint positionVBO = 0;
	glGenBuffers( 1, &positionVBO );
	glBindBuffer( GL_ARRAY_BUFFER, positionVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof(kPositions), kPositions, GL_STATIC_DRAW );

	static float const kColors[] = {
		1.f, 1.f, 0.f,
		1.f, 0.f, 1.f,
		0.f, 1.f, 1.f,

		1.f, 0.f, 0.f,  // red
		0.f, 1.f, 0.f,  // green
		0.f, 0.f, 1.f,  // blue
		

		1.f, 1.f, 1.f,  // white
		0.5f, 0.5f, 0.5f, // gray
		0.f, 0.f, 0.f,  // black
	};

	GLuint colorVBO = 0;
	glGenBuffers( 1, &colorVBO );
	glBindBuffer( GL_ARRAY_BUFFER, colorVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof(kColors), kColors, GL_STATIC_DRAW );

	GLuint vao = 0;
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	glBindBuffer( GL_ARRAY_BUFFER, positionVBO );
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray( 0 );

	glBindBuffer( GL_ARRAY_BUFFER, colorVBO );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray( 1 );

	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	glDeleteBuffers( 1, &positionVBO );
	glDeleteBuffers( 1, &colorVBO );

	// Animation state
	auto last = Clock::now();

	float angle = 0.f;

	// Main loop
	while( !glfwWindowShouldClose( window ) )
	{
		// Let GLFW process events
		glfwPollEvents();
		
		// Check if window was resized.
		{
			int nwidth, nheight;
			glfwGetFramebufferSize( window, &nwidth, &nheight );


			if( 0 == nwidth || 0 == nheight )
			{
				// Window minimized? Pause until it is unminimized.
				// This is a bit of a hack.
				do
				{
					glfwWaitEvents();
					glfwGetFramebufferSize( window, &nwidth, &nheight );
				} while( 0 == nwidth || 0 == nheight );
			}

			glViewport( 0, 0, nwidth, nheight );
		}

		// Update state
		auto const now = Clock::now();
		float dt = std::chrono::duration_cast<Secondsf>(now-last).count();
		last = now;

		angle += dt * std::numbers::pi_v<float> * 0.3f;
		if( angle >= 2.f*std::numbers::pi_v<float> )
			angle -= 2.f*std::numbers::pi_v<float>;

		// Draw scene
		OGL_CHECKPOINT_DEBUG();

		//TODO: draw frame
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(prog.programId());

		Mat22f rotation = make_rotation_2d(angle);
		glUniformMatrix2fv(1, 1, GL_FALSE, &rotation._00);

		static float const baseColor[] = { 0.2f, 1.f, 1.f };
		glUniform3fv(0, 1, baseColor);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 9);

		glBindVertexArray(0);
		glUseProgram(0);

		OGL_CHECKPOINT_DEBUG();

		// Display results
		glfwSwapBuffers( window );
	}

	// Cleanup.
	state.prog = nullptr;

	//TODO: additional cleanup
	
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

		if( auto* state = static_cast<State_*>(glfwGetWindowUserPointer( aWindow )) )
		{
			// R-key reloads shaders.
			if( GLFW_KEY_R == aKey && GLFW_PRESS == aAction )
			{
				if( state->prog )
				{
					try
					{
						state->prog->reload();
						std::print( stderr, "Shaders reloaded and recompiled.\n" );
					}
					catch( std::exception const& eErr )
					{
						std::print( stderr, "Error when reloading shader:\n" );
						std::print( stderr, "{}\n", eErr.what() );
						std::print( stderr, "Keeping old shader.\n" );
					}
				}
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

