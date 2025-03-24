import java.io.*;
import java.net.*;

public class Client {

    private Socket clientSocket = null;
    private PrintWriter socketOutput = null;
    private BufferedReader socketInput = null;

    public void interactWithServer(String command, String option) {
        try {
            
            clientSocket = new Socket("localhost", 4242);

            
            socketOutput = new PrintWriter(clientSocket.getOutputStream(), true);
            socketInput = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));

            // Send the command to the server
            if (command.equalsIgnoreCase("list")) {
                socketOutput.println("list");
            } else if (command.equalsIgnoreCase("vote") && option != null) {
                socketOutput.println("vote " + option);
            } else {
                System.out.println("Invalid command.");
                return;
            }

            // Read and print the server's response
            String fromServer;
            while ((fromServer = socketInput.readLine()) != null) {
                System.out.println(fromServer);
            }
            socketOutput.close();
            socketInput.close();
            clientSocket.close();

        } catch (UnknownHostException e) {
            System.err.println("Unknown host: localhost");
        } catch (IOException e) {
            System.err.println("Couldn't get I/O for the connection to the server.");
        }
    }

    public static void main(String[] args) {
        if (args.length < 1) {
            return;
        }

        String command = args[0];
        String option = args.length > 1 ? args[1] : null;

        Client client = new Client();
        client.interactWithServer(command, option);
    }
}
