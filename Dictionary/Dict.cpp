#include <algorithm>

#include "Dict.h"

DictEntry::DictEntry() : erased{false} {}

DictEntry::DictEntry(const Super &entry) : Super(entry), erased{false} {}

Dict::Dict() : type_{DictType::Unknown}, erased_{0} {}

Dict::Dict(DictType type) : type_{type}, erased_{0} {}

Dict::size_type Dict::size() const { return entries_.size() - erased_; }

DictType Dict::type() const { return type_; }

bool Dict::is_erased(size_type idx) const {
    return entries_[idx].erased;
}

DictEntry *Dict::at_idx(size_type idx) {
    return idx >= entries_.size() || is_erased(idx) ? nullptr : &entries_[idx];
}

DictEntry const *Dict::at_idx(size_type idx) const {
    return idx >= entries_.size() || is_erased(idx) ? nullptr : &entries_[idx];
}

Entry const *Dict::operator[](size_type idx) const { return at_idx(idx); }

Entry const *Dict::at(size_type idx) const {
    return !at_idx(idx) ? nullptr : &entries_.at(idx);
}

Dict::size_type Dict::insert(const Entry &entry) {
    entries_.push_back(DictEntry(entry));
    auto idx = entries_.size() - 1;

    auto &new_entry = entries_.back();
    words_.insert(new_entry.word, (int) idx);
    auto defs = new_entry.defs();
    for (auto &def: defs)
        defs_.insert(*def, (int) idx);

    return idx;
}

void Dict::erase(size_type idx) {
    if (idx != entries_.size() && !is_erased(idx)) {
        words_.erase(at_idx(idx)->word, (int) idx);
        auto defs = at_idx(idx)->defs();
        for (auto &def: defs)
            defs_.erase(*def, (int) idx);

        erased_ += 1;
        at_idx(idx)->erased = true;
    }
}

void Dict::clear() {
    entries_.clear();
    words_.clear();
    defs_.clear();
    erased_ = 0;
}

std::vector<Dict::size_type> Dict::search_word(const std::string &query, size_type count) const {
    auto res = words_.search(query, count);

    return {res.begin(), res.end()};
}

std::vector<Dict::size_type> Dict::search_def(const std::string &query, size_type count) const {
    auto res = defs_.search(query, count);

    return {res.begin(), res.end()};
}

bool Dict::edit_def(size_type idx, unsigned int type_idx, unsigned int def_idx, SavableString def) {
    if (idx == entries_.size()
        || is_erased(idx)
        || at_idx(idx)->types.size() <= type_idx
        || at_idx(idx)->types[type_idx].defs.size() <= def_idx) {

        return false;
    }

    auto &entry_def = at_idx(idx)->types[type_idx].defs[def_idx];
    defs_.erase(entry_def, (int) idx);
    entry_def = std::move(def);
    defs_.insert(entry_def, (int) idx);

    return true;
}

bool Dict::add_def(size_type idx, unsigned int type_idx, std::string new_def) {
    if (idx == entries_.size()
        || is_erased(idx)
        || at_idx(idx)->types.size() <= type_idx) {

        return false;
    }

    defs_.insert(new_def, (int) idx);
    auto &type = at_idx(idx)->types[type_idx];
    type.defs.push_back(std::move(new_def));

    return true;
}

bool Dict::delete_def(size_type idx, unsigned int type_idx, unsigned int def_idx) {
    if (idx == entries_.size()
        || is_erased(idx)
        || at_idx(idx)->types.size() <= type_idx
        || at_idx(idx)->types[type_idx].defs.size() <= def_idx) {

        return false;
    }

    auto &type = at_idx(idx)->types[type_idx];
    defs_.erase(type.defs[def_idx], (int) idx);
    type.defs.erase(type.defs.begin() + def_idx);

    return true;
}

bool Dict::add_type(Dict::size_type idx, std::string new_type) {
    if (idx == entries_.size()
        || is_erased(idx)) {

        return false;
    }

    at_idx(idx)->types.emplace_back(new_type);

    return true;
}

bool Dict::delete_type(Dict::size_type idx, Dict::size_type type_idx) {
    if (idx == entries_.size()
        || is_erased(idx)
        || at_idx(idx)->types.size() <= type_idx) {

        return false;
    }

    auto &type = at_idx(idx)->types[type_idx];
    for(auto &def : type.defs) {
        defs_.erase(def, (int) idx);
    }
    at_idx(idx)->types.erase(at_idx(idx)->types.begin() + (int) type_idx);

    return true;
}

bool Dict::edit_example(size_type idx, unsigned int type_idx, unsigned int ex_idx, Example ex) {
    if (idx == entries_.size()
        || is_erased(idx)
        || at_idx(idx)->types.size() <= type_idx
        || at_idx(idx)->types[type_idx].examples.size() <= ex_idx) {

        return false;
    }

    at_idx(idx)->types[type_idx].examples[ex_idx] = std::move(ex);

    return true;
}

Dict::size_type Dict::random() const {
    size_type idx;
    do {
        idx = rand() % entries_.size();
    } while (is_erased(idx));

    return idx;
}

void Dict::debug() const {
    std::cout << "size " << size() << '\n';
    for (size_type idx = 0; idx < entries_.size(); idx++) {
        std::cout << idx + 1 << ".\n";
        if (!is_erased(idx))
            std::cout << "Word: " << at_idx(idx)->word << "\nTypes: " << at_idx(idx)->types.size()
                      << '\n';
    }
}
