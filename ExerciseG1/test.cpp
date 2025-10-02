#include <format>
#include <iostream>
#include <string>

int main() {
    std::string name = "World";
    std::cout << std::format("Hello, {}!\n", name);
    return 0;
}
