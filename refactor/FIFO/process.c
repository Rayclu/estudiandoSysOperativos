#include "process.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

// Función para crear un nuevo proceso
SystmProcess* create_process(const char *name, int priority) {
    SystmProcess *process = malloc(sizeof(SystmProcess)); // Asignar memoria para el proceso
    if (!process) return NULL; // Verificar si la asignación fue exitosa

    // Copiar el nombre de la aplicación
    strncpy(process->name, name, sizeof(process->name) - 1);
    process->name[sizeof(process->name) - 1] = '\0'; // Asegurar que la cadena esté terminada
    process->priority = priority; // Asignar prioridad
    process->arrival_t = (int)time(NULL); // Registrar tiempo de llegada
    process->pointer = NULL; // Inicializar puntero
    process->end_time = 0; // Inicializar tiempo de finalización
    process->start_time = 0; // Inicializar tiempo de inicio
    process->answer_t = 0; // Inicializar tiempo de respuesta
    process->waiting_t = 0; // Inicializar tiempo de espera
    process->execution_time = 0; // Inicializar tiempo de ejecución
    process->Resourse_usage = 0; // Inicializar uso de recursos
    if (priority == 1) {
        process->priority = 1;
        printf("App added as root\n");
    }else {
        process->priority = 0;
        printf("App added as normal user\n");
    }   
    return process; // Retornar el proceso creado
}

// Función para liberar la memoria de un proceso
void free_process(SystmProcess *process) {
    if (process) {
        free(process->pointer); // Liberar memoria del puntero
        free(process); // Liberar memoria del proceso
    }
}

//void GetStats(SystmProcess*SProcess){
    /*
    **Function for measuring the stats of the process,
    */


//}
