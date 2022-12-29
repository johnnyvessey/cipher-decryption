#include "Encrypt.h"
#include "Decrypt.h"
#include "Process.h"

#include <string>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

void printWordFrequencies(unordered_map<char, double> map)
{
    for (const auto &pair : map)
    {
        cout << pair.first << ": " << pair.second << endl;
    }
}

int main()
{
    const string message = "Four score and seven years ago our fathers brought we can not dedicate we can not consecrate we can not hallow, this ground The brave men, living and dead, who struggled here";
    const string alphabet = "bcdefgnhjklmiopqsrtuwvxyza";
    auto messageList = Process::SplitMessageIntoWords(message);
    auto encryptedWords = Encrypt::EncryptMessage(messageList, alphabet);

    string encryptedMessage;
    for (const auto &word : encryptedWords)
    {
        encryptedMessage += word;
        encryptedMessage += " ";
    }

    cout << Decrypt::DecryptMessage(encryptedMessage, "unigram_freq.csv") << endl;

    return 0;
}