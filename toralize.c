/* toralize.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "toralize.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <dlfcn.h>

/*
Library to toralize a command

./toralize <host> <port>

e.g usage: ./toralize 1.2.3.4 80

*/


Req *request(struct sockaddr_in *sock2){

    // setup the request according to the socks4 rfc

    Req *req;
    req = malloc(reqsize);

    req->vn = 4;
    req->cd = 1;
    req->dstport = sock2->sin_port;
    req->dstip = sock2-> sin_addr.s_addr;
    strncpy((char *)req->userid, USERNAME, 8);

    return req;    
}

int connect(int s2, const struct sockaddr* sock2, socklen_t address_len){

    // setup variables
    int s; 
    struct sockaddr_in sock;
    Req *req;
    Res *res;
    char buffer[ressize];
    int success;
    char tmp[512];
    int (*p)(int, const struct sockaddr*, socklen_t);


    // intercept the connect call
    p = dlsym(RTLD_NEXT, "connect");

    // create a socket
    s = socket(AF_INET, SOCK_STREAM, 0);

    // verify the socket
    if (s < 0){
        perror("socket");
        return -1;
    }

    // setup the proxy
    sock.sin_family = AF_INET;
    sock.sin_port = htons(PROXYPORT);
    sock.sin_addr.s_addr = inet_addr(PROXY);

    // connect to the proxy
    if (p(s, (struct sockaddr * )& sock, sizeof(sock)) != 0){
        perror("connect");
        return -1;   
    }
    printf("Connected to proxy\n");

    // create the request
    req = request((struct sockaddr_in *)sock2);
    write(s, req, reqsize);
    memset(buffer,0,ressize);

    // read the response
    if(read(s, buffer, ressize) < 1){
        perror("read");
        free(req);
        close(s);
        return(-1);
    }

    // verify the response
    res = (Res *)buffer;
    success = (res->cd== 90);
    if (!success){
        fprintf(stderr, "Unable to traverse the proxy, error code: %d\n", res->cd);
        free(req);
        close(s);

        return(-1);
    }

    printf("Successfully connected through the proxy\n");


    // duplicate the socket
    dup2(s,s2);

    // free the request, no need to close the connection
    free(req);
    return 0;  
}