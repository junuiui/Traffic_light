#include "hal/buzzer.h"
#include "../../app/include/utils.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

// Macros
#define BUZZER_PERIOD       "/dev/bone/pwm/0/a/period"
#define BUZZER_DUTY_CYCLE   "/dev/bone/pwm/0/a/duty_cycle"
#define BUZZER_ENABLE       "/dev/bone/pwm/0/a/enable"

// static functions
static void buzzer_on();
static void buzzer_off();
static void buzzer_play_v1();
static void buzzer_play_v2();
static void buzzer_play_v3();
static long long period;
static long long duty_cycle;
static int buzzer_state = 0;
static int shouldRun = 1;
int flag = 3;

void buzzer_init(){
    runCommand("config-pin p9_22 pwm");
}

void buzzer_cleanup(){
    buzzer_off();
    printf("\n*********\n");
    printf("Cleaning Buzzer\n");

    printf("\tTurning off Buzzer... ");
    shouldRun = 0;
    
    printf(" DONE\n");

    printf("Buzzer Successfully Cleaned\n");
    printf("*********\n");
}

void buzzer_changeFlag(int x){
    flag = x;
}


void buzzer_playSound(int version){
    
    switch(version){
        case 1:
            buzzer_play_v1();
            break;
        case 2:
            buzzer_play_v2();
            break;
        default:
            return;
    }
    sleepForMs(10);
}


static void buzzer_on(){
    writeToFile(BUZZER_ENABLE, "1");
    buzzer_state = 1;
}

static void buzzer_off(){
    writeToFile(BUZZER_ENABLE, "0");
    buzzer_state = 0;
}

static void buzzer_play_v1(){

    if (!buzzer_state) { buzzer_on(); }

    writeToFile(BUZZER_PERIOD, "300000");
    writeToFile(BUZZER_DUTY_CYCLE, "50000");
    sleepForMs(50);

    writeToFile(BUZZER_PERIOD, "500000");
    writeToFile(BUZZER_DUTY_CYCLE, "90000");
    sleepForMs(100);

    writeToFile(BUZZER_PERIOD, "100000");
    writeToFile(BUZZER_DUTY_CYCLE, "50000");
    sleepForMs(50);

    buzzer_off();
}

static void buzzer_play_v2(){
    if (!buzzer_state ) { buzzer_on(); }

    writeToFile(BUZZER_PERIOD, "10001000");
    writeToFile(BUZZER_DUTY_CYCLE, "500000");
    sleepForMs(50);

    buzzer_off();
    sleepForMs(10);
    buzzer_on();

    writeToFile(BUZZER_PERIOD, "10200000");
    writeToFile(BUZZER_DUTY_CYCLE, "500000");
    sleepForMs(150);

    buzzer_off();
}

static void buzzer_play_v3(){
    if (!buzzer_state ) { buzzer_on(); }

    writeToFile(BUZZER_PERIOD, "300000");
    writeToFile(BUZZER_DUTY_CYCLE, "50000");
    sleepForMs(400);

    buzzer_off();
}
