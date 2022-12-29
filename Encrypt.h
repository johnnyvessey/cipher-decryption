#include <string>
#include <vector>
#include <unordered_map>

#pragma once

using namespace std;
class Encrypt
{
public:
    static char IntToChar(size_t x);

    static unordered_map<char, char> CreateCipher(const string &alphabet);

    static vector<string> EncryptMessage(vector<string> const &message, std::string const &alphabet);
};
