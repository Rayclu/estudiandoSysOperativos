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

SystmProcess* poppet(SystemQueue *Queue, int *index) {
    if (index == NULL || Queue->front == -1 || Queue->front > Queue->rear) {
        return NULL; // Verificar si el índice es válido y la cola no está vacía
    }

    SystemQueue beforeIndex;
    SystemQueue afterIndex;
    initialize(&beforeIndex);
    initialize(&afterIndex);

    // Separar los procesos en dos colas
    for (int i = Queue->front; i <= Queue->rear; i++) {
        if (i < *index) {
            push(Queue->data[i], &beforeIndex); // Procesos antes del índice
        } else if (i > *index) {
            push(Queue->data[i], &afterIndex); // Procesos después del índice
        }
    }
    
    // Reiniciar la cola original
    Queue->front = 0;
    Queue->rear = -1;

    // Agregar procesos de beforeIndex a la cola original
    for (int i = beforeIndex.front; i <= beforeIndex.rear; i++) {
        push(beforeIndex.data[i], Queue);
    }

    // Agregar procesos de afterIndex a la cola original
    for (int i = afterIndex.front; i <= afterIndex.rear; i++) {
        push(afterIndex.data[i], Queue);
    }

    // Retornar el proceso que estaba en el índice especificado
    SystmProcess *proc = Queue->data[*index];

    // Mover el índice frontal hacia adelante
    Queue->front++;

    // Verificar si la cola está vacía después de la operación
    if (Queue->front > Queue->rear) {
        Queue->front = Queue->rear = -1; // Reiniciar la cola si está vacía
    }
    
    return proc; // Retornar el proceso extraído
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
    if (AdminQueue == NULL) {
        printf("Memory allocation failed for AdminQueue.\n");
        return NULL; // Manejo de error en la asignación de memoria
    }
    initialize(AdminQueue);

    // Usar un índice separado para iterar sobre la cola
    for (int i = q->front; i <= q->rear; i++) {
        SystmProcess *proc = q->data[i];
        if (proc->priority == 1) {
            push(proc, AdminQueue); // Agregar a la cola de administradores
            // Eliminar el proceso de la cola original
            // Aquí se debe usar pop para eliminar el proceso de la cola original
            poppet(q, &i); // Esto eliminará el proceso de la cola original
            i--; // Decrementar el índice para evitar saltar el siguiente proceso
        }
    }

    // Verificar si hay procesos en la cola de administradores
    if (AdminQueue->front != -1) {
        printf("The queue contains admin priority processes\n"); 
        return AdminQueue;
    } else {
        printf("The queue does not contain admin priority processes\n");
        free(AdminQueue); // Liberar memoria si no hay procesos
        return NULL;
    }
}