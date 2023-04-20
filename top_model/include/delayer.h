#pragma once
#include <cadmium/core/modeling/atomic.hpp>

#ifndef NO_LOGGING
#include <iostream>
#endif
#include <queue>
#include <limits>

#include <cstdlib>
    struct DelayerState{
        std::queue<bool> q;
        DelayerState() {}
    };
#ifndef NO_LOGGING

    std::ostream& operator<<(std::ostream &out, const DelayerState& state) {
        return out;
    }
#endif

struct Delayer: public cadmium::Atomic<DelayerState> {
        cadmium::Port<bool> out;
        cadmium::Port<bool> in;

        Delayer(): Atomic<DelayerState>("Delayer", DelayerState()) {
            out = addOutPort<bool>("out");
            in = addInPort<bool>("in");
        }


        void internalTransition(DelayerState& state) const override {
            state.q.pop();
        }


        void externalTransition(DelayerState& state, double e) const override {
            state.q.emplace(in->getBag()[0]);
            printf("Hello EXTERNAL");
        }

        void output(const DelayerState& state) const override {
            out->addMessage(state.q.front());
        }


        [[nodiscard]] double timeAdvance(const DelayerState& state) const override {
            return state.q.empty()?std::numeric_limits<double>::infinity():1;
        }
    };
