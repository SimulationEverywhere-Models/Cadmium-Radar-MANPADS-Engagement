#ifndef _ENGAGEMENTUTILS_CPP__
#define _ENGAGEMENTUTILS_CPP__


//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Json exporter header
#include <dynamic_json_exporter.hpp>

//Time class header
#include <NDTime.hpp>

//Messages structures
#include "../data_structures/message.hpp"

//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs
#include "../atomics/gunner.hpp"
#include "../atomics/missile.hpp"
#include "../atomics/aircraft.hpp"
#include "../atomics/radar.hpp"

//C++ headers
#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

/***** Define input port for coupled models *****/
/************************************************/

// Air Defense System (ADS) Coupled Model
struct ads_in: public in_port<messageAircraftPosition>{};

// Radar_Augmented MANPADS Engagement Coupled Model
// N/A

/***** Define output ports for coupled models *****/
/**************************************************/

// Air Defense System (ADS) Coupled Model
struct ads_out: public out_port<messageMissileStatus>{};

// Radar_Augmented MANPADS Engagement Coupled Model
struct radar_MANPADS_engagement_out: public out_port<messageMissileStatus>{};

int main(int argc, char *argv[]){

    if (argc == 2) {
        ReadOptions(argv[1]);
    }

    /****** Atomic model instantiation *******************/
    /*****************************************************/
    shared_ptr<dynamic::modeling::model> myRadar;
    myRadar = dynamic::translate::make_dynamic_atomic_model<Radar, TIME>("myRadar");

    shared_ptr<dynamic::modeling::model> myMissile;
    myMissile = dynamic::translate::make_dynamic_atomic_model<Missile, TIME>("myMissile");

    shared_ptr<dynamic::modeling::model> myGunner;
    myGunner = dynamic::translate::make_dynamic_atomic_model<Gunner, TIME>("myGunner");

    shared_ptr<dynamic::modeling::model> myAircraft;
    myAircraft = dynamic::translate::make_dynamic_atomic_model<Aircraft, TIME>("myAircraft");

    /******* Air Defense System (ADS) Coupled MODEL********/
    /******************************************************/
    dynamic::modeling::Ports iports_ADS;
    iports_ADS = {typeid(ads_in)};

    dynamic::modeling::Ports oports_ADS;
    oports_ADS = {typeid(ads_out)};

    dynamic::modeling::Models submodels_ADS;
    submodels_ADS = {myRadar, myMissile, myGunner};

    dynamic::modeling::EICs eics_ADS;
    eics_ADS = {
            dynamic::translate::make_EIC<ads_in,Radar_defs::inAircraftPosition>("myRadar")
    };

    dynamic::modeling::EOCs eocs_ADS;
    eocs_ADS = {
            dynamic::translate::make_EOC<Missile_defs::outMissileStatus,ads_out>("myMissile")
    };

    dynamic::modeling::ICs ics_ADS;
    ics_ADS = {
            dynamic::translate::make_IC<Radar_defs::outDetectedPosition,Gunner_defs::inDetectedPosition>("myRadar","myGunner"),
            dynamic::translate::make_IC<Gunner_defs::outDistanceGunnerAircraft,Missile_defs::inDistanceGunnerAircraft>("myGunner","myMissile"),
            dynamic::translate::make_IC<Missile_defs::outMissileStatus,Gunner_defs::inMissileStatus>("myMissile","myGunner")
    };

    shared_ptr<dynamic::modeling::coupled<TIME>> ADS_MODEL;
    ADS_MODEL = make_shared<dynamic::modeling::coupled<TIME>>(
            "ADS_Model", submodels_ADS, iports_ADS, oports_ADS, eics_ADS, eocs_ADS, ics_ADS
    );

    /******* Radar_Augmented MANPADS Engagement Coupled MODEL********/
    /****************************************************************/
    dynamic::modeling::Ports iports_TOP;
    iports_TOP = {};

    dynamic::modeling::Ports oports_TOP;
    oports_TOP = {typeid(radar_MANPADS_engagement_out)};

    dynamic::modeling::Models submodels_TOP;
    submodels_TOP = {myAircraft, ADS_MODEL};

    dynamic::modeling::EICs eics_TOP;
    eics_TOP = {};

    dynamic::modeling::EOCs eocs_TOP;
    eocs_TOP = {
            dynamic::translate::make_EOC<ads_out,radar_MANPADS_engagement_out>("ADS_Model")
    };

    dynamic::modeling::ICs ics_TOP;
    ics_TOP = {
            dynamic::translate::make_IC<Aircraft_defs::outAircraftPosition, ads_in>("myAircraft","ADS_Model"),
            dynamic::translate::make_IC<ads_out, Aircraft_defs::inMissileStatus>("ADS_Model","myAircraft")
    };

    shared_ptr<cadmium::dynamic::modeling::coupled<TIME>> TOP;
    TOP = make_shared<dynamic::modeling::coupled<TIME>>(
            "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP
    );

    /*************** Loggers *******************/
    /*******************************************/
    static ofstream out_messages("../simulation_results/radar_MANPADS_engagement_output_messages.txt");
    struct oss_sink_messages{
        static ostream& sink(){
            return out_messages;
        }
    };

    static ofstream out_state("../simulation_results/radar_MANPADS_engagement_output_state.txt");
    struct oss_sink_state{
        static ostream& sink(){
            return out_state;
        }
    };

    using state=logger::logger<logger::logger_state, dynamic::logger::formatter<TIME>, oss_sink_state>;
    using log_messages=logger::logger<logger::logger_messages, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_mes=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_sta=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_state>;

    using logger_top=logger::multilogger<state, log_messages, global_time_mes, global_time_sta>;

    /************** Runner call ************************/
    /***************************************************/
    dynamic::engine::runner<NDTime, logger_top> r(TOP, {0});
    r.run_until(NDTime("00:30:00:000"));

    /************** JSON Exporter call ************************/
    /**********************************************************/
    static ofstream out_JSON("radar_MANPADS_engagement_json.json");
	dynamic_export_model_to_json(out_JSON, TOP);

    return 0;
}

#endif  // _ENGAGEMENTUTILS_CPP__