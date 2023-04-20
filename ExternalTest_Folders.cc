#include <iostream>
#include <string>
#include <filesystem>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cout << "Error: no folder selected" << std::endl;
        return -1;
    }

    std::string parameter = "";

    if (argc == 3) {
        parameter = argv[2];
    }

    if (parameter == "--help") {
        std::cout << "--with-names - show names" << std::endl;
        return 0;
    }

    std::string folder = argv[1];

    int files = 0;

    std::cout << "Folder: " + folder << std::endl;

    try {
        if (parameter == "--with-names") {
            for (const auto & entry : std::filesystem::directory_iterator(folder)) {
                std::string path = entry.path();
                std::string filename = path.substr(path.find_last_of("/\\") + 1);
                std::cout << "Name for " << ++files << ": " << filename << std::endl;
            }
        } else {
            for (const auto & entry : std::filesystem::directory_iterator(folder)) files++;
        }
    } catch (std::filesystem::filesystem_error) {
        std::cout << "Error: no such directory" << std::endl;
        return -1;
    }

    std::cout << "Files: " << files << std::endl;

    return 0;
}