#pragma once

#include <memory>
#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>


using namespace std::chrono;


class ITransition {
protected:
    bool started = false;
    bool done = false;

public:
    virtual ~ITransition() {};

    enum Direction {
        IN,
        OUT
    };

    high_resolution_clock::time_point t_start;
    float speed = 100.0;

    Direction direction = Direction::IN;

    void start() {
        started = true;
        t_start = high_resolution_clock::now();
    };

    float get_time() {
        auto t_now = high_resolution_clock::now();
        return duration_cast<duration<float>>(t_now - this->t_start).count();
    }

    bool is_started() { return this->started; };
    bool is_done() { return this->done; };

    virtual void transform(glm::mat4& mvp) = 0;
    virtual void color(glm::vec4& mvp) = 0;
};
