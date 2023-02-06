#ifndef _ENGAGEMENT_SIMULATION_MESSAGE_HPP__
#define _ENGAGEMENT_SIMULATION_MESSAGE_HPP__

#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

// messageMissileStatus
struct messageMissileStatus {
    messageMissileStatus() {}
    messageMissileStatus(int iMissileStatus):missileStatus(iMissileStatus) {}
        int missileStatus;
};

ostream& operator<<(ostream& os, const messageMissileStatus& msg);
istream& operator>> (istream& is, messageMissileStatus& msg);

// messageAircraftPosition
struct messageAircraftPosition {
    messageAircraftPosition(){}
    messageAircraftPosition(int ipx, int ipy):aircraftPositionX(ipx),aircraftPositionY(ipy){}
        int aircraftPositionX;
        int aircraftPositionY;
};

ostream& operator<<(ostream& os, const messageAircraftPosition& msg);
istream& operator>> (istream& is, messageAircraftPosition& msg);

// messageDetectionPosition
struct messageDetectedPosition {
    messageDetectedPosition(){}
    messageDetectedPosition(int ipx, int ipy):detectedPositionX(ipx),detectedPositionY(ipy){}
    int detectedPositionX;
    int detectedPositionY;
};

ostream& operator<<(ostream& os, const messageDetectedPosition& msg);
istream& operator>> (istream& is, messageDetectedPosition& msg);

// messageDistance
struct messageDistance {
    messageDistance(){}
    messageDistance(int i_distance):distance(i_distance){}
    int distance;
};

ostream& operator<<(ostream& os, const messageDistance& msg);
istream& operator>> (istream& is, messageDistance& msg);

#endif // _ENGAGEMENT_SIMULATION_MESSAGE_HPP__