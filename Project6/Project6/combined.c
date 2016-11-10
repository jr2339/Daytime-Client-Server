//
//  combined.c
//  Project6
//
//  Created by jr2339 on 11/10/16.
//  Copyright © 2016 jr2339. All rights reserved.
//

#include "combined.h"
/*=============================================Connect to Gov Server=======================*/
char* read_line(int socket){
    char buffer[256];
    int count = 0;
    char out = 0;
    // Keep reading until we either run out of space or hit the on time character
    while (out != OTM_CHAR && count < 256) {
        read(socket,&out,1);
        buffer[count] = out;
        count++;
    }
    // Null terminate the string one space before the on time marker as per the documentation
    buffer[count - 2] = '\0';
    // Copy our string out of the local scope for return, removing extra array space
    // during the process.
    char* ret = strdup(buffer);
    return ret;
}

void conect_gov_server(){
    // --- SET UP CONNECTION ---
    // These are used for getaddrinfo and the subsequent steps below
    struct addrinfo* result;
    struct addrinfo* res;
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
        perror("Failure to resolve server name.");
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
        if(connect(sockfd, res->ai_addr, res->ai_addrlen) != -1) {
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
    char* ans = read_line(sockfd);
    printf("%s",ans);
    
    // --- CLEAN UP CONNECTION ---
    close(sockfd);
    
}


/*=============================================Create New Server for Client=======================*/




int main(){
    
}

