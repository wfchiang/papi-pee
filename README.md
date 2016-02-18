
# papi-pee
PAPI based Process level Energy Estimator
---- 



## Requirment

You must have CPUs which use Intel Sandy Bridge microarchitecture or newer. 

PAPI-PEE requires the following tools/libraries.

- MSR driver
    * You may install it through commands **sudo apt-get install msr-tools** and then **modprobe msr**.
    * The script **install.py** would do a "best-effort" installation of the MSR driver. 
    
- libmsr https://github.com/LLNL/libmsr

- PAPI http://icl.cs.utk.edu/papi/
    * Please be sure that PAPI is built with components **libmsr** and **rapl**. 

