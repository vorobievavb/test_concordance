//
// Created by veronika on 27.05.2021.
//

#include <algorithm>
#include <utility>
#include <fstream>
#include <vector>
#include <iostream>
#include "Concordance.h"

namespace Test{
    Concordance::Concordance(IgnoreSet &&ignore):
        ignore(ignore)
    {
    }

    void Concordance::ParseText(std::string &input) {
        uint32_t wordBeginning = 0;
        while(wordBeginning < input.length()) {
            auto wordLength = input.find_first_of(separators, wordBeginning);
            if(wordLength == std::string::npos)
                wordLength = input.length();

            wordLength -= wordBeginning;
            auto newWord = Meta(std::string(input, wordBeginning, wordLength), wordBeginning);

            if(wordLength == 0){
                auto symbol = input[wordBeginning];
                if(symbol != ' ' && symbol != '\n' && symbol != '\t') {
                    auto empl = words.emplace(std::string("."), wordBeginning);
                    if (!empl.second) {
                        auto &punct = const_cast<Meta&>(*empl.first);
                        punct.Update(wordBeginning);
                    }
                }
                wordBeginning += wordLength + 1;
                continue;
            }

            auto find = words.find(newWord);
            if(find != words.end()){
                auto& word = const_cast<Meta&>(*find);
                word.Update(wordBeginning);
            }
            else{
                if(ignore.find(newWord.word) == ignore.end())
                    words.emplace(newWord);
            }

            wordBeginning += wordLength + 1;
        }

    }

    void Concordance::Execute(const std::string& inputFile, Order order) {
        auto input = ReadFile(inputFile);
        ParseText(input);
        WriteSorted(order);
    }

    std::string Concordance::ReadFile(const std::string &fileName) {
        std::ifstream inputFile(fileName);
        std::string input;
        if(inputFile.is_open()){
            input.assign((std::istreambuf_iterator<char>(inputFile)),
                          (std::istreambuf_iterator<char>()));
            inputFile.close();
        }
        else
            std::cout << "Could not open input file" << std::endl;
        return input;
    }

    Concordance::Meta::Meta(std::string word, uint32_t firstAppearance):
        word(std::move(word)),
        firstAppearance(firstAppearance),
        quantity(1),
        lastAppearance(firstAppearance),
        meanDistance(0)
    {
    }

    void Concordance::Meta::Update(uint32_t newWordBeginning) {
        quantity++;
        meanDistance = (meanDistance * (quantity - 2) +
                        newWordBeginning - lastAppearance - 1) / (quantity - 1);
        lastAppearance = newWordBeginning;
    }

    template<Order T>
    bool Concordance::Meta::Compare(const Concordance::Meta &meta1, const Concordance::Meta &meta2) {
        return false;
    }


    template<>
    bool Concordance::Meta::Compare<Order::Alphabetical>(const Concordance::Meta &meta1, const Concordance::Meta &meta2){
        //Converting to lowercase to preserve alphabetical order over upper and lower case order
        std::string temp1, temp2;

        temp1.resize(meta1.word.length());
        temp2.resize(meta2.word.length());
        std::transform(meta1.word.begin(), meta1.word.end(), temp1.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        std::transform(meta2.word.begin(), meta2.word.end(), temp2.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        return temp1 < temp2;
    }

    template<>
    bool Concordance::Meta::Compare<Order::FirstAppearance>(const Concordance::Meta &meta1, const Concordance::Meta &meta2){
        return meta1.firstAppearance < meta2.firstAppearance;
    }

    template<>
    bool Concordance::Meta::Compare<Order::Quantitative>(const Concordance::Meta &meta1, const Concordance::Meta &meta2){
        return meta1.quantity < meta2.quantity;
    }

    template<>
    bool Concordance::Meta::Compare<Order::MeanDistance>(const Concordance::Meta &meta1, const Concordance::Meta &meta2){
        return meta1.meanDistance < meta2.meanDistance;
    }

    void Concordance::WriteSorted(Order order) {
        std::vector<Meta> wordsVec(words.begin(), words.end());
        switch(order){
            case Order::Alphabetical:
                std::sort(wordsVec.begin(), wordsVec.end(), Meta::Compare<Order::Alphabetical>);
                break;
            case Order::Quantitative:
                std::sort(wordsVec.begin(), wordsVec.end(), Meta::Compare<Order::Quantitative>);
                break;
            case Order::FirstAppearance:
                std::sort(wordsVec.begin(), wordsVec.end(), Meta::Compare<Order::FirstAppearance>);
                break;
            case Order::MeanDistance:
                std::sort(wordsVec.begin(), wordsVec.end(), Meta::Compare<Order::MeanDistance>);
                break;
        }
        std::ofstream output;
        output.open("out.txt");
        for(auto& word : wordsVec) {
            output << word.word << ' ';
        }

        output.close();
    }

} //namespace Test
