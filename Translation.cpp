#include "Translation.h"

#include "fmt/include/fmt/core.h"

using namespace LevelAPI::Frontend;

std::string translation_language = "en";

std::map<std::string, Translation::translation_unit_t> Translation::translation_keymap = {
    // from main.cpp
    {
        "lapi.main.alpha", {
            "LevelAPI is in alpha state so please report any bugs to my GitHub repository!",
            "LevelAPI находится в стадии альфы, поэтому прошу репортить об каких либо багах на мой Гитхаб репозиторий!"
        }
    },
    {
        "lapi.main.portstart", {
            "[LevelAPI] Running on port ",
            "[LevelAPI] Запущен на порту "
        }
    },
    // from Time.cpp
    {
        "lapi.time.jan", {
            "Jan",
            "января"
        }
    },
    {
        "lapi.time.feb", {
            "Feb",
            "февраля"
        }
    },
    {
        "lapi.time.mar", {
            "Mar",
            "марта"
        }
    },
    {
        "lapi.time.apr", {
            "Apr",
            "апреля"
        }
    },
    {
        "lapi.time.may", {
            "May",
            "мая"
        }
    },
    {
        "lapi.time.jun", {
            "Jun",
            "июня"
        }
    },
    {
        "lapi.time.jul", {
            "Jul",
            "июля"
        }
    },
    {
        "lapi.time.Aug", {
            "Aug",
            "августа"
        }
    },
    {
        "lapi.time.sep", {
            "Sep",
            "сентября"
        }
    },
    {
        "lapi.time.oct", {
            "Oct",
            "октября"
        }
    },
    {
        "lapi.time.nov", {
            "Nov",
            "ноября"
        }
    },
    {
        "lapi.time.dec", {
            "Dec",
            "декабря"
        }
    },
    {
        "lapi.time.dateorder", {
            "mdy",
            "dmy"
        }
    },
    {
        "lapi.time.dayformat", {
            "th",
            "-го"
        }
    },
    {
        "lapi.time.timepointer", {
            "at",
            "в"
        }
    },
    // from ThreadSafeLevelParser.cpp
    {
        "lapi.tslp.logicerror", {
            "[LevelAPI WARN] Got logic_error exception!",
            "[LevelAPI ПРЕД] Получена логическая ошибка!"
        }
    },
    // from test_gd_parsers.cpp
    {
        "lapi.gdparsertests.start", {
            "[LevelAPI] Running GD parser tests...\n",
            "[LevelAPI] Запускается тестирование парсера ГД...\n"
        }
    },
    {
        "lapi.gdparsertests.test1result", {
            "[LevelAPI TEST 1] gd.cpp returned {} levels\n",
            "[LevelAPI ТЕСТ 1] gd.cpp вернул {} уровней\n"
        }
    },
    {
        "lapi.gdparsertests.test2result", {
            "[LevelAPI TEST 2] gd.cpp returned ",
            "[LevelAPI ТЕСТ 2] gd.cpp вернул "
        }
    },
    {
        "lapi.gdparsertests.complete", {
            "[LevelAPI] GD parser test complete\n",
            "[LevelAPI] Тестирование ГД парсера завершено\n"
        }
    },
    // from test_curl_connection.cpp
    {
        "lapi.curltest.test0result", {
            "[LevelAPI TEST 0] Google returned %d %d\n",
            "[LevelAPI ТЕСТ 0] Google вернул %d %d\n"
        }
    },
    {
        "lapi.curltest.start", {
            "[LevelAPI] Running cURL connection test...\n",
            "[LevelAPI] Выполняется тестирование подключения к cURL...\n"
        }
    },
    {
        "lapi.curltest.complete", {
            "[LevelAPI] cURL connection test complete\n",
            "[LevelAPI] Тестирование подключения cURL завершено\n"
        }
    },
    {
        "lapi.curltest.fail", {
            "[LevelAPI] cURL connection test FAILED\n[LevelAPI] Don't expect connection backend to work!\n",
            "[LevelAPI] Тестирование подключения cURL ПРОВАЛЕНО\n[LevelAPI] Не ожидайте работу внутреннего бакенда!\n"
        }
    },
    // from NodeRunner.cpp
    {
        "lapi.noderunner.recount.complete", {
            "[LevelAPI {}] Recount task complete: {} levels in total.",
            "[LevelAPI {}] Задача пересчёта завершена: {} уровней вместе взятых."
        }
    },
    {
        "lapi.noderunner.resolver.wait.start", {
            "[LevelAPI resolver {}] Waiting {}s",
            "[LevelAPI resolver {}] Ожидание {} сек."
        }
    },
    {
        "lapi.noderunner.resolver.wait.end", {
            "[LevelAPI resolver {}] Waiting complete.",
            "[LevelAPI resolver {}] Ожидание завершено."
        }
    },
    {
        "lapi.noderunner.resolver.lvlwait.start", {
            "[LevelAPI resolver {}] Waiting for {} {}",
            "[LevelAPI resolver {}] Ожидание {} {}"
        }
    },
    {
        "lapi.noderunner.downloader.sleeping.fetch", {
            "[LevelAPI downloader {}] Sleeping {}s and then fetching level {}",
            "[LevelAPI downloader {}] Ожидание {} сек и последующее получение уровня {}"
        }
    },
    {
        "lapi.noderunner.resolver.error.ratelimit", {
            "[LevelAPI resolver {}] Impossible to fetch: Rate Limit",
            "[LevelAPI resolver {}] Невозможно получить: Слишком много соединений за короткий промежуток времени"
        }
    },
    {
        "lapi.noderunner.resolver.error.disabled", {
            "[LevelAPI resolver {}] Impossible to fetch: Disable",
            "[LevelAPI resolver {}] Невозможно получить: Отключено"
        }
    },
    {
        "lapi.noderunner.resolver.event.ratelimit", {
            "[LevelAPI resolver {}] RATE LIMIT for {}s",
            "[LevelAPI resolver {}] ОГРАНИЧЕНИЕ СОЕДИНЕНИЙ на {} сек."
        }
    },
    {
        "lapi.noderunner.downloader.event.levelfetched", {
            "[LevelAPI downloader {}] Fetched level {}",
            "[LevelAPI downloader {}] Получен уровень {}"
        }
    },
    {
        "lapi.noderunner.start", {
            "[LevelAPI] Running node {}",
            "[LevelAPI] Запускается узел {}"
        }
    },
    {
        "lapi.noderunner.start.modification.basement", {
            "[LevelAPI {}] Selected modification: Basement GDPS",
            "[LevelAPI {}] Выбранная модификация: Подвал ГДшеров"
        }
    },
    {
        "lapi.noderunner.start.modification.notselected", {
            "[LevelAPI {}] Selected modification: Not selected.",
            "[LevelAPI {}] Выбранная модификация: Не выбрана."
        }
    },
    {
        "lapi.noderunner.downloader.error.queue.empty", {
            "[LevelAPI downloader {}] Queue is empty. Skipping",
            "[LevelAPI downloader {}] Очередь пуста. Пропуск"
        }
    },
    {
        "lapi.noderunner.readonly", {
            "[LevelAPI {}] Running node in READ-ONLY mode!",
            "[LevelAPI {}] Запускается узел в режиме ТОЛЬКО ДЛЯ ЧТЕНИЯ!"
        }
    },
    {
        "lapi.noderunner.resolver.linear.wait", {
            "[LevelAPI linear resolver {}] Waiting {}s",
            "[LevelAPI linear resolver {}] Ожидание {} сек."
        }
    },
    {
        "lapi.noderunner.resolver.linear.fetch", {
            "[LevelAPI linear resolver {}] Fetching level {}...",
            "[LevelAPI linear resolver {}] Получение уровня {}..."
        }
    },
    {
        "lapi.noderunner.resolver.linear.event.fetch.end", {
            "[LevelAPI linear resolver {}] Fetched level {}",
            "[LevelAPI linear resolver {}] Получен уровень {}"
        }
    },
    {
        "lapi.noderunner.resolver.linear.event.ratelimit", {
            "[LevelAPI linear resolver {}] RATE LIMIT for {}s",
            "[LevelAPI linear resolver {}] ОГРАНИЧЕНИЕ СОЕДИНЕНИЙ на {} сек."
        }
    },
    {
        "lapi.noderunner.downloader.wait", {
            "[LevelAPI downloader {}] Waiting {}s",
            "[LevelAPI downloader {}] Ожидание {} сек."
        }
    },
    {
        "lapi.noderunner.resolver.linear.error.disabled", {
            "[LevelAPI {}] WARN: Tried to run linear resolver with disabled resolver!",
            "[LevelAPI {}] ПРЕД: Проведена попытка запуска линейного резолвера с отключенным резолвером!"
        }
    },
    {
        "lapi.noderunner.downloader.recenttab.fetch", {
            "[LevelAPI downloader {}] Fetching recent levels...",
            "[LevelAPI downloader {}] Получение последних уровней..."
        }
    },
    {
        "lapi.noderunner.downloader.event.complete.noresolver", {
            "[LevelAPI downloader {}] Level saved without level string: {} \"{}\"",
            "[LevelAPI downloader {}] Сохранён уровень без его строки: {} \"{}\""
        }
    },
    // from Level.cpp
    {
        "lapi.level.embed.title", {
            "New Level",
            "Новый уровень"
        }
    },
    {
        "lapi.level.embed.description", {
            "**New {} level** appeared on the server on `{}`!",
            "**Новый {} уровень** появился на сервере: опубликован `{}`."
        }
    },
    {
        "lapi.level.embed.field.id", {
            "ID:",
            "ID:"
        }
    },
    {
        "lapi.level.embed.field.name", {
            "Name:",
            "Название:"
        }
    },
    {
        "lapi.level.embed.field.author", {
            "Author:",
            "Автор:"
        }
    },
    {
        "lapi.level.embed.field.info", {
            "**More Info:**",
            "**Подробности:**"
        }
    },
    {
        "lapi.level.embed.field.info.value.metadata", {
            "Metadata",
            "Метаданные"
        }
    },
    // from DiscordInstance.cpp
    {
        "lapi.bot.ready", {
            "[LevelAPI DiscordInstance] Bot is ready!",
            "[LevelAPI DiscordInstance] Бот запущен!"
        }
    },
    {
        "lapi.bot.command.registerchannel.success", {
            "This channel would be used for sending notifications about new levels being uploaded.",
            "Данный канал будет использоваться для отправки уведомлений о новых выщенных уровнях."
        }
    },
    // from CURLConnection.cpp
    {
        "curl.event.warning.perform", {
            "[cURL WARN] {}\n",
            "[cURL ПРЕД] {}\n"
        }
    },
    {
        "curl.debug.parameters", {
            "Parameters: {}\n",
            "Параметры: {}\n"
        }
    },
    {
        "curl.event.warning.connection", {
            "[cURL WARN] Couldn't connect to server: {}\n",
            "[cURL ПРЕД] Не удалось подключиться к серверу: {}\n"
        }
    }
};

// template <typename... Args>
// std::string Translation::getByKey(std::string key, Args... args) {
//     std::string str = "";
//     if (Translation::translation_keymap.find(key) == Translation::translation_keymap.end()) {
//         str = key;
//     } else {
//         auto multistr = Translation::translation_keymap[key];
//         if (translation_language == "en") {
//             str = multistr.en;
//         }
//         if (translation_language == "ru") {
//             str = multistr.ru;
//         }
//     }
//     fmt::format_string<Args...> data = str;
//     return fmt::format(data, std::forward<Args>(args)...);
// }