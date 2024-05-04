/**
 * Description: Codes for controlling PRU
 * 
 * Author: Junui Hong
 * Last Modified: 2024-05-04
*/

// .h files
#include "sharedDataStruct.h"
#include "resource_table_empty.h"
#include <stdint.h>
#include <stdbool.h>
#include <pru_cfg.h>

// Macros
#define CYCLE_PER_MS (200000000 / 1000)
#define DELAY_250_MS 50000000

// Macros for LEDs
#define DATA_PIN 15
#define STR_LEN         8       // # LEDs in our string
#define oneCyclesOn     700/5   // Stay on 700ns
#define oneCyclesOff    800/5
#define zeroCyclesOn    350/5
#define zeroCyclesOff   600/5
#define resetCycles     60000/5 // Must be at least 50u, use 60u

// Macros for COLOURS
// format: 0x00112233
// 00: GREEN
// 11: RED
// 22: BLUE
// 33: WHITE
#define GREEN_BRIGHT 0x07000000
#define GREEN 0x01000000
#define RED_BRIGHT 0x00070000
#define RED 0x00010000
#define BLUE_BRIGHT 0x00001f00
#define BLUE 0x00000300
#define NO_COLOR 0x00000000

// P8_11 for output (on R30), PRU0
#define DATA_PIN 15       // Bit number to output on

// volatile register --> interacting with hardware
volatile register uint32_t __R30;   // OUTPUT GPIO register
volatile register uint32_t __R31;   // INPUT  GPIO register

// JOYSTICK
#define JOYSTICK_RIGHT_MASK (1 << 15)   // for exit
#define JOYSTICK_DOWN_MASK (1 << 14)    // for chaning mode

// Shared Memory Configuration
// -----------------------------------------------------------
#define THIS_PRU_DRAM 0x00000 // Address of DRAM
#define OFFSET 0x200 // Skip 0x100 for Stack,
 // 0x100 for Heap (from makefile)
#define THIS_PRU_DRAM_USABLE (THIS_PRU_DRAM + OFFSET)
// This works for both PRU0 and PRU1 as both map their own memory to 0x0000000
volatile sharedMemStruct_t *pSharedMemStruct = (volatile void *)THIS_PRU_DRAM_USABLE;

// Array for holding colours
// index 0 --> bottom of LED
// index 7 --> top of LED
static uint32_t color[STR_LEN];

/**
 * Delaying for given miliseconds
 * @param numMS to delay for 
*/
void delayForMS(int numMS){
    for (int i = 0; i < numMS; i++){
        __delay_cycles(CYCLE_PER_MS);
    }
}

/**
 * Displaying LED on neoPixel
*/
void Led_display(){
    // Display colors
    for(int j = 0; j < STR_LEN; j++) {
        for(int i = 31; i >= 0; i--) {
            if(color[j] & ((uint32_t)0x1 << i)) {
                __R30 |= 0x1<<DATA_PIN;      // Set the GPIO pin to 1
                __delay_cycles(oneCyclesOn-1);
                __R30 &= ~(0x1<<DATA_PIN);   // Clear the GPIO pin
                __delay_cycles(oneCyclesOff-2);
            } 
            else {
                __R30 |= 0x1<<DATA_PIN;      // Set the GPIO pin to 1
                __delay_cycles(zeroCyclesOn-1);
                __R30 &= ~(0x1<<DATA_PIN);   // Clear the GPIO pin
                __delay_cycles(zeroCyclesOff-2);
            }
        }
    }

    // Send Reset
    __R30 &= ~(0x1<<DATA_PIN);   // Clear the GPIO pin
    __delay_cycles(resetCycles);
}

/**
 * Turn off LED
*/
void Led_turnoff(){
    // Display colors
    for (int i = 0; i < STR_LEN; i++){
        color[i] = NO_COLOR;
    }
    Led_display();
}


/**
 * Set Led to Black (3rd, 6st)
*/
void Led_setBlack(){
    color[2] = NO_COLOR;
    color[5] = NO_COLOR;
    Led_display();
}

/**
 * Set Led to Red
 * color[7,6]
*/
void Led_Red(){
    for (int i = 0; i < 8; i++){
        if (i < 6){
            color[i] = NO_COLOR;
        }
        else{
            color[i] = RED_BRIGHT;
        }
    }
    Led_setBlack();
}

