#include "utils.hpp"

#if defined(TARGET_LINUX)
    #include <sys/time.h>
#elif defined(TARGET_WINDOWS)
    #include <Windows.h>
    #define GL_CLAMP_TO_EDGE 0x812F
#endif

#include <dirent.h>


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

    bool ends_with(string value, string suffix) {
        return (value.size() >= suffix.size() &&
            value.rfind(suffix) == (value.size() - suffix.size()));
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
                        if (PI_UTILS::ends_with(string(name), string(file_filter[i]))) {
                            add = true;
                            break;
                        }
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

    long get_time_in_ms() {
        #if defined(TARGET_WINDOWS)
            SYSTEMTIME time;
            GetSystemTime(&time);
            return (time.wSecond * 1000) + time.wMilliseconds;
        #else
            timeval time;
            gettimeofday(&time, NULL);
            return (time.tv_sec * 1000) + (time.tv_usec / 1000);
        #endif
    }
}
