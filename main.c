#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "process.h"
#include "queue.h"
#include "simulator.h"

// Función para obtener el directorio de una aplicación
int GetDir(const char *app_name, char *path, size_t size) {
    // Lista de posibles directorios donde buscar la aplicación
    const char *paths[] = {"~/","~/Downloads", "~/Desktop", "~/Documents", "~/", "/snap/bin", "/usr/local/bin", "/usr/bin", "/bin", "/opt", NULL};
    struct dirent *entry;
    DIR *dir;

    // Iterar sobre cada directorio en la lista
    for (int i = 0; paths[i] != NULL; i++) {
        dir = opendir(paths[i]);
        if (dir != NULL) {
            // Leer entradas en el directorio
            while ((entry = readdir(dir)) != NULL) {
                // Comparar el nombre de la entrada con el nombre de la aplicación
                if (strcmp(entry->d_name, app_name) == 0) {
                    snprintf(path, size, "%s/%s", paths[i], entry->d_name);
                    closedir(dir);
                    return 0; // Directorio encontrado
                }
                
            }    
        }
        closedir(dir);
    }
    for (int i = 0; paths[i] != NULL; i++) {
        dir = opendir(paths[i]);
        if (dir != NULL) {
            while ((entry = readdir(dir)) != NULL) {
                if (strcmp(entry->d_name, app_name) == 0) {
                    snprintf(path, size, "%s/%s", paths[i], entry->d_name);
                    closedir(dir);
                    return 0;
                }
            }
            closedir(dir);
        }
    }
    return -1; // Directorio no encontrado
}

// Función para obtener un proceso del usuario
int GetProcess(SystemQueue *Queue) {
    char name[128]; // Nombre de la aplicación
    int priority; // Prioridad del proceso
    char PathApp[250]; // Ruta de la aplicación

    printf("What app do you want to open? ");
    if (scanf("%127s", name) == -1) return -1; // Leer nombre de la aplicación
    
    printf("Assign priority (0 for normal user, 1 for root): ");
    scanf("%d", &priority); // Leer prioridad
    // Crear un nuevo proceso
    SystmProcess *process = create_process(name, priority);
    if (!process) return -1; // Verificar si el proceso fue creado correctamente

    // Obtener el directorio de la aplicación
    if (GetDir(process->name, PathApp, sizeof(PathApp)) == 0) {
        process->pointer = strdup(PathApp); // Duplicar la ruta
        if (process->pointer == NULL) {
            free_process(process); // Liberar memoria si falla
            return -1;
        }
        printf("Path found: %s\n", process->pointer);

        // Agregar el proceso a la cola
        if (push(process, Queue) != 0) {
            free_process(process);
            printf("Error adding process to queue.\n");
            return -1;
        }
        printf("Process added to queue.\n");
        return 0; // Proceso agregado exitosamente
    } else {
        printf("Application not found.\n");
        free_process(process);
        return GetProcess(Queue); // Intentar nuevamente si no se encuentra la aplicación
    }
}

// Nueva función auxiliar para convertir segundos a formato HH:MM:SS
void formatTime(int seconds, char *buffer) {
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int secs = seconds % 60;
    sprintf(buffer, "%02d:%02d:%02d", hours, minutes, secs);
}
void PrintUsage(SystmProcess *process){
    //struct rusage usage;
    //getrusage(RUSAGE_SELF, &usage);
    //printf("Memory Usage: %ld bytes\n", usage.ru_maxrss);

}

SystemQueue* AdminExecution(SystemQueue *Queue){
    SystemQueue *AdminQueue = malloc(sizeof(SystemQueue));
    if (!AdminQueue) return NULL;
    initialize(&AdminQueue); 

    if (Queue->front == -1 || Queue->front > Queue->rear) return NULL;
        
    if (Queue->data[Queue->front]->priority == 0 ){
        
        Queue->front++;
        return AdminExecution(Queue);

    } else if (Queue->data[Queue->front]->priority == 1){
    
        push(Queue->data[Queue->front], AdminQueue);
        Queue->front++;
        return AdminExecution(Queue);
    
    }else{

        printf("Error: Invalid process priority.\n");
        return NULL;

    }
    return AdminQueue;
   /*
    if (AdminQueue->front == -1 || AdminQueue->front > AdminQueue->rear){
        printf("Error: AdminQueue is empty.\n");
        return NULL;
    }else if (AdminQueue->front != -1 && AdminQueue->front <= AdminQueue->rear){
        
        Simulate_execution(AdminQueue->data[AdminQueue->front], AdminQueue);
    }
    */   
}


// Simula la ejecución de un proceso
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
    
    // Corregir cálculos de tiempos
    process->execution_time = process->end_time - process->start_time;
    process->waiting_t = process->start_time - process->arrival_t;
    process->answer_t = process->end_time - process->arrival_t;

    //Calculate averages
    if (averages->numb_of_processes != 0){    averages->waiting_t += process->waiting_t;
        averages->turnaround_t += process->answer_t;
        averages->response_t += process->execution_time;
        
        formatTime(averages->waiting_t=averages->waiting_t/averages->numb_of_processes, timeBuffer);
        formatTime(averages->turnaround_t=averages->turnaround_t/averages->numb_of_processes, timeBuffer);
        formatTime(averages->response_t=averages->response_t/averages->numb_of_processes, timeBuffer);
        printf("Averages:\n");
        printf("Waiting Time: %f\n", averages->waiting_t);
        printf("Turnaround Time: %f\n", averages->turnaround_t);
        printf("Response Time: %f\n", averages->response_t);
        }

    //Format timers
    formatTime(process->answer_t, timeBuffer);
    printf("-> Answer Time: %s (%d seconds)\n", timeBuffer, process->answer_t);

    
    formatTime(process->waiting_t, timeBuffer);
    printf("-> Waiting Time: %s (%d seconds)\n", timeBuffer, process->waiting_t);
    
    formatTime(process->execution_time, timeBuffer);
    printf("-> Execution Time: %s (%d seconds)\n", timeBuffer, process->execution_time);
    
    printf("-> Path: %s\n", process->pointer);
    printf("\n___________________________________________________________");

   
}

// Muestra los procesos en la cola
void displayQueue(SystemQueue *q) {
    if (q->front == -1 || q->front > q->rear) {
        printf("Queue is empty.\n");
    } else {
        printf("Processes in queue:\n");
        for (int i = q->front; i <= q->rear; i++) {
            SystmProcess *proc = q->data[i];
            printf("-> %s (Path: %s)\n", proc->name, proc->pointer);
        }
    }
}
SystmProcess* pop(SystemQueue*Queue){
    if (Queue->front == -1 || Queue->front > Queue->rear) return NULL;
    SystmProcess *proc = Queue->data[Queue->front];
    Queue->front++;

    if (Queue->front>Queue->rear)
    {
        Queue->front = Queue->rear = -1;
    }
    return proc;         
}

// Mejorar el menú principal
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

int main() {
    SystemQueue Sq; // Cola de procesos
    SystemQueue AdminQueue; // Cola de procesos con prioridad 1
    Averages averages = {0}; // Estructura para promedios
    SystemStats stats = {0}; // Estructura para estadísticas del sistema
    int option; // Opción del menú
    
    initialize(&Sq); // Inicializar la cola
    initialize(&AdminQueue); // Inicializar la cola de administración
    stats.start_time = time(NULL); // Registrar el tiempo de inicio
    
    do {
        showMenu(); // Mostrar menú
        scanf("%d", &option); // Leer opción del usuario
        int admin_execution;

        switch(option) {
            case 1:
                // Agregar un nuevo proceso

                if (GetProcess(&Sq) == 0) {
                //int add_process = GetProcess(&Sq);
                //printf("add_process: %d\n", add_process);
                //if (add_process == 0) {
                    stats.total_processes++; // Incrementar total de procesos
                    printf("Process added successfully.\n");
                } else {
                    printf("Error adding process to queue.\n");
                }
                break;
            case 2:
                displayQueue(&Sq); // Mostrar cola de procesos
                break;
           
            case 3:
                // Ejecutar un proceso
                SystemQueue* AdminQueueResult = HaveThereAdminProcess(&Sq);
                if(AdminQueueResult) {
                    printf("|______________________________________\n");
                    printf("|->Processes with Admin priority in queue:\n");
                        for (int i = AdminQueueResult->front; i <= AdminQueueResult->rear; i++) {
                            Simulate_execution(AdminQueueResult->data[i], &averages);
                            stats.completed_processes++;
                            free_process(AdminQueueResult->data[i]);
                        }                    
                    free(AdminQueueResult);
                    printf("|______________________________________\n");
                }
                
                SystmProcess *proc = poppet(&Sq, &Sq.front);
                if (proc != NULL) {
                    Simulate_execution(proc, &averages); // Simular ejecución
                    stats.completed_processes++; // Incrementar procesos completados
                    free_process(proc); // Liberar memoria del proceso
                }
                break;
            case 4:
                // Mostrar estadísticas del sistema
                printf("\n System Statistics:\n");
                printf("\n Total processes: %d\n", stats.total_processes);
                printf("\n Completed processes: %d\n", stats.completed_processes);
                printf("\n Start time: %s\n", ctime(&stats.start_time));
                printf("\n End time: %s\n", ctime(&stats.end_time));
                
                // Mostrar promedios
                printf("+-------------------------#Averages#-------------------------+\n");
                printf("|  1. Waiting Time: %f                                       |\n", averages.waiting_t);
                printf("|  2. Turnaround Time: %f                                    |\n", averages.turnaround_t);
                printf("|  3. Response Time: %f                                      |\n", averages.response_t);
                printf("|  4. Execution Time: %f                                     |\n", averages.execution_t);
                printf("|  5. Number of processes: %d                                |\n", averages.numb_of_processes);
                printf("+-------------------------#Averages#-------------------------+\n");
                printf("\n");
                
                break;
            case 5:
                printf("Exiting...\n"); // Salir del programa
                break;
            default: 
                printf("Invalid option\n"); // Opción inválida
                return -1;
        }
    } while(option != 5);

    // Liberar memoria de procesos restantes en la cola
    while (Sq.front != -1 && Sq.front <= Sq.rear) {
        SystmProcess *proc = poppet(&Sq, &Sq.front);
        free_process(proc);
    }
    
    return 0; // Fin del programa
    while (Sq.front != -1 && Sq.front <= Sq.rear){
        SystmProcess *proc = pop(&Sq);
        free(proc->pointer);
        free(proc);
    }
    
    return 0;
}