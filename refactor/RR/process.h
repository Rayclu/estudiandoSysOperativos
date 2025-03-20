#ifndef PROCESS_H
#define PROCESS_H

#include <time.h>

typedef struct {
    int end_time;
    int start_time;
    int arrival_t;
    int answer_t;
    int waiting_t;
    int execution_time;

    char *pointer;
    char name[128];

    int priority;
    int Resourse_usage;

    int  completed;
} SystmProcess;

//void GetStats(SystmProcess*SProcess);
SystmProcess* create_process(const char *name, int priority);
void free_process(SystmProcess *process);

#endif // PROCESS_H