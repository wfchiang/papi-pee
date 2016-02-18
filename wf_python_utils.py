
import os
import subprocess as subp 

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
