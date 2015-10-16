NTupleProduction
================

## Brief Description
Software for nTuples production from MiniAOD files for ttbar+X differential cross section analysis.
The master branch corresponds to the Run 2 analysis path. 
For the Run 1 analysis path, please refer to branch 'run1' as well as the relevant releases.

## General Recipe

To setup the code:

```
#set GITHUBUSERNAME if not previously set
export GITHUBUSERNAME=yourGITHUBUsername
git config --global user.github $GITHUBUSERNAME
#on soolin:
export CMSSW_GIT_REFERENCE=/storage/.cmsgit-cache


# Set up the CMSSW release
export SCRAM_ARCH=slc6_amd64_gcc491

cmsrel CMSSW_7_4_10_patch1
cd CMSSW_7_4_10_patch1/src/
cmsenv
git cms-init
# Do merge-topics and addpkgs first if needed
git cms-merge-topic -u cms-met:METCorUnc74X

# Clone our main ntuple producing software and checkout run2 branch
git clone git@github.com:BristolTopGroup/NTupleProduction.git BristolAnalysis/NTupleTools
cd BristolAnalysis/NTupleTools
# optional for development
git remote rename origin upstream
git remote add origin git@github.com:<Your Git name with forked repo>/NTupleProduction.git
git fetch --all
# for ntuple production once a tag is available
git checkout CMSSW_7_4_10_patch1_v2
cd ../../

# Clone our version of the TopSkimming software and checkout run2 branch
git clone git@github.com:BristolTopGroup/TopSkimming.git TopQuarkAnalysis/TopSkimming
cd TopQuarkAnalysis/TopSkimming
git remote rename origin upstream
cd ../../

# Compile
scramv1 b -j 8

#test release
### Not yet available
```

## Notes
More information can be found at [Bristol Ntuple Recipes twiki page](https://twiki.cern.ch/twiki/bin/view/CMS/BristolNTuplerRecipes), although it's outdated.

## Bugs
Please report any problems on our [issues page](https://github.com/BristolTopGroup/NTupleProduction/issues).
