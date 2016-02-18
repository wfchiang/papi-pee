
#include "rapl_utils.h" 

void run_test () {
  sleep(1); 
  /*
  int i;
  const int l_arr = 1000000; 
  float arr[l_arr];
  // double arr[l_arr]; 

  for (i = 0 ; i < l_arr ; i++) {
    arr[i] = 0.0000001 * i;
  }

  for (i = 1 ; i < l_arr ; i++) {
    arr[0] += arr[i];
  }
  */
}

int main (int argc, char **argv)
{
  // prepare RAPL 
  // prepareRAPL(argc, argv); 
  
  /* Start Counting */
  // long long before_time = StartRAPLCount(); 
  
  /* Run test */
  run_test(); 
  
  /* Stop Counting */
  // long long after_time = StopRAPLCount(); 
  
  // report
  // ReportRAPLCount(before_time, after_time);

  // get event value
  /*
  char *unit = NULL;
  long long evalue = ReadRAPLEvent("PACKAGE_ENERGY", &unit);
  assert(unit != NULL); 
  printf("PACKAGE_ENERGY: %.lld %s\n", evalue, unit); 
  */
  
  /* Done, clean up */
  // CleanupRAPL(); 
  
  return 0;
}

