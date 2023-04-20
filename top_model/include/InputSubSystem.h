#pragma once

#include <cadmium/core/modeling/coupled.hpp>
#ifdef RT_ARM_MBED
	#include <cadmium/core/real_time/arm_mbed/io/digitalInput.hpp>
#endif

#include "InputDemux.h"
#include "CommandMessage.h"


#ifdef RT_ARM_MBED
#include "../mbed.h"
	#include "PinNames.h"
#else
    #include "MockButton.h"
#endif

namespace sim {
    struct InputSubSystem: public cadmium::Coupled {
        cadmium::Port<CommandPacket> output;
        InputSubSystem(): Coupled("Button Signal Subsystem") {

#ifdef RT_ARM_MBED
            auto brakes = addComponent<cadmium::DigitalInput>("Brake Button", PD_7);
            auto left = addComponent<cadmium::DigitalInput>("Left Turn Button", PD_6);
            auto right = addComponent<cadmium::DigitalInput>("Right Turn Button", PD_5);
#else
            auto brakeQ = std::queue<std::pair<double,bool>>();
            brakeQ.emplace(1, true);
            brakeQ.emplace(10,false);
            auto leftQ= std::queue<std::pair<double,bool>>();
            leftQ.emplace(1, true);
            leftQ.emplace(30, false);

            auto brakes = addComponent<MockButton>("Brake Button Sim", brakeQ);
            auto left = addComponent<MockButton>("Left Turn Button", leftQ);
            auto right = addComponent<MockButton>("Right Turn Button", std::queue<std::pair<double,bool>>());
#endif
            auto controller = addComponent<InputDemux>();
            output = addOutPort<CommandPacket>("Output");

            addCoupling(brakes->out, controller->brakePort);
            addCoupling(left->out, controller->leftPort);
            addCoupling(right->out, controller->rightPort);
            addCoupling(controller->out, output);

        }
    };
}
