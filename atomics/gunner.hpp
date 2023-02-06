/**
* Gunner atomic model
*/

#ifndef _GUNNER_HPP__
#define _GUNNER_HPP__

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
struct Gunner_defs {
    struct inDetectedPosition : public in_port<messageDetectedPosition> {};
    struct inMissileStatus : public in_port<messageMissileStatus> {};
    struct outDistanceGunnerAircraft : public out_port<messageDistance> {};
};

// Atomic model class
template<typename TIME> class Gunner {

    enum phase {WAITING, LAUNCHING, LOADING, HIT};

    public:
    // ports definition
    using input_ports=tuple<typename Gunner_defs::inDetectedPosition, typename Gunner_defs::inMissileStatus>;
    using output_ports=tuple<typename Gunner_defs::outDistanceGunnerAircraft>;

    // state definition
    struct state_type{
        TIME sigma;
        phase currPhase;
        double gunnerPositionX;
        double gunnerPositionY;
        double detectedPositionX;
        double detectedPositionY;
        int missileNumber;
        double gunnerRange;
        TIME missileReloadTime;
        TIME reload_time;
        double distanceGunnerAircraft;
        int missileStatus;
    };

    state_type state;

    // default constructor
    Gunner() {
        state.sigma = std::numeric_limits<TIME>::infinity();
        state.currPhase = WAITING;
        state.gunnerPositionX = SELECTED_GUNNER_POSITION_X;
        state.gunnerPositionY = SELECTED_GUNNER_POSITION_Y;
        state.missileNumber = SELECTED_MISSILE_NUMBER;
        state.gunnerRange = 6000;
        state.missileReloadTime = TIME("00:00:40");
    }

    // internal transition
    void internal_transition() {

        switch(state.currPhase) {
            case LAUNCHING:
                state.currPhase = WAITING;
                state.sigma = std::numeric_limits<TIME>::infinity();
                break;

            case LOADING:
                state.currPhase = WAITING;
                state.sigma = std::numeric_limits<TIME>::infinity();
                break;
        }
    }

    // external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {

        for (const auto &x : get_messages<typename Gunner_defs::inMissileStatus>(mbs)) {

            if (state.currPhase == LOADING) {
                state.sigma = state.reload_time;
                state.reload_time = state.sigma;
            }

            if ((state.currPhase == WAITING) && (state.distanceGunnerAircraft <= state.gunnerRange)) {
                state.missileStatus = x.missileStatus;

                if (state.missileStatus == 1) {
                    state.currPhase = HIT;
                    state.sigma = std::numeric_limits<TIME>::infinity();

                } else if (state.missileStatus == 0) {
                    state.missileNumber -= 1;
                    if (state.missileNumber > 0) {
                        state.currPhase = LOADING;
                        state.sigma = state.missileReloadTime;
                        state.reload_time = state.sigma;
                    } else {
                        state.currPhase = WAITING;
                        state.sigma = std::numeric_limits<TIME>::infinity();
                    }
                }
            }
        }

        for (const auto &x : get_messages<typename Gunner_defs::inDetectedPosition>(mbs)) {

            if (state.currPhase == LOADING) {
                state.sigma = state.reload_time - e;
                state.reload_time = state.sigma;
            }

            if (state.currPhase == WAITING) {
                state.detectedPositionX = x.detectedPositionX;
                state.detectedPositionY = x.detectedPositionY;

                state.distanceGunnerAircraft = sqrt(pow(state.detectedPositionX - state.gunnerPositionX, 2) +
                                                    pow(state.detectedPositionY - state.gunnerPositionY, 2));

                if ((state.distanceGunnerAircraft <= state.gunnerRange) && (state.missileNumber > 0))   {
                    state.currPhase = LAUNCHING;
                    state.sigma = TIME();
                } else {
                    state.currPhase = WAITING;
                    state.sigma = std::numeric_limits<TIME>::infinity();
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

        if (state.currPhase == LAUNCHING) {
            get_messages<typename Gunner_defs::outDistanceGunnerAircraft>(bags).push_back(messageDistance(state.distanceGunnerAircraft));
        }
            return bags;
    }

    // time_advance function
    TIME time_advance() const {
        return state.sigma;
    }

    friend ostringstream& operator<<(ostringstream& os, const typename Gunner<TIME>::state_type& currState) {

        switch(currState.currPhase) {

            case WAITING:
                os << "WAITING (i.e., either the aircraft is not within range or the gunner has no missile)";
                // For JSON file
                //os << "<" << currState.currPhase << ">";
                break;

            case LAUNCHING:
                os << "LAUNCHING (i.e., aircraft is within range). Distance between gunner and aircraft is: " << currState.distanceGunnerAircraft << " meters.";
                // For JSON file
                //os << "<" << currState.currPhase << ", " << currState.distanceGunnerAircraft << ">";
                break;

            case LOADING:
                os << "LOADING (i.e., gunner is loading a new missile). Number of missiles left is: " << currState.missileNumber;
                // For JSON file
                //os << "<" << currState.currPhase << ", " << currState.missileNumber << ">";
                break;

            case HIT:
                os << "HIT (i.e., aircraft has been hit)";
                // For JSON file
                //os << "<" << currState.currPhase << ">";
                break;
        }

        return os;
    }
};

#endif //_GUNNER_HPP__