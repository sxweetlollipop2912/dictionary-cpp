#pragma once

#include "AVLTree.h"

template<typename T_Key>
class Set : public AVLTree<T_Key> {
public:
    using value_type = T_Key;
    using size_type = std::size_t;

private:
    using Super = AVLTree<T_Key>;

public:
    Set() = default;

    explicit Set(const std::function<bool(const value_type &, const value_type &)> &comp)
            : AVLTree<T_Key>(comp) {}

    Set(const std::initializer_list<value_type> &source) {
        for (const auto &e: source) Super::insert(e);
    }
};