#pragma once

enum class Lang {
    Unknown,
    Eng,
    Vie
};
const std::string lang_to_str[3] = {"unknown", "eng", "vie"};

enum class DictType {
    Unknown,
    EE,
    EV,
    VE,
    Slang,
    Emo,
    Test
};