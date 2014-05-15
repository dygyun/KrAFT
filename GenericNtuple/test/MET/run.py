#!/usr/bin/env python
import sys,os

Sample = ["ntuple_pu25","ntuple_pu45","ntuple_DYToMuMu_pu25"]
#Sample = ["ntuple_DYToMuMu_pu25"]

sSample = ["MuMu","ElEl","MuEl"]

for i in range(len(Sample)) :
    cmd = "root -b -q run.C\\(\\\"%s\\\""% Sample[i]
#    cmd = cmd+",\\\"%s\\\""% sSample[i]
#    cmd = cmd+",%f"% CX[i]
#    cmd = cmd+",%d"% isZ[i]
#    cmd = cmd+",%d"% v[i]
    cmd = cmd+",\\\"MuMu\\\"\\)"
    print cmd
    os.system(cmd)

#for i in range(len(Sample)) :
#    cmd = "root -b -q run.C\\(\\\"%s\\\""% Sample[i]
#   cmd = cmd+",\\\"%s\\\""% sSample[i]
#    cmd = cmd+",\\\"ElEl\\\"\\)"
#    print cmd
#    os.system(cmd)

#for i in range(len(Sample)) :
#    cmd = "root -b -q run.C\\(\\\"%s\\\""% Sample[i]
 #   cmd = cmd+",\\\"%s\\\""% sSample[i]
#    cmd = cmd+",\\\"MuEl\\\"\\)"
#    print cmd
#    os.system(cmd)

