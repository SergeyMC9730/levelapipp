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
            "[LevelAPI] Running on port {}",
            "[LevelAPI] Запущен на порту {}"
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
    // from GDServer.cpp
    {
        "lapi.gdserver.name", {
            "Basic GD server component",
            "Базовый компонент ГД сервера"
        }
    },
    // from GDServer_19GDPS.cpp
    {
        "lapi.gdserver_19gdps.name", {
            "1.9 GDPS server",
            "1.9 GDPS сервер"
        }
    },
    // from GDServer_BasementGDPS.cpp
    {
        "lapi.gdserver_basementgdps.name", {
            "Basement GDPS server",
            "Сервер \"Подвал ГДшеров\""
        }
    },
    // from GDServer_BasementLike21.cpp
    {
        "lapi.gdserver_basementlike21.name", {
            "Standard Basement server",
            "Стандартный сервер Подвала"
        }
    },
    // from GDServer_Boomlings.cpp
    {
        "lapi.gdserver_boomlings.name", {
            "Boomlings (official) server",
            "Бумлингс (официальный) сервер"
        }
    },
    // from GDServer_BoomlingsExperimental22.cpp
    {
        "lapi.gdserver_boomlingsexperimental22.name", {
            "Boomlings (official) server (with 2.2 features)",
            "Бумлингс (официальный) сервер (с фичами из 2.2)"
        }
    },
    // from GDServer_BoomlingsLike19.cpp
    {
        "lapi.gdserver_boomlingslike19.name", {
            "Standard 1.9 server",
            "Стандартный 1.9 сервер"
        }
    },
    // from GDServer_BoomlingsLike21.cpp
    {
        "lapi.gdserver_boomlingslike21.name", {
            "Standard 2.1 server",
            "Стандартный 2.1 сервер"
        }
    },
    // from GDServer_BoomlingsLike22.cpp
    {
        "lapi.gdserver_boomlingslike22.name", {
            "Standard 2.2 server",
            "Стандартный 2.2 сервер"
        }
    },
    // from GDServer_BoomlingsLike22.cpp
    {
        "lapi.gdserver_boomlingslike22.name", {
            "Standard 2.2 server",
            "Стандартный 2.2 сервер"
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
    // from Node.cpp
    {
        "lapi.node.selected_server", {
            "[LevelAPI {}] Selected server: {}",
            "[LevelAPI {}] Выбранный сервер: {}"
        }
    },
    {
        "lapi.node.import.open.start", {
            "opening {}",
            "открытие {}"
        }
    },
    {
        "lapi.node.import.ifstream", {
            "created ifstream for {}",
            "создан ifstream для {}"
        }
    },
    {
        "lapi.node.import.open.end", {
            "opened {}",
            "открыт {}"
        }
    },
    {
        "lapi.node.import.level", {
            "Imported level {} \"{}\"",
            "Импортирован уровень {} \"{}\""
        }
    },
    {
        "lapi.node.import.end1", {
            "{} levels were imported.",
            "{} уровней были импортированы."
        }
    },
    {
        "lapi.node.import.open.end2", {
            "{} levels are from different nodes, they were not imported.",
            "{} уровней из других узлов, они не были импортированы."
        }
    },
    // from NodeRunnerLegacy.cpp (and shared with NodeRunner.cpp)
    {
        "lapi.noderunner.recount.time", {
            "[LevelAPI {}] {}ms were taken to cache IDs.",
            "[LevelAPI {}] {} мс потрачено на кеширование айдишников."
        }
    },
    {
        "lapi.noderunner.logger.leveladded", {
            "[OK] Added level \"{}\" by {} ({})",
            "[OK] Добавлен уровень \"{}\" от {} ({})"
        }
    },
    {
        "lapi.noderunner.logger.init", {
            "[OK] Created GD client",
            "[OK] Создан ГД клиент"
        }
    },
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
        "lapi.noderunner.error.pban", {
            "[LevelAPI {}] ATTENTION! THIS IS NOT A WARNING! HOST'S IP WAS PERMANENTLY BANNED!",
            "[LevelAPI {}] ВНИМАНИЕ! ЭТО НЕ ПРЕДУПРЕЖДЕНИЕ! АЙПИ ХОСТА БЫЛ НАВСЕГДА ЗАБЛОКИРОВАН!"
        }
    },
    {
        "lapi.noderunner.error.pban.halt", {
            "[LevelAPI {}] FRONTEND IS GONNA HALT!",
            "[LevelAPI {}] ФРОНТЕНД БУДЕТ ЗАМОРОЖЕН!"
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
        "lapi.noderunner.downloader.user.fetch", {
            "[LevelAPI downloader {}] Fetching user levels...",
            "[LevelAPI downloader {}] Получение уровней пользователя..."
        }
    },
    {
        "lapi.noderunner.downloader.event.complete.noresolver", {
            "[LevelAPI downloader {}] Level saved without level string: {} \"{}\"",
            "[LevelAPI downloader {}] Сохранён уровень без его строки: {} \"{}\""
        }
    },
    {
        "lapi.noderunner.resolver.event.download.progress", {
            "[LevelAPI resolver {}] Downloading level {}",
            "[LevelAPI resolver {}] Скачивание уровня {}"
        }
    },
    {
        "lapi.noderunner.resolver.event.download.complete", {
            "[LevelAPI resolver {}] Level {} have been downloaded and saved",
            "[LevelAPI resolver {}] Уровень {} был скачан и сохранён"
        }
    },
    {
        "lapi.noderunner.downloader.skipped", {
            "[LevelAPI downloader {}] {} levels were skipped (already exists)",
            "[LevelAPI downloader {}] {} уровней было пропущено (уже существуют)"
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
        "lapi.level.register.embed.description", {
            "**This {} level** was registered by LevelAPI on `{}`.",
            "**Данный {} уровень** был зарегистрирован с помощью LevelAPI `{}`."
        }
    },
    {
        "lapi.level.rate.embed.description", {
            "**This {} level** was rated on `{}`.",
            "**Данный {} уровень** был рейтнут `{}`."
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
    {
        "lapi.level.embed.field.info.value.rawdata", {
            "Raw Data",
            "Исходник"
        }
    },
    // from iAndy.cpp
    {
        "iandy.prefix.m", {
            "M",
            " млн."
        }
    },
    {
        "iandy.prefix.k", {
            "K",
            " тыс."
        }
    },
    // FROM DCommandStats.cpp
    {
        "lapi.command.stats.execute.error.title", {
            "Error!",
            "Ошибка"
        }
    },
    {
        "lapi.command.stats.execute.error.description", {
            "**Node doesn't exist!** Select a different one.",
            "**Узел не существует!** Выберите другой."
        }
    },
    {
        "lapi.command.stats.execute.success.title", {
            "Statistics",
            "Статистика"
        }
    },
    {
        "lapi.command.stats.execute.success.field.levels", {
            "Levels",
            "Уровней"
        }
    },
    {
        "lapi.command.stats.execute.success.field.qjobs", {
            "Queued Jobs",
            "Запланированных задач"
        }
    },
    {
        "lapi.command.stats.execute.success.field.qdjobs", {
            "Queued Download Jobs",
            "Запланированных задач загрузчика"
        }
    },
    {
        "lapi.command.stats.execute.success.ldl", {
            "Latest Downloaded Levels:",
            "Последние скачанные уровни:"
        }
    },
    {
        "lapi.command.stats.description", {
            "Get Node statistics",
            "Получить статистику Узла"
        }
    },
    {
        "lapi.command.stats.node.description", {
            "Selected node",
            "Выбранный узел"
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
        "lapi.bot.command.create", {
            "[LevelAPI DiscordInstance] Creating commands..",
            "[LevelAPI DiscordInstance] Создание команд.."
        }
    },
    {
        "lapi.bot.command.register", {
            "[LevelAPI DiscordInstance] Registering command {}",
            "[LevelAPI DiscordInstance] Регистрация команды {}"
        }
    },
    {
        "lapi.bot.command.run", {
            "[LevelAPI DiscordInstance] Running command {}",
            "[LevelAPI DiscordInstance] Запуск команды {}"
        }
    },
    {
        "lapi.bot.status.levels", {
            "{} levels so far!",
            "{} уровней на данный момент!"
        }
    },
    {
        "lapi.bot.command.registerchannel.success", {
            "This channel would be used for sending notifications about new levels being uploaded.",
            "Данный канал будет использоваться для отправки уведомлений о новых выщенных уровнях."
        }
    },
    {
        "lapi.bot.command.lapiget.info.base", {
            "Getting metadata about any recorded level.",
            "Получение метаданных о каком либо зарегистрированном уровне."
        }
    },
    {
        "lapi.bot.command.lapiget.option.type.info", {
            "Search Filter",
            "Фильтр поиска"
        }
    },
    {
        "lapi.bot.command.lapiget.option.type.field", {
            "Filter String",
            "Строка фильтра"
        }
    },
    {
        "lapi.bot.command.lapiget.option.type.v1", {
            "By Level ID",
            "По айди уровня"
        }
    },
    {
        "lapi.bot.command.lapiget.option.type.v1.field", {
            "ID",
            "ID"
        }
    },
    {
        "lapi.bot.command.lapiget.option.type.v2", {
            "By User ID",
            "По айди пользователя"
        }
    },
    {
        "lapi.bot.command.lapiget.option.type.v2.field", {
            "ID",
            "ID"
        }
    },
    {
        "lapi.bot.command.lapiget.option.type.v3", {
            "By Account ID",
            "По айди аккаунта"
        }
    },
    {
        "lapi.bot.command.lapiget.option.type.v3.field", {
            "ID",
            "ID"
        }
    },
    {
        "lapi.bot.command.lapiget.option.type.v4", {
            "By Nickname",
            "По никнейнму"
        }
    },
    {
        "lapi.bot.command.lapiget.option.type.v4.field", {
            "Nickname",
            "Никнейм"
        }
    },
    {
        "lapi.bot.command.lapiget.option.type.v5", {
            "By Name",
            "По названию"
        }
    },
    {
        "lapi.bot.command.lapiget.option.type.v5.field", {
            "Name",
            "Название"
        }
    },
    {
        "lapi.bot.command.lapiget.option.type.v6", {
            "By Description",
            "По описанию"
        }
    },
    {
        "lapi.bot.command.lapiget.option.type.v6.field", {
            "Description",
            "Описание"
        }
    },
    {
        "lapi.bot.command.lapiget.option.str.info", {
            "Search string",
            "Строка поиска"
        }
    },
    {
        "lapi.bot.command.lapiget.option.sort.info", {
            "Result sorting",
            "Сортировка результатов"
        }
    },
    {
        "lapi.bot.command.lapiget.option.sort.v1", {
            "Most Liked",
            "Самый залайканный"
        }
    },
    {
        "lapi.bot.command.lapiget.option.filter.info", {
            "Result filtering",
            "Фильтрация результатов"
        }
    },
    {
        "lapi.bot.command.lapiget.option.filter.v1", {
            "Stars",
            "Звёзд"
        }
    },
    {
        "lapi.bot.command.lapiget.option.filter.v1.field", {
            "Amount",
            "Количество"
        }
    },
    {
        "lapi.bot.command.lapiget.option.sort.v2", {
            "Most Downloaded",
            "Самый популярный"
        }
    },
    {
        "lapi.bot.command.lapiget.option.node.info", {
            "Node",
            "Узел"
        }
    },
    {
        "lapi.bot.command.lapiget.ui.page", {
            "Page {}",
            "Страница {}"
        }
    },
    {
        "lapi.bot.command.lapiget.ui.nextpage", {
            "Next page",
            "Следующая страница"
        }
    },
    {
        "lapi.bot.command.lapiget.ui.prevpage", {
            "Previous page",
            "Предыдущая страница"
        }
    },
    {
        "lapi.bot.command.lapiget.ui.cell", {
            "[**Level \"{}\"**]({}) by {} — {} downloads, {} likes",
            "[**Уровень \"{}\"**]({}) от {} — {} скачиваний, {} лайков"
        }
    },
    {
        "lapi.bot.command.lapiget.output.test", {
            "test1123 {}",
            "тест1123 {}"
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