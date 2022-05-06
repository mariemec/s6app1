#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>

int main(int argc, char** argv)
{
    std::ifstream file_in;
    int multiplication_factor;
    if (argc >= 3 && (strcmp(argv[2], "-") != 0)) {
        multiplication_factor = strtol(argv[1], NULL, 10);
        file_in.open(argv[2]);
        if (file_in.is_open()) {
            std::cin.rdbuf(file_in.rdbuf());
            std::cerr << "Using " << argv[2] << "..." << std::endl;
        } else {
            std::cerr   << "Error: Cannot open '"
                        << argv[1] 
                        << "', using stdin (press CTRL-D for EOF)." 
                        << std::endl;
        }
    } else {
        std::cerr << "Using stdin (press CTRL-D for EOF)." << std::endl;
    }

    while (!std::cin.eof()) {

        std::string line;

        std::getline(std::cin, line);
        if (!line.empty()) {
            int val = atoi(line.c_str());
            std::cout << val * multiplication_factor << std::endl;
        }
    }

    if (file_in.is_open()) {
        file_in.close();
    }

    return 0;
}
