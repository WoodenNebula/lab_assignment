/* Algorithm (Vigenere and Vernam Cipher):
1. Start
2. Vigenere Cipher:
   a. Convert plaintext and key to uppercase letters.
   b. Repeat key to match plaintext length.
   c. Encrypt: C[i] = (P[i] + K[i]) mod 26.
   d. Decrypt: P[i] = (C[i] - K[i] + 26) mod 26.
3. Vernam Cipher (XOR stream cipher form):
   a. For each character i: C[i] = P[i] XOR K[i].
   b. For readability, represent cipher bytes in hexadecimal.
   c. Decrypt similarly: P[i] = C[i] XOR K[i].
4. Print encryption/decryption results for both ciphers.
5. End
*/

#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "src/commons.hpp"

std::string alphaUpper(const std::string& text) {
    std::string out;
    for (char ch : text)
        if (std::isalpha((unsigned char)ch))
            out.push_back((char)std::toupper((unsigned char)ch));
    return out;
}

std::string vigenereEncrypt(const std::string& plain, const std::string& key) {
    std::string p = alphaUpper(plain);
    std::string k = alphaUpper(key);
    std::string c;

    for (int i = 0; i < (int)p.size(); i++) {
        int x = p[i] - 'A';
        int y = k[i % (int)k.size()] - 'A';
        c.push_back((char)('A' + (x + y) % 26));
    }

    return c;
}

std::string vigenereDecrypt(const std::string& cipher, const std::string& key) {
    std::string c = alphaUpper(cipher);
    std::string k = alphaUpper(key);
    std::string p;

    for (int i = 0; i < (int)c.size(); i++) {
        int x = c[i] - 'A';
        int y = k[i % (int)k.size()] - 'A';
        p.push_back((char)('A' + (x - y + 26) % 26));
    }

    return p;
}

std::string vernamEncryptHex(const std::string& plain, const std::string& key) {
    std::ostringstream out;

    for (int i = 0; i < (int)plain.size(); i++) {
        unsigned char byte = (unsigned char)(plain[i] ^ key[i % (int)key.size()]);
        out << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
            << (int)byte;
    }

    return out.str();
}

int hexValue(char ch) {
    if (ch >= '0' && ch <= '9') return ch - '0';
    return 10 + (ch - 'A');
}

std::string vernamDecryptHex(const std::string& hexCipher, const std::string& key) {
    std::string plain;

    for (int i = 0; i < (int)hexCipher.size(); i += 2) {
        int value = hexValue(hexCipher[i]) * 16 + hexValue(hexCipher[i + 1]);
        char ch = (char)(value ^ key[(i / 2) % (int)key.size()]);
        plain.push_back(ch);
    }

    return plain;
}

int main() {
    Header("VIGENERE AND VERNAM CIPHER");

    std::string vigPlain = "ATTACK AT DAWN";
    std::string vigKey = "LEMON";

    std::string vigCipher = vigenereEncrypt(vigPlain, vigKey);
    std::string vigDec = vigenereDecrypt(vigCipher, vigKey);

    std::cout << "[Vigenere]\n";
    std::cout << "Plain Text : " << alphaUpper(vigPlain) << std::endl;
    std::cout << "Key        : " << alphaUpper(vigKey) << std::endl;
    std::cout << "Cipher Text: " << vigCipher << std::endl;
    std::cout << "Decrypted  : " << vigDec << std::endl;

    std::string vernamPlain = "HELLOCRYPTO";
    std::string vernamKey = "XMCKLXMCKLX";

    std::string vernamCipherHex = vernamEncryptHex(vernamPlain, vernamKey);
    std::string vernamDec = vernamDecryptHex(vernamCipherHex, vernamKey);

    std::cout << "\n[Vernam]\n";
    std::cout << "Plain Text : " << vernamPlain << std::endl;
    std::cout << "Key        : " << vernamKey << std::endl;
    std::cout << "Cipher Hex : " << vernamCipherHex << std::endl;
    std::cout << "Decrypted  : " << vernamDec << std::endl;

    Footer();
    return 0;
}
