#include "transitions/crossfade.hpp"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void Crossfade::color(glm::vec4& color) {
    float alpha = this->get_time() * glm::radians(this->speed);

    if (alpha >= 1.0) {
        this->done = true;
        alpha = 1.0;
    }

    if (this->direction == Direction::OUT)
        alpha = 1.0 - alpha;

    color[3] = alpha;
}
