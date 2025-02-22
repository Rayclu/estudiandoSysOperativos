#ifndef QUEUE_H
#define QUEUE_H

#include "process.h"

#define MAX 20

typedef struct {
    SystmProcess *data[MAX];
    int front;
    int rear;
} SystemQueue;

void initialize(SystemQueue *q);
int push(SystmProcess *proc, SystemQueue *Queue);
SystmProcess* poppet(SystemQueue *Queue, int *index);
void displayQueue(SystemQueue *q);
SystemQueue* HaveThereAdminProcess(SystemQueue *q);

#endif // QUEUE_H