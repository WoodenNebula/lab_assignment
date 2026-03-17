/* Algorithm (Hill Cipher Encryption - 2x2 Key Matrix):
1. Start
2. Preprocess plaintext:
   a. Keep letters only and convert to uppercase.
   b. If length is odd, append filler 'X'.
3. Use 2x2 key matrix K:
      [k00 k01]
      [k10 k11]
4. For every plaintext pair (P1, P2):
   a. Convert letters to numbers (A=0,...,Z=25).
   b. Compute:
      C1 = (k00*P1 + k01*P2) mod 26
      C2 = (k10*P1 + k11*P2) mod 26
   c. Convert C1, C2 back to letters.
5. Concatenate all cipher pairs.
6. End
*/

#include <cctype>
#include <iostream>
#include <string>
#include <vector>

#include "src/commons.hpp"

std::string onlyUpperLetters(const std::string& text) {
    std::string out;

    for (char ch : text)
        if (std::isalpha((unsigned char)ch))
            out.push_back((char)std::toupper((unsigned char)ch));

    return out;
}

std::string hillEncrypt2x2(const std::string& plain,
    const std::vector<std::vector<int>>& key) {
    std::string p = onlyUpperLetters(plain);

    if (p.size() % 2 != 0)
        p.push_back('X');

    std::string cipher;

    for (int i = 0; i < (int)p.size(); i += 2) {
        int p1 = p[i] - 'A';
        int p2 = p[i + 1] - 'A';

        int c1 = (key[0][0] * p1 + key[0][1] * p2) % 26;
        int c2 = (key[1][0] * p1 + key[1][1] * p2) % 26;

        cipher.push_back((char)('A' + c1));
        cipher.push_back((char)('A' + c2));
    }

    return cipher;
}

int main() {
    Header("HILL CIPHER");

    std::string plainText = "HELP";

    // Common invertible 2x2 key matrix
    std::vector<std::vector<int>> key = {
        { 3, 3 },
        { 2, 5 }
    };

    std::string cipherText = hillEncrypt2x2(plainText, key);

    std::cout << "Plain Text : " << onlyUpperLetters(plainText) << std::endl;
    std::cout << "Key Matrix : [[3, 3], [2, 5]]" << std::endl;
    std::cout << "Cipher Text: " << cipherText << std::endl;

    Footer();
    return 0;
}
