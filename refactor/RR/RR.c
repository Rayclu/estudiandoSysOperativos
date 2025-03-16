#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "queue.h"

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

int AddProcToCIcle(SystemQueue*Array , SystmProcess*Process){
    /*
    *Add new process to th Array
    */
    int error = 0;//Boolean value, verify if the Array are initialized
    int index = Array->rear;
    index == -1 ? initialize(Array) : 0 ; 
    
    if(LimitProc(Array) == 0){
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

}