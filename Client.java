/**
 * Created by jr2339 and cb936 on 11/10/16.
 */
import java.net.*;
import java.io.*;

public class Client {
    //The port that we will connect on
    private static int port = 23657;
    private static String server_name = "localhost";

    //This is our main function
    //Network code can throw an exception
    public static void main(String[] args) throws IOException {

        try (
                // Resources for the try block; these will be automatically closed if
                // the try block fails at some point or when it finishes.
                // Create server socket
                Socket server_sock = new Socket(InetAddress.getByName(server_name), port);
                // Buffered Input and output streams for talking to the server
                PrintWriter OutStream = new PrintWriter(server_sock.getOutputStream(), true);
                BufferedReader InStream = new BufferedReader(new InputStreamReader(server_sock.getInputStream()));
                // Buffered Input stream for user input
                BufferedReader userInput = new BufferedReader(new InputStreamReader(System.in))
        ) {
            // User will select from a menu, store it in this variable
            int select;

            // This while loop is for the client
            while (true) {
                // This is a small options menu for the user
                System.out.println("+--------- Java Timer Client ----------+");
                System.out.println("|     1. Echo Year                     |");
                System.out.println("|     2. Echo Month                    |");
                System.out.println("|     3. Echo Day                      |");
                System.out.println("|     4. Echo Hour                     |");
                System.out.println("|     5. Echo Minute                   |");
                System.out.println("|     6. Echo Second                   |");
                System.out.println("|     7. Quit                          |");
                System.out.println("|     8. Quit and Shutdown Server      |");
                System.out.println("+--------------------------------------+");
                System.out.print("What would you like to do: ");

                // Getting the input from the user for their choice
                select = Integer.parseInt(userInput.readLine());

                // Call the appropriate method based on user selection
                // Values based on text menu above
                // Any invalid number will present the error and try again
                if (select == 1) {
                    OutStream.println('1');
                    // Year only gets the last two digits, so manually include the "20" here.
                    System.out.println("The year is 20" + InStream.readLine());
                } else if (select == 2) {
                    OutStream.println('2');
                    System.out.println("The month is " + InStream.readLine());
                } else if (select == 3) {
                    OutStream.println('3');
                    System.out.println("The day is " + InStream.readLine());
                } else if (select == 4) {
                    OutStream.println('4');
                    System.out.println("The hour is " + InStream.readLine());
                } else if (select == 5) {
                    OutStream.println('5');
                    System.out.println("The minute is " + InStream.readLine());
                } else if (select == 6) {
                    OutStream.println('6');
                    System.out.println("The second is " + InStream.readLine());
                } else if (select == 7) {
                    // We don't need to write anything here, just break the loop to close the socket
                    break;
                } else if (select == 8) {
                    // But we do need to tell the server to shutdown first here before we break
                    OutStream.println('8');
                    break;
                } else {
                    System.out.println("You must enter a number inclusively between 1 and 8.\n");
                }
            }

        // Catch potential errors
        } catch (UnknownHostException e) {
            System.err.println("Unknown host at " + server_name);
            System.exit(1);
        } catch (IOException e) {
            System.err.println("Unable to get I/O connection for " + server_name);
            System.exit(1);
        }
        System.out.println("Shutting down.");
    }
}