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
    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
    int error;
    
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
    
    for (res = res0; res; res = res->ai_next) {
        if (getnameinfo(res->ai_addr, res->ai_addr->sa_len, hbuf, sizeof(hbuf), sbuf,
                        sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV)) {
            errx(1, "could not get numeric hostname");
            /*NOTREACHED*/
        }
        printf("host=%s, serv=%s\n", hbuf, sbuf);
    }
    freeaddrinfo(res0);
    
    getchar();
    
    return 0;
}
