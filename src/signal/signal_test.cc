/**
 * linux signal
 *
 */
#include <signal.h>
#include <stdlib.h>
#include <iostream>

void signal_handler(int signum)
{
    std::cout<<"quit."<<std::endl;
    exit(signum);
}

int main(int argc, char *argv[])
{
    signal(SIGINT,signal_handler);

    while(true){
        std::cout<<"hello,world.\n";
    }
    return 0;
}
