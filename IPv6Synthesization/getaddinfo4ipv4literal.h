//
//  getaddinfo4ipv4literal.h
//  IPv6Synthesization
//
//  Created by Gao Song on 06/11/2016.
//  Copyright © 2016 Gao Song. All rights reserved.
//

#ifndef getaddinfo4ipv4literal_h
#define getaddinfo4ipv4literal_h

#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <err.h>

/**
 * get synthesized IP Address for literal IPv4 address
 * usage: similar to getaddrinfo
 * principle: 
 *           1. get synthesized address list for "ipv4only.arpa"
 *           2. override the address list using the address passed
 *           3. remove the duplicated items
 * note: service parameter just for numberic one
 */
int	getaddrinfo4ipv4literal(const char *, const char *,
			    const struct addrinfo *,
			    struct addrinfo **);

#endif /* getaddinfo4ipv4literal_h */
