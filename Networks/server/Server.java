import java.net.*;
import java.io.*;
import java.util.concurrent.*;

public class Server
{
	//Note: Declaring attributes go above the constructor class level
	private ExecutorService executor;
	private volatile boolean running;
	private ServerSocket serverSock;
	
	
	public Server(int number){
	//Note: Seting the values (init) of attributes can go above or here in the constructor
		this.executor = Executors.newFixedThreadPool(number);
		this.running = false;
	}

	private class ClientHandler implements Runnable {
        private final Socket clientSocket;
		private String[] args;

        public ClientHandler(Socket clientSocket,String[] args) {
            this.clientSocket = clientSocket;
			this.args = args;
        }

        @Override
        public void run() {
            try {
                System.out.println("Handling client on " + Thread.currentThread().getName());
                PrintWriter writer = new PrintWriter(clientSocket.getOutputStream());
				writer.println(args);
                Thread.sleep(1000);
                clientSocket.close();
                System.out.println("Client connection closed on " + Thread.currentThread().getName());
            } catch (IOException e) {
                System.out.println(e.getMessage());
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                System.out.println(e.getMessage());
            }
        }
    }

	public void run(String[] args){
		try{
			serverSock = new ServerSocket(4242);
			running = true;
			System.out.println("Server started on port 4242");
			while (running) {
				// Server logic here
				Socket sock = serverSock.accept();
				System.out.println("New connection accepted");
				ClientHandler handler = new ClientHandler(sock,args);
				executor.execute(handler);
			}
		}
		catch(IOException e){
			System.out.println(e);
		} finally{
			stop();
		}
	}

	public void stop(){
		running = false;
		System.out.println("Stopping server...");
		try {
            if (serverSock != null && !serverSock.isClosed()) {
                serverSock.close();
            }
        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
		executor.shutdown();

		try {
            if (!executor.awaitTermination(5, TimeUnit.SECONDS)) {
                System.out.println("Forcing thread pool shutdown...");
                executor.shutdownNow();
            }
            System.out.println("Server stopped successfully");
        } catch (InterruptedException e) {
            executor.shutdownNow();
            Thread.currentThread().interrupt();
            System.out.println("Shutdown interrupted");
        }
	}

	public static void main( String[] args )
	{
		if (args.length < 2){
			System.out.println("Please provide at least 2 arguments");
			return;
		}

		Server server = new Server(30);
		server.run(args);
	}
}

