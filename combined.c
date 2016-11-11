//
//  combined.c
//  Project6
//
//  Created by jr2339 and cb936 on 11/10/16.
//  Copyright © 2016 jr2339 and cb936. All rights reserved.
//

#include "combined.h"

/*=============================================Connect to Gov Server=======================*/
char *read_line(int socket) {
    char buffer[256];
    char out = 0;
    int length = 0;
    // Keep reading until we either run out of space or hit the on time character
    while (out != OTM_CHAR && length < 256) {
        read(socket, &out, 1);
        buffer[length] = out;
        length++;
    }
    // Null terminate the string one space before the on time marker as per the documentation
    buffer[length - 2] = '\0';
    // Copy our string out of the local scope for return, removing extra array space
    // during the process.
    char *ret = strdup(buffer);
    return ret;
}

char *connect_gov_server() {
    // --- SET UP CONNECTION ---
    // These are used for getaddrinfo and the subsequent steps below
    struct addrinfo *result;
    struct addrinfo *res;
    struct addrinfo hints;

    // Set up our hints structure with zeroes
    memset(&hints, 0, sizeof hints);
    // Only return IPv4 addresses
    hints.ai_family = AF_INET;
    // That are TCP connections
    hints.ai_socktype = SOCK_STREAM;

    // Get the address info related to our name/port and store it in result
    int error = getaddrinfo(SERVER_NAME, PORT, &hints, &result);
    if (error != 0) {
        perror("Failure to resolve server name");
        exit(1);
    }
    int sockfd = -1;
    // For each potential connection returned try to connect to it
    for (res = result; res != NULL; res = res->ai_next) {
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd == -1) {
            // Invalid socket, try the next one
            continue;
        }
        // We've got a valid socket, now we can try to connect to it
        if (connect(sockfd, res->ai_addr, res->ai_addrlen) != -1) {
            // If we successfully connected, then we don't need to check any more sockets
            break;
        }
        // Else we had a bad connection, so close the connection to clean up for the next try
        close(sockfd);
    }
    // None of our connections were valid ones, error out
    if (res == NULL) {
        perror("Could not connect to server");
        exit(1);
    }

    // --- AT THIS POINT WE HAVE A VALID CONNECTION TO THE DAYLIGHT SERVICE ---

    // We don't need the generated address info anymore, so we can free that up
    freeaddrinfo(result);
    // Call our read function and print the response
    char *ans = read_line(sockfd);
    //printf("%s\n",ans);

    // --- CLEAN UP CONNECTION ---
    close(sockfd);
    return ans;


}

/*=============================================Create New Server for Client=======================*/
int main() {
    // Creating required variables for TCP socket
    int server_socket;                 // descriptor of server socket
    struct sockaddr_in server_address; // for naming the server's listening socket

    // Create a TCP socket
    printf("Initializing socket.\n");
    if ((server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("Error creating socket");
        exit(1);
    }
    // Zero out the structure
    memset((char *) &server_address, 0, sizeof(server_address));

    // Setting address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT_NUMBER);

    //bind socket to port
    printf("Binding socket.\n");
    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        perror("Error binding socket");
        exit(1);
    }

    // listen for client connections (pending connections get put into a queue)
    printf("Listening.\n");
    if (listen(server_socket, NUM_CONNECTIONS) == -1) {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }
    /*===================================================================================*/
    bool halt = false;
    // As long as we aren't given a shutdown command
    while (!halt) {
        // Get a client
        printf("Socket ready. Waiting for client.\n");
        int client_socket;
        if ((client_socket = accept(server_socket, NULL, NULL)) == -1) {
            perror("Error accepting client");
        } else {
            printf("Accepted client.\n");
            bool quit = false;
            // Now that we have a client, keep their connection open until it's time to close it
            while (!quit) {
                char choice;
                // Read a single character from the client
                switch (read(client_socket, &choice, sizeof(char))) {
                    case 0:
                        quit = true;
                        printf("End of stream, returning.\n");
                        break;
                    case -1:
                        perror("Error reading from network\n");
                        quit = true;
                        break;
                    default:
                        break;
                }
                /* The char's received and their corresponding requests:
                 * 1 - Year
                 * 2 - Month
                 * 3 - Day
                 * 4 - Hour
                 * 5 - Minute
                 * 6 - Second
                 * 7 - N/A, client command to close, results in End of Stream for the server
                 * 8 - Server shutdown command. Causes the server to shutdown completely.
                 * */
                if (choice == '1') {
                    if (!get_value(client_socket, YEAROFF)) {
                        // Communication failure with client, abort the connection
                        break;
                    }
                } else if (choice == '2') {
                    if (!get_value(client_socket, MONTHOFF)) {
                        // Communication failure with client, abort the connection
                        break;
                    }
                } else if (choice == '3') {
                    if (!get_value(client_socket, DAYOFF)) {
                        // Communication failure with client, abort the connection
                        break;
                    }
                } else if (choice == '4') {
                    if (!get_value(client_socket, HOUROFF)) {
                        // Communication failure with client, abort the connection
                        break;
                    }
                } else if (choice == '5') {
                    if (!get_value(client_socket, MINOFF)) {
                        // Communication failure with client, abort the connection
                        break;
                    }
                } else if (choice == '6') {
                    if (!get_value(client_socket, SECOFF)) {
                        // Communication failure with client, abort the connection
                        break;
                    }
                } else if (choice == '8') {
                    // Server halt command, break the loop, close the connection, and shutdown
                    halt = true;
                    break;
                } else {
                    // Invalid choice, abort the connection
                    break;
                }
                // Handle the already sent newline character in preparation for the next read.
                read(client_socket, &choice, sizeof(char));
            }

            // cleanup
            if (close(client_socket) == -1) {
                perror("Error closing socket");
                exit(EXIT_FAILURE);
            } else {
                printf("Client handled.\n");
            }
        }
    }

    printf("Shutting down.\n");
    return 0;
}

// NIST String format:
// JJJJJ YR-MO-DA HH:MM:SS TT L H msADV UTC(NIST) OTM
// All values are only two digits long, so we can just manually pass in the offset we need

bool get_value(int client_socket, int offset) {
    char buffer[3];
    // Display server side message for this request
    printf("Client requesting local server time.\n");
    char* ans = connect_gov_server();
    printf("Recieved answer: %s\n", ans);
    // Duplicate the string pointer for iteration without loss
    char* p = ans;
    // Skip the first offset characters of the string
    for (int i = 0; i < offset; ++i) {
        p++;
        if (*p == '\0') {
            // we've reached the end of our answer string, so either a bad answer or a bad offset.
            // Error out here
            fprintf(stderr, "Invalid time string or value offset.\n");
            return false;
        }
    }
    // Copy the two desired digits into our buffer
    strncpy(buffer, p, 2);
    // And null terminate the new string so we can use it for printing
    buffer[2] = '\0';

    // We're done with both ans and p now, free that memory to prevent leakage
    free(ans);
    printf("Value retrieved. Writing %s out to client.\n", buffer);
    // Because we're writing out to a java client instead of null terminating our buffer we are
    // going to end our write string with a newline character instead to allow for easy use of
    // Java's readLine() function.
    buffer[2] = '\n';
    ssize_t n = write(client_socket, buffer, 3);
    if (n < 0) {
        perror("Error sending message to client");
        return false;
    }
    return true;
}