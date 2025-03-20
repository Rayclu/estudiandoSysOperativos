#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "queue.h"
#include "simulator.h"

//#define LimitOfProcessIntoTheArray = 5
/*
** 
**
*/


void CreateArray(SystemQueue*Array){
    initialize(&Array);
}

int LimitProc(SystemQueue*Array){
    /*
    *
    **This Function verify if the Aray don't have empty slots. In that case alert 
    *to AddPRoc for this function overwrite the spaces of the Array.
    * 
    */
    int LIMIT = 5;
    if(Array->rear >= LIMIT) return -1;
    return 0;
}

int AddProcToCicle(SystemQueue*Array , SystmProcess*Process){
    /*
    *Add new process to th Array
    */
    int error = 0;//Boolean value, verify if the Array are initialized
    int index = Array->rear;
    index == -1 ? initialize(Array) : NULL ; 
    
    if(LimitProc(&Array) == 0){
        Array->rear+=1;
        Array->data[Array->front] = Process;
        return 0;//Success
    }


    return -1; //Error Adding the process



}
int main(){
    SystemQueue *CircularArray;
    SystmProcess *Process;

    initialize(&CircularArray);
    showMenu();
    int option;
    do{
        switch (option){
            case 1:
                // Add new process
                break;
            case 2:
                // Show list
                
                break;
            case 3:
                // Execute simulation
                break;
            case 4:
                // Show statistics
                
                break;
            case 5:
                // exit.
                printf("Exiting...");
                break;
            default:
                printf("Error: Invalid option, select an option again: \n\t");
                scanf("%127s", &option);
                break;
       }
    } while (option != 5);
    
    

}