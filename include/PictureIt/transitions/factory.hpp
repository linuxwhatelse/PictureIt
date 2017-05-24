#pragma once

#include "config.hpp"
#include "transitions/itransition.hpp"

#include "transitions/crossfade.hpp"
#include "transitions/slide.hpp"


class TransitionFactory {
public:
    /**
     * Creates a new transition
     * @param t what transition to create
     * @return A new transition instance or nullptr if not found.
     */
    static ITransition* make_transition(PI::Config::Transition::Type t) {
        switch (t) {
            case PI::Config::Transition::Type::CROSSFADE:
                return new Crossfade();

            case PI::Config::Transition::Type::SLIDE:
                return new Slide();
        }

        return nullptr;
    };
};
