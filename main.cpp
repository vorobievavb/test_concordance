#include <iostream>
#include "Concordance.h"

int main() {
    std::cout << "Please print words to ignore separated by spaces:" << std::endl;
    std::string ignoreIn;
    std::getline(std::cin, ignoreIn);
    std::cout << "Please enter desired order:\n"
                 "0 -   Alphabetical,\n"
                 "1 -   By quantity,\n"
                 "2 -   Order of first appearance,\n"
                 "3 -   By mean distance between words." << std::endl;
    int order = getchar() - '0';

    Test::Concordance concordance(ignoreIn);
    //Input file should be in the same directory as the executable, otherwise provide the full path
    concordance.Execute(std::string("in.txt"), static_cast<Test::Order>(order));
    return 0;
}
