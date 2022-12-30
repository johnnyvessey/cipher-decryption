# cipher-decryption
Decrypting substitution cipher encrypted text without knowing the cipher pattern

A substitution cipher is when every letter of the alphabet has been swapped with another letter based on a specific mapping.
For example, all a's in the original message might become q, for example, based on the mapping that was determined.

This code uses hill climbing to decrypt a message, without knowing anything about the cipher mapping.
It does this by swapping possible letter mappings until the encrypted message looks more and more like English text.

To quantify what "looking like English text" means, I computed the frequency of digrams (two letter combination) from a list of the
most common English words, and then I can score a message on how "English" it seems, but looking at all of the digrams in the message
and seeing if it matches the common digrams in English. This process works because we only need a few correct guesses of the mapping before the 
message starts to match the digram frequencies of English better.
