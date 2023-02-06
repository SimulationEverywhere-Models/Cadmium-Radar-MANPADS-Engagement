#include <math.h> 
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "message.hpp"

// messageMissileStatus
ostream& operator<<(ostream& os, const messageMissileStatus& msg) {
  os << "<" << msg.missileStatus << ">";
  return os;
}

istream& operator>> (istream& is, messageMissileStatus& msg) {
  is >> msg.missileStatus;
  return is;
}

// messageAircraftPosition
ostream& operator<<(ostream& os, const messageAircraftPosition& msg) {
    os << "<" << msg.aircraftPositionX << "," << msg.aircraftPositionY << ">";
    return os;
}

istream& operator>> (istream& is, messageAircraftPosition& msg) {
    is >> msg.aircraftPositionX;
    is >> msg.aircraftPositionY;
    return is;
}

// messageDetectedPosition
ostream& operator<<(ostream& os, const messageDetectedPosition& msg) {
    os << "<" << msg.detectedPositionX << "," << msg.detectedPositionY << ">";
    return os;
}

istream& operator>> (istream& is, messageDetectedPosition& msg) {
    is >> msg.detectedPositionX;
    is >> msg.detectedPositionY;
    return is;
}

// messageDistance
ostream& operator<<(ostream& os, const messageDistance& msg) {
    os << "<" << msg.distance << ">";
    return os;
}

istream& operator>> (istream& is, messageDistance& msg) {
    is >> msg.distance;
    return is;
}