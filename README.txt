This folder contains the Radar-Augmented MANPADS Engagement DEVS model implemented in Cadmium

/**************************/
/****FILES ORGANIZATION****/
/**************************/

makefile
Radar_Augmented_MANPADS_Engagement_MODEL.docx
Radar_MANPADS_Engagement.xml
README.txt	

atomics [This folder contains atomic models implemented in Cadmium]
				\atomics\aircraft.hpp
				\atomics\gunner.hpp
				\atomics\missile.hpp
				\atomics\radar.hpp

bin [This folder will be created automatically the first time you compile the project. It will contain the executables and the required JSON file for the DEVS Web Viewer.]

build [This folder will be created automatically the first time you compile the project. It will contain the build files (.o) generated during compilation.]

data_structures [This folder contains message data structure used in the model.]
				\data_structures\message.cpp
				\data_structures\message.hpp

input_data [This folder contains the input data to run the tests and experiments.]
				\input_data\ads_input_test.txt
				\input_data\aircraft_input_test.txt
				\input_data\aircraft_input_test_t2.txt
				\input_data\engagement_parameters.txt
				\input_data\engagement_parameters_experiment1.txt
				\input_data\engagement_parameters_experiment2.txt
				\input_data\gunner_detected_position_input_test.txt
				\input_data\gunner_missile_status_input_test.txt
				\input_data\gunner_missile_status_input_test_t2.txt
				\input_data\gunner_missile_status_input_test_t3.txt
				\input_data\gunner_missile_status_input_test_t4.txt
				\input_data\missile_input_test.txt
				\input_data\radar_input_test.txt
	
model [This folder contains the Radar Augmented MANPADS Engagement top model]
				\model\radar_MANPADS_engagement.cpp

simulation_results [This folder will be created automatically the first time you compile the project. It will store the outputs from your simulation and tests.]

test [This folder contains the atomic and coupled tests.]
				\test\ads_test.cpp
				\test\aircraft_test.cpp
				\test\gunner_test.cpp
				\test\missile_test.cpp
				\test\radar_test.cpp
	
urslib [This folder contains a function allowing the user to change the default mission scenario values]  
				\usrlib\engagementUtils.cpp
				\usrlib\engagementUtils.hpp

viewer [This folder contains the required files to use the DEVS Web Viewer]
				\viewer\diagram.svg
				\viewer\messages.log
				\viewer\structure.json

/*************/
/****STEPS****/
/*************/

0 - The document "Radar_Augmented_MANPADS_Engagement_MODEL.docx" contains the description of this model.

1 - Update include path in the makefile in this folder and subfolders. You need to update the following lines:

				INCLUDECADMIUM=-I ../../cadmium/include
				INCLUDEDESTIMES=-I ../../DESTimes/include
				INCLUDEJSON=-I ../../CadmiumModelJSONExporter/include
				Update the relative path to cadmium/include from the folder where the makefile is. You need to take into account where you copied the folder during the installation process
				Example: INCLUDECADMIUM=-I ../../cadmium/include
				Do the same for the DESTimes and CadmiumModelJSONExporter libraries.
	
2 - Compile the project and the tests

	2.1 - Open the terminal (Ubuntu terminal for Linux and Cygwin for Windows) in the "Radar_MANPADS_Engagement" main folder/
	2.2 - To compile the project and the tests, type in the terminal:
				make clean; make all

3 - Run tests

	3.1 - The test executables in the bin directory are:
        	    a. ADS_TEST.exe
        	    b. AIRCRAFT_TEST.exe
        	    c. GUNNER_TEST.exe
        	    d. MISSILE_TEST.exe
        	    e. RADAR_TEST.exe
	
	3.2 - Open the terminal in the bin folder.
	
	3.3 - To run an individual test, type in the terminal "./NAME_OF_THE_COMPILED_FILE" (For windows, "./NAME_OF_THE_COMPILED_FILE.exe").
	For example, if you wish to test the aircraft atomic model, you would type the following:
                ./AIRCRAFT_TEST (or ./AIRCRAFT_TEST.exe for Windows)
	
	3.4 - Each test has an optional ability to specify an options file as the first command line parameter. This allows the user to  change the parameters of the mission scenario.
				./AIRCRAFT_TEST ../input_data/engagement_parameters_default.txt
    
	3.5 - To check the output results of the test, go to the folder simulation_results and open the appropriate output_messages and output_state files.
				For example, if you run AIRCRAFT_TEST, the files containing the results of this test will be "aircraft_test_output_messages.txt" and "aircraft_test_output_state.txt".
    
	3.6 - The input data to run the individual tests are found in the input_data folder. The tests are set to run the input data file for test case #1 explained in the "Radar_Augmented_MANPADS_Engagement_MODEL.docx" document. The input data for the other test cases discussed in the Word document are also included in the input data folder but are named differently (e.g., if there is a test case #2 for a specific model, the input data file will be indicated by "t2").

4 - Run the top model (RADAR_MANPADS_ENGAGEMENT)

    4.1 - Open the terminal (Ubuntu terminal for Linux and Cygwin for Windows) in the bin folder.
	
	4.2 - To run the model, type in the terminal "./RADAR_MANPADS_ENGAGEMENT". No input data is required as this model is self-contained.
	The simulation will run using the default mission scenario parameters:
				SELECTED_AIRCRAFT_INITIAL_POSITION_X=-30000;
				SELECTED_AIRCRAFT_INITIAL_POSITION_Y=0;
				SELECTED_AIRCRAFT_FINAL_POSITION_X=30000;
				SELECTED_AIRCRAFT_FINAL_POSITION_Y=0;
				SELECTED_AIRCRAFT_SPEED=60;
				SELECTED_RADAR_POSITION_X=0;
				SELECTED_RADAR_POSITION_Y=0;
				SELECTED_RADAR_RANGE=20000;
				SELECTED_GUNNER_POSITION_X=-10000;
				SELECTED_GUNNER_POSITION_Y=0;
				SELECTED_MISSILE_NUMBER=4;
				SELECTED_POH_4_6=30;
				SELECTED_POH_1_4=50;
				SELECTED_POH_0_1=20;
		
	4.3 - To run your own experiments using different parameters than the default values, follow these steps:
				a. Amend the file "engagement_parameters.txt" with your own parameters. This file is located in the "input_data" folder.
				b. Open the terminal (Ubuntu terminal for Linux and Cygwin for Windows) in the bin folder.
				c. Type in the terminal "./RADAR_MANPADS_ENGAGEMENT ../input_data/engagement_parameters.txt" (For windows, "./RADAR_MANPADS_ENGAGEMENT.exe ../input_data/engagement_parameters.txt"). 
		
	4.4 - To check the output results of the model, go to the folder simulation_results and open the following files:
				"radar_MANPADS_engagement_output_messages.txt"
				"radar_MANPADS_engagement_output_state.txt"
	If you want to keep the simulation results, ensure to rename your output files. Otherwise they will be overwritten when you run the next simulation.

5 - View simulation results using the DEVS Web Viewer
	
	5.1 - The DEVS Web Viewer requires the three following files:
				\viewer\diagram.svg
				\viewer\messages.log
				\viewer\structure.json
	The "messages.log" contains the output messages from experiment #1.
	
	5.2 - If you wish to visualize your own experiments, you will need to obtain a new "messages.log" for your own simulation results. Follow these steps:
				a. Go to the DEVS WebViewer (http://ec2-3-235-245-192.compute-1.amazonaws.com:8080/devs-viewer/app-simple/).
				b. Drag and drop these files:
						\bin\radar_MANPADS_engagement_json.json
						\simulation_results\radar_MANPADS_engagement_output_messages.txt
				c. Click "Load Simulation", click "OK" on the error message, and then click the botton corner right blue button for "Download normalized simulation files". Pressing this button downloads three files into your computer.
				d. You now have a new "messages.log". DISREGARD the two other files provided (i.e., options.json and structure.json).
				
	5.3 - The files "diagram.svg" and "structure.json" provided in the "viewer" folder have been created and modified to support this model. Do not change them. These two files remains the same for any simulation runs that you wish to visualized into the DEVS Web Viewer.
	
	5.4 - Finally, go back to the DEVS WebViewer (http://ec2-3-235-245-192.compute-1.amazonaws.com:8080/devs-viewer/app-simple/), then drag and drop the three required files (messages.log, diagram.svg, and structure.json), and click "Load Simulation". Et voilà!

