/* Algorithm (Rail Fence Cipher - Encrypt and Decrypt):
1. Start
2. Encryption:
   a. Create `key` rails (rows).
   b. Traverse plaintext in zig-zag (down then up) and place each character
      into the current rail.
   c. Concatenate rails from top to bottom to get cipher text.
3. Decryption:
   a. Mark zig-zag path positions in a matrix.
   b. Fill marked positions row by row using cipher text characters.
   c. Traverse matrix again in zig-zag order to reconstruct plaintext.
4. In `main`, show encrypted and decrypted results.
5. End
*/

#include <iostream>
#include <string>
#include <vector>

#include "src/commons.hpp"

std::string railFenceEncrypt(const std::string& text, int key) {
    std::vector<std::string> rails(key);

    int row = 0;
    int dir = 1;

    for (char ch : text) {
        rails[row].push_back(ch);

        if (row == 0) dir = 1;
        else if (row == key - 1) dir = -1;

        row += dir;
    }

    std::string cipher;
    for (const auto& r : rails)
        cipher += r;

    return cipher;
}

std::string railFenceDecrypt(const std::string& cipher, int key) {
    int n = (int)cipher.size();
    std::vector<std::vector<char>> mat(key, std::vector<char>(n, '\n'));

    int row = 0, dir = 1;
    for (int col = 0; col < n; col++) {
        mat[row][col] = '*';

        if (row == 0) dir = 1;
        else if (row == key - 1) dir = -1;

        row += dir;
    }

    int idx = 0;
    for (int r = 0; r < key; r++)
        for (int c = 0; c < n; c++)
            if (mat[r][c] == '*' && idx < n)
                mat[r][c] = cipher[idx++];

    std::string plain;
    row = 0;
    dir = 1;

    for (int col = 0; col < n; col++) {
        plain.push_back(mat[row][col]);

        if (row == 0) dir = 1;
        else if (row == key - 1) dir = -1;

        row += dir;
    }

    return plain;
}

int main() {
    Header("RAIL FENCE CIPHER");

    std::string plainText = "WEAREDISCOVEREDFLEEATONCE";
    int key = 3;

    std::string cipherText = railFenceEncrypt(plainText, key);
    std::string decrypted = railFenceDecrypt(cipherText, key);

    std::cout << "Plain Text : " << plainText << std::endl;
    std::cout << "Key        : " << key << std::endl;
    std::cout << "Cipher Text: " << cipherText << std::endl;
    std::cout << "Decrypted  : " << decrypted << std::endl;

    Footer();
    return 0;
}
