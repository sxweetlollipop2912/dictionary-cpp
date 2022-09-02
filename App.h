#pragma once

#include <vector>

#include "Dictionary/Dict.h"
#include "Enums.h"
#include "UI/SceneManager.h"

class App {
private:
    std::vector<Dict::size_type> history_, fav_tmp_;
    Set<Dict::size_type> fav_;

    bool load_dict_from_data();

    /**
     * Load dictionary from save file.
     *
     * @return <tt>false</tt> if save file cannot be opened. Otherwise <tt>true</tt>.
     */
    bool load_dict();

    /**
     * Save dictionary to save file.
     *
     * @return <tt>false</tt> if there is any directory failure. Otherwise <tt>true</tt>.
     */
    bool save_dict() const;

    /**
     * Load favorites from save file.
     *
     * @return <tt>false</tt> if save file cannot be opened. Otherwise <tt>true</tt>.
     */
    bool load_favs();

    /**
     * Save favorites to save file.
     *
     * @return <tt>false</tt> if there is any directory failure. Otherwise <tt>true</tt>.
     */
    bool save_favs() const;

    /**
     * Load history from save file.
     *
     * @return <tt>false</tt> if save file cannot be opened. Otherwise <tt>true</tt>.
     */
    bool load_history();

    /**
     * Save history to save file.
     *
     * @return <tt>false</tt> if there is any directory failure. Otherwise <tt>true</tt>.
     */
    bool save_history() const;

public:
    Dict dict;

    SceneManager scenes;

    App();

    explicit App(DictType);

    /**
     * Set type for dictionary. Only works if type is not set yet.
     *
     * @param type <tt>DictType</tt> to be set.
     * @return <tt>false</tt> if type is already set, otherwise true.
     */
    bool set_dict_type(DictType type);

    /**
     * Unload all data (including dict type). Note that this does not save.
     */
    void unload();

    /**
     * Load dictionary, history, favorites from saves.\n
     * Attempt to load dictionary from raw data if save is not found/readable.
     *
     * @return <tt>false</tt> if all save files cannot be opened. Otherwise <tt>true</tt>.
     */
    bool load();

    /**
     * Save dictionary, history, favorites to saves.
     *
     * @return <tt>false</tt> if there is any directory failure. Otherwise <tt>true</tt>.
     */
    bool save() const;

    /**
     * Get a <tt>count</tt> of favorite words of current dictionary.
     *
     * @param count number of words. <tt>-1</tt> to get all.
     * @return <tt>vector</tt> of word indexes.
     */
    [[nodiscard]] std::vector<Dict::size_type> favorite(Dict::size_type count = -1) const;

    /**
     * Add a word to favorites of current dictionary.
     *
     * @param index of the word.
     */
    void add_favorite(Dict::size_type index);

    /**
     * Remove a word from favorites of current dictionary.
     *
     * @param index of the word.
     */
    void remove_favorite(Dict::size_type index);

    /**
     * Get a <tt>count</tt> of recently searched words of current dictionary.
     *
     * @param count number of words. <tt>-1</tt> to get all.
     * @return <tt>vector</tt> of word indexes.
     */
    [[nodiscard]] std::vector<Dict::size_type> history(Dict::size_type count = -1) const;

    /**
     * Add a word to history of current dictionary (treated as most recent).
     *
     * @param index of the word.
     */
    void add_history(Dict::size_type index);

    /**
     * Reset everything, with respect to current dict type.
     */
    void reset();
};