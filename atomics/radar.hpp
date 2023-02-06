/**
* Radar atomic model
*/

#ifndef _RADAR_HPP__
#define _RADAR_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>

#include "../data_structures/message.hpp"
#include "../usrlib/engagementUtils.hpp"

using namespace cadmium;
using namespace std;

// Port definition
struct Radar_defs {
    struct inAircraftPosition : public in_port<messageAircraftPosition> {};
    struct outDetectedPosition : public out_port<messageDetectedPosition> {};
};

// Atomic model class
template<typename TIME> class Radar {

    enum phase {SEARCHING, DETECTED};

    public:

    // ports definition
    using input_ports=tuple<typename Radar_defs::inAircraftPosition>;
    using output_ports=tuple<typename Radar_defs::outDetectedPosition>;

    // state definition
    struct state_type{
        TIME  sigma;
        double aircraftPositionX;
        double aircraftPositionY;
        phase currPhase;
        double distanceRadarAircraft;
        double detectedPositionX;
        double detectedPositionY;
        double radarPositionX;
        double radarPositionY;
        double radarRange;
    };

    state_type state;

    // default constructor
    Radar() {
        state.sigma = std::numeric_limits<TIME>::infinity();
        state.detectedPositionX = 0;
        state.detectedPositionY = 0;
        state.currPhase = SEARCHING;
        state.radarPositionX = SELECTED_RADAR_POSITION_X;
        state.radarPositionY = SELECTED_RADAR_POSITION_Y;
        state.radarRange = SELECTED_RADAR_RANGE;
    }

    // internal transition
    void internal_transition() {

        switch(state.currPhase) {
            case DETECTED:
                state.currPhase = SEARCHING;
                state.sigma = std::numeric_limits<TIME>::infinity();
                break;
        }
    }

    // external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {

        for (const auto &x : get_messages<typename Radar_defs::inAircraftPosition>(mbs)) {

            state.detectedPositionX = x.aircraftPositionX;
            state.detectedPositionY = x.aircraftPositionY;

            state.distanceRadarAircraft = sqrt(pow(state.detectedPositionX - state.radarPositionX, 2) +
                                         pow(state.detectedPositionY - state.radarPositionY, 2));

            if (state.distanceRadarAircraft <= state.radarRange) {
                state.currPhase = DETECTED;
                state.sigma = TIME("00:00:01.000");
            } else {
                state.currPhase = SEARCHING;
                state.sigma = std::numeric_limits<TIME>::infinity();
            }
        }
    }

    // confluence transition
    void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
        internal_transition();
        external_transition(TIME(), move(mbs));
    }

    // output function
    typename make_message_bags<output_ports>::type output() const {
        typename make_message_bags<output_ports>::type bags;

        if(state.currPhase == DETECTED) {
            get_messages<typename Radar_defs::outDetectedPosition>(bags).push_back(messageDetectedPosition(state.detectedPositionX, state.detectedPositionY));
        }
            return bags;
    }

    // time_advance function
    TIME time_advance() const {
        return state.sigma;
    }

    friend ostringstream& operator<<(ostringstream& os, const typename Radar<TIME>::state_type& currState) {

        switch(currState.currPhase) {

            case SEARCHING:
               os << "SEARCHING (i.e., aircraft is not within detection range). Distance between the radar and the aircraft is: " << currState.distanceRadarAircraft << " meters.";
               // For JSON file
               // os << "<" << currState.currPhase << ", " << currState.distanceRadarAircraft << ">";
               break;

            case DETECTED:
               os << "DETECTED (i.e., aircraft is within the detection range). Distance between the radar and the aircraft is: " << currState.distanceRadarAircraft << " meters.";
               // For JSON file
               //os << "<" << currState.currPhase << ", " << currState.distanceRadarAircraft << ">";
                break;
        }

        return os;

    }
};

#endif  // _RADAR_HPP__