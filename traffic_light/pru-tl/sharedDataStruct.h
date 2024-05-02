// from CMPT433 PRU Guide

#ifndef _SHARED_DATA_STRUCT_H_
#define _SHARED_DATA_STRUCT_H_
#include <stdbool.h>
#include <stdint.h>
// WARNING:
// Fields in the struct must be aligned to match ARM's alignment
// bool/char, uint8_t: byte aligned
// int/long, uint32_t: word (4 byte) aligned
// double, uint64_t: dword (8 byte) aligned
// Add padding fields (char _p1) to pad out to alignment.
// My Shared Memory Structure
// ----------------------------------------------------------------

typedef struct {

    bool isRightPressed;    // Right Press for exit program
    bool isDownPressed;     // Down Press for change mode
    bool mode;
    char padding;           // PADDING
    int rTime;
    int yTime;
    int gTime;

} sharedMemStruct_t;

#endif
