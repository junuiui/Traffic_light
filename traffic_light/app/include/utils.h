/**
 * Utility functions
*/

#ifndef _UTIL_H_
#define _UTIL_H_

void sleepForMs(long long);
void runCommand(char*);
void writeToFile(char*, char*);
void writeToFile2(char*, char*);
int initI2cBus(char*, int);
void writeI2cReg(int, unsigned char, unsigned char);
unsigned char readI2cReg(int, unsigned char);

#endif