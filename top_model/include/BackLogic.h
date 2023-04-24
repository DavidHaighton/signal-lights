#pragma once
#include <cadmium/core/modeling/atomic.hpp>
#include <limits>
#ifndef NO_LOGGING
#include <iostream>
#endif

#include <cstdlib>
struct BackLogicState{
    bool blinking = false;
    bool turning = false;
    bool extTrans = false;
    bool braking = false;
    bool blinkOut() const{
        return blinking && turning || braking;
    }
    BackLogicState() {}
};
#ifndef NO_LOGGING

std::ostream& operator<<(std::ostream &out, const BackLogicState& state) {
    out << "{BlinkingIN:" << (state.blinking?"ON":"OFF");
    out << ", TurningIN: " << (state.turning?"ON":"OFF");
    out << ", BrakingIN" << (state.braking?"ON":"OFF");
    out << ", BlinkingOUT: "<<(state.blinkOut()) <<"}";
    return out;
}
#endif

struct BackLogic: public cadmium::Atomic<BackLogicState> {
    cadmium::Port<bool> out;
    cadmium::Port<bool> blinkIn;
    cadmium::Port<bool> turnIn;
    cadmium::Port<bool> brakeIn;

    explicit BackLogic(const std::string& name): Atomic<BackLogicState>(name, BackLogicState()) {
        out = addOutPort<bool>("out");
        blinkIn = addInPort<bool>("blinkIn");
        turnIn = addInPort<bool>("turnIn");
        brakeIn = addInPort<bool>("brakeIn");
    }

    void internalTransition(BackLogicState& state) const override {
        state.extTrans = false;
    }


    void externalTransition(BackLogicState& state, double e) const override {
        if(!turnIn->empty()){
            state.turning = turnIn->getBag()[0];
        }
        if(!blinkIn->empty()){
            state.blinking = blinkIn->getBag()[0];
        }
        if(!brakeIn->empty()){
            state.braking= brakeIn->getBag()[0];
        }
        state.extTrans = true;
    }

    void output(const BackLogicState& state) const override {
        out->addMessage(state.blinkOut());
    }

    [[nodiscard]] double timeAdvance(const BackLogicState& state) const override {
        return state.extTrans?0.0:std::numeric_limits<double>::infinity();
    }
};