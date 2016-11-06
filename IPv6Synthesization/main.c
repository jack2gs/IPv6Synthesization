//
//  main.c
//  IPv6Synthesization
//
//  Created by Gao Song on 06/11/2016.
//  Copyright © 2016 Gao Song. All rights reserved.
//

#include <stdio.h>
#include "getaddinfo4ipv4literal.h"
#include <string.h>
#include <unistd.h>

int main(int argc, const char * argv[]) {
    
    uint8_t ipv4[4] = {115, 239, 210, 27};
    struct addrinfo hints, *res, *res0;
    int error, s;
    const char *cause = NULL;
    
    char ipv4_str_buf[INET_ADDRSTRLEN] = { 0 };
    const char *ipv4_str = inet_ntop(AF_INET, &ipv4, ipv4_str_buf, sizeof(ipv4_str_buf));
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_DEFAULT;
    error =	getaddrinfo4ipv4literal(ipv4_str, "80", &hints, &res0);
    if (error) {
        errx(1, "%s", gai_strerror(error));
        /*NOTREACHED*/
    }
    s = -1;
    for (res = res0; res; res = res->ai_next) {
        s = socket(res->ai_family, res->ai_socktype,
                   res->ai_protocol);
        if (s < 0) {
            cause = "socket";
            continue;
        }
        
        if (connect(s, res->ai_addr, res->ai_addrlen) < 0) {
            cause = "connect";
            close(s);
            s = -1;
            continue;
        }
        
        break;  /* okay we got one */
    }
    if (s < 0) {
        err(1, "%s", cause);
        /*NOTREACHED*/
    }
    freeaddrinfo(res0);
    
    getchar();
    
    return 0;
}
