#pragma once
#include <limits>
#include <queue>
#include <cadmium/core/modeling/atomic.hpp>
#include <cadmium/core/modeling/port.hpp>
#include "CommandMessage.h"
namespace sim{
    struct InputControllerState{
        std::queue<CommandPacket> queue;
    };

    std::ostream& operator<<(std::ostream& out, const InputControllerState& s) {
        out <<"{queue_size:"<<s.queue.size()
            <<"}";
        return out;
    }
    struct InputDemux: public cadmium::Atomic<InputControllerState>{
        cadmium::Port<bool> brakePort, leftPort, rightPort;
        cadmium::Port<CommandPacket> out;
        InputDemux(): Atomic<InputControllerState>("Input Controller", InputControllerState()){
            brakePort = addInPort<bool>("BrakeIN");
            leftPort = addInPort<bool>("leftIN");
            rightPort = addInPort<bool>("rightIN");
            out = addOutPort<CommandPacket>("out");
        }

        void internalTransition(InputControllerState& s) const override {
            if(!s.queue.empty())
                s.queue.pop();
        }

        void externalTransition(InputControllerState &s, double e) const override {
            printf("External Transition!\n");
            if(!brakePort->empty()){
                s.queue.emplace(CommandPacketType::BRAKE, brakePort->getBag()[0]);
            }
            if(!leftPort->empty()){
                s.queue.emplace(CommandPacketType::LEFT_TURN, leftPort->getBag()[0]);
            }
            if(!rightPort->empty()){
                s.queue.emplace(CommandPacketType::RIGHT_TURN, rightPort->getBag()[0]);
            }
        }

        void output(const InputControllerState& s) const override {
            out->addMessage(s.queue.front());
        }

        [[nodiscard]] double timeAdvance(const InputControllerState& s) const override {
            return s.queue.empty()?std::numeric_limits<double>::infinity():0.01;
        }

    };
}
