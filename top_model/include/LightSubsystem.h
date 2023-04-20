#pragma once

#include <cadmium/core/modeling/coupled.hpp>
#include <queue>

#ifdef RT_ARM_MBED
    #include <cadmium/core/real_time/arm_mbed/io/digitalOutput.hpp>
	#include <cadmium/core/real_time/arm_mbed/io/digitalInput.hpp>
#endif

#include "LightMultiplexer.h"
#include "LightSubsystem.h"
#include "LightController.h"
#include "CommandMessage.h"

#ifdef RT_ARM_MBED
    #include "../mbed.h"
	#include "PinNames.h"
#else
    #include "MockLight.h"
#endif
namespace sim {
    struct LightSubsystem: public cadmium::Coupled {
        cadmium::Port<CommandPacket> input;
        LightSubsystem(): Coupled("Light Signal Subsystem") {
            input = addInPort<CommandPacket>("InPort");
            auto frontLeftController = addComponent<LightController>("FRONT_LEFT_CONTROLLER"),
                    frontRightController = addComponent<LightController>("FRONT_RIGHT_CONTROLLER"),
                    backLeftController = addComponent<LightController>("BACK_LEFT_CONTROLLER"),
                    backRightController = addComponent<LightController>("BACK_RIGHT_CONTROLLER");
#ifdef RT_ARM_MBED
            auto frontLeft = addComponent<cadmium::DigitalOutput>("FL LED", PB_1),
                    frontRight = addComponent<cadmium::DigitalOutput>("FR LED", PB_11),
                    backLeft = addComponent<cadmium::DigitalOutput>("BL LED",PC_0),
                    backRight = addComponent<cadmium::DigitalOutput>("BR LED",PA_3);
#else
            auto frontLeft = addComponent<MockLight>("FL LED"),
                    frontRight = addComponent<MockLight>("FR LED"),
                    backLeft = addComponent<MockLight>("BL LED"),
                    backRight = addComponent<MockLight>("BR LED");
#endif
            auto mux = addComponent<LightMux>();
            addCoupling(input, mux->in);
            addCoupling(mux->frontLeftPort, frontLeftController->in);
            addCoupling(frontLeftController->out, frontLeft->in);
            addCoupling(mux->frontRightPort, frontRightController->in);
            addCoupling(frontRightController->out, frontRight->in);

            addCoupling(mux->backLeftPort, backLeftController->in);
            addCoupling(backLeftController->out, backLeft->in);

            addCoupling(mux->backRightPort, backRightController->in);
            addCoupling(backRightController->out, backRight->in);
        }
    };
}
