#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

void sleepForMs(long long delayInMs){
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;

    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

void runCommand(char* command) {

    FILE *pipe = popen(command, "r");

    if (pipe == NULL) {
        perror("popen");
        exit(-1);
    }

    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL) {
            break;
        }
    }

    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0) {
        exit(-1);
    }
}

void writeToFile(char* fileName, char* value) {
	FILE* pFile = fopen(fileName, "w");

    if(pFile == NULL){
        printf("ERROR: In %s.\n", fileName);
        exit(1);
    }

    int charWritten = fprintf(pFile, "%s", value);
    if (charWritten <= 0) {
        printf("ERROR WRITING DATA");
        exit(1);
    }

    fclose(pFile);
}

void writeToFile2(char* fileName, char* value) {
	FILE* pfile = fopen(fileName, "w");
	if (pfile == NULL) {
		printf("ERROR: Unable to open %s.\n", fileName);
		exit(1);
	}
	fprintf(pfile, "%s", value);
	// Write to data to the file using fprintf():
	fclose(pfile);
}

// Initializes the I2C device
int initI2cBus(char* bus, int address)
{
    // Enable Bus
    int i2cFileDesc = open(bus, O_RDWR);
    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
    if (result < 0) {
        perror("I2C: Unable to set I2C device to slave address.");
        exit(1);
    }
    return i2cFileDesc;
}

void writeI2cReg(int i2cFileDesc, unsigned char regAddr,unsigned char value)
{
    unsigned char buff[2];
    buff[0] = regAddr;
    buff[1] = value;
    int res = write(i2cFileDesc, buff, 2);
    if (res != 2) {
        perror("I2C: Unable to write i2c register.");
        exit(1);
    }
}

unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr) {
    // To read a register, must first write the address
    int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
    if (res != sizeof(regAddr)) {
        perror("I2C: Unable to write to i2c register.");
        exit(1);
    }

    // Now read the value and return it
    char value = 0;
    res = read(i2cFileDesc, &value, sizeof(value));
    if (res != sizeof(value)) {
        perror("I2C: Unable to read from i2c register");
        exit(1);
    }
    return value;
}