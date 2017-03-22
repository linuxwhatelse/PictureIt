#include "abstract/OGLRenderer.hpp"


void OGLRenderer::prepare() {
    // save OpenGL original state
    glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT);

    // Clear The Screen And The Depth Buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // OpenGL projection matrix setup
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // Coordinate-System:
    //     screen top left:     ( -1, -1 )
    //     screen center:       (  0,  0 )
    //     screen bottom right: (  1,  1 )
    glOrtho( -1, 1, 1, -1, -1, 1 );

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}


void OGLRenderer::finish() {
    // return OpenGL matrices to original state
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    // restore OpenGl original state
    glPopAttrib();
}
