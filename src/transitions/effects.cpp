#include "effects/effects.hpp"

#include <sys/time.h>
#include <GL/gl.h>

#include <algorithm>

EFXBase::EFXBase() {
    this->tl = new GLfloat[2]();
    this->tr = new GLfloat[2]();
    this->bl = new GLfloat[2]();
    this->br = new GLfloat[2]();

    this->tex_tl = new GLfloat[2]();
    this->tex_tr = new GLfloat[2]();
    this->tex_bl = new GLfloat[2]();
    this->tex_br = new GLfloat[2]();
}
EFXBase::~EFXBase() {
    delete[] this->tl;
    delete[] this->tr;
    delete[] this->bl;
    delete[] this->br;

    delete[] this->tex_tl;
    delete[] this->tex_tr;
    delete[] this->tex_bl;
    delete[] this->tex_br;
}

long int EFXBase::get_current_time_ms() {
    struct timeval current_time;
    gettimeofday( &current_time, NULL );

    return current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
}