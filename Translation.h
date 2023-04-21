#pragma once

#include <string>
#include <map>

extern std::string translation_language;

#include "fmt/include/fmt/core.h"

namespace LevelAPI {
    namespace Frontend {
        namespace Translation {
            typedef struct translation_unit_t {
                std::string en;
                std::string ru;
            } translation_unit_t;

            extern std::map<std::string, translation_unit_t> translation_keymap;
        
            template <typename... Args>
            inline std::string getByKey(std::string key, Args... args) {
                std::string str = "";
                if (translation_keymap.find(key) == translation_keymap.end()) {
                    str = key;
                } else {
                    auto multistr = translation_keymap[key];
                    
                    if (translation_language == "en") str = multistr.en;
                    if (translation_language == "ru") str = multistr.ru;
                }
                fmt::format_string<Args...> data = str;
                return fmt::format(data, std::forward<Args>(args)...);
            }

            void init();
        }
    }
}