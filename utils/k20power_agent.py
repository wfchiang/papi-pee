#!/usr/bin/env python

import os
import sys
import subprocess as subp 

K20POWER = "/home/wfchiang/k20power/K20Power"


# ========
# sub-routines
# ========



# ========
# main
# ========

# ---- pre-condition ----
assert(len(sys.argv) >= 4)
assert(os.path.isfile(K20POWER))


# ---- prepare to run K20Power ----
N_REPEATS = int(sys.argv[1])
MARGIN = int(sys.argv[2])
assert(2*MARGIN < N_REPEATS) 
    
os.system("rm K20Power_*.trace")

print ("N_REPEATS: " + str(N_REPEATS))
print ("MARGIN: " + str(MARGIN)) 


# ---- run k20power ---- 
for r in range(0, N_REPEATS):
    print ("==== k20power test " + str(r) + " ====") 
    
    str_command = K20POWER
    for a in range(3, len(sys.argv)):
        str_command = str_command + " " + sys.argv[a] 

    os.system(str_command)


# ---- parse outputs ---- 
onames = []

stdout_ls = subp.Popen("ls K20Power_*.trace", shell=True, stdout=subp.PIPE).stdout

for aline in stdout_ls:
    aline = aline.strip()

    if (aline == ""):
        continue

    onames.append(aline)

assert(len(onames) == N_REPEATS)

TIMES = []
ENERGYS = [] 

for oname in onames:
    active_time = None
    active_energy = None

    ofile = open(oname, "r")

    for aline in ofile:
        aline = aline.strip()

        if (aline.endswith("#active time [s]")):
            active_time = float(aline[0:len(aline)-16]) 

        elif (aline.endswith("#active energy [J]")):
            active_energy = float(aline[0:len(aline)-18]) 

        else:
            continue 

    ofile.close()

    assert(active_time is not None)
    assert(active_energy is not None)

    TIMES.append(active_time)
    ENERGYS.append(active_energy)

assert(len(TIMES) == N_REPEATS)
assert(len(ENERGYS) == N_REPEATS)


# ---- report ----
TIMES.sort()
ENERGYS.sort()

TIMES = TIMES[ MARGIN : N_REPEATS-MARGIN ]
ENERGYS = ENERGYS[ MARGIN : N_REPEATS-MARGIN ]

print ("Average Time: " + str( sum(TIMES) / float(len(TIMES)) ))
print ("Average Energy: " + str( sum(ENERGYS) / float(len(ENERGYS)) ))

