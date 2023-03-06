#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "composite.h"
#include "processFD.h"
#include "systemWide.h"
#include "INode.h"
#include "thresh.h"


int main(int argc, char *argv[]) {
    int per_process = 0;
    int system_wide = 0;
    int vnodes = 0;
    int composite = 0;
    int threshold = -1;
    int pid = -1;

    static struct option long_options[] = {
        {"per-process", no_argument, 0, 'p'},
        {"systemWide", no_argument, 0, 's'},
        {"Vnodes", no_argument, 0, 'v'},
        {"composite", no_argument, 0, 'c'},
        {"threshold", required_argument, 0, 't'},
        {0, 0, 0, 0}
    };

    while (1) {
        int option_index = 0;
        int c = getopt_long(argc, argv,"t:", long_options,&option_index);
        
        if (c == -1)
            break;

        switch (c) {
            case 't':
                threshold = atoi(optarg);
                break;
            case 'p':
                per_process = 1;
                break;
            case 's':
                system_wide = 1;
                break;
            case 'v':
                vnodes = 1;
                break;
            case 'c':
                composite = 1;
                break;
            case '?':
                exit(EXIT_FAILURE);
            default:
                break;
        }
    }

   if (optind < argc) {
      pid=atoi(argv[optind]);
      optind++;
      if(optind<argc){
          printf("Error: Too many positional arguments\n");
          exit(EXIT_FAILURE);
      }
   }

   // Your code here

    if (per_process != 0){
        if(pid == -1){
        getALLProcessFD();
        }
        else{
            print_fds(pid);
        }
    }

    if(system_wide != 0){
        if(pid == -1){
        allSWide();}

        else {systemWideFD(pid);}
    }

    if (vnodes != 0){
        if(pid == -1){
        printAllINode();}

        else {
            printINodeForPID(pid);
            }
    }

    if (composite != 0){
        if (pid == -1){
        printComposite();}

        else{
            char* str2;
            sprintf(str2, "%d", pid);
            printCompositeForPid(str2);
            }
    }

    if (threshold != -1){
        thresh(threshold);
    }

    if (per_process == 0 && system_wide == 0 && vnodes == 0 && composite == 0 && threshold == -1 && pid == -1){
        printComposite();
    }

    
   return EXIT_SUCCESS;
}