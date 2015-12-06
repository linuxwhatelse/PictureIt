#include "pictureit.h"
#include "utils.h"

#include <algorithm>
#include <GL/gl.h>

PictureIt::~PictureIt() {
    delete this->efx;

    glDeleteTextures(2, img_texture_ids);
}

EFX PictureIt::get_img_transition_efx() {
    return img_transition_efx;
}

bool PictureIt::set_img_transition_efx(EFX efx) {
    if ( ! img_effect_finished )
        return false;

    delete this->efx;

    switch (efx) {
        default:
        case EFX::CROSSFADE:
            this->efx = new EFXCrossfade();
            img_transition_efx = EFX::CROSSFADE;
            break;
        case EFX::SLIDE:
            this->efx = new EFXSlide();
            img_transition_efx = EFX::SLIDE;
            break;
    }

    this->efx->image_width  = image_width;
    this->efx->image_height = image_height;

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
    int index = rand() % images.size();

    return images[index].c_str();
}

const char* PictureIt::get_next_image() {
    if ( img_current_index < images.size() )
        img_current_index++;
    else
        img_current_index = 0;

    return images[img_current_index].c_str();
}

bool PictureIt::render() {
    start_render();

    if ( ! images.empty() ) {
        if ( img_update_by_interval && img_effect_finished && time(0) >= ( img_last_updated + img_update_interval ))
            img_update = true;

        if ( img_update == true ) {
            img_last_updated    = time(0);
            img_effect_finished = false;
            img_update          = false;

            const char* img_path;
            if ( img_pick_random )
                img_path = get_random_image();
            else
                img_path = get_next_image();

            bool success = PI_UTILS::load_image( img_path, img_texture_ids[1], image_width, image_height );
            if ( ! success ) {
                // Faild loading image, so when drawing the next frame we immediatelly try to get a new one
                // If we'd do it in the "load_image" methode we could end up in an endless-loop in the main-thread
                // if only broken images are available within a preset
                img_update = true;
            } else {
                efx->image_width  = image_width;
                efx->image_height = image_height;
            }
        }

        // Window size might suddenly change, therefore we update it every frame
        efx->window_width  = window_width;
        efx->window_height = window_height;

        if ( img_effect_finished ) {
            // From now on we keep drawing the current image ourself up to the point
            // where a new image will be displayed which will be done by an effect again
            efx->draw_image(img_texture_ids[0]);
        } else {
            if ( glIsTexture(img_texture_ids[0]) )
                img_effect_finished = efx->render(img_texture_ids[0], img_texture_ids[1]);
            else
                img_effect_finished = efx->render(0, img_texture_ids[1]);

            // Effect finished, therefore we have to swapp the position of both textures
            if ( img_effect_finished ) {
                swap(img_texture_ids[0], img_texture_ids[1]);

                // e.g. Kodi needs that. Without it, it looks like one frame is missing once
                // the effect finished.
                // It doesn't hurt so it's fine for now
                efx->draw_image(img_texture_ids[0]);
            }
        }
    }

    if ( spectrum_enabled )
        draw_spectrum();

    finish_render();

    return img_effect_finished;
}

void PictureIt::update_image(bool force_update) {
    if (force_update)
        img_update = true;
    else
        if (img_effect_finished)
            img_update = true;
}

void PictureIt::load_images(const char *image_root_dir) {
    img_current_index = -1;

    images.clear();
    PI_UTILS::list_dir(image_root_dir, images, true, true, image_filter, sizeof(image_filter));
    sort(images.begin(), images.end());
}
