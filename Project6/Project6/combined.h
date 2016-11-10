//
//  combined.h
//  Project6
//
//  Created by jr2339 on 11/10/16.
//  Copyright © 2016 jr2339. All rights reserved.
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

#define PORT "13" // DAYLIGHT protocol works on port 13
#define SERVER_NAME "time.nist.gov" // government time server
#define OTM_CHAR '*' // on time character
void conect_gov_server();
char* read_line(int socket);
#endif /* combined_h */

