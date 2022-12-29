#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <algorithm>
#include <random>

#include "Process.h"
#include "Decrypt.h"

using namespace std;

int charToInt(char c)
{
    return (int)c - (int)'a';
}

char intToChar(int x)
{
    return (char)(x + (int)'a');
}

pair<vector<string>, vector<uint64_t>> Process::GetWordList(const string &fileName)
{
    std::ifstream file(fileName);
    std::string line;

    const size_t maxWords = 50000;
    size_t numWords = 0;

    vector<string> wordList;
    vector<uint64_t> wordFrequencyList;

    wordList.reserve(maxWords);
    wordFrequencyList.reserve(maxWords);

    // skip header line of csv file
    std::getline(file, line);

    while (std::getline(file, line) && numWords < maxWords)
    {
        const int commaIndex = line.find(',');

        wordList.push_back(line.substr(0, commaIndex));
        wordFrequencyList.push_back(stoull(line.substr(commaIndex + 1)));
        numWords++;
    }

    return pair<vector<string>, vector<uint64_t>>(wordList, wordFrequencyList);
}

vector<string> Process::SplitMessageIntoWords(const string &message)
{
    stringstream stream(message);
    string word;
    vector<string> words;
    while (getline(stream, word, ' '))
    {
        string processedString;
        processedString.reserve(word.size());

        for (char c : word)
        {
            c = (char)tolower(c);
            if ((int)'a' <= (int)c && (int)c <= (int)'z')
            {
                processedString += c;
            }
        }

        words.push_back(processedString);
    }

    return words;
}

unordered_map<char, double> Process::GetLetterFrequencies(const vector<string> &wordList, const vector<uint64_t> &wordFrequencies)
{
    unordered_map<char, uint64_t> letterCounts;
    uint64_t numCharacters = 0;

    for (size_t i = 0; i < wordList.size(); i++)
    {
        for (const char &c : wordList[i])
        {
            numCharacters += wordFrequencies[i];
            letterCounts[c] += wordFrequencies[i];
        }
    }

    unordered_map<char, double> letterFrequencies;

    for (const auto &pair : letterCounts)
    {
        letterFrequencies[pair.first] = (double)pair.second / numCharacters;
    }

    return letterFrequencies;
}

unordered_set<string> Process::ToSet(const vector<string> &list)
{
    unordered_set<string> set;
    set.reserve(list.size());

    for (const string &word : list)
    {
        set.insert(word);
    }

    return set;
}

double Process::EnglishScore(const vector<string> &words, const unordered_set<string> &wordSet)
{
    size_t wordCount = words.size();
    size_t correctWords = 0;

    for (const string &word : words)
    {
        if (wordSet.find(word) != wordSet.end())
        {
            correctWords += 1;
        }
    }

    return (double)correctWords / wordCount;
}

map<double, char, greater<double>> GetMostFrequentLetters(const unordered_map<char, double> lf)
{
    map<double, char, greater<double>> mostFrequent;

    for (const auto &pair : lf)
    {
        mostFrequent[pair.second] = pair.first;
    }

    return mostFrequent;
}

vector<string> Process::DecryptByCipher(const vector<string> &words, const vector<int> &cipher)
{
    vector<string> decrypted;
    decrypted.reserve(words.size());

    for (const string &word : words)
    {
        string decryptedWord;
        decryptedWord.reserve(word.size());

        for (const char &c : word)
        {
            decryptedWord += intToChar(cipher[charToInt(c)]);
        }

        decrypted.push_back(decryptedWord);
    }

    return decrypted;
}

vector<double> Process::GetDigramFrequencies(const vector<string> &words, const vector<uint64_t> &wordFrequencies)
{
    vector<uint64_t> digramCounts(26 * 26, 0);

    uint64_t totalDigramCount = 0;

    for (size_t wordIndex = 0; wordIndex < words.size(); wordIndex++)
    {
        const string &word = words[wordIndex];

        if (word.size() >= 2)
        {
            int first = charToInt(word[0]);
            int second;

            for (size_t i = 1; i < word.size(); i++)
            {
                second = charToInt(word[i]);
                digramCounts[first * 26 + second] += wordFrequencies[wordIndex];
                totalDigramCount += wordFrequencies[wordIndex];
                first = second;
            }
        }
    }

    vector<double> digramFrequencies;
    digramFrequencies.reserve(26 * 26);

    for (const uint64_t &count : digramCounts)
    {
        digramFrequencies.push_back((double)count / totalDigramCount);
    }

    return digramFrequencies;
}

double Process::GetDigramScore(const vector<string> &messageWords, const vector<double> &digramFrequencies)
{
    double score = 0.0;

    for (const string &word : messageWords)
    {
        if (word.size() >= 2)
        {
            int first = charToInt(word[0]);
            int second;
            for (size_t i = 1; i < word.size(); i++)
            {
                second = charToInt(word[i]);
                score += digramFrequencies.at(first * 26 + second);
                first = second;
            }
        }
    }

    return score;
}

double Process::GetDigramScoreFromCipher(const vector<string> &originalMessageWords, const vector<double> &digramFrequencies, const vector<int> &cipher, const unordered_set<string> &wordSet)
{
    double score = 0.0;

    int englishWords = 0;
    for (const string &word : originalMessageWords)
    {
        if (word.size() >= 2)
        {
            int first = cipher[charToInt(word.at(0))];
            int second;
            for (size_t i = 1; i < word.size(); i++)
            {
                second = cipher.at(charToInt(word.at(i)));
                score += digramFrequencies.at(first * 26 + second);
                first = second;
            }
        }

        string newString;
        newString.reserve(word.size());

        for (const char &c : word)
        {
            newString += intToChar(cipher.at(charToInt(c)));
        }

        if (wordSet.find(newString) != wordSet.end())
        {
            englishWords++;
        }
    }

    double englishScore = (double)englishWords / originalMessageWords.size();

    // hacky and the optimal "magic" number changes with every new message; instead have it be based on whether the english score is high enough
    // or the proportion of words that are english is high enough
    return score + (score > .6 ? englishScore : 0);
}