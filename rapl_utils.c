
#include "rapl_utils.h"

int EventSet = PAPI_NULL;
int N_EVENTS = 0; 
char EVENT_NAMES[MAX_RAPL_EVENTS][PAPI_MAX_STR_LEN];
char EVENT_UNITS[MAX_RAPL_EVENTS][PAPI_MIN_STR_LEN];
long long *EVENT_VALUES = NULL;


void prepareRAPL () { 

  const PAPI_component_info_t *cmpinfo = NULL;

  EventSet = PAPI_NULL;
  N_EVENTS = 0;
  EVENT_VALUES = NULL;

  int cid; 
  int retval = PAPI_NULL; 

  // PAPI Initialization 
  retval = PAPI_library_init( PAPI_VER_CURRENT );
  if ( retval != PAPI_VER_CURRENT ) {
    fprintf(stderr, "PAPI_library_init failed...\n");
    exit(1); 
  }

  // get the # of components and confirm the components
  int rapl_cid = -1; 
  int n_comps = PAPI_num_components();
  assert(n_comps > 0); 

  for(cid = 0 ; cid < n_comps ; cid++) {
    cmpinfo = PAPI_get_component_info(cid);
    
    if (cmpinfo == NULL) { 
      fprintf(stderr, "PAPI_get_component_info %d failed...\n", cid);
      exit(1); 
    }
#ifdef VERB_RAPL_UTILS
    else {
      fprintf(stdout, "Component %d name: %s\n", cid, cmpinfo->name); 
    }
#endif
          
    if (strstr(cmpinfo->name,"rapl")) {
      assert(!(cmpinfo->disabled)); 
#ifdef VERB_RAPL_UTILS
      printf("    : Found a RAPL component\n");
#endif 
      assert(rapl_cid == -1);
      rapl_cid = cid; 
    }
#ifdef VERB_RAPL_UTILS
    else {
      printf("    : Not a RAPL component\n"); 
    }
#endif 
  }

  // create event set
  retval = PAPI_create_eventset( &EventSet );
  if ( retval != PAPI_OK ) {
    fprintf(stderr, "PAPI_create_eventset failed...\n"); 
    exit(1); 
  }

  // Add all events 
  N_EVENTS = 0; 
  int code = PAPI_NATIVE_MASK;

  assert(rapl_cid != -1); 
 
  while ( 1 ) {
    if (N_EVENTS == 0)
      retval = PAPI_enum_cmp_event( &code, PAPI_ENUM_FIRST, rapl_cid );
    else
      retval = PAPI_enum_cmp_event( &code, PAPI_ENUM_EVENTS, rapl_cid ); 

    if (retval != PAPI_OK) break; 
    
    memset(&EVENT_NAMES[N_EVENTS], 0, PAPI_MAX_STR_LEN); 
    
    retval = PAPI_event_code_to_name( code, EVENT_NAMES[N_EVENTS]); 
    if ( retval != PAPI_OK ) {
      fprintf(stderr, "PAPI_event_code_to_name (%d) failed...\n", code);
      exit(1);
    }
#ifdef VERB_RAPL_UTILS
    else {
      fprintf(stdout, "PAPI_event_code_to_name (%d) : %s\n", code, EVENT_NAMES[N_EVENTS]); 
    }
#endif 

    PAPI_event_info_t evinfo;
    
    retval = PAPI_get_event_info(code,&evinfo);
    if ( retval != PAPI_OK ) {
      fprintf(stderr, "PAPI_get_event_info failed...\n");
      exit(1);
    }

    // check unit
    memset(EVENT_UNITS[N_EVENTS], 0, PAPI_MIN_STR_LEN); 
    strncpy(EVENT_UNITS[N_EVENTS], evinfo.units, PAPI_MIN_STR_LEN-1);

    // add event 
    retval = PAPI_add_event( EventSet, code );
    if ( retval != PAPI_OK ) {
      fprintf(stderr, "PAPI_add_event failed...\n");
      exit(1);
    }

    N_EVENTS++; 
  }

  assert(N_EVENTS > 0); 

  EVENT_VALUES = (long long*) calloc(N_EVENTS, sizeof(long long)); 
  assert(EVENT_VALUES != NULL);

}


long long
StartRAPLCount () {
  assert(EventSet != PAPI_NULL);
  
  RAPL_BEFORE_TIME = PAPI_get_real_nsec();
  int retval = PAPI_start( EventSet );
  if ( retval != PAPI_OK ) {
    fprintf(stderr, "PAPI_start failed...\n");
    exit(1);
  }
  return RAPL_BEFORE_TIME; 
}


long long
StopRAPLCount () {
  assert(EventSet != PAPI_NULL);
  assert(EVENT_VALUES != NULL);
  
  RAPL_AFTER_TIME = PAPI_get_real_nsec();
  int retval = PAPI_stop( EventSet, EVENT_VALUES ); 
  if ( retval != PAPI_OK ) {
    fprintf(stderr, "PAPI_stop failed...\n"); 
    exit(1);
  }
  return RAPL_AFTER_TIME; 
}


void ReportRAPLCount () {
  assert(RAPL_BEFORE_TIME > 0); 
  assert(RAPL_AFTER_TIME > RAPL_BEFORE_TIME); 
  assert(N_EVENTS > 0);
  assert(EVENT_VALUES != NULL); 

  int i; 
  double elapsed_time = ((double)(RAPL_AFTER_TIME - RAPL_BEFORE_TIME)) / 1.0e9;
  printf("Elapse time %.5f\n", elapsed_time);

  for(i = 0 ; i < N_EVENTS ; i++) {
    if (strstr(EVENT_UNITS[i], "nJ")) {
      printf("%-40s%lld %s\t(Average Power %.1fW)\n",
	     EVENT_NAMES[i], 
	     EVENT_VALUES[i],
	     EVENT_UNITS[i], 
	     ((double)EVENT_VALUES[i]/1.0e9)/elapsed_time);
    }
    else {
      printf("%-40s%lld %s\n", 
	     EVENT_NAMES[i], 
	     EVENT_VALUES[i],
	     EVENT_UNITS[i]); 
    }
  }  
}


long long
ReadRAPLEvent (char *ename, char **unit) {
  int i; 
  *unit = NULL;

  assert(EVENT_VALUES != NULL);

  char freg_ename[PAPI_MAX_STR_LEN];
  memset(freg_ename, 0, PAPI_MAX_STR_LEN);

  sprintf(freg_ename, ":::%s:", ename);

  long long evalue = -1; 

  for (i = 0 ; i < N_EVENTS ; i++) {
    if (strstr(EVENT_NAMES[i], freg_ename)) {
      assert(*unit == NULL);
      *unit = &(EVENT_UNITS[i][0]); 
      evalue = EVENT_VALUES[i]; 
    }
  }

  return evalue; 
}


void CleanupRAPL () {
  assert(EventSet != PAPI_NULL);
  
  int retval = PAPI_cleanup_eventset( EventSet );
  if ( retval != PAPI_OK ) {
    fprintf(stderr, "PAPI_cleanup_eventset failed...\n"); 
    exit(1); 
  }
    
  retval = PAPI_destroy_eventset( &EventSet );
  if ( retval != PAPI_OK ) {
    fprintf(stderr, "PAPI_destroy_eventset failed...\n");
    exit(1);
  }
}



