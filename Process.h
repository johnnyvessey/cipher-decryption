#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#pragma once

using namespace std;

class Process
{
public:
    static pair<vector<string>, vector<uint64_t>> GetWordList(const string &fileName);

    static vector<string> SplitMessageIntoWords(const string &message);

    static unordered_map<char, double> GetLetterFrequencies(const vector<string> &wordList, const vector<uint64_t> &wordFrequencies);

    static double EnglishScore(const vector<string> &words, const unordered_set<string> &wordSet);

    static unordered_set<string> ToSet(const vector<string> &list);

    static vector<string> DecryptByCipher(const vector<string> &words, const vector<int> &cipher);

    static vector<double> GetDigramFrequencies(const vector<string> &words, const vector<uint64_t> &wordFrequencies);

    static double GetDigramScore(const vector<string> &messageWords, const vector<double> &digramFrequencies);

    static double GetDigramScoreFromCipher(const vector<string> &originalMessageWords, const vector<double> &digramFrequencies, const vector<int> &cipher, const unordered_set<string> &wordSet);
};