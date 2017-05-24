#pragma once

#include "config.hpp"
#include "transitions/itransition.hpp"


class Slide: public ITransition {
public:
    Slide() {};
    ~Slide() {};

    glm::vec4 color(glm::vec4 base) { return base; };
    glm::mat4 transform(glm::mat4);
};
