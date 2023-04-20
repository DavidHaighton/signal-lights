#ifndef CADMIUM_EXAMPLE_BLINKY_HPP_
#define CADMIUM_EXAMPLE_BLINKY_HPP_

#include <cadmium/core/modeling/coupled.hpp>

#ifdef RT_ARM_MBED
	#include <cadmium/core/real_time/arm_mbed/io/digitalOutput.hpp>
	#include <cadmium/core/real_time/arm_mbed/io/digitalInput.hpp>
#endif

#include "delayer.h"
#ifdef RT_ARM_MBED
	#include "../mbed.h"
	#include "PinNames.h"
#else
	#include "generator.hpp"
#endif

namespace cadmium::blinkySystem {

	struct blinkySystem : public Coupled {

		/**
		 * Constructor function for the blinkySystem model.
		 * @param id ID of the blinkySystem model.
		 */
		blinkySystem(const std::string& id) : Coupled(id) {
#ifdef RT_ARM_MBED
            // the commented code below this comment works.
			//auto digitalOutput = addComponent<DigitalOutput>("digitalOutput", PA_3);
			//auto digitalInput  = addComponent<DigitalInput>("digitalInput", PD_7);
            //auto delayer = addComponent<Delayer>();
			//addCoupling(digitalInput->out, delayer->in);
			//addCoupling(delayer->out, digitalOutput->in);

            //the following code produces A LOT more logs even though the only difference is the name of the component
            auto digitalOutput = addComponent<DigitalOutput>("digitalOutput", PA_3);
			auto digitalInput  = addComponent<DigitalInput>("dibitalInput", PD_7); //i changed a g for a b
            auto delayer = addComponent<Delayer>();
			addCoupling(digitalInput->out, delayer->in);
			addCoupling(delayer->out, digitalOutput->in);

            // the following code produces different behaviour and crashes upon pressing a button:
           	//auto digitalOutput = addComponent<DigitalOutput>("digitalOutput", PA_3);
			//auto digitalInput  = addComponent<DigitalInput>("leftTurn", PD_6);
			//auto digitalInput  = addComponent<DigitalInput>("rightTurn", PD_5);
			//auto digitalInput  = addComponent<DigitalInput>("brakes", PD_7);
            //auto delayer = addComponent<Delayer>();
            //addCoupling(digitalInput->out, delayer->in);
			//addCoupling(delayer->out, digitalOutput->in);
#else
			auto generator = addComponent<Generator>("generator");
#endif
		}
	};
}  //namespace cadmium::blinkySystem

#endif //CADMIUM_EXAMPLE_BLINKY_HPP_
