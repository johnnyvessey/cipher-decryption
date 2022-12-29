#include <iostream>
#include <string>
#include <unordered_map>

#include "Encrypt.h"

using namespace std;

char Encrypt::IntToChar(size_t x)
{
    return (char)(x + int('a'));
}

unordered_map<char, char> Encrypt::CreateCipher(const string &alphabet)
{
    unordered_map<char, char> mapping;

    for (size_t i = 0; i < alphabet.size(); i++)
    {
        mapping[Encrypt::IntToChar(i)] = alphabet[i];
    }

    return mapping;
}
vector<string> Encrypt::EncryptMessage(vector<string> const &messageWords, string const &alphabet)
{
    if (alphabet.size() != 26)
    {
        throw("Alphabet is incorrect size");
    }

    const unordered_map<char, char> mapping = CreateCipher(alphabet);
    vector<string> encrypted = messageWords;

    for (size_t wordIndex = 0; wordIndex < messageWords.size(); wordIndex++)
    {
        for (size_t i = 0; i < messageWords[wordIndex].size(); i++)
        {
            encrypted[wordIndex][i] = mapping.at(messageWords[wordIndex][i]);
        }
    }

    return encrypted;
}