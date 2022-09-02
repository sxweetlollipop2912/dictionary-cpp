#include <filesystem>
#include <sstream>
#include <chrono>

#include "App.h"
#include "Dictionary/Parser.h"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

namespace fs = std::filesystem;

App::App() = default;

App::App(DictType dict_type) : dict{Dict(dict_type)} {}

bool App::load_dict_from_data() {
    auto path =
            dict.type() == DictType::VE ? CONST::PATH::VE_DICT :
            dict.type() == DictType::EV ? CONST::PATH::EV_DICT :
            dict.type() == DictType::EE ? CONST::PATH::EE_DICT :
            dict.type() == DictType::Slang ? CONST::PATH::SLANG_DICT :
            dict.type() == DictType::Emo ? CONST::PATH::EMO_DICT :
            dict.type() == DictType::Test ? CONST::PATH::TEST_DICT :
            "";

    std::stringstream is;
    if (Utils::read_into_sstream(is, path)) {
        auto entries = Parser::parse(is, dict.type());
        for (const auto &entry: entries)
            dict.insert(entry);

        return true;
    } else
        return false;
}

bool App::load_dict() {
    std::stringstream is;
    if (Utils::read_into_sstream(is, Utils::dict_save_path(dict.type()))) {
        std::vector<Dict::size_type *> old_idxes;
        for (auto &e: history_)
            old_idxes.push_back(&e);
        for (auto &e: fav_tmp_)
            old_idxes.push_back(&e);

        dict.load(is, old_idxes);

        return true;
    }

    return false;
}

bool App::save_dict() const {
    std::ofstream ofs(Utils::dict_save_path(dict.type()));
    if (!ofs.is_open())
        return false;

    dict.save(ofs);
    ofs.close();

    return true;
}

bool App::load_history() {
    std::stringstream is;
    if (Utils::read_into_sstream(is, Utils::history_save_path(dict.type()))) {
        std::size_t size_;
        is >> size_;
        history_.resize(size_);
        for (auto i = 0; i < size_; i++)
            is >> history_[i];

        return true;
    }

    return false;
}

bool App::save_history() const {
    std::ofstream ofs(Utils::history_save_path(dict.type()));
    if (!ofs.is_open())
        return false;

    std::size_t del = 0;
    for (const auto &e: history_) {
        del += !dict[e];
    }
    ofs << (history_.size() - del) << ' ';
    for (auto &e: history_) {
        if (dict[e])
            ofs << e << ' ';
    }

    ofs.close();

    return true;
}

bool App::load_favs() {
    std::stringstream is;
    if (Utils::read_into_sstream(is, Utils::fav_save_path(dict.type()))) {
        std::size_t size_;
        is >> size_;
        fav_tmp_.resize(size_);
        for (auto i = 0; i < size_; i++)
            is >> fav_tmp_[i];

        return true;
    }

    return false;
}

bool App::save_favs() const {
    std::ofstream ofs(Utils::fav_save_path(dict.type()));
    if (!ofs.is_open())
        return false;

    auto vec = fav_.to_vec();
    std::size_t del = 0;
    for (const auto &e: vec) {
        del += !dict[e];
    }
    ofs << (vec.size() - del) << ' ';
    for (auto &e: vec) {
        if (dict[e])
            ofs << e << ' ';
    }

    ofs.close();

    return true;
}

bool App::load() {
    if (dict.type() == DictType::Unknown)
        return false;

    auto t1 = high_resolution_clock::now();
    std::cout << "Loading...\n";

    bool re = false;
    if (!load_favs()) {
        std::cout << "Error loading favorites from saves.\n";
    } else re = true;
    if (!load_history()) {
        std::cout << "Error loading history from saves.\n";
    } else re = true;
    if (!load_dict()) {
        std::cout << "Error loading dictionary from saves. Attempting to load from data.\n";
        if (!load_dict_from_data()) {
            std::cout << "Error loading dictionary from data.\n";
        } else re = true;
    } else re = true;

    for (const auto &e: fav_tmp_)
        fav_.insert(e);
    fav_tmp_.clear();

    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << "Loaded " << dict.size() << " entries.\n";
    std::cout << "Loading time: " << ms_double.count() << "ms\n";

    return re;
}

bool App::save() const {
    if (dict.type() == DictType::Unknown)
        return false;

    auto t1 = high_resolution_clock::now();
    std::cout << "Saving...\n";

    fs::remove_all(Utils::saves_path(dict.type()));
    fs::create_directories(Utils::saves_path(dict.type()));

    bool re = true;
    if (!save_favs()) {
        std::cout << "Error saving favorites.\n";
        re = false;
    }
    if (!save_history()) {
        std::cout << "Error saving history.\n";
        re = false;
    }
    if (!save_dict()) {
        std::cout << "Error saving dictionary.\n";
        re = false;
    }

    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << "Saved " << dict.size() << " entries.\n";
    std::cout << "Saving time: " << ms_double.count() << "ms\n";

    return re;
}

std::vector<Dict::size_type> App::favorite(Dict::size_type count) const {
    auto vec = fav_.to_vec();

    std::vector<Dict::size_type> res;
    for (auto i = (int) vec.size() - 1; i >= 0 && res.size() != count; i--)
        if (dict[vec[i]])
            res.push_back(vec[i]);

    return res;
}

void App::add_favorite(Dict::size_type entry) { fav_.insert(entry); }

void App::remove_favorite(Dict::size_type entry) { fav_.erase(entry); }

std::vector<Dict::size_type> App::history(Dict::size_type count) const {
    std::vector<Dict::size_type> res;
    for (auto i = (int) history_.size() - 1; i >= 0 && res.size() != count; i--)
        if ((res.empty() || res.back() != history_[i]) && dict[history_[i]])
            res.push_back(history_[i]);

    return res;
}

void App::add_history(Dict::size_type entry) {
    if (history_.empty() || history_.back() != entry) history_.push_back(entry);
}

void App::reset() {
    dict.clear();
    history_.clear();
    fav_tmp_.clear();
    fav_.clear();

    load_dict_from_data();
}

bool App::set_dict_type(DictType type) {
    if (dict.type() == DictType::Unknown) {
        dict.clear();
        dict.type_ = type;

        return true;
    }

    return false;
}

void App::unload() {
    dict.clear();
    dict.type_ = DictType::Unknown;
    history_.clear();
    fav_tmp_.clear();
    fav_.clear();
}