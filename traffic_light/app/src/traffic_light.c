/**
 * Description: Main .c file for execution
 * 
 * Author: Junui Hong
 * Last Modified: 2024-04-24
*/

#include <hal/buzzer.h>
#include <hal/sharedMem-Linux.h>

#include <stdio.h>
#include <pthread.h>

int main(){

    // initialize
    sharedMem_init();
    buzzer_init();
    printf("INIT: Successful!\n");

    // pthread
    pthread_t sharedMemThread;
    printf("PTHREAD: Successful!\n");

    // pthread create
    if (pthread_create(&sharedMemThread, NULL, sharedMem_thread, NULL) != 0) {
        fprintf(stderr, "Error creating sharedMem thread\n");
        return 1;
    }
    printf("PTHREAD_CREATE: Successful!\n");

    // main loop
    while (sharedMem_getStatus()){
        ; // loop
    }
    printf("Main Loop Ended!\n");

    // clean up
    buzzer_cleanup();
    sharedMem_cleanup();

    return 0;
}