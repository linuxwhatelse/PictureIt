#pragma once

#include "config.hpp"
#include "transitions/itransition.hpp"


class Crossfade: public ITransition {
public:
    Crossfade() {};
    ~Crossfade() {};

    void transform(glm::mat4&) {};
    void color(glm::vec4&);
};
