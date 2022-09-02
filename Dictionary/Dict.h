#pragma once

#include <vector>
#include <set>

#include "../DataStructures/Set.h"
#include "Entry.h"
#include "../DataStructures/Map.h"
#include "../DataStructures/Trie.h"
#include "Savable.h"

struct DictEntry : public Entry {
    using Super = Entry;

    bool erased;

    DictEntry();

    DictEntry(const Super &);
};

class Dict : public Savable {
    friend class App;

private:
    using entries_t = std::vector<DictEntry>;

public:
    using size_type = typename entries_t::size_type;

    const size_type npos = -1;

private:
    DictType type_;
    entries_t entries_;
    Trie words_, defs_;

    size_type erased_;

    /// O(1)
    [[nodiscard]] DictEntry *at_idx(size_type);

    /// O(1)
    [[nodiscard]] DictEntry const *at_idx(size_type) const;

    [[nodiscard]] bool is_erased(size_type) const;

    Dict();

    explicit Dict(DictType);

public:
    /**
     * @return Number of words of current dictionary.
     */
    [[nodiscard]] size_type size() const;

    /**
     * @return Enum <tt>DictType</tt> of current dictionary.
     */
    [[nodiscard]] DictType type() const;

    /**
     * Time complexity: O(1)
     *
     * @param index of an <tt>Entry</tt> (a word) in dictionary.
     * @return
     * const pointer to the <tt>Entry</tt> at <tt>index</tt>.\n
     * <tt>nullptr</tt> if <tt>Entry</tt> is erased, or <tt>index</tt> is invalid.
     */
    Entry const *operator[](size_type index) const;

    /**
     * Time complexity: O(1)
     *
     * @param index of an <tt>Entry</tt> (a word) in dictionary.
     * @return
     * const pointer to the <tt>Entry</tt> at <tt>index</tt>.\n
     * <tt>nullptr</tt> if <tt>Entry</tt> is erased, or <tt>index</tt> is invalid.
     */
    [[nodiscard]] Entry const *at(size_type index) const;

    /**
     * Insert an <tt>Entry</tt> into dictionary.\n
     * Time complexity: TODO: tree complexity
     *
     * @param entry <tt>Entry</tt> of the word.
     * @return index of the inserted <tt>Entry</tt>.
     */
    size_type insert(const Entry &entry);

    /**
     * Erase a word from dictionary.\n
     * Time complexity: TODO: tree complexity
     *
     * @param index of the word to be erased.
     */
    void erase(size_type index);

    /**
     * Clear the dict (dict type is kept).
     */
    void clear();

    /**
     * Query search <tt>Entries</tt> based on their <tt>word</tt> field.\n
     * Time complexity: TODO: tree complexity
     *
     * @param query <tt>string</tt> query to search by.
     * @param count max number of <tt>Entries</tt> to be returned.
     * @return <tt>vector</tt> of indexes of returned <tt>Entries</tt>.
     */
    [[nodiscard]] std::vector<size_type> search_word(const std::string &query, size_type count = -1) const;

    /**
     * Query search <tt>Entries</tt> based on their <tt>types.defs</tt> field.\n
     * Time complexity: TODO: tree complexity
     *
     * @param query <tt>string</tt> query to search by.
     * @param count max number of <tt>Entries</tt> to be returned.
     * @return <tt>vector</tt> of indexes of returned <tt>Entries</tt>.
     */
    [[nodiscard]] std::vector<size_type> search_def(const std::string &, size_type = -1) const;

    /**
     * Edit an <tt>Entry</tt> def.\n
     * Time complexity: TODO: tree complexity
     *
     * @param index of the <tt>Entry</tt>.
     * @param type_index index of the <tt>Type</tt> that holds the desired def.
     * @param def_index index of the desired def in the above <tt>Type</tt>.
     * @param new_def <tt>LangString</tt> of the new def to replaced with.
     * @return <tt>false</tt> if indexes are invalid, or <tt>new_def</tt> language is incorrect. Otherwise <tt>true</tt>.
     */
    bool edit_def(size_type index, unsigned int type_index, unsigned int def_index, SavableString new_def);

    /**
     * Edit an <tt>Entry</tt> def.\n
     * Time complexity: TODO: tree complexity
     *
     * @param index of the <tt>Entry</tt>.
     * @param type_index index of the <tt>Type</tt> that holds the desired def.
     * @param new_def <tt>LangString</tt> of the new def to replaced with.
     * @return <tt>false</tt> if indexes are invalid. Otherwise <tt>true</tt>.
     */
    bool add_def(size_type index, unsigned int type_index, std::string new_def);

    /**
     * Edit an <tt>Entry</tt> def.\n
     * Time complexity: TODO: tree complexity
     *
     * @param index of the <tt>Entry</tt>.
     * @param type_index index of the <tt>Type</tt> that holds the desired def.
     * @param new_def <tt>LangString</tt> of the new def to replaced with.
     * @return <tt>false</tt> if indexes are invalid. Otherwise <tt>true</tt>.
     */
    bool delete_def(size_type index, unsigned int type_index, unsigned int def_index);

    /**
     * Edit an <tt>Entry</tt> def.\n
     * Time complexity: TODO: tree complexity
     *
     * @param index of the <tt>Entry</tt>.
     * @param type_index index of the <tt>Type</tt> that holds the desired def.
     * @param new_def <tt>LangString</tt> of the new def to replaced with.
     * @return <tt>false</tt> if indexes are invalid. Otherwise <tt>true</tt>.
     */
    bool add_type(size_type index, std::string new_type);

    /**
     * Edit an <tt>Entry</tt> def.\n
     * Time complexity: TODO: tree complexity
     *
     * @param index of the <tt>Entry</tt>.
     * @param type_index index of the <tt>Type</tt> that holds the desired def.
     * @param def_index index of the desired def in the above <tt>Type</tt>.
     * @param new_def <tt>LangString</tt> of the new def to replaced with.
     * @return <tt>false</tt> if indexes are invalid. Otherwise <tt>true</tt>.
     */
    bool delete_type(size_type index, size_type type_index);

    /**
     * Edit an <tt>Entry</tt> example.\n
     * Time complexity: O(1)
     *
     * @param index of the <tt>Entry</tt>.
     * @param type_index index of the <tt>Type</tt> that holds the desired <tt>Example</tt>.
     * @param example_index index of the desired <tt>Example</tt> in the above <tt>Type</tt>.
     * @param new_example the new <tt>Example</tt> to replaced with.
     * @return <tt>false</tt> if indexes are invalid, or <tt>new_example</tt> language is incorrect. Otherwise <tt>true</tt>.
     */
    bool edit_example(size_type index, unsigned int type_index, unsigned int example_index, Example new_example);

    /**
     * @return <tt>index</tt> of a random word.
     */
    [[nodiscard]] size_type random() const;

    void debug() const;

    // TODO: Implement save trie from txt
    std::ostream &save(std::ostream &os) const override {
        os << (int) type_ << ' ';

        os << entries_.size() << ' ';
        os << size() << ' ';
        for (auto i = 0; i < entries_.size(); i++) {
            if (!is_erased(i)) {
                entries_[i].save(os);
                os << i << ' ';
            }
        }

        words_.save(os);
        defs_.save(os);

        return os;
    }

    // TODO: Implement load trie from txt
    std::istream &load(std::istream &is) override {
        int i;
        is >> i;
        type_ = (DictType) i;

        size_type old_size_, size_;
        is >> old_size_ >> size_;
        entries_.resize(size_);

        for (i = 0; i < size_; i++) {
            entries_t::value_type e;
            e.load(is);

            size_type old_idx;
            is >> old_idx;

            entries_[i] = e;
            words_.insert(entries_[i].word, i);
            auto defs = entries_[i].defs();
            for (auto &def: defs)
                defs_.insert(*def, i);
        }

        words_.load(is);
        defs_.load(is);

        return is;
    }

    // TODO: Implement load trie from txt
    std::istream &load(std::istream &is, std::vector<size_type *> &old_idxes) {
        int i;
        is >> i;
        type_ = (DictType) i;

        size_type old_size_, size_;
        is >> old_size_ >> size_;
        entries_.resize(size_);

        std::vector<size_type> map(old_size_, npos);
        for (i = 0; i < size_; i++) {
            entries_t::value_type e;
            e.load(is);

            size_type old_idx;
            is >> old_idx;
            map[old_idx] = i;

            entries_[i] = e;
            words_.insert(entries_[i].word, i);
            auto defs = entries_[i].defs();
            for (auto &def: defs)
                defs_.insert(*def, i);
        }

        words_.load(is);
        defs_.load(is);

        for (auto &e: old_idxes) {
            if (*e < map.size())
                *e = map[*e];
        }
        words_.foreach([&](TrieNode *node) {
            if (node->value < map.size())
                node->value = (int) map[node->value];
        }, words_.root);
        defs_.foreach([&](TrieNode *node) {
            if (node->value < map.size())
                node->value = (int) map[node->value];
        }, defs_.root);

        return is;
    }
};