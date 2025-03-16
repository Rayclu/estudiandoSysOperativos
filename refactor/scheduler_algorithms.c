#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FIFO.h"

typedef struct{
    char *name;
    char *Description;
    int  IdAlgorithm;
    

}Algorithm;


void Create(){
    Algorithm *newScenario = malloc(sizeof(Algorithm));
    
    //List potential names
    const char *names[] = {
        "FIFO",
        "Round Robin",
        "Priority Scheduling",
        "Shortest Job First",
        "Multilevel Queue"
    };

    //List potential descriptions
    const char *descriptions[] = {
        "FIFO:\nThe First process that enters the queue is the first to leave this",
        "Round Robin:\n Each process is assigned a fixed time in cyclic order"
    };
    char *name_entered_for_user;
    scanf("Please select an algorithm to continue with the simulation of the process managment:\n\t%s",
    name_entered_for_user);
    
}
