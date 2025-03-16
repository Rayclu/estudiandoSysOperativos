#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "process.h"
#include "queue.h"

typedef struct averages {
    float waiting_t;
    float turnaround_t;
    float response_t;
    float execution_t;
    int numb_of_processes;
} Averages;

typedef struct {
    int total_processes;
    int completed_processes;
    time_t start_time;
    time_t end_time;
} SystemStats;

void formatTime(int seconds, char *buffer);
void Simulate_execution(SystmProcess *process, Averages *averages);
void showMenu();

#endif // SIMULATOR_H