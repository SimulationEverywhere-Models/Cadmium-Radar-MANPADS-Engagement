#ifndef _ENGAGEMENTUTILS_HPP__
#define _ENGAGEMENTUTILS_HPP__

#include <iostream>
#include <fstream>
#include <string>
#include <boost/rational.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>

using namespace std;

extern double SELECTED_AIRCRAFT_INITIAL_POSITION_X;     //Aircraft initial position in x in meters
extern double SELECTED_AIRCRAFT_INITIAL_POSITION_Y;     //Aircraft initial position in y in meters
extern double SELECTED_AIRCRAFT_FINAL_POSITION_X;       //Aircraft final position in x in meters
extern double SELECTED_AIRCRAFT_FINAL_POSITION_Y;       //Aircraft final position in y in meters
extern double SELECTED_AIRCRAFT_SPEED;                  //Aircraft speed in meters per second
extern double SELECTED_RADAR_POSITION_X;                //Radar position x in meters
extern double SELECTED_RADAR_POSITION_Y;                //Radar position y in meters
extern double SELECTED_RADAR_RANGE;                     //Detection range of the radar in meters
extern double SELECTED_GUNNER_POSITION_X;               //Gunner position x in meters
extern double SELECTED_GUNNER_POSITION_Y;               //Gunner position x in meters
extern int SELECTED_MISSILE_NUMBER;                     //Number of missiles available for the gunner
extern int SELECTED_POH_4_6;                            //Probability of hit of the missile at launching range between 4 and 6 km
extern int SELECTED_POH_1_4;                            //Probability of hit of the missile at launching range between 1 and 4 km
extern int SELECTED_POH_0_1;                            //Probability of hit of the missile at launching range less than 1 km

void ReadOptions(string sOptFile);		// Allows the user to override the default scenario parameters with an external file

#endif  // _ENGAGEMENTUTILS_HPP__