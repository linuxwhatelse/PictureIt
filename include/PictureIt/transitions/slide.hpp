#pragma once

#include "config.hpp"
#include "transitions/itransition.hpp"


class Slide: public ITransition {
public:
    Slide() {};
    ~Slide() {};

    void transform(glm::mat4&);
    void color(glm::vec4&) {};
};
