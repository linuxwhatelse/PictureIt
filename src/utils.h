#pragma once

#include <string>
#include <vector>

#include <GL/gl.h>

using namespace std;

namespace PI_UTILS {
    /*!
     * @brief Returns the current time in milliseconds
     * @return current time in milliseconds
     */
    long int get_current_time_ms();

    /*!
     * @brief Joins two strings together and adds a "/" if not present
     * @param a initial path
     * @param b secondary path that should be added to :a:
     * @return path combined from :a: and :b:
     */
    const char* path_join(string a, string b);

    /*!
     * @brief List a directories content
     * @param path the path whichs content should be listed
     * @param store a data-store where found content should be written to
     * @param recursive if the contents should be added recursively
     * @param incl_full_path whether to add the full, absolute path or only the name
     * @param file_filter only add to :store: if the element matches one of the filter criteria
     * @param filter_size size of the filter
     * @return true if successful, false otherwise
     */
    bool list_dir(const char *path, vector<string> &store, bool recursive = false, bool incl_full_path = true, const char *file_filter[] = NULL, int filter_size = 0);

    /*!
     * @brief Loads a image into a OpenGL texture
     * @param img_path path to an image
     * @param texture_id OpenGL texture-id to map bind the image to (check OpenGLs :glGenTextures(): on how to create one)
     * @return true if successful, false otherwise
     */
    bool load_image(const char *img_path, GLuint texture_id);

    /*!
     * @brief Draw an OpenGL texture to the screen
     * @param texture_id OpenGL texture-id representing whatever should be drawn
     * @param tl array where tl[0] = x-pos and tl[1] = y-pos of the textures "TOP LEFT" corner
     * @param tr array where tr[0] = x-pos and tr[1] = y-pos of the textures "TOP RIGHT" corner
     * @param bl array where bl[0] = x-pos and bl[1] = y-pos of the textures "BOTTOM LEFT" corner
     * @param br array where br[0] = x-pos and br[1] = y-pos of the textures "BOTTOM RIGHT" corner
     * @param opacity Opacity of the texture ranging from 0.0f to 1.0f
     *
     * Coordinate-System:
     *  TOP LEFT     : x = -1.0f | y = -1.0f
     *  TOP RIGHT    : x =  1.0f | y = -1.0f
     *  BOTTOM LEFT  : x = -1.0f | y =  1.0f
     *  BOTTOM RIGHT : x =  1.0f | y =  1.0f
     */
    void draw_image(GLuint texture_id, GLfloat tl[] = NULL, GLfloat tr[] = NULL, GLfloat bl[] = NULL, GLfloat br[] = NULL, float opacity = 1.0f);
}