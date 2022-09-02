#pragma once

#include <iostream>
#include "../Utils.h"

struct Savable {
    virtual std::ostream &save(std::ostream &os) const = 0;

    virtual std::istream &load(std::istream &is) = 0;
};

struct SavableString : public std::string, public Savable {
    using Super = std::string;

    SavableString() = default;

    SavableString(Super);

    std::ostream &save(std::ostream &os) const override {
        os << Utils::out_style((Super &) (*this)) << ' ';
        return os;
    }

    std::istream &load(std::istream &is) override {
        is >> (Super &) (*this);
        (Super &) (*this) = Utils::out_to_normal((Super &) (*this));
        return is;
    }
};