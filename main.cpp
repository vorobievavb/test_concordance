#include <iostream>
#include "Concordance.h"

int main() {
    std::cout << "Please print words to ignore separate by a space:" << std::endl;
    std::string ignoreIn;
    std::getline(std::cin, ignoreIn);
    Test::Concordance concordance(ignoreIn);
    //Input file should be in the same directory as the executable, otherwise provide the full path
    concordance.Execute(std::string("in.txt"), Test::Order::Alphabetical);
    return 0;
}
