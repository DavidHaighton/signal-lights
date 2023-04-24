#ifndef CADMIUM_EXAMPLE_BLINKY_HPP_
#define CADMIUM_EXAMPLE_BLINKY_HPP_

#include <cadmium/core/modeling/coupled.hpp>

#ifdef RT_ARM_MBED
#include <cadmium/core/real_time/arm_mbed/io/digitalOutput.hpp>
	#include <cadmium/core/real_time/arm_mbed/io/digitalInput.hpp>
#endif

#ifdef RT_ARM_MBED
#include "BlinkGenerator.h"
#include "../mbed.h"
	#include "PinNames.h"
#else
#endif
#include "FrontLogic.h"
#include "BackLogic.h"
namespace cadmium::blinkySystem {

    struct blinkySystem : public Coupled {

        /**
         * Constructor function for the blinkySystem model.
         * @param id ID of the blinkySystem model.
         */
        blinkySystem(const std::string& id) : Coupled(id) {
#ifdef RT_ARM_MBED
            auto generator = addComponent<Blinker>("Blink Generator");
            auto fll = addComponent<FrontLogic>("Front Left Logic");
            auto frl = addComponent<FrontLogic>("Front Right Logic");
            auto bll = addComponent<BackLogic>("Back Left Logic");
            auto brl = addComponent<BackLogic>("Back Right Logic");
            auto frontLeft = addComponent<cadmium::DigitalOutput>("FL LED", PB_1),
                    frontRight = addComponent<cadmium::DigitalOutput>("FR LED", PB_11),
                    backLeft = addComponent<cadmium::DigitalOutput>("BL LED",PC_0),
                    backRight = addComponent<cadmium::DigitalOutput>("BR LED",PA_3);
			auto brakes  = addComponent<DigitalInput>("brakes", PD_7);
			auto left = addComponent<DigitalInput>("left", PD_6);
            auto right = addComponent<DigitalInput>("right", PD_5);
			addCoupling(fll->out, frontLeft->in);
            addCoupling(generator->out, fll->blinkIn);
            addCoupling(left->out, fll->turnIn);

            addCoupling(frl->out, frontRight->in);
            addCoupling(generator->out, frl->blinkIn);
            addCoupling(right->out, frl->turnIn);

            addCoupling(bll->out, backLeft->in);
            addCoupling(generator->out, bll->blinkIn);
            addCoupling(left->out, bll->turnIn);
            addCoupling(brakes->out, bll->brakeIn);

            addCoupling(brl->out, backRight->in);
            addCoupling(generator->out, brl->blinkIn);
            addCoupling(right->out, brl->turnIn);
            addCoupling(brakes->out, brl->brakeIn);
			//addCoupling(brakes->out, frontRight->in);
			//addCoupling(left->out, backLeft->in);
			//addCoupling(right->out, backRight->in);
#else
#endif
        }
    };
}  //namespace cadmium::blinkySystem

#endif //CADMIUM_EXAMPLE_BLINKY_HPP_