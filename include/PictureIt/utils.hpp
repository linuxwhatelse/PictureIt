#pragma once

#include <string>
#include <vector>

using namespace std;

namespace PI_UTILS {
    /*!
     * @brief Joins two strings together and adds a "/" if not present
     * @param a initial path
     * @param b secondary path that should be added to :a:
     * @return path combined from :a: and :b:
     */
    string path_join(string a, string b);

    /*!
     * @brief Check whether a given value ends with a given suffix
     * @param value The value to check against
     * @param the suffix to be found at the end of the given value
     * @return true if found, false otherwise
     */
    bool ends_with(string value, string suffix);

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
     * @brief Get current time in milliseconds
     * @return Current time in milliseconds
     */
    long get_time_in_ms();
}
