#include <iostream>
#include <unordered_map>
#include <string>
#include "./Math.h"

using Vowels = std::unordered_map<char, int>;
Vowels getVowels(const std::string& word) {
  Vowels v = { { 'a', 0 }, { 'e', 0 }, { 'i', 0 }, { 'o', 0 }, { 'u', 0 } };

  for (const char& c : word) {
    if (v.contains(c)) {
      v[c]++;
    }
  }
  return v;
}

int countWords(const std::string& word) {
  int wordLen = word.size();
  Vowels vowels = getVowels(word);

  int vowelCount = 0;
  int divideBy = 1;

  for (const auto& [c, count] : vowels) {
    vowelCount += count;
    divideBy *= fact(count);
  }

  int oddPosCount = ceil(wordLen / 2.0);

  int consCount = wordLen - vowelCount;
  int vowelPerm = perm(oddPosCount, vowelCount);
  int consPerm = perm(wordLen - vowelCount, consCount);

  int wordPerms = vowelPerm * consPerm / divideBy;

  return wordPerms;
}

int main() {
  std::string word;
  std::cout << "Enter a word: ";
  std::cin >> word;

  std::cout << "Number of all possile words with vowels in odd places = " << countWords(word) << std::endl;
}