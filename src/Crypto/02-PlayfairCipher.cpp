/* Algorithm (Playfair Cipher Encryption):
1. Start
2. Preprocess key:
   a. Convert key to uppercase letters only.
   b. Replace J with I.
   c. Remove duplicates and build a 5x5 key square with remaining alphabet.
3. Preprocess plaintext:
   a. Convert to uppercase, keep letters only, replace J with I.
   b. Create digraphs (pairs):
      i. If pair letters are same, insert filler X after first letter.
      ii. If length is odd, append X.
4. Encrypt each digraph using Playfair rules:
   a. Same row    -> replace each letter by right neighbor (circular).
   b. Same column -> replace each letter by below neighbor (circular).
   c. Rectangle   -> replace each letter by letter in same row and other
      letter's column.
5. Concatenate all encrypted digraphs to get cipher text.
6. End
*/

#include <cctype>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "src/commons.hpp"

std::string sanitizeLetters(const std::string& text) {
    std::string out;

    for (char ch : text) {
        if (std::isalpha((unsigned char)ch)) {
            char up = (char)std::toupper((unsigned char)ch);
            if (up == 'J') up = 'I';
            out.push_back(up);
        }
    }

    return out;
}

std::string buildKeySquare(const std::string& key) {
    std::string used;
    std::string cleanedKey = sanitizeLetters(key);

    for (char ch : cleanedKey)
        if (used.find(ch) == std::string::npos)
            used.push_back(ch);

    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (ch == 'J') continue;
        if (used.find(ch) == std::string::npos)
            used.push_back(ch);
    }

    return used; // length = 25
}

std::pair<int, int> findPos(const std::string& square, char ch) {
    int idx = (int)square.find(ch);
    return { idx / 5, idx % 5 };
}

std::string makeDigraphText(const std::string& plain) {
    std::string p = sanitizeLetters(plain);
    std::string prepared;

    for (int i = 0; i < (int)p.size(); ) {
        char a = p[i];
        char b = (i + 1 < (int)p.size()) ? p[i + 1] : 'X';

        if (a == b) {
            prepared.push_back(a);
            prepared.push_back('X');
            i += 1;
        }
        else {
            prepared.push_back(a);
            prepared.push_back(b);
            i += 2;
        }
    }

    if (prepared.size() % 2 != 0)
        prepared.push_back('X');

    return prepared;
}

std::string playfairEncrypt(const std::string& plain, const std::string& key) {
    std::string square = buildKeySquare(key);
    std::string prepared = makeDigraphText(plain);
    std::string cipher;

    for (int i = 0; i < (int)prepared.size(); i += 2) {
        char a = prepared[i];
        char b = prepared[i + 1];

        auto [r1, c1] = findPos(square, a);
        auto [r2, c2] = findPos(square, b);

        if (r1 == r2) {
            cipher.push_back(square[r1 * 5 + (c1 + 1) % 5]);
            cipher.push_back(square[r2 * 5 + (c2 + 1) % 5]);
        }
        else if (c1 == c2) {
            cipher.push_back(square[((r1 + 1) % 5) * 5 + c1]);
            cipher.push_back(square[((r2 + 1) % 5) * 5 + c2]);
        }
        else {
            cipher.push_back(square[r1 * 5 + c2]);
            cipher.push_back(square[r2 * 5 + c1]);
        }
    }

    return cipher;
}

void printKeySquare(const std::string& key) {
    std::string square = buildKeySquare(key);

    std::cout << "Key Square (" << key << "):\n";
    for (int r = 0; r < 5; r++) {
        for (int c = 0; c < 5; c++)
            std::cout << square[r * 5 + c] << " ";
        std::cout << "\n";
    }
}

int main() {
    Header("PLAYFAIR CIPHER");

    std::string plain1 = "MEET ME AFTER THE TOGA PARTY";
    std::string key1 = "MONARCHY";

    std::string plain2 = "THE IMITATION GAME";
    std::string key2 = "ALAN TURING";

    printKeySquare(key1);
    std::cout << "Plain Text : " << plain1 << std::endl;
    std::cout << "Cipher Text: " << playfairEncrypt(plain1, key1) << std::endl;

    std::cout << "\n";

    printKeySquare(key2);
    std::cout << "Plain Text : " << plain2 << std::endl;
    std::cout << "Cipher Text: " << playfairEncrypt(plain2, key2) << std::endl;

    Footer();
    return 0;
}
