#!/usr/bin/env python

import os
import subprocess as subp 


# ========
# sub-routines
# ========
def BoolInput (mess):
    assert(type(mess) is str)

    while (True):
        sin = raw_input(mess).strip()

        if (sin in ["true", "True", "yes", "y", "Yes"]):
            return True
        
        if (sin in ["false", "False", "no", "n", "No"]):
            return False
        

def hasCommand (c):
    assert(type(c) is str)
    ret = subp.Popen("which "+c, shell=True, stdout=subp.PIPE).stdout.read()
    return (len(ret) > 0)


def GetCPUInfo ():
    if (not hasCommand("lscpu")):
        print ("WARNING: failed on checking CPU info...")
        print ("         Your system may not be able to run PAPI-PEE.")
        return []

    stdout_lscpu = subp.Popen("lscpu", shell=True, stdout=subp.PIPE, stderr=subp.PIPE).stdout

    cpuinfo = {} 

    for aline in stdout_lscpu:
        aline = aline.strip()

        if (aline == ""):
            continue

        if (aline.startswith("CPU(s):")):
            aline = aline[7:].strip() 
            cpuinfo["# cpus"] = int(aline)

        elif (aline.startswith("Model name:")):
            aline = aline[11:].strip()
            cpuinfo["model"] = aline

        else:
            continue

    return cpuinfo


def GetLibInfo (lname):
    assert(type(lname) is str)

    if (not lname.endswith(".so")):
        lname = lname + ".so" 
    
    if (not hasCommand("ldconfig")):
        print ("WARNING: \"ldconfig\" is not available in your system.")
        print ("         Cannot check library " + lname)
        return {}

    stdout_lib = subp.Popen("ldconfig -v | grep "+lname, shell=True, stdout=subp.PIPE).stdout

    libinfo = {} 

    for aline in stdout_lib:
        aline = aline.strip()

        if (aline == ""):
            continue

        if (aline.startswith(lname)):
            aline = aline[len(lname):].strip()

            if (aline.startswith("->")):
                aline = aline[2:].strip()

                if (("reference" in libinfo.keys()) and (libinfo["reference"] != aline)): 
                    print ("WARNING: inconsistant library (" + lname + ") reference versions...")
                else:
                    libinfo["reference"] = aline

    return libinfo


def isRAPLCapableMicroArchitecture (cpuinfo = {}):
    if ("model" not in cpuinfo.keys()):
        print ("WARNING: insufficient cpuinfo...")
        print ("         Not sure about the microarchitecture of your cpu(s)...")
        print ("         Your (intel) cpu's microarchitecture must not be older than Sandy Bridge.")
        return False

    capable_models = []
    capable_models.append("Intel(R) Core(TM) i7-3770K CPU")

    for m in capable_models:
        if (cpuinfo["model"].startswith(m)):
            return True

    print ("WARNING: not sure about your cpu's microarchitecture...")
    print ("         Your (intel) cpu's microarchitecture must not be older than Sandy Bridge.")
    return False


def CheckMSR (cpuinfo = {}):
    assert("# cpus" in cpuinfo.keys())
    assert(type(cpuinfo["# cpus"]) is int)

    good_msr = True 

    for cid in range(0, cpuinfo["# cpus"]):
        if (not os.path.exists("/dev/cpu/"+str(cid)+"/msr")):
            good_msr = False 

    if (not good_msr):
        print ("WARNING: no proper MSR found.")
        print ("         Try my best to fix now...")

        os.system("sudo apt-get autoremove")
        os.system("sudo apt-get udpate") 
        os.system("sudo apt-get install msr-tools")

        if (hasCommand("modprobe")): 
            os.system("sudo modprobe msr")

    for cid in range(0, cpuinfo["# cpus"]):
        if (not os.path.exists("/dev/cpu/"+str(cid)+"/msr")):
            print ("Error: filed on installing MSR...")
            exit(1)

    if (BoolInput("Set read permission to MSR? [y/n] ")): 
        os.system("sudo chmod 666 /dev/cpu/*/msr")
        
        

# ========
# main
# ========

# ---- check CPU ---- 
CPUINFO = GetCPUInfo()

if (not isRAPLCapableMicroArchitecture(CPUINFO)):
    print ("WARNING: uncertain on your cpu microarchitecture.")
    print ("         PAPI-PEEE may not run properly.")
    
    if (BoolInput("continue? [y/n] ")):
        pass
    else:
        exit(0)

        
# ---- check msr ----
if ("# cpus" not in CPUINFO.keys()):
    print ("WARNING: not sure about the # of cpu(s) you have.")
    print ("         Assume one.")
    
    CPUINFO["# cpus"] = 1 

CheckMSR(CPUINFO)





