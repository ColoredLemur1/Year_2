import java.net.*;
import java.io.*;
import java.util.concurrent.*;
import java.util.*;

public class Server
{
	//Note: Declaring attributes go above the constructor class level
	private ExecutorService executor;
	private volatile boolean running;
	private ServerSocket serverSock;
	private final Map<String, Integer> votes;
	
	
	public Server(int number){
	//Note: Seting the values (init) of attributes can go above or here in the constructor
		this.executor = Executors.newFixedThreadPool(number);
		this.running = false;
		this.votes = new ConcurrentHashMap<>();
	}

	private class ClientHandler implements Runnable {
        private final Socket clientSocket;
        private final String[] args;

        public ClientHandler(Socket clientSocket, String[] args) {
            this.clientSocket = clientSocket;
            this.args = args;
        }

        @Override
        public void run() {
            try {
                System.out.println("Handling client on " + Thread.currentThread().getName());
                PrintWriter writer = new PrintWriter(clientSocket.getOutputStream(), true);
                BufferedReader reader = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));

                String command = reader.readLine();
                if (command != null) {
                    String clientIP = clientSocket.getInetAddress().getHostAddress();
                    String logEntry = "";

                    if (command.equalsIgnoreCase("list")) {
                        writer.println("Available options to vote:");
                        //listing voting options
                        synchronized (votes) {
                            for (Map.Entry<String, Integer> entry : votes.entrySet()) {
                                writer.println("'" + entry.getKey() + "' has " + entry.getValue() + " vote(s)");
                            }
                        }
                        writer.flush();
                        logEntry = generateLogEntry(clientIP, command);
					//voting
                    } else if (command.startsWith("vote ")) {
                        String vote = command.substring(5).trim();
                        if (votes.containsKey(vote)) {
                            synchronized (votes) {
                                votes.put(vote, votes.get(vote) + 1);
                            }
                            writer.println("Incremented the number of votes for '" + vote + "'");
                            logEntry = generateLogEntry(clientIP, command);
                        } else {
                            writer.println("Invalid vote. Option '" + vote + "' does not exist.");
                        }
                    } else {
                        writer.println("Invalid command. Use 'list' or 'vote <option>'.");
                    }

                    if (!logEntry.isEmpty()) {
                        logRequest(logEntry);
                    }
                }

                clientSocket.close();
                System.out.println("Client connection closed on " + Thread.currentThread().getName());
            } catch (IOException e) {
                System.out.println(e.getMessage());
            }
        }

		//this is the format for the log
        private String generateLogEntry(String clientIP, String request) {
            String timestamp = new java.text.SimpleDateFormat("yyyy-MM-dd|HH:mm:ss").format(new java.util.Date());
            return timestamp + "|" + clientIP + "|" + request;
        }

		// Log the valid request
        private void logRequest(String logEntry) {
            try (FileWriter fw = new FileWriter("log.txt", true);
                 BufferedWriter bw = new BufferedWriter(fw);
                 PrintWriter out = new PrintWriter(bw)) {
                out.println(logEntry);
            } catch (IOException e) {
                System.err.println("Error writing to log file: " + e.getMessage());
            }
        }
    }

	public void run(String[] args){
		for (String arg : args) {
            votes.put(arg, 0);
        }

		try{
			serverSock = new ServerSocket(4242);
			running = true;
			System.out.println("Server started on port 4242");
			while (running) {
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

