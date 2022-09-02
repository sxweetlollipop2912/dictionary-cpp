#include "Entry.h"

#include <utility>

Example::Example(SavableString example, SavableString meaning) : ex{std::move(example)}, meaning{std::move(meaning)} {}

bool Example::operator==(const Example &other) const {
    return ex == other.ex && meaning == other.meaning;
}

Type::Type(std::string type) : type{std::move(type)} {}

bool Type::operator==(const Type &other) const {
    return type == other.type && defs == other.defs && examples == other.examples;
}

Entry::Entry(SavableString word) : word{std::move(word)} {}

Entry::Entry(SavableString word, std::vector<Type> types) : word{std::move(word)}, types{std::move(types)} {}

std::vector<SavableString const *> Entry::defs() const {
    std::vector<SavableString const *> res;
    for (auto &type: types)
        for (auto &def: type.defs)
            res.push_back(&def);

    return res;
}

SavableString const &Entry::random_def() const {
    auto vec = defs();
    return *vec[rand() % vec.size()];
}

void Entry::display(std::ostream &os) const {
    os << word << '\n';

    for (const auto &type: types) {
        os << "* " << type.type << '\n';
        os << " Def:\n";
        for (const auto &def: type.defs) {
            os << " - " << def << '\n';
        }
        os << " Ex:\n";
        for (const auto &ex: type.examples) {
            os << " - " << ex.ex << " / " << ex.meaning << '\n';
        }
    }
}

bool Entry::operator==(const Entry &other) const {
    return word == other.word && types == other.types;
}
