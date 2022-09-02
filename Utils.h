#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "Constants.h"
#include "Enums.h"

class Utils {
public:
    static std::string normalize(std::string const &);

    static std::string trim(std::string const &);

    static std::string to_lower(std::string const &);

    static std::string normalize_lower_separate(std::string const &);

    static std::string out_style(std::string);

    static std::string out_to_normal(std::string);

    static bool read_into_sstream(std::stringstream &, const std::string &);

    static std::string saves_path(DictType);

    static std::string dict_save_path(DictType);

    static std::string fav_save_path(DictType);

    static std::string history_save_path(DictType);
};