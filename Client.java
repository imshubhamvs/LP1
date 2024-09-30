import java.io.*;
import java.net.Socket;

public class Client {

    public static void main(String[] args) {
        final String SERVER_ADDRESS = "10.10.14.126";
        final int PORT = 5000;

        try (Socket socket = new Socket(SERVER_ADDRESS, PORT);
             BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
             PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
             BufferedReader userInput = new BufferedReader(new InputStreamReader(System.in))
        ) {
            String input;
            System.out.println("Enter mathematical expressions (type 'exit' to quit):");
            while ((input = userInput.readLine()) != null) {
                if ("exit".equalsIgnoreCase(input)) {
                    break;
                }
                out.println(input); // Send expression to server
                System.out.println("Server response: " + in.readLine()); // Print server response
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
