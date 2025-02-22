#include "queue.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void initialize(SystemQueue *q) {
    q->front = -1;
    q->rear = -1;
}

int push(SystmProcess *proc, SystemQueue *Queue) {
    if (Queue->rear == MAX - 1) return -1;
    if (Queue->front == -1) Queue->front = 0;
    if (proc->arrival_t == 0) proc->arrival_t = time(NULL);
    
    Queue->data[++Queue->rear] = proc;
    return 0;
}

SystmProcess* pop(SystemQueue *Queue) {
    if (Queue->front == -1 || Queue->front > Queue->rear) return NULL;
    SystmProcess *proc = Queue->data[Queue->front];
    Queue->front++;

    if (Queue->front > Queue->rear) {
        Queue->front = Queue->rear = -1;
    }
    return proc;
}

void displayQueue(SystemQueue *q) {
    if (q->front == -1 || q->front > q->rear) {
        printf("Queue is empty.\n");
    } else {
        printf("Processes in queue:\n");
        for (int i = q->front; i <= q->rear; i++) {
            SystmProcess *proc = q->data[i];
            printf("-> %s (Path: %s)(Priority: %d)\n", proc->name, proc->pointer, proc->priority);
        }
    }
}

SystemQueue* HaveThereAdminProcess(SystemQueue *q) {

    SystemQueue* AdminQueue = malloc(sizeof(SystemQueue));
    initialize(AdminQueue);
    for (int i = q->front; i <= q->rear; i++) {
        SystmProcess *proc = q->data[i];
        if (proc->priority == 1) {
           push(proc, AdminQueue);
           pop(q);
          
           i--;
        }
    }
    if (AdminQueue->front != -1) {
        printf("The queue contain admin priority processes\n"); 
        return AdminQueue;
    }else{
        printf("The queue does not contain admin priority processes\n");
        free(AdminQueue);
        return NULL;
    }
}