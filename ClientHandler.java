import java.io.*;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class ClientHandler implements Runnable {
    private Socket clientSocket;

    public ClientHandler(Socket socket) {
        this.clientSocket = socket;
    }

    @Override
    public void run() {
        try (
            BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
        ) {
            String expression;
            while ((expression = in.readLine()) != null) {
                System.out.println("Received expression: " + expression);
                String result = evaluateExpression(expression);
                out.println(result); // Send the result back to the client
            }
        } catch (IOException e) {
            // e.printStackTrace();
        } finally {
            try {
                clientSocket.close();
            } catch (IOException e) {
                // e.printStackTrace();
            }
        }
    }

    private String evaluateExpression(String expression) {
        try {
            // Tokenize the expression using space as delimiter
            List<String> tokens = new ArrayList<>(Arrays.asList(expression.split(" ")));
            
            // Process multiplication and division first
            for (int i = 0; i < tokens.size(); i++) {
                if (tokens.get(i).equals("*") || tokens.get(i).equals("/")) {
                    double leftOperand = Double.parseDouble(tokens.get(i - 1));
                    double rightOperand = Double.parseDouble(tokens.get(i + 1));
                    double result;
                    
                    if (tokens.get(i).equals("*")) {
                        result = leftOperand * rightOperand;
                    } else {
                        if (rightOperand == 0) {
                            return "Error: Division by zero";
                        }
                        result = leftOperand / rightOperand;
                    }
                    
                    // Replace the operation and operands with the result
                    tokens.set(i - 1, String.valueOf(result));
                    tokens.remove(i); // Remove the operator
                    tokens.remove(i); // Remove the operand
                    i--; // Adjust index for next iteration
                }
            }
            
            // Process addition and subtraction
            double finalResult = Double.parseDouble(tokens.get(0));
            for (int i = 1; i < tokens.size(); i += 2) {
                String operator = tokens.get(i);
                double operand = Double.parseDouble(tokens.get(i + 1));
                
                if (operator.equals("+")) {
                    finalResult += operand;
                } else if (operator.equals("-")) {
                    finalResult -= operand;
                } else {
                    return "Error: Invalid operator";
                }
            }
            
            return String.valueOf(finalResult);
        } catch (NumberFormatException e) {
            return "Error: Invalid number";
        } catch (Exception e) {
            return "Error: Invalid expression";
        }
    }
}
