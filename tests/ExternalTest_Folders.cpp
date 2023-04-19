#include <iostream>
#include <string>
#include <filesystem>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cout << "Error: no folder selected" << std::endl;
        return -1;
    }

    std::string folder = argv[1];

    int files = 0;

    std::cout << "Folder: " + folder << std::endl;

    try {
        for (const auto & entry : std::filesystem::directory_iterator(folder)) files++;
    } catch (std::filesystem::filesystem_error) {
        std::cout << "Error: no such directory" << std::endl;
        return -1;
    }

    std::cout << "Files: " << files << std::endl;

    return 0;
}