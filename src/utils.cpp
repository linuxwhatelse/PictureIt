#include "utils.hpp"

#include <chrono>
#include <dirent.h>
#include <random>


namespace PI { namespace Utils {
    string path_join(string a, string b) {
        // Apparently Windows does understand a "/" just fine...
        // Haven't tested it though, but for now I'm just believing it

        // a ends with "/" so lets remove it
        if (a.substr(a.length() - 1, a.length()) == "/")
            a = a.substr(0, a.size() - 1);

        // b starts with "/" so lets remove it
        if (b.substr(0, 1) == "/")
            b = b.substr(1, b.size());

        // b ends with "/" so lets remove it
        if (b.substr(b.length() - 1, b.length()) == "/")
            b = b.substr(0, b.size() -1);

        return a + "/" + b;
    }

    bool ends_with(string value, string suffix) {
        return (value.size() >= suffix.size() &&
            value.rfind(suffix) == (value.size() - suffix.size()));
    }

    bool list_dir(const char *path, vector<string>& store, bool recursive,
                  bool incl_full_path, const char *file_filter[], int filter_size) {
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

            if (entry->d_type == DT_DIR && name && name[0] != '.') {
                if (! file_filter) {
                    add = true;
                }

                if (recursive)
                    list_dir(
                        path_join(p, name).c_str(),
                        store,
                        recursive,
                        incl_full_path,
                        file_filter,
                        filter_size);
            } else if (entry->d_type != DT_DIR && name && name[0] != '.') {
                if (file_filter) {
                    for (unsigned int i = 0; i < filter_size / sizeof(file_filter[0]); i++) {
                        if (ends_with(string(name),
                                                string(file_filter[i]))) {
                            add = true;
                            break;
                        }
                    }
                }
            }

            if (add) {
                if (incl_full_path) {
                    store.push_back(path_join(p, name).c_str());
                } else {
                    store.push_back(name);
                }
                add = false;
            }
        }

        closedir(dp);
        return true;
    }

    long get_time_in_ms() {
        return std::chrono::system_clock::now().time_since_epoch() /
            std::chrono::milliseconds(1);
    }

    int prev_rand_num = -1;
    int get_random(int begin, int end) {
        std::random_device rd;
        std::mt19937 engine{rd()};
        std::uniform_int_distribution<int> dist(begin, end);

        int num = dist(engine);
        if (num == prev_rand_num)
            return get_random(begin, end);

        prev_rand_num = num;
        return num;
    }
}}
