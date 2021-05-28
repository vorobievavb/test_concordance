The project is meant to be build with cmake.

Complexity of Concordance::ParseText is O(m*s+n) on average, where m is the number of symbols in in.txt, s is number of separators, n is the number of unique words (approximately, complexity for string::find_first_of is unspecified).
Complexity of the sorting algorithm is O(log(n)*n), where n is the number of unique words.
