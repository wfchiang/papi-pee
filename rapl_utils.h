
#include <stdio.h>
#include <stdlib.h>
// #include "papi_test.h"
#include "papi.h"
#include <unistd.h> 
#include <assert.h>
#include <string.h>


#define MAX_RAPL_EVENTS 64

// #define VERB_RAPL_UTILS

void prepareRAPL (int argc, char **argv); 

long long
StartRAPLCount (); 

long long
StopRAPLCount (); 

void ReportRAPLCount (long long before_time,
		      long long after_time);  

long long
ReadRAPLEvent (char *ename, char **unit); 

void CleanupRAPL (); 
