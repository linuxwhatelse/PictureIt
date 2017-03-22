#pragma once

#include "IRenderer.hpp"

#include <GL/gl.h>


class OGLRenderer : public IRenderer {
    public:
        ~OGLRenderer() {};

        void init() {};

        void prepare();
        void finish();
};

