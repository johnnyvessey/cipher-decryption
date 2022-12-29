#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>

#pragma once

using namespace std;

class Decrypt
{
public:
    static string DecryptMessage(const string &message, const string &fileName);

    static vector<string> HillClimb(const vector<string> &messageWords, const vector<int> &startCipher, const vector<double> &digramFrequencies, const unordered_set<string> &wordSet);
};