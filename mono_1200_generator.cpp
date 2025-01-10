// Copyright © 2024 Isaac Liu <IsaacLiu@outlook.com>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#include <cstdlib>
#include <cstdint>

#include <string>
#include <format>
#include <iostream>
#include <sstream>

constexpr uintmax_t hash(const std::string_view& str) {
    uintmax_t hash = 0u;
    for (const char ch: str) {
        hash *= 131u;
        hash += ch;
    }
    return hash;
}   
int system_command(const std::string& str) {
    return std::system(str.c_str());
}
std::string read_command(std::istream& is) {
    std::string command;
    while (is.peek() == '\n') is.ignore();
    std::getline(is, command);
    return command;
}
void to_ttf(const std::string_view& str) {
    std::cout << std::format("Transfering {0:s}.otf to {0:s}.ttf ...", str) << std::endl;
    system_command(std::format("otf2ttf \"./src/{0:s}.otf\" -o \"./output/{0:s}.ttf\"", str));
}
void ttf_to_mono1200(const std::string_view& str) {
    std::cout << std::format("Generating {0:s}-Mono 1200.ttf from {0:s}.ttf ...", str) << std::endl;
    system_command(std::format("monoback mono1200 \"./output/{:s}.ttf\"", str));
}
void otf_to_mono1200(const std::string_view& str) {
    to_ttf(str);
    ttf_to_mono1200(str);
    std::remove(std::format("./output/{:s}.ttf", str).c_str());
}
int main() {
    std::cout << "Generator for CJK Fonts.\nRemember to install dependencies[otf2ttf, monoback] first." << std::endl;
    std::string command_name;
    while (true) {
        std::cout << "Mono 1200 Generator:~$ ";
        std::istringstream iss(read_command(std::cin));
        iss >> command_name;
        switch (hash(command_name)) {
            case hash("install"): {
                std::string package_name;
                if (iss >> package_name) {
                    if (package_name != "otf2ttf" && package_name != "monoback")
                        std::cout << "Unknown package." << std::endl;
                    else 
                        system_command(std::format("sudo pip install {}", package_name));
                    break;
                }
                system_command("sudo pip install otf2ttf");
                system_command("sudo pip install monoback");
                break;
            }
            case hash("auto_generate"): {
                otf_to_mono1200("NotoSansMonoCJKsc-Regular");
                otf_to_mono1200("NotoSansMonoCJKsc-Bold");
                break;
            }
            case hash("generate"): {
                std::string filename;
                iss >> filename;
                const auto ext = filename.rfind('.');
                switch(hash(filename.substr(ext))) {
                    case hash(".otf"): otf_to_mono1200(filename.substr(0, ext)); break;
                    case hash(".ttf"): ttf_to_mono1200(filename.substr(0, ext)); break;
                    default: std::cout << "Unsupported file format: " << filename.substr(ext) << std::endl;
                }
            }
            case hash("exit"): return 0;
            default:
                std::cout << "Unknown command." << std::endl;
        }
    }
    return 0;
}