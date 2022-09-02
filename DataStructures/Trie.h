#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <stack>
#include <assert.h>

#include "Map.h"
#include "Set.h"
#include "../Dictionary/Savable.h"

#define LABEL first
#define NEXT second

typedef std::pair<std::string, int> p_t;

struct TrieNode {
    std::vector<std::pair<char, TrieNode *>> edges;

    int value = -1; // value = -1 -> not end of string

    TrieNode() = default;

    [[nodiscard]] int lower_bound(char c) const;

    void all_value_in_subtree(Set<int> &, unsigned int) const;
};

class Trie : Savable {
    friend class Dict;

private:
    TrieNode *root;

    void build(const std::vector<p_t> &S, const std::vector<int> &P, int l, int r, int pos, TrieNode *node);

    [[nodiscard]] TrieNode const *find_node(const std::string &) const;

    TrieNode *find_node(const std::string &);

    bool erase(const std::string &s, int value, TrieNode *node, int pos);

    void clear(TrieNode *node);

    void dfs(std::vector<std::pair<char, int>> &Container, const TrieNode *node) const;

    void foreach(const std::function<void(TrieNode *)> &, TrieNode *);

    void foreach(const std::function<void(TrieNode const *)> &, TrieNode const *) const;

public:
    Trie();

    // S.size() must > 1, otherwise bug
    explicit Trie(std::vector<p_t> S);

    ~Trie();

    bool insert(const std::string &s, int _value);

    bool erase(const std::string &s, int value);

    void clear();

    std::vector<int> search(const std::string &s, unsigned int count = -1) const;

    std::ostream &save(std::ostream &os) const override;

    std::istream &load(std::istream &is) override;
};