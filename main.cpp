#include <iostream>
#include "Concordance.h"

Test::IgnoreSet GetIgnore(){
    Test::IgnoreSet ignore;
    unsigned char ch;
    std::string newIgnore;
    do {
        ch = std::cin.get();
        if(ch != ' ' && ch != '\t' && ch != '\n')
            newIgnore.push_back(ch);
        else {
            if (!newIgnore.empty() )
                ignore.insert(newIgnore);
            newIgnore.clear();
        }
    }while(ch != '\n');
    return ignore;
}

int GetOrder(){
    int order;
    while(true) {
        std::cout << "Please enter desired order:\n"
                     "0 -   Alphabetical,\n"
                     "1 -   By quantity,\n"
                     "2 -   Order of first appearance,\n"
                     "3 -   By mean distance between words." << std::endl;
        order = std::cin.get() - '0';
        //Getting enter symbol
        std::cin.get();
        if(order > 3 || order < 0){
            std::cout << "Wrong input" << std::endl;
        }
        else
            break;
    }
    return order;
}

int main() {
    std::cout << "Please print words to ignore separated by spaces:" << std::endl;

    Test::Concordance concordance(std::forward<Test::IgnoreSet>(GetIgnore()));

    int order = GetOrder();
    concordance.Execute(std::string("in.txt"), static_cast<Test::Order>(order));
    return 0;
}
