#pragma once
#include <cadmium/core/modeling/atomic.hpp>
#include <limits>
#ifndef NO_LOGGING
#include <iostream>
#endif

#include <cstdlib>
struct FrontLogicState{
    bool blinking = false;
    bool turning = false;
    bool extTrans = false;
    bool blinkOut() const{
        return blinking && turning;
    }
    FrontLogicState() {}
};
#ifndef NO_LOGGING

std::ostream& operator<<(std::ostream &out, const FrontLogicState& state) {
    out << "{BlinkingIN:" << (state.blinking?"ON":"OFF");
    out << ", TurningIN: " << (state.turning?"ON":"OFF");
    out << ", BlinkingOUT: "<<(state.blinkOut()) <<"}";
    return out;
}
#endif

struct FrontLogic: public cadmium::Atomic<FrontLogicState> {
    cadmium::Port<bool> out;
    cadmium::Port<bool> blinkIn;
    cadmium::Port<bool> turnIn;

    explicit FrontLogic(const std::string& name): Atomic<FrontLogicState>(name, FrontLogicState()) {
        out = addOutPort<bool>("out");
        blinkIn = addInPort<bool>("blinkIn");
        turnIn = addInPort<bool>("turnIn");
    }

    void internalTransition(FrontLogicState& state) const override {
        state.extTrans = false;
    }


    void externalTransition(FrontLogicState& state, double e) const override {
        if(!turnIn->empty()){
            state.turning = turnIn->getBag()[0];
        }
        if(!blinkIn->empty()){
            state.blinking = blinkIn->getBag()[0];
        }
        state.extTrans = true;
    }

    void output(const FrontLogicState& state) const override {
        out->addMessage(state.blinkOut());
    }

    [[nodiscard]] double timeAdvance(const FrontLogicState& state) const override {
        return state.extTrans?0:std::numeric_limits<double>::infinity();
    }
};