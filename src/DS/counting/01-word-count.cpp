#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>


int fact(int n) {
  if (n <= 0) {
    return 1;
  }

  return n * fact(n - 1);
}
using Map = std::unordered_map<char, int>;
    
Map findRepeats(const std::string& word) {
  std::string lowerWord = word;
  Map repeats;

  for (char& c : lowerWord) {
    c = std::tolower(c);
  }

  for (const char& c : lowerWord) {
    repeats[c]++;
  }

  return repeats;
}

int countWords(const std::string& word) {
  int count = word.size();

  int wordCount = fact(count);

  Map repeats = findRepeats(word);

  for (const auto& [c, count] : repeats) {
    wordCount /= fact(count);
  }

  return wordCount;
}

int main() {
  std::string word;
  std::cout << "Enter a word: ";
  std::cin >> word;

  std::cout << "Number of all possile words = " << countWords(word) << std::endl;

}