
#include "rapl_utils.h"


int main (int argc, char **argv)
{
  int i;
  assert(argc == 2);

  // prepare RAPL 
  prepareRAPL(); 
  
  /* Start Counting */
  long long before_time = StartRAPLCount(); 
  
  /* Run test */
  system(argv[1]); 
  
  /* Stop Counting */
  long long after_time = StopRAPLCount(); 
  
  // report
  ReportRAPLCount(before_time, after_time);

  // get event value
  char *unit = NULL;
  long long evalue = ReadRAPLEvent("PACKAGE_ENERGY", &unit);
  assert(strstr(unit, "nJ")); 
  printf("PACKAGE_ENERGY: %.10f %s\n", (double)(evalue/1e9), "J");
  printf("TIME: %lld\n", (after_time - before_time)); 

  /* Done, clean up */
  CleanupRAPL();
  
  return 0;
}
