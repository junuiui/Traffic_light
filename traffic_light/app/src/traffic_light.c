/**
 * Description: Main .c file for execution
 * 
 * Author: Junui Hong
 * Last Modified: 2024-04-24
*/

#include <hal/buzzer.h>

#include <stdio.h>

int main(){

    printf("First init... Testing CMAKE\n");

    buzzer_init();
    buzzer_playSound(1);
    buzzer_playSound(2);
    buzzer_playSound(3);

    return 0;
}