#include "utils.hpp"

//#include <fnmatch.h> //TODO: how to use fnmatch.h under Windows?
#if defined(TARGET_WINDOWS)
  #include <Windows.h>
  #define GL_CLAMP_TO_EDGE 0x812F
#endif

#include <GL/gl.h>

#include <dirent.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#define STBI_ONLY_BMP
#include "stb_image.h"

namespace PI_UTILS {
    string path_join(string a, string b) {
        // Apparently Windows does understand a "/" just fine...
        // Haven't tested it though, but for now I'm just believing it
    
        // a ends with "/" so lets remove it
        if ( a.substr( a.length() - 1, a.length() ) == "/" )
            a = a.substr( 0, a.size() - 1 );
    
        // b starts with "/" so lets remove it
        if ( b.substr( 0, 1 ) == "/" )
            b = b.substr( 1, b.size() );
    
        // b ends with "/" so lets remove it
        if ( b.substr( b.length() - 1, b.length() ) == "/" )
            b = b.substr( 0, b.size() -1 );

        return a + "/" + b;
    }
    
    bool list_dir(const char *path, vector<string> &store, bool recursive, bool incl_full_path, const char *file_filter[], int filter_size) {
        string p = path;
        struct dirent *entry;
        DIR *dp;
    
        dp = opendir(path);
        if (dp == NULL)
            return false;
    
        bool add = false;
        char* name;
        while ((entry = readdir(dp))) {
            name = entry->d_name;
    
            if ( entry->d_type == DT_DIR && name && name[0] != '.' ) {
                if ( ! file_filter ) {
                    add = true;
                }
    
                if ( recursive )
                    PI_UTILS::list_dir( PI_UTILS::path_join( p, name).c_str(), store, recursive, incl_full_path, file_filter, filter_size );
            } else if ( entry->d_type != DT_DIR && name && name[0] != '.' ) {
                if ( file_filter ) {
                    for ( unsigned int i = 0; i < filter_size / sizeof( file_filter[0] ); i++) {
						//TODO: how to use fnmatch.h under Windows?
                        //if ( fnmatch( file_filter[i], name, FNM_CASEFOLD ) == 0) {
                        //    add = true;
                        //    break;
                        //}
                    }
                }
            }
    
            if ( add ) {
                if ( incl_full_path ) {
                    store.push_back( PI_UTILS::path_join( p, name ).c_str() );
                } else {
                    store.push_back( name );
                }
                add = false;
            }
        }
    
        closedir(dp);
        return true;
    }

    bool load_image(const char *img_path, GLuint texture_id, int &width, int &height) {
        if ( ! texture_id ) {
            return false;
        }

        int x, y, n;
        unsigned char *data = stbi_load(img_path, &x, &y, &n, 0);
    
        width = x;
        height = y;

        if(data == nullptr) {
            return false;
        }
    
        glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture_id);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,      GL_CLAMP_TO_EDGE );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,      GL_CLAMP_TO_EDGE );
        
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glDisable(GL_TEXTURE_2D);

        stbi_image_free(data);

        return true;
    }
}
