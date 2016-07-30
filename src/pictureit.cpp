#include "pictureit.hpp"
#include "utils.hpp"

#include <algorithm>
#include <GL/gl.h>

const char *PictureIt::image_filter[3] = { ".jpg", ".png", ".jpeg" };

PictureIt::~PictureIt() {
    delete this->efx;

    glDeleteTextures(2, this->img_texture_ids);
}

EFX PictureIt::get_img_transition_efx() {
    return this->img_transition_efx;
}

bool PictureIt::set_img_transition_efx(EFX efx) {
    if ( ! this->img_effect_finished )
        return false;

    MODE image_mode = MODE::ZOOM;
    if ( this->efx )
        image_mode = this->efx->image_mode;

    delete this->efx;

    switch ( efx ) {
        default:
        case EFX::CROSSFADE:
            this->efx = new EFXCrossfade();
            this->img_transition_efx = EFX::CROSSFADE;
            break;
        case EFX::SLIDE:
            this->efx = new EFXSlide();
            this->img_transition_efx = EFX::SLIDE;
            break;
    }

    this->efx->image_mode = image_mode;

    this->efx->new_image_width  = this->image_width;
    this->efx->new_image_height = this->image_height;

    return true;
};

void PictureIt::start_render() {
    // save OpenGL original state
    glPushAttrib( GL_ENABLE_BIT | GL_TEXTURE_BIT );

    // Clear The Screen And The Depth Buffer
    glClear( GL_COLOR_BUFFER_BIT );

    // OpenGL projection matrix setup
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();

    // Coordinate-System:
    //     screen top left:     ( -1, -1 )
    //     screen center:       (  0,  0 )
    //     screen bottom right: (  1,  1 )
    glOrtho( -1, 1, 1, -1, -1, 1 );

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
}

void PictureIt::finish_render() {
    // return OpenGL matrices to original state
    glPopMatrix();
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();

    // restore OpenGl original state
    glPopAttrib();
}

const char* PictureIt::get_random_image() {
    int index = rand() % this->images.size();

    return this->images[index].c_str();
}

const char* PictureIt::get_next_image() {
    if ( this->img_current_index < this->images.size() )
        this->img_current_index++;
    else
        this->img_current_index = 0;

    return this->images[this->img_current_index].c_str();
}

bool PictureIt::render() {
    start_render();

    if ( ! this->images.empty() ) {
        if ( this->img_update_by_interval && this->img_effect_finished &&
            PI_UTILS::get_time_in_ms() >= ( this->img_last_updated + (this->img_update_interval * 1000) ))
            img_update = true;

        if ( this->img_update == true ) {
            this->img_last_updated    = PI_UTILS::get_time_in_ms();
            this->img_effect_finished = false;
            this->img_update          = false;

            const char* img_path;
            if ( this->img_pick_random )
                img_path = get_random_image();
            else
                img_path = get_next_image();

            efx->old_image_width  = efx->new_image_width;
            efx->old_image_height = efx->new_image_height;

            bool success = PI_UTILS::load_image( img_path, img_texture_ids[1], image_width, image_height );
            if ( ! success ) {
                // Faild loading image, so when drawing the next frame we immediatelly try to get a new one
                // If we'd do it in the "load_image" methode we could end up in an endless-loop in the main-thread
                // if only broken images are available within a preset
                this->img_update = true;
            } else {
                efx->new_image_width  = this->image_width;
                efx->new_image_height = this->image_height;
            }
        }

        // Window size might suddenly change, therefore we update it every frame
        efx->window_width  = this->window_width;
        efx->window_height = this->window_height;

        if ( this->img_effect_finished ) {
            // From now on we keep drawing the current image ourself up to the point
            // where a new image will be displayed which will be done by an effect again
            efx->draw_image(this->img_texture_ids[0], true);
        } else {
            if ( glIsTexture(this->img_texture_ids[0]) ) {
                this->img_effect_finished = efx->render(this->img_texture_ids[0], this->img_texture_ids[1]);
            } else {
                this->img_effect_finished = efx->render(0, this->img_texture_ids[1]);
            }

            // Effect finished, therefore we have to swapp the position of both textures
            if ( this->img_effect_finished ) {
                swap(this->img_texture_ids[0], this->img_texture_ids[1]);

                // e.g. Kodi needs that. Without it, it looks like one frame is missing once
                // the effect finished.
                // It doesn't hurt so it's fine for now
                efx->draw_image(this->img_texture_ids[0], true);
            }
        }
    }

    if ( this->spectrum_enabled ) {
        draw_spectrum();
    }

    finish_render();

    return this->img_effect_finished;
}

void PictureIt::update_image(bool force_update) {
    if ( force_update ) {
        this->img_update = true;
    } else {
        if ( this->img_effect_finished ) {
            this->img_update = true;
        }
    }
}

void PictureIt::load_images(const char *image_root_dir) {
    img_current_index = -1;
    images.clear();

    PI_UTILS::list_dir(image_root_dir, this->images, true, true, this->image_filter, sizeof( this->image_filter ));
    sort( this->images.begin(), this->images.end() );
}
