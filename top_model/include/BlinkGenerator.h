#pragma once
#include <cadmium/core/modeling/atomic.hpp>

#ifndef NO_LOGGING
#include <iostream>
#endif

#include <cstdlib>
    struct BlinkerState{
        bool on;
        BlinkerState() {}
    };
#ifndef NO_LOGGING

    std::ostream& operator<<(std::ostream &out, const BlinkerState& state) {
        out << (state.on?"ON":"OFF");
        return out;
    }
#endif

struct Blinker: public cadmium::Atomic<BlinkerState> {
        cadmium::Port<bool> out;

        explicit Blinker(const std::string& name): Atomic<BlinkerState>(name, BlinkerState()) {
            out = addOutPort<bool>("out");
        }

        void internalTransition(BlinkerState& state) const override {
            state.on = !state.on;
        }


        void externalTransition(BlinkerState& state, double e) const override {
        }

        void output(const BlinkerState& state) const override {
            out->addMessage(state.on);
        }

        [[nodiscard]] double timeAdvance(const BlinkerState& state) const override {
            return 0.2;
        }
    };