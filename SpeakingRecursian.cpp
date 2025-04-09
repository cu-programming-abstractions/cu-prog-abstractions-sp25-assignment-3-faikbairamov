
#include "SpeakingRecursian.h"
#include "error.h"
using namespace std;

Vector<string> helper(int syllablesLeft, bool isFirst) {
    if (syllablesLeft == 0) return {""}; // base case

    Vector<string> result;

    Vector<char> vowels = {'e', 'i', 'u'};
    Vector<char> consonants = {'b', 'k', 'n', 'r', 's', '\''};

    // option 1: if first syllable, try using a single vowel
    if (isFirst) {
        for (char v : vowels) {
            for (string suffix : helper(syllablesLeft - 1, false)) {
                result += string(1, v) + suffix;
            }
        }
    }

    // option 2: trying all consonant + vowel combinations
    for (char c : consonants) {
        for (char v : vowels) {
            for (string suffix : helper(syllablesLeft - 1, false)) {
                result += string(1, c) + string(1, v) + suffix;
            }
        }
    }

    return result;
}

Vector<string> allRecursianWords(int numSyllables) {
    if (numSyllables < 0) {
        error("Number of syllables cannot be negative.");
    }



    // Recursive helper function
    return helper(numSyllables, true);
}




/* * * * * Test Cases Below This Point * * * * */
#include "GUI/SimpleTest.h"
#include "GUI/TextUtils.h"
#include "set.h"

PROVIDED_TEST("allRecursianWords works in simple cases.") {
    EXPECT_EQUAL(allRecursianWords(0), {""});
    EXPECT_ERROR(allRecursianWords(-1));
    EXPECT_ERROR(allRecursianWords(-137));
}

PROVIDED_TEST("allRecursianWords works for length 1.") {
    auto wordsUnsorted = allRecursianWords(1);

    /* Sort the words using a set. This will also eliminate duplicates. */
    Set<string> words;
    for (string word: wordsUnsorted) {
        /* All one-syllable words have length one or two. */
        EXPECT(word.length() == 1 || word.length() == 2);

        words += word;
    }

    /* Should be the same number of words once we sort them. */
    EXPECT_EQUAL(wordsUnsorted.size(), words.size());

    Set<string> expected = {
        "'e", "'i", "'u", "be", "bi", "bu", "e", "i",
        "ke", "ki", "ku", "ne", "ni", "nu", "re", "ri",
        "ru", "se", "si", "su", "u"
    };

    EXPECT_EQUAL(words, expected);
}

PROVIDED_TEST("allRecursianWords has the right quantities of words.") {
    EXPECT_EQUAL(allRecursianWords(0).size(), 1);
    EXPECT_EQUAL(allRecursianWords(1).size(), 21);
    EXPECT_EQUAL(allRecursianWords(2).size(), 378);
    EXPECT_EQUAL(allRecursianWords(3).size(), 6804);
    EXPECT_EQUAL(allRecursianWords(4).size(), 122472);
}

namespace {
    bool isConsonant(char ch) {
        return ch == 'b' || ch == 'k' || ch == 'n' || ch == 'r' || ch == 's' || ch == '\'';
    }
    bool isVowel(char ch) {
        return ch == 'e' || ch == 'i' || ch == 'u';
    }
}

PROVIDED_TEST("allRecursianWords produces words consisting of consonants and vowels.") {
    /* There are too many words here to check the answer against a reference - and it's
     * infeasible to manually create a reference. Instead, we will generate a list of
     * words and confirm that each one is either a consonant or a vowel. If we see
     * anything else, we know the answer is wrong.
     *
     * This won't catch everything, though. It's entirely possible to have a word made
     * entirely of vowels pass, or a word of way too many consonants pass. You will
     * need to write your own test cases to cover cases not detected here.
     */
    auto words = allRecursianWords(4);
    EXPECT_NOT_EQUAL(words.size(), 0);

    for (string word: words) {
        for (char ch: word) {
            EXPECT(isConsonant(ch) || isVowel(ch));
        }
    }
}
PROVIDED_TEST("All 2-syllable words start with valid first syllable") {
    auto words = allRecursianWords(2);

    for (string word : words) {
        if (isVowel(word[0])) {
            EXPECT(word.length() == 3); // e.g. "e" + "bi"
            EXPECT(isConsonant(word[1]));
            EXPECT(isVowel(word[2]));
        } else {
            EXPECT(isConsonant(word[0]));
            EXPECT(isVowel(word[1]));
            EXPECT(word.length() == 4); // e.g. "bi" + "ku"
            EXPECT(isConsonant(word[2]));
            EXPECT(isVowel(word[3]));
        }
    }
}

PROVIDED_TEST("All 3-syllable words have valid lengths (5 or 6)") {
    auto words = allRecursianWords(3);
    for (string word : words) {
        EXPECT(word.length() == 5 || word.length() == 6);
    }
}