/**
 * Set Led to Yellow
 * color[4,3]
*/
void Led_Yellow(){
    for (int i = 0; i < 8; i++){
        
        if (i == 4 || i == 3){
            color[i] = 0x07070000;
        }
        else{
            color[i] = NO_COLOR;
        }
    }
    Led_setBlack();
}

/**
 * Set Led to Green
 * color[1,0]
*/
void Led_Green(){
    for (int i = 0; i < 8; i++){
        if (i < 3){
            color[i] = GREEN_BRIGHT;
        }
        else{
            color[i] = NO_COLOR;
        }
    }
    Led_setBlack();
}


void Led_setAllYellow(){
    for (int i = 0; i < 8; i++){
        color[i] = 0x07070000;
    }
    Led_setBlack();
}

/**
 * Set Led to Yellow and flashing
*/
void Led_setYellowFlashing(){

    int count = 0;

    Led_setAllYellow();
    while (count < 250){
        count++;
        delayForMS(2);
        pSharedMemStruct->isRightPressed = (__R31 & JOYSTICK_RIGHT_MASK) == 0;
        pSharedMemStruct->isDownPressed = (__R31 & JOYSTICK_DOWN_MASK) == 0;
        if (pSharedMemStruct->isRightPressed || pSharedMemStruct->isDownPressed){
            return;
        }
    }

    count = 0;
    Led_turnoff();
    while (count < 250){
        count++;
        delayForMS(2);
        pSharedMemStruct->isRightPressed = (__R31 & JOYSTICK_RIGHT_MASK) == 0;
        pSharedMemStruct->isDownPressed = (__R31 & JOYSTICK_DOWN_MASK) == 0;
        if (pSharedMemStruct->isRightPressed || pSharedMemStruct->isDownPressed){
            return;
        }
    }
}

/**
 * Red --> Yello --> Green --> Red
 * Default Time Distribution:
 * RED: 40%
 * YELLOW: 10%
 * GREEN: 50% 
*/
void Led_normal(){

    int count = 0; 
    int tempTime;

    tempTime = pSharedMemStruct->rTime / 1000;
    Led_Red();
    while (count < 1000){
        count++;
        delayForMS(tempTime);
        pSharedMemStruct->isRightPressed = (__R31 & JOYSTICK_RIGHT_MASK) == 0;
        pSharedMemStruct->isDownPressed = (__R31 & JOYSTICK_DOWN_MASK) == 0;
        if (pSharedMemStruct->isRightPressed || pSharedMemStruct->isDownPressed){
            return;
        }
    }

    count = 0;
    tempTime = pSharedMemStruct->yTime / 1000;
    Led_Yellow();
    while (count < 1000){
        count++;
        delayForMS(tempTime);
        pSharedMemStruct->isRightPressed = (__R31 & JOYSTICK_RIGHT_MASK) == 0;
        pSharedMemStruct->isDownPressed = (__R31 & JOYSTICK_DOWN_MASK) == 0;
        if (pSharedMemStruct->isRightPressed || pSharedMemStruct->isDownPressed){
            return;
        }
    }

    count = 0;
    tempTime = pSharedMemStruct->gTime / 1000;
    Led_Green();
    while (count < 1000){
        count++;
        delayForMS(tempTime);
        pSharedMemStruct->isRightPressed = (__R31 & JOYSTICK_RIGHT_MASK) == 0;
        pSharedMemStruct->isDownPressed = (__R31 & JOYSTICK_DOWN_MASK) == 0;
        if (pSharedMemStruct->isRightPressed || pSharedMemStruct->isDownPressed){
            return;
        }
    }
}

void main(void){
    // Clear SYSCFG[STANDBY_INIT] to enable OCP master port
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
    __delay_cycles(resetCycles);

    while (!pSharedMemStruct->isRightPressed){

        // check for right pressed
        pSharedMemStruct->isRightPressed = (__R31 & JOYSTICK_RIGHT_MASK) == 0;
        pSharedMemStruct->isDownPressed = (__R31 & JOYSTICK_DOWN_MASK) == 0;

        // interact with modes
        if (pSharedMemStruct->mode){
            Led_normal();
        }
        else{
            Led_setYellowFlashing();    
        }
        
        // Change mode if down pressed
        if (pSharedMemStruct->isDownPressed){
            pSharedMemStruct->mode = !pSharedMemStruct->mode;
        }
        
        // delay for prevent duplicate presses
        delayForMS(100);
    }

    Led_turnoff();

    __halt();
}


