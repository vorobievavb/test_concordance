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
    Concordance::Concordance(std::string &ignoreIn) {
        uint32_t wordBeginning = 0;
        while(wordBeginning < ignoreIn.length()) {
            auto wordLength = ignoreIn.find(' ');
            if(wordLength == std::string::npos)
                wordLength = ignoreIn.length();

            wordLength -= wordBeginning;
            ignore.emplace(ignoreIn, wordBeginning, wordLength);
            wordBeginning += wordLength + 1;
        }
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
                auto symbol = std::string(input, wordBeginning, 1);
                if(symbol != std::string(" ")) {
                    auto empl = words.emplace(std::string("."), wordBeginning);
                    if (!empl.second) {
                        auto punct = *empl.first;
                        punct.quantity++;
                        punct.meanDistance = (punct.meanDistance * (punct.quantity - 1) +
                                              wordBeginning - punct.lastAppearance) / punct.quantity;
                        punct.lastAppearance = wordBeginning;
                    }
                }
                wordBeginning += wordLength + 1;
                continue;
            }

            auto find = words.find(newWord);
            if(find != words.end()){
                auto word = *find;
                word.quantity++;
                word.meanDistance = (word.meanDistance * (word.quantity - 1) +
                                      wordBeginning - word.lastAppearance) / word.quantity;
                word.lastAppearance = wordBeginning;
            }
            else{
                auto ignored = false;
                for(auto& i : ignore)
                    if (newWord.word == i) {
                        ignored = true;
                        break;
                    }
                if(!ignored)
                    words.emplace(newWord);
            }

            wordBeginning += wordLength + 1;
        }

    }

    void Concordance::Execute(std::string inputFile, Order order) {
        auto input = ReadFile(inputFile);
        ParseText(input);
        WriteSorted(order);
    }

    std::string Concordance::ReadFile(std::string &fileName) {
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

    template<Order T>
    bool Concordance::Meta::Compare(const Concordance::Meta &meta1, const Concordance::Meta &meta2) {
        return false;
    }

    template<>
    bool Concordance::Meta::Compare<Order::Alphabetical>(const Concordance::Meta &meta1, const Concordance::Meta &meta2){
        //Converting to lowercase to preserve alphabetical order over upper and lower case order
        //TODO: figure out why std::trasform doesn't work
        std::string temp1, temp2;

        for(auto& c : meta1.word)
            temp1.push_back(std::tolower(c));
        for(auto& c : meta2.word)
            temp2.push_back(std::tolower(c));

//        temp1.reserve(meta1.word.length());
//        temp2.reserve(meta2.word.length());
//        std::transform(meta1.word.begin(), meta1.word.end(), temp1.begin(),
//                       [](unsigned char c){ return std::tolower(c); });
//        std::cout << meta1.word;
//        std::transform(meta2.word.begin(), meta2.word.end(), temp2.begin(),
//                       [](unsigned char c){ return std::tolower(c); });
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
