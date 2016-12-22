//
//  getaddinfo4ipv4literal.c
//  IPv6Synthesization
//
//  Created by Gao Song on 06/11/2016.
//  Copyright © 2016 Gao Song. All rights reserved.
//

#include "getaddinfo4ipv4literal.h"
#include <stdlib.h>
#include <string.h>

/**
 * see if two ipv4 socket address are equal
 * note: ignore the port
 */
int ipv4equal(struct sockaddr_in *left, struct sockaddr_in *right)
{
    return left->sin_addr.s_addr == right->sin_addr.s_addr? 1:0;
}

/**
 * see if two ipv6 socket address are equal
 * note: ignore the port
 */
int ipv6equal(struct sockaddr_in6 *left, struct sockaddr_in6 *right)
{
    int equal = 1;
    const int length = 16;
    
    for (int i = 0; i < length; i++)
    {
        if (left->sin6_addr.s6_addr[i] != right->sin6_addr.s6_addr[i])
        {
            equal = 0;
            break;
        }
    }
    
    return equal;
}

/**
 * see if two ip addresses are equal
 */
int ipequal(struct addrinfo *left, struct addrinfo *right)
{
    int equal;
    if (left->ai_family == right->ai_family) {
        if (left->ai_family == AF_INET) {
            equal = ipv4equal((struct sockaddr_in *)left->ai_addr, (struct sockaddr_in *)right->ai_addr);
        }
        else
        {
            equal = ipv6equal((struct sockaddr_in6 *)left->ai_addr, (struct sockaddr_in6 *)right->ai_addr);
        }
    }
    else
    {
        equal = 0;
    }
    
    return equal;
}

/**
 * remove duplicated items
 */
void removeduplicateditems(struct addrinfo **res)
{
    struct addrinfo *current, *checking, *previousChecking;
    
    current = *res;
    
    while (current != NULL && current->ai_next != NULL) {
        checking = current->ai_next;
        previousChecking = current;
        while (checking != NULL) {
            if (ipequal(checking,current)) {
                previousChecking->ai_next = checking->ai_next;
                checking->ai_next = NULL;
                freeaddrinfo(checking);
                checking = previousChecking->ai_next;
            }
            else
            {
                checking = checking->ai_next;
            }
        }
        
        current = current->ai_next;
    }
}

int getaddrinfo4ipv4literal(const char *hostname, const char *servname,
            const struct addrinfo *hints, struct addrinfo **res)
{
    int rlt = 0;
    // 1. get address list
    rlt = getaddrinfo("ipv4only.arpa", "http", hints, res);
    if (rlt == 0) {
        in_addr_t ipv4 = inet_addr(hostname);
        in_port_t port = htons(atoi(servname));
       // 2. look through the address list and replace the ipv4 address and port by ours
        struct addrinfo *tempRes;
        for (tempRes = *res; tempRes; tempRes = tempRes->ai_next) {
            if (tempRes->ai_family == AF_INET)// IPv4
            {
                struct sockaddr_in *dest = (struct sockaddr_in *)tempRes->ai_addr;
                // overwrite
                dest->sin_addr.s_addr = ipv4;
                dest->sin_port = port;
            }
            else if(tempRes->ai_family == AF_INET6)// IPv6
            {
                struct sockaddr_in6 *dest = (struct sockaddr_in6 *)tempRes->ai_addr;
                // overwrite the last four bytes
                memcpy(dest->sin6_addr.s6_addr + 12, &ipv4, sizeof(in_addr_t));
                dest->sin6_port = port;
            }
        }
        // 3. remove duplicated items
        removeduplicateditems(res);
    }
    
    return rlt;

}
