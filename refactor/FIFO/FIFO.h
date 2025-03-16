#ifndef mainFIFO_h
#define mainFIFO_h

#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "queue.h"

int GetDir(const char *app_name, char *path, size_t size);
int GetProcess(SystemQueue *queue);
int mainFIFO();
#endif