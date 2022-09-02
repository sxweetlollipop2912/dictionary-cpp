#include "Trie.h"

static std::vector<int> sort_string(std::vector<p_t> &S) {
    int n = S.size();
    int max_size = 0;
    std::vector<int> cnt(1);
    for (auto &[s, _]: S) {
        int s_size = s.size();
        if (s_size > max_size) {
            max_size = s_size;
            cnt.resize(max_size + 1);
        }
        cnt[s_size]++;
    }
    for (int i = 1; i <= max_size; ++i) cnt[i] += cnt[i - 1];
    std::vector<int> p(n);
    for (int i = n - 1; i >= 0; --i) p[--cnt[S[i].first.length()]] = i;
    std::vector<int> res[2];
    int cur = 0;
    for (int i = max_size, j = n - 1; i >= 0; --i) {
        int nxt = cur ^ 1;
        cnt.assign(127, 0);
        int tmp = j;
        while (j >= 0 && (int) S[p[j]].first.size() >= i - 1) j--;
        cnt[31] = tmp - j;
        for (int id: res[cur]) cnt[S[id].first[i]]++;
        for (int k = 32; k <= 126; ++k) cnt[k] += cnt[k - 1];
        res[nxt].resize(cnt[126]);
        for (int k = j + 1; k <= tmp; ++k) res[nxt][k - j - 1] = p[k];
        for (int k = (int) res[cur].size() - 1; k >= 0; --k) res[nxt][--cnt[S[res[cur][k]].first[i]]] = res[cur][k];
        cur = nxt;
    }
    return res[cur];
}

void TrieNode::all_value_in_subtree(Set<int> &ans, unsigned int count) const {
    if (ans.size() == count) return;

    if (value != -1) ans.insert(value);

    for (auto &[_, child]: edges) {
        child->all_value_in_subtree(ans, count);
    }
}

int TrieNode::lower_bound(char c) const {
    auto i = std::lower_bound(edges.begin(), edges.end(), c, [&](const auto &e, char x) {
        return e.LABEL < x;
    });

    return (int) (i - edges.begin());
}

void Trie::build(const std::vector<p_t> &S, const std::vector<int> &P, int l, int r, int pos, TrieNode *node) {
    if ((int) S[P[l]].first.size() == pos) {
        node->value = S[P[l]].second;
        ++l;
    }
    if (l > r) return;
    for (int i = l; i <= r; ++i)
        if (i == r || S[P[i]].first[pos] != S[P[i + 1]].first[pos]) {
            auto new_node = new TrieNode();
            node->edges.emplace_back(S[P[i]].first[pos], new_node);
            build(S, P, l, i, pos + 1, node->edges.back().NEXT);

            l = i + 1;
        }
}

[[nodiscard]] TrieNode const *Trie::find_node(const std::string &s) const {
    auto cur = root;
    for (char ch: s) {
        auto idx = cur->lower_bound(ch);
        if (idx != cur->edges.size() && cur->edges[idx].LABEL == ch) {
            cur = cur->edges[idx].NEXT;
        } else
            return nullptr;
    }
    return cur;
}

TrieNode *Trie::find_node(const std::string &s) {
    auto cur = root;
    for (char ch: s) {
        auto idx = cur->lower_bound(ch);
        if (idx != cur->edges.size() && cur->edges[idx].LABEL == ch) {
            cur = cur->edges[idx].NEXT;
        } else
            return nullptr;
    }
    return cur;
}

bool Trie::erase(const std::string &s, int value, TrieNode *node, int pos) {
    int idx = (int) node->edges.size();
    TrieNode *next = nullptr;

    if (pos < (int) s.size()) {
        idx = node->lower_bound(s[pos]);
        if (idx != node->edges.size() && node->edges[idx].LABEL == s[pos]) {
            next = node->edges[idx].NEXT;
            if (!next || !erase(s, value, next, pos + 1))
                return false;
        } else if (pos == (int) s.size()) {
            if (node->value != value)
                return false;

            node->value = -1;
        }
    }

    if (next && next->edges.empty() && node->value == -1) {
        delete next;
        node->edges.erase(node->edges.begin() + idx);
    }

    return true;
}

void Trie::clear(TrieNode *node) {
    for (auto &[_, next]: node->edges) {
        clear(next);
        delete next;
    }
    node->edges.clear();
}

void Trie::dfs(std::vector<std::pair<char, int>> &container, const TrieNode *node) const {
    for (auto &[label, next]: node->edges) {
        container.emplace_back(label, next->value);
        dfs(container, next);
        container.emplace_back(0, 0);
    }
}

void Trie::foreach(const std::function<void(TrieNode *)> &function, TrieNode *node) {
    function(node);
    for (auto &[_, next]: node->edges) {
        foreach(function, next);
    }
}

void Trie::foreach(const std::function<void(TrieNode const *)> &function, TrieNode const *node) const {
    function(node);
    for (auto &[_, next]: node->edges) {
        foreach(function, next);
    }
}

Trie::Trie() { root = new TrieNode; }

Trie::Trie(std::vector<p_t> S) {
    root = new TrieNode;

    std::vector<int> P = sort_string(S);
    build(S, P, 0, (int) S.size() - 1, 0, root);
}

Trie::~Trie() {
    clear();
    delete root;
}

bool Trie::insert(const std::string &s, int _value) {
    auto cur = root;

    for (char ch: s) {
        int idx = cur->lower_bound(ch);
        if (idx == cur->edges.size() || cur->edges[idx].LABEL != ch) {
            auto new_node = new TrieNode();
            cur->edges.insert(cur->edges.begin() + idx, {ch, new_node});
        }
        cur = cur->edges[idx].NEXT;
    }

    if (cur->value != -1)
        return false;

    cur->value = _value;
    return true;
}

bool Trie::erase(const std::string &s, int value) {
    return erase(s, value, root, 0);
}

void Trie::clear() {
    clear(root);
}

std::vector<int> Trie::search(const std::string &s, unsigned int count) const {
    Set<int> set;
    auto node = find_node(s);
    if (node)
        node->all_value_in_subtree(set, count);

    return set.to_vec();
}

std::ostream &Trie::save(std::ostream &os) const {
    std::vector<std::pair<char, int>> container;
    dfs(container, root);
    os << container.size() << ' ';
    for (auto query: container) {
        if (query.first) {
            SavableString s;
            s += query.first;
            s.save(os);

            os << query.second << ' ';
        } else
            os << "UP ";
    }

    return os;
}

std::istream &Trie::load(std::istream &is) {
    clear();

    int T;
    is >> T;

    std::stack<TrieNode *> st;
    st.push(root);

    while (T--) {
        SavableString s;
        s.load(is);
        assert(!s.empty());

        assert(!st.empty());
        TrieNode *cur = st.top();

        if (s == "UP")
            st.pop();
        else {
            auto new_node = new TrieNode();
            st.push(new_node);
            is >> st.top()->value;
            cur->edges.emplace_back(s[0], st.top());
        }
    }

    return is;
}