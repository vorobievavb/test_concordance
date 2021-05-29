//
// Created by veronika on 27.05.2021.
//

#ifndef TEST_CONCORDANCE_H
#define TEST_CONCORDANCE_H

#include <cstdint>
#include <string>
#include <unordered_set>

namespace Test {
    using IgnoreSet = std::unordered_set<std::string>;

    //Sorting order
    enum struct Order{
       Alphabetical = 0,
       Quantitative,
       FirstAppearance,
       MeanDistance      //If the word appears in text only once, mean distance is considered to be zero
    };

    class Concordance {
    public:
        //Constructor receives an std::string with words to ignore separated by a space
        explicit Concordance(IgnoreSet&&);
        ~Concordance() = default;

        //Main method, pass input file name and desired order
        void Execute(const std::string& fileName, Order);
        static std::string ReadFile(const std::string& fileName);

        //Separates input text into unique words and fills metadata.
        //A word is defined as a string of characters with the beginning of the text,
        //a space, a tab, the end of the line or the end of the text from either side.
        //All punctuation symbols are considered to be the same word encoded as a dot symbol.
        void ParseText(std::string &);

        //Sorts words according to the chosen order and writes them in file "out.txt"
        //using space as a separator.
        void WriteSorted(Order);

        //Metadata of words containing desired characteristics and necessary methods
        struct Meta{
            Meta(std::string, uint32_t);
            std::string word;
            uint32_t quantity;
            uint32_t firstAppearance;
            double meanDistance;
            uint32_t lastAppearance;

            //Custom comparator to pass in std::sort depending on chosen order
            template<Order T>
            static bool Compare(const Meta &meta1, const Meta &meta2);

            bool operator==(const Meta& meta)const{
                return word == meta.word;
            };

            //Custom hash function, uses the standard hash function for string
            class CustomHash{
            public:
                std::size_t operator()(const Meta& meta) const {
                    return std::hash<std::string>()(meta.word);
                }
            };

            void Update(uint32_t);
        };

    private:
        //Default C list of characters cosidered punctuation + space + tab + new line
        std::string separators = R"( !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~)"+std::string("\n\t");

        IgnoreSet ignore;
        std::unordered_set<Meta, Meta::CustomHash> words;

    }; //class Concordance
} //namespace Test

#endif //TEST_CONCORDANCE_H
