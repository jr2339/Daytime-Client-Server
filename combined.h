//
//  combined.h
//  Project6
//
//  Created by jr2339 and cb936 on 11/10/16.
//  Copyright © 2016 jr2339 and cb936. All rights reserved.
//

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef combined_h
#define combined_h

#include <stdio.h>
#include <stdbool.h>

#define PORT "13" // DAYLIGHT protocol works on port 13
#define SERVER_NAME "utcnist.colorado.edu" // government time server
#define OTM_CHAR '*' // on time character

#define PORT_NUMBER 23657              // port the server will listen on
#define NUM_CONNECTIONS 1

// NIST String format:
// JJJJJ YR-MO-DA HH:MM:SS TT L H msADV UTC(NIST) OTM
#define YEAROFF 7
#define MONTHOFF 10
#define DAYOFF 13
#define HOUROFF 16
#define MINOFF 19
#define SECOFF 22

bool get_value(int client_socket, int offset);

char* connect_gov_server();
char* read_line(int socket);
#endif /* combined_h */

