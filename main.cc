#include "automaton.h"
#include <iostream>
#include <string>


//Main functie
int main() {
    Automaton automaton;
    std::string command, arg;
    
    while (std::cin >> command) {
        if (command == "end") {
            break;
        }
        else if (command == "exp") {
            std::cin >> arg;
            try {
                automaton.parse_expression(arg);
                std::cout << "Expressie is geparsed\n";
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        }
        else if (command == "dot") {
            std::cin >> arg;
            try {
                automaton.write_dot(arg);
                std::cout << "Dot file is weggeschreven\n";
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        }
        else if (command == "mat") {
            std::cin >> arg;
            try {
        if (automaton.match_string(arg)) {
            std::cout << "match\n";
        } else {
            std::cout << "geen match\n";
        }
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        }
        else {
            std::cout << "Onbekend commando\n";
        }
    }
    
    return 0;
}