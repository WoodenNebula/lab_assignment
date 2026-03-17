/* Algorithm (Caesar Cipher - Encrypt and Decrypt):
1. Start
2. Define `caesarEncrypt(text, key)`:
   a. For each character in text:
      i. If alphabetic, shift it by `key` positions in cyclic order.
      ii. Keep spaces/symbols unchanged.
   b. Return encrypted text.
3. Define `caesarDecrypt(cipher, key)`:
   a. Call encryption with negative key, i.e., shift backward.
4. In `main`:
   a. Set plaintext and key.
   b. Generate cipher text.
   c. Decrypt cipher text back to plaintext.
   d. Also show output for a changed key value.
5. End
*/

#include <cctype>
#include <iostream>
#include <string>

#include "src/commons.hpp"

int normalizeKey(int key) {
    key %= 26;
    if (key < 0) key += 26;
    return key;
}

char shiftChar(char ch, int key) {
    if (!std::isalpha((unsigned char)ch)) return ch;

    char base = std::isupper((unsigned char)ch) ? 'A' : 'a';
    return (char)(base + (ch - base + key) % 26);
}

std::string caesarEncrypt(const std::string& text, int key) {
    key = normalizeKey(key);

    std::string cipher = text;
    for (int i = 0; i < (int)cipher.size(); i++)
        cipher[i] = shiftChar(cipher[i], key);

    return cipher;
}

std::string caesarDecrypt(const std::string& cipher, int key) {
    return caesarEncrypt(cipher, -key);
}

int main() {
    Header("CAESAR CIPHER");

    std::string plainText = "MEET ME AFTER THE TOGA PARTY";
    int key = 3;

    std::string cipherText = caesarEncrypt(plainText, key);
    std::string decrypted = caesarDecrypt(cipherText, key);

    std::cout << "Plain Text : " << plainText << std::endl;
    std::cout << "Key        : " << key << std::endl;
    std::cout << "Cipher Text: " << cipherText << std::endl;
    std::cout << "Decrypted  : " << decrypted << std::endl;

    int changedKey = 7;
    std::cout << "\nCipher with changed key (" << changedKey << "): "
        << caesarEncrypt(plainText, changedKey) << std::endl;

    Footer();
    return 0;
}
