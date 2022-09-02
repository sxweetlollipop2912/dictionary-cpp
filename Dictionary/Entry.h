#pragma once

#include <string>
#include <vector>
#include <utility>

#include "../Enums.h"
#include "../Utils.h"
#include "Savable.h"

struct Example : public Savable {
    /**
     * Example <tt>string</tt>.
     */
    SavableString ex;
    /**
     * Translation of the example.
     */
    SavableString meaning;

    Example() = default;

    Example(SavableString, SavableString);

    bool operator==(const Example &other) const;

    std::ostream &save(std::ostream &os) const override {
        ex.save(os);
        meaning.save(os);

        return os;
    }

    std::istream &load(std::istream &is) override {
        ex.load(is);
        meaning.load(is);

        return is;
    }
};

struct Type : public Savable {
    /**
     * name of the type.
     */
    SavableString type;
    /**
     * <tt>vector</tt> of def <tt>strings</tt> belong to this type.
     */
    std::vector<SavableString> defs;
    /**
     * <tt>vector</tt> of <tt>Examples</tt> of the defs.
     */
    std::vector<Example> examples;

    Type() = default;

    explicit Type(std::string);

    bool operator==(const Type &other) const;

    std::ostream &save(std::ostream &os) const override {
        type.save(os);

        os << defs.size() << ' ';
        for (auto &e: defs)
            e.save(os);

        os << examples.size() << ' ';
        for (auto &e: examples)
            e.save(os);

        return os;
    }

    std::istream &load(std::istream &is) override {
        type.load(is);

        std::size_t sz;
        is >> sz;
        defs.resize(sz);
        for (auto i = 0; i < sz; i++)
            defs[i].load(is);

        is >> sz;
        examples.resize(sz);
        for (auto i = 0; i < sz; i++)
            examples[i].load(is);

        return is;
    }
};

struct Entry : public Savable {
public:
    /**
     * <tt>string</tt> of the word.
     */
    SavableString word;
    /**
     * <tt>vector</tt> of different <tt>Types</tt> of the word, i.e. <tt>noun</tt>, <tt>adj</tt>.
     */
    std::vector<Type> types;

    Entry() = default;

    explicit Entry(SavableString);

    Entry(SavableString, std::vector<Type>);

    [[nodiscard]] std::vector<SavableString const *> defs() const;

    SavableString const &random_def() const;

    void display(std::ostream &os = std::cout) const;

    bool operator==(const Entry &other) const;

    virtual ~Entry() = default;

    std::ostream &save(std::ostream &os) const override {
        word.save(os);

        os << types.size() << ' ';
        for (auto &e: types)
            e.save(os);

        return os;
    }

    std::istream &load(std::istream &is) override {
        word.load(is);

        std::size_t sz;
        is >> sz;
        types.resize(sz);
        for (auto i = 0; i < sz; i++)
            types[i].load(is);

        return is;
    }
};