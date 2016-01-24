# A short Roadmap for NTupleProduction

## One rule: Keep it simple
There is no reason to re-invent the wheel or overcomplicate things. 
Readability & understandability first, functionality second and performance third.
For the production of NTuples this means: Find the most straight-forward way for a user to
1. Prepare the data: adding information or performing corrections, etc (like PF2PAT)
2. Select the data
	a. select the events: determine if events are of interest by applying N cuts on event contents
	b. select the event content to be saved
3. Provide a summary:
	a. the output file (data)
	b. meta-data (in JSON format): 
		- event content before step 1
		- event content after step 1
		- selection efficiency of 2a
		- event content after step 2b
		- how many events did we process per second? 

## 	Step 1	
Involves:
	- preparing all the event weight information
	- preparing (not measuring!) all the scale factors
	- applying all the corrections
All the data coming into the event here, should come from other modules (i.e. ProduceWeights).

## Step 2a
While the draft for creating selection is currently included in this project, it does not have to be. E.g. the selection could go into a 'eventselection' module that can be used in other places.

## Step 2b
This step should be the meat of the NTupleProduction code. Get data A and extract subset B. Do not change the data format (e.g. ROOT) since this is not the purpose of this project. This way one can easily construct their own project by using meta packages (e.g. http://conda.pydata.org/docs/building/meta-pkg.html)

## Step 3
It is very important to keep track of what happened. Examples include, but are not exclusive to
 - rereco of the same data: ntuple production should have an event record in order to compare
 - new nTuple files with different event content: ROOT files do not support `diff` but the summary file will be diff-able!
 - ntuple version or version tags of software used
 - General reporting, automated or not
For this step it is only important to be able to define meta data which will add up in metadata.json.
Furthermore, meta data should be mergable.

	
