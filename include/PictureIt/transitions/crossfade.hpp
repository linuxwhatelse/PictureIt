#pragma once

#include "config.hpp"
#include "transitions/itransition.hpp"


class Crossfade: public ITransition {
public:
    Crossfade() {};
    ~Crossfade() {};

    glm::vec4 color(glm::vec4);
    glm::mat4 transform(glm::mat4 base) { return base; };
};
