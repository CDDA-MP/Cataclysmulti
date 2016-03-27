#include <uv.h>
#include <stdio.h>
#include <stdlib.h>
#include "network.h"
#include "version.h"

int main(int argc, char *argv[]){
    if (argc < 3){
        puts(VERSION_NAME " " VERSION_PREFIX " " VERSION_VERSION"\n"
            "Usage: cmulti <address> <port>\n");
    }
    const char *address = argv[1];
    int port = atoi(argv[2]);

    uv_loop_t *loop = uv_default_loop(); //get default loop
    Network::connected = false;
    Network::connect(loop,address,port); //connect to the server
    
    uv_run(loop, UV_RUN_DEFAULT); //run uv loop
     
    return 0;
}