#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "FIFO/process.h"
#include "FIFO/queue.h"
#include "FIFO/simulator.h"

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

int mainFIFO() {
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
}