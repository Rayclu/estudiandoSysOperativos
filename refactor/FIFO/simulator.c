#include "simulator.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>

void formatTime(int seconds, char *buffer) {
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int secs = seconds % 60;
    sprintf(buffer, "%02d:%02d:%02d", hours, minutes, secs);
}

void Simulate_execution(SystmProcess *process, Averages *averages) {
    if (!process) {
        printf("Invalid process.\n");
        return;
    }
    
    char timeBuffer[9];
    printf("\nExecuting Process:\n");
    printf("-> Name: %s\n", process->name);
    
    process->start_time = time(NULL);
    system(process->pointer);
    process->end_time = time(NULL);
    
    process->execution_time = process->end_time - process->start_time;
    process->waiting_t = process->start_time - process->arrival_t;
    process->answer_t = process->end_time - process->arrival_t;

    if (averages->numb_of_processes != 0) {
        averages->waiting_t += process->waiting_t;
        averages->turnaround_t += process->answer_t;
        averages->response_t += process->execution_time;
        
        formatTime(averages->waiting_t = averages->waiting_t / averages->numb_of_processes, timeBuffer);
        formatTime(averages->turnaround_t = averages->turnaround_t / averages->numb_of_processes, timeBuffer);
        formatTime(averages->response_t = averages->response_t / averages->numb_of_processes, timeBuffer);
        printf("Averages:\n");
        printf("Waiting Time: %f\n", averages->waiting_t);
        printf("Turnaround Time: %f\n", averages->turnaround_t);
        printf("Response Time: %f\n", averages->response_t);
    }

    formatTime(process->answer_t, timeBuffer);
    printf("-> Answer Time: %s (%d seconds)\n", timeBuffer, process->answer_t);
    
    formatTime(process->waiting_t, timeBuffer);
    printf("-> Waiting Time: %s (%d seconds)\n", timeBuffer, process->waiting_t);
    
    formatTime(process->execution_time, timeBuffer);
    printf("-> Execution Time: %s (%d seconds)\n", timeBuffer, process->execution_time);
    
    printf("-> Path: %s\n", process->pointer);
    printf("\n___________________________________________________________");
}

void showMenu() {
    printf("\n=== System Process Simulator ===\n");
    printf("|  1. Add new process           |\n");
    printf("|  2. Show current queue        |\n");
    printf("|  3. Execute simulation        |\n");
    printf("|  4. Show statistics           |\n");
    printf("|  5. Exit                      |\n");
    printf("+-------------------------------+\n");
    printf("\nSelect an option: ");
}