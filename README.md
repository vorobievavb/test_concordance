The project is meant to be build with cmake.

Complexity of Concordance::ParseText is O(m + n*i) on average, where m is the number of symbols in in.txt, n is the number of unique words, i is the number of ignored words.
Complexity of the sorting algorithm is O(log(n)*n), where n is the number of unique words.
