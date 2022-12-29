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

string VectorToString(const vector<string> &decryptionAttempt)
{
    string decryptedMessage;
    decryptedMessage.reserve(5 * decryptionAttempt.size());

    for (const auto &word : decryptionAttempt)
    {
        decryptedMessage += word;
        decryptedMessage += " ";
    }

    return decryptedMessage;
}

void indexSwap(vector<int> &cipher, int i, int j)
{
    int tmp = cipher[i];
    cipher[i] = cipher[j];
    cipher[j] = tmp;
}

vector<string> Decrypt::HillClimb(const vector<string> &originalMessageWords, const vector<int> &startCipher, const vector<double> &digramFrequencies, const unordered_set<string> &wordSet)
{
    bool foundMaximum = false;

    vector<int> cipher = startCipher;
    vector<int> bestCipher = startCipher;
    int steps = 0;
    double currentDigramScore = 0;

    while (!foundMaximum && steps < 30)
    {
        foundMaximum = true;

        // have this weight English words + their frequency once many of the words are english
        // have english score that takes into account word frequency
        currentDigramScore = Process::GetDigramScoreFromCipher(originalMessageWords, digramFrequencies, cipher, wordSet);

        for (int i = 0; i < 25; i++)
        {
            for (int j = i + 1; j < 26; j++)
            {
                indexSwap(cipher, i, j);
                double tryScore = Process::GetDigramScoreFromCipher(originalMessageWords, digramFrequencies, cipher, wordSet);
                if (tryScore > currentDigramScore)
                {
                    currentDigramScore = tryScore;
                    foundMaximum = false;
                    bestCipher = cipher;
                }

                // swap values back for next iteration
                indexSwap(cipher, i, j);
            }
        }

        cipher = bestCipher;
        steps++;
    }

    const vector<string> answer = Process::DecryptByCipher(originalMessageWords, cipher);
    return answer;
}

string Decrypt::DecryptMessage(const string &message, const string &fileName)
{
    const auto wordPair = Process::GetWordList(fileName);
    const vector<string> &wordList = wordPair.first;
    const vector<uint64_t> &wordFrequencies = wordPair.second;

    const unordered_map<char, double> wordListLetterFrequencies = Process::GetLetterFrequencies(wordList, wordFrequencies);

    const vector<string> messageWords = Process::SplitMessageIntoWords(message);

    const unordered_set<string> wordSet = Process::ToSet(wordList);

    const unordered_map<char, double> letterFrequenciesOfMessage = Process::GetLetterFrequencies(messageWords, wordFrequencies);

    const vector<double> digramFrequencies = Process::GetDigramFrequencies(wordList, wordFrequencies);

    auto rd = std::random_device{};
    auto rng = std::default_random_engine{rd()};

    const vector<int> cipher = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};

    const int numStartingLocations = 100;
    const double stopThreshold = 0.95;

    // create starting ciphers, then perform stochastic hill climbing (find local maximum)
    for (size_t i = 0; i < numStartingLocations; i++)
    {
        if (i > 0 && i % 1000 == 0)
        {
            cout << "Steps: " << i << endl;
        }
        vector<int> startCipher = cipher;
        shuffle(begin(startCipher), end(startCipher), rng);

        vector<string> decryptionAttempt = HillClimb(messageWords, startCipher, digramFrequencies, wordSet);

        if (Process::EnglishScore(decryptionAttempt, wordSet) >= stopThreshold)
        {
            // found match
            cout << "Found after " << i + 1 << " steps" << endl;
            return VectorToString(decryptionAttempt);
        }
    }
    return "No translations found";
}