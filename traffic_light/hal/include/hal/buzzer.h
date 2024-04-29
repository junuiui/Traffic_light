#ifndef _BUZZER_H
#define _BUZZER_H

void buzzer_init(void);
void buzzer_cleanup(void);
void* buzzer_thread(void*);
void buzzer_playSound(int);

#endif