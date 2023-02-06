/**
* Missile atomic model
*/

#ifndef _MISSILE_HPP__
#define _MISSILE_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>
#include <random>

#include "../data_structures/message.hpp"
#include "../usrlib/engagementUtils.hpp"

using namespace cadmium;
using namespace std;

// Port definition
struct Missile_defs {
    struct inDistanceGunnerAircraft : public in_port<messageDistance> {};
    struct outMissileStatus : public out_port<messageMissileStatus> {};
};

// Atomic model class
template<typename TIME> class Missile {

    enum phase {WAITING, LAUNCHED};

    public:

    // ports definition
    using input_ports=tuple<typename Missile_defs::inDistanceGunnerAircraft>;
    using output_ports=tuple<typename Missile_defs::outMissileStatus>;

    // state definition
    struct state_type{
        TIME sigma;
        phase currPhase;
        double distanceMissileAircraft;
        int missileStatus;
        TIME flying_time;
        int poh_4_6;
        int poh_1_4;
        int poh_0_1;
    };

    state_type state;

    // default constructor
    Missile() {
        state.sigma = std::numeric_limits<TIME>::infinity();
        state.currPhase = WAITING;
        state.poh_4_6 = SELECTED_POH_4_6;
        state.poh_1_4 = SELECTED_POH_1_4;
        state.poh_0_1 = SELECTED_POH_0_1;
    }

    // internal transition
    void internal_transition() {

        switch(state.currPhase) {
            case LAUNCHED:
                state.currPhase = WAITING;
                state.sigma = std::numeric_limits<TIME>::infinity();
                break;
        }
    }

    // external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {

        for (const auto &x : get_messages<typename Missile_defs::inDistanceGunnerAircraft>(mbs)) {

            if (state.currPhase == WAITING) {
                state.distanceMissileAircraft = x.distance;

                if (state.distanceMissileAircraft <= 6000) {

                    state.currPhase = LAUNCHED;

                    if ((state.distanceMissileAircraft <= 6000) && (state.distanceMissileAircraft >= 4000)) {
                        state.missileStatus = ((rand() % 100) < state.poh_4_6);
                        state.flying_time = TIME("00:00:07");
                        state.sigma = state.flying_time;

                    } else if ((state.distanceMissileAircraft <= 4000) && (state.distanceMissileAircraft >= 1000)) {
                        state.missileStatus = ((rand() % 100) < state.poh_1_4);
                        state.flying_time = TIME("00:00:05");
                        state.sigma = state.flying_time;

                    } else if  (state.distanceMissileAircraft < 1000) {
                        state.missileStatus = ((rand() % 100) < state.poh_0_1);
                        state.flying_time = TIME("00:00:05");
                        state.sigma = state.flying_time;
                    }

                } else {
                    state.currPhase = WAITING;
                    state.sigma = std::numeric_limits<TIME>::infinity();
                }
            } else if (state.currPhase == LAUNCHED) {
                state.sigma = state.flying_time - e;
                state.flying_time = state.sigma;
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

        if(state.currPhase == LAUNCHED) {
            get_messages<typename Missile_defs::outMissileStatus>(bags).push_back(messageMissileStatus(state.missileStatus));
        }
            return bags;
    }

    // time_advance function
    TIME time_advance() const {
        return state.sigma;
    }

    friend ostringstream& operator<<(ostringstream& os, const typename Missile<TIME>::state_type& currState) {

        switch(currState.currPhase) {

            case LAUNCHED:
                os << "LAUNCHED. Missile status is: " << currState.missileStatus  << " (1 = hit and 0 = miss)";
                // For JSON file
                //os << "<" << currState.currPhase << ", " << currState.missileStatus << ">";
                break;

            case WAITING:
                os << "WAITING (i.e., missile is not within reach of aircraft).";
                // For JSON file
                //os << "<" << currState.currPhase << ">";
                break;
        }

        return os;

    }
};

#endif // _MISSILE_HPP__