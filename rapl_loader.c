
#include "rapl_utils.h"

int main (int argc, char **argv)
{
  int i;
  int l_command = 0; 
  assert(argc >= 2);

  for (i = 1 ; i < argc ; i++) {
    l_command = l_command + 1 + strlen(argv[i]); 
  }
  char *str_command = (char *)malloc(sizeof(l_command));
  memset(str_command, 0, sizeof(l_command));

  int lc = 0; 
  for (i = 1 ; i < argc ; i++) {
    sprintf(&str_command[lc], "%s ", argv[i]); 
    lc = lc + 1 + strlen(argv[i]); 
  }

  str_command[l_command-1] = '\0'; 

  // prepare RAPL 
  prepareRAPL(argc, argv); 
  
  /* Start Counting */
  long long before_time = StartRAPLCount(); 
  
  /* Run test */
  system(str_command); 
  
  /* Stop Counting */
  long long after_time = StopRAPLCount(); 
  
  // report
  // ReportRAPLCount(before_time, after_time);

  // get event value
  char *unit = NULL;
  long long evalue = ReadRAPLEvent("PACKAGE_ENERGY", &unit);
  assert(strstr(unit, "nJ")); 
  printf("PACKAGE_ENERGY: %.10f %s\n", (double)(evalue/1e9), "J"); 
  
  /* Done, clean up */
  CleanupRAPL(); 
  
  return 0;
}
