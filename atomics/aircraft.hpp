/**
* Aircraft atomic model
*/

#ifndef _AIRCRAFT_HPP__
#define _AIRCRAFT_HPP__

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
struct Aircraft_defs {
    struct inMissileStatus : public in_port<messageMissileStatus> {};
    struct outAircraftPosition : public out_port<messageAircraftPosition> {};
};

// Atomic model class
template<typename TIME> class Aircraft {

    enum phase {FLYING, MISSION_COMPLETED, HIT};

    public:

    // ports definition
    using input_ports=tuple<typename Aircraft_defs::inMissileStatus>;
    using output_ports=tuple<typename Aircraft_defs::outAircraftPosition>;

    // state definition
    struct state_type{
        TIME sigma;
        phase currPhase;
        double aircraftPositionX;
        double aircraftPositionY;
        double aircraftInitialPositionX;
        double aircraftInitialPositionY;
        double aircraftFinalPositionX;
        double aircraftFinalPositionY;
        double aircraftSpeed;
        double distanceFlown;
        int missileStatus;
        double flightPathDistance;
        double directionX;
        double directionY;
    };

    state_type state;

    // default constructor
    Aircraft() {
        state.aircraftPositionX = SELECTED_AIRCRAFT_INITIAL_POSITION_X;
        state.aircraftPositionY = SELECTED_AIRCRAFT_INITIAL_POSITION_Y;
        state.aircraftInitialPositionX = SELECTED_AIRCRAFT_INITIAL_POSITION_X;
        state.aircraftInitialPositionY = SELECTED_AIRCRAFT_INITIAL_POSITION_Y;
        state.aircraftFinalPositionX = SELECTED_AIRCRAFT_FINAL_POSITION_X;
        state.aircraftFinalPositionY = SELECTED_AIRCRAFT_FINAL_POSITION_Y;
        state.aircraftSpeed = SELECTED_AIRCRAFT_SPEED;
        state.flightPathDistance = (sqrt(pow((SELECTED_AIRCRAFT_FINAL_POSITION_X - SELECTED_AIRCRAFT_INITIAL_POSITION_X),2) + pow((SELECTED_AIRCRAFT_FINAL_POSITION_Y - SELECTED_AIRCRAFT_INITIAL_POSITION_Y),2)));
        state.directionX = (SELECTED_AIRCRAFT_FINAL_POSITION_X - SELECTED_AIRCRAFT_INITIAL_POSITION_X) / (sqrt(pow((SELECTED_AIRCRAFT_FINAL_POSITION_X - SELECTED_AIRCRAFT_INITIAL_POSITION_X),2) + pow((SELECTED_AIRCRAFT_FINAL_POSITION_Y - SELECTED_AIRCRAFT_INITIAL_POSITION_Y),2)));
        state.directionY = (SELECTED_AIRCRAFT_FINAL_POSITION_Y - SELECTED_AIRCRAFT_INITIAL_POSITION_Y) / (sqrt(pow((SELECTED_AIRCRAFT_FINAL_POSITION_X - SELECTED_AIRCRAFT_INITIAL_POSITION_X),2) + pow((SELECTED_AIRCRAFT_FINAL_POSITION_Y - SELECTED_AIRCRAFT_INITIAL_POSITION_Y),2)));
        state.distanceFlown = 0;
        state.currPhase = FLYING;
        state.sigma = TIME();
    }

    // internal transition
    void internal_transition() {

            // Calculate new aircraft position
            state.aircraftPositionX = state.aircraftPositionX + (state.directionX * state.aircraftSpeed);
            state.aircraftPositionY = state.aircraftPositionY + (state.directionY * state.aircraftSpeed);
            state.distanceFlown = (sqrt(pow((state.aircraftPositionX - state.aircraftInitialPositionX),2) + pow((state.aircraftPositionY - state.aircraftInitialPositionY),2)));

            // Verify if aircraft has reached the end of its predefined path
            if (state.distanceFlown >= state.flightPathDistance) {
                state.sigma=std::numeric_limits<TIME>::infinity();
                state.currPhase = MISSION_COMPLETED;
            } else {
                state.sigma = TIME("00:00:01.000");
                state.currPhase = FLYING;
            }
    }

    // external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {

        for(const auto &x : get_messages<typename Aircraft_defs::inMissileStatus>(mbs)) {

            if (state.distanceFlown < state.flightPathDistance) {
                state.missileStatus = x.missileStatus;

                if (state.missileStatus == 1) {
                    state.currPhase = HIT;
                    state.sigma = std::numeric_limits<TIME>::infinity();
                } else {
                    state.currPhase = FLYING;
                    state.sigma = TIME("00:00:01.000");
                }
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

        if(state.currPhase == FLYING) {
            get_messages<typename Aircraft_defs::outAircraftPosition>(bags).push_back(messageAircraftPosition(state.aircraftPositionX,state.aircraftPositionY));
        }

        return bags;
    }

    // time_advance function
    TIME time_advance() const {
        return state.sigma;
    }

    friend ostringstream& operator<<(ostringstream& os, const typename Aircraft<TIME>::state_type& currState) {
        os << "Aircraft position: [" << currState.aircraftPositionX  << "," << currState.aircraftPositionY << "]";
        // For JSON file:
        //os << "<" << currState.currPhase << ", " << currState.aircraftPositionX << ", " << currState.aircraftPositionY << ">";

        return os;
    }
};

#endif // _AIRCRAFT_HPP__