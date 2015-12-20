This directory has files for Aritificial Neural Network.

To get final output(in Input-Output/ann):
````
    ./build.sh && ./run.sh
````
FILE DESCRIPTION
getData.cpp: gets data from SQL

normalizeCSV.py: normalizes the output from getData.cpp

denormalizeCSV.py: denormalizes the output of java project

src: source code for java project for Aritificial Neural Network

build.xml: this file is used by Apache Ant to build java project from source from command line

./build.sh: this builds getData.cpp and java files in ./src

./run: this runs the executables to produce final output

jars: contains dependency jar for java ann

minmaxFile: generated from normalizeCSV.py and contains min, max, sd, etc of the data

.project: hidden file which helps to get this dir imported in eclipse

Steps to import this in eclipse IDE as a project
1. Click File>Import>General>Existing Projects into Workspace>Next
2. Click on Browse button next to "Select root directory:" and select this dir: Visibility-forecast-at-IGI-airport/ann/
3. The project will get listed under "Projects:". Click "Finish" button
4. Now "Ann" will get listed in eclipse's "Project Explorer". Right Click on it >Build path> Configure Build Path>Java Build Path>Libraries>Add External jar. Select all the jars in Visibility-forecast-at-IGI-airport/ann/jars. Click Ok>Ok
5. Run the project by clicking on green button in toolbar. It may give some errors. ignore it. (This creates a run configuration.)
6. Now provide program arguments(if any). To do that, right Click project in "Project Explorer" >Build path> Configure Build Path>Run/Debug Settings. There will be one "Launch configurations for Ann" present. Click on it>Edit>Arguments. paste programs arguments in "Program arguments:" text box. Then click OK>OK

Now the project is ready to run.

7. build.xml has already been included so that we can build the java src from command line using "ant". If you want to create build.xml yourself from eclipse, Click File>Export>General>Ant Buildfiles>Next>Select All>Finish. build.xml will get created in the project dir.
