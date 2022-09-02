#include <fstream>
#include <sstream>

#include "Utils.h"

std::string Utils::normalize(std::string const &s) {
    std::string re;
    for (const auto &c: s) {
        if (c == ' ' && (re.empty() || re.back() == ' '))
            continue;
        re += c;
    }
    if (!re.empty() && re.back() == ' ') re.pop_back();

    return re;
}

std::string Utils::trim(const std::string &s) {
    int l = 0;
    while (l < s.size() && s[l] == ' ') ++l;
    int r = (int) s.size() - 1;
    while (r >= 0 && s[r] == ' ') --r;

    return s.substr(l, r - l + 1);
}

std::string Utils::to_lower(std::string const &s) {
    std::string re(s.begin(), s.end());
    for (auto &c: re) {
        if (c >= 'A' && c <= 'Z')
            c += 'a' - 'A';
    }

    return re;
}

std::string Utils::normalize_lower_separate(std::string const &s) {
    std::string str(s.begin(), s.end());
    for (auto &c: str) {
        if (CONST::CHAR::CHAR_SEPARATOR.contains(c))
            c = ' ';
    }
    str = to_lower(normalize(str));

    return str;
}

std::string Utils::out_style(std::string s) {
    for (auto i = s.find(' '); i < s.size(); i = s.find(' ', i + 1)) {
        s[i] = CONST::CHAR::DUMMY_SPACE;
    }
    for (auto i = s.find('\n'); i < s.size(); i = s.find('\n', i + 1)) {
        s[i] = CONST::CHAR::DUMMY_ENDL;
    }
    if (s.empty()) s += CONST::CHAR::DUMMY_EMPTY;

    return s;
}

std::string Utils::out_to_normal(std::string s) {
    for (auto i = s.find(CONST::CHAR::DUMMY_SPACE); i < s.size(); i = s.find(CONST::CHAR::DUMMY_SPACE, i + 1)) {
        s[i] = ' ';
    }
    for (auto i = s.find(CONST::CHAR::DUMMY_ENDL); i < s.size(); i = s.find(CONST::CHAR::DUMMY_ENDL, i + 1)) {
        s[i] = '\n';
    }
    if (s[0] == CONST::CHAR::DUMMY_EMPTY) s.clear();

    return s;
}

bool Utils::read_into_sstream(std::stringstream &ss, const std::string &path) {
    std::ifstream file(path);
    if (file) {
        /*
         * Get the size of the file
         */
        file.seekg(0, std::ios::end);
        std::streampos length = file.tellg();
        file.seekg(0, std::ios::beg);

        /*
         * Read the whole file into the buffer.
         */
        auto buffer = new char[length];
        file.read(&buffer[0], length);

        file.close();

        /*
         * Create your string stream.
         */
        std::stringstream().swap(ss); // clear stringstream
        ss << buffer;

        delete[] buffer;

        return true;
    }

    return false;
}

std::string Utils::saves_path(DictType type) {
    switch (type) {
        case DictType::VE:
            return CONST::PATH::VE_SAVES;
        case DictType::EV:
            return CONST::PATH::EV_SAVES;
        case DictType::EE:
            return CONST::PATH::EE_SAVES;
        case DictType::Slang:
            return CONST::PATH::SLANG_SAVES;
        case DictType::Emo:
            return CONST::PATH::EMO_SAVES;
        case DictType::Test:
            return CONST::PATH::TEST_SAVES;
        default:
            return {};
    }
}


std::string Utils::dict_save_path(DictType type) {
    return saves_path(type) + CONST::PATH::DICT_SAVE_FILENAME;
}

std::string Utils::fav_save_path(DictType type) {
    return saves_path(type) + CONST::PATH::FAV_SAVE_FILENAME;
}

std::string Utils::history_save_path(DictType type) {
    return saves_path(type) + CONST::PATH::HISTORY_SAVE_FILENAME;
}
