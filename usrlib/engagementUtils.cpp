#include <iostream>
#include <fstream>
#include <string>
#include <boost/rational.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>

using namespace std;

double SELECTED_AIRCRAFT_INITIAL_POSITION_X = -30000;
double SELECTED_AIRCRAFT_INITIAL_POSITION_Y = 0;
double SELECTED_AIRCRAFT_FINAL_POSITION_X = 30000;
double SELECTED_AIRCRAFT_FINAL_POSITION_Y = 0;
double SELECTED_AIRCRAFT_SPEED = 60;
double SELECTED_RADAR_POSITION_X = 0;
double SELECTED_RADAR_POSITION_Y = 0;
double SELECTED_RADAR_RANGE = 20000;
double SELECTED_GUNNER_POSITION_X = -10000;
double SELECTED_GUNNER_POSITION_Y = 0;
int SELECTED_MISSILE_NUMBER = 4;
int SELECTED_POH_4_6 = 30;
int SELECTED_POH_1_4 = 30;
int SELECTED_POH_0_1 = 30;

// Allows the user to override base options with an external file if required
void ReadOptions(string sOptFile) {

    string sLine;
	string sKey;
	string sValue;
	
	cout << "Reading Options File: " << sOptFile << "\n";
	
	ifstream fOptFile (sOptFile);	
	if(fOptFile.is_open())
	{
		while(getline(fOptFile,sLine))
		{
			sKey=sLine.substr(0,sLine.find("="));
			sValue=sLine.substr(sLine.find("=")+1,sLine.length());
			sValue.erase(std::remove(sValue.begin(), sValue.end(), '\n'),sValue.end());
			sValue.erase(std::remove(sValue.begin(), sValue.end(), '\r'),sValue.end());			

			if(sKey.compare("SELECTED_AIRCRAFT_INITIAL_POSITION_X")==0)
			{
                SELECTED_AIRCRAFT_INITIAL_POSITION_X = stoi(sValue);
				cout << "Updated 'aircraft initial position x' to: " << SELECTED_AIRCRAFT_INITIAL_POSITION_X << " m\n";

			}else if(sKey.compare("SELECTED_AIRCRAFT_INITIAL_POSITION_Y")==0)
			{
                SELECTED_AIRCRAFT_INITIAL_POSITION_Y=stoi(sValue);
				cout << "Updated 'aircraft initial position y' to: " << SELECTED_AIRCRAFT_INITIAL_POSITION_Y << " m\n";

			}else if(sKey.compare("SELECTED_AIRCRAFT_FINAL_POSITION_X")==0)
			{
                SELECTED_AIRCRAFT_FINAL_POSITION_X=stoi(sValue);
				cout << "Updated 'aircraft final position x' to: " << SELECTED_AIRCRAFT_FINAL_POSITION_X << " m\n";

			}else if(sKey.compare("SELECTED_AIRCRAFT_FINAL_POSITION_Y")==0)
			{
                SELECTED_AIRCRAFT_FINAL_POSITION_Y=stoi(sValue);
				cout << "Updated 'aircraft final position y' to: " << SELECTED_AIRCRAFT_FINAL_POSITION_Y << " m\n";

			}else if(sKey.compare("SELECTED_AIRCRAFT_SPEED")==0)
			{
                SELECTED_AIRCRAFT_SPEED=stoi(sValue);
				cout << "Updated 'aircraft speed' to: " << SELECTED_AIRCRAFT_SPEED << " m/s\n";

			}else if(sKey.compare("SELECTED_RADAR_POSITION_X")==0)
			{
                SELECTED_RADAR_POSITION_X=stoi(sValue);
				cout << "Updated 'radar position x' to: " << SELECTED_RADAR_POSITION_X << " m\n";

			}else if(sKey.compare("SELECTED_RADAR_POSITION_Y")==0)
			{
                SELECTED_RADAR_POSITION_Y=stoi(sValue);
				cout << "Updated 'radar position y' to: " << SELECTED_RADAR_POSITION_Y << " m\n";

			}else if(sKey.compare("SELECTED_RADAR_RANGE")==0)
			{
                SELECTED_RADAR_RANGE=stoi(sValue);
				cout << "Updated 'radar range' to: " << SELECTED_RADAR_RANGE << " m\n";

			}else if(sKey.compare("SELECTED_GUNNER_POSITION_X")==0)
			{
                SELECTED_GUNNER_POSITION_X=stoi(sValue);
				cout << "Updated 'gunner position x' to: " << SELECTED_GUNNER_POSITION_X << " m\n";

			}else if(sKey.compare("SELECTED_GUNNER_POSITION_Y")==0)
			{
                SELECTED_GUNNER_POSITION_Y=stoi(sValue);
				cout << "Updated 'gunner position y' to: " << SELECTED_GUNNER_POSITION_Y << " m\n";

			}else if(sKey.compare("SELECTED_MISSILE_NUMBER")==0)
			{
                SELECTED_MISSILE_NUMBER=stoi(sValue);
				cout << "Updated 'missile number' to: " << SELECTED_MISSILE_NUMBER << "\n";

			}else if(sKey.compare("SELECTED_POH_4_6")==0)
			{
                SELECTED_POH_4_6=stoi(sValue);
				cout << "Updated 'probability of hit for missile launch range between 4 and 6 km' to: " << SELECTED_POH_4_6 << "\n";

			}else if(sKey.compare("SELECTED_POH_1_4")==0)
			{
                SELECTED_POH_1_4=stoi(sValue);
				cout << "Updated 'probability of hit for missile launch range between 1 and 4 km' to: " << SELECTED_POH_1_4 << "\n";

			}else if(sKey.compare("SELECTED_POH_0_1")==0)
			{
                SELECTED_POH_0_1=stoi(sValue);
				cout << "Updated 'probability of hit for missile launch range less than 1 km' to: " << SELECTED_POH_0_1 << "\n";
			}
		}

		fOptFile.close();

	} else {
		cout << "Unable to Open Option File: " << sOptFile << "\n"; 
	}
}