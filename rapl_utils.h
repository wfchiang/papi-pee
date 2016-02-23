
#include <stdio.h>
#include <stdlib.h>
#include "papi.h"
#include <unistd.h> 
#include <assert.h>
#include <string.h>


#define MAX_RAPL_EVENTS 64

// #define VERB_RAPL_UTILS

long long RAPL_BEFORE_TIME, RAPL_AFTER_TIME; 

void prepareRAPL (); 

long long
StartRAPLCount (); 

long long
StopRAPLCount (); 

void ReportRAPLCount (); 

long long
ReadRAPLEvent (char *ename, char **unit); 

void CleanupRAPL (); 
