#pragma once

#include <string>
#include <algorithm>
#include <stdexcept>

#include "Entry.h"
#include "../Utils.h"
#include "../Constants.h"
#include "../Enums.h"

namespace Parser {
    struct Chunk {
        std::vector<std::string> lines;

        Chunk() = default;
    };

    Entry parse_chunk(const Chunk &chunk, DictType dict_type) {
        Entry entry;
        for (const auto &line: chunk.lines) {
            if (line.size() < 2) continue;

            auto c = line[0];
            auto content = Utils::normalize(line.substr(1));

            switch (c) {
                case CONST::CHAR::NEW_WORD: {
                    if (!entry.word.empty())
                        throw std::logic_error("Two instances of word in a chunk.");

                    entry.word = content;

                    break;
                }
                case CONST::CHAR::TYPE: {
                    entry.types.emplace_back(content);

                    break;
                }
                case CONST::CHAR::DEF: {
                    if (entry.types.empty())
                        entry.types.emplace_back();
                    auto &type = entry.types.back();
                    type.defs.emplace_back(content);

                    break;
                }
                case CONST::CHAR::EXAMPLE: {
                    auto pos = content.find(CONST::CHAR::EX_MEANING);
                    auto ex = Utils::trim(content.substr(0, pos));
                    auto meaning = Utils::trim(content.substr(pos + 1));

                    if (entry.types.empty())
                        entry.types.emplace_back();
                    auto &type = entry.types.back();
                    type.examples.emplace_back(ex, meaning);

                    break;
                }
                default:
                    break;
            }
        }

        return entry;
    }

    std::vector<Entry> parse(std::istream &is, DictType dict_type, const int count = -1) {
        std::vector<Entry> re;
        Chunk chunk;
        for (std::string line; getline(is, line);) {
            line = Utils::normalize(line);
            if (line[0] == CONST::CHAR::NEW_WORD && !chunk.lines.empty()) {
                if (re.size() == count - 1)
                    break;

                re.push_back(parse_chunk(chunk, dict_type));
                chunk = {};
            }

            chunk.lines.push_back(line);
        }
        re.push_back(parse_chunk(chunk, dict_type));

        return re;
    }
}