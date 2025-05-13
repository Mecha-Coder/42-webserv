#include <iostream>
#include <string>

int main() {
    std::string text = "alpha beta gamma beta delta";
    std::string word = "beta";

    size_t pos1 = text.rfind(word);
    size_t pos2 = text.find(word);

    std::cout << text.substr(pos1) << " | " << text.substr(pos2) << std::endl;

    return 0;
}