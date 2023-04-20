#include <cadmium/core/simulation/root_coordinator.hpp>

#ifndef NO_LOGGING
	#ifdef RT_ARM_MBED
		#include <cadmium/core/logger/rt.hpp>
	#else
		#include <cadmium/core/logger/csv.hpp>
	#endif
#endif

#include <limits>
#include "blinkySystem.hpp"
#include "CarSim.h"
#ifdef RT_ARM_MBED
	#include "../mbed.h"
#endif

using namespace cadmium::blinkySystem;

int main(int argc, char *argv[]) {
    auto model = std::make_shared<blinkySystem>("Blinky");
    //auto model = std::make_shared<sim::CarSim>();
	auto rootCoordinator = cadmium::RootCoordinator(model);
#ifndef NO_LOGGING

	#ifdef RT_ARM_MBED
		auto logger = std::make_shared<cadmium::RTLogger>(";");
	# else
		auto logger = std::make_shared<cadmium::CSVLogger>("signalLog.csv",";"); // new
	#endif

	rootCoordinator.setLogger(logger);
#endif

	rootCoordinator.start();
 	rootCoordinator.simulate(std::numeric_limits<double>::infinity());
	//rootCoordinator.simulate(1000.0);
	rootCoordinator.stop();
	return 0;
}