#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct wordPackage
{
    vector<char> correctPlace;
    vector<char> incorrectPlace;
    vector<char> correctLetter;
    vector<char> incorrectLetter;
};

void fillWordList(vector<string>&);
void enterWord(vector<string>&);
wordPackage parseWord(string);
void filterIncorrectLetters(vector<string>&, wordPackage);
void filterCorrectLetters(vector<string>&, wordPackage);
void filterIncorrectPlacement(vector<string>&, wordPackage);
void filterCorrectPlacement(vector<string>&, wordPackage);
void printWords(vector<string>);

int main()
{
    vector<string> wordList;
    bool promptContinue = true;
    int promptChoice;

    fillWordList(wordList);

    while (promptContinue)
    {
        // display menu
        cout << "\n Possible Words = " << wordList.size() << "\n";
        cout << "\n   Wordle Solver 2.0   " << endl;
        cout << "------------------------" << endl;
        cout << "1) Enter Clue" << endl;
        cout << "2) Print Words" << endl;
        cout << "3) Reset" << endl;
        cout << "4) Exit" << endl;
        cout << "------------------------" << endl;
        cout << "Enter Selection: ";
        cin >> promptChoice;

        // test user choice
        switch (promptChoice)
        {
            case 1:
                enterWord(wordList);
                break;
            case 2:
                printWords(wordList);
                break;
            case 3:
                fillWordList(wordList);
                break;
            case 4:
                promptContinue = false;
                break;
            default:
                cout << "\nError: invalid selection" << endl;
        }
    }

    return 0;
}

void fillWordList(vector<string>& wordList)
{
    ifstream inputFile;
    string word;

    // opening file
    inputFile.open("words.txt");

    // testing for file opening error
    if (!inputFile)
    {
        cout << "\nError: file not opened" << endl;
        exit(1);
    }
    else
    {
        // clearing current contents of wordList
        wordList.clear();

        // putting all file contents into wordList
        while (inputFile >> word)
        {
            wordList.push_back(word);
        }
    }

    // closing file
    inputFile.close();
}

void enterWord(vector<string>& wordList)
{
    string userWord;
    wordPackage parsedWord;

    // display menu/get word
    cout << "\n---------------------------------" << endl;
    cout << "# = Correct Placement of Letter" << endl;
    cout << "$ = Incorrect Placement of Letter" << endl;
    cout << "---------------------------------" << endl;
    cout << "Please enter word: ";
    cin >> userWord;

    // parse word
    parsedWord = parseWord(userWord);

    // filter words
    filterIncorrectLetters(wordList, parsedWord);
    filterCorrectLetters(wordList, parsedWord);
    filterIncorrectPlacement(wordList, parsedWord);
    filterCorrectPlacement(wordList, parsedWord);
}

wordPackage parseWord(string userWord)
{
    wordPackage parsedWord;

    // clear vectors for input
    parsedWord.correctPlace.clear();
    parsedWord.incorrectPlace.clear();
    parsedWord.correctLetter.clear();
    parsedWord.incorrectLetter.clear();

    // parse word
    for (int i = 0; i < userWord.size(); i++)
    {
        if (userWord[i] == '#')
        {
            i++;
            parsedWord.correctPlace.push_back(userWord[i]); // fill places
            parsedWord.incorrectPlace.push_back('-');
            parsedWord.correctLetter.push_back(userWord[i]); // add letter to correct
        }
        else if (userWord[i] == '$')
        {
            i++;
            parsedWord.correctPlace.push_back('-'); // fill places
            parsedWord.incorrectPlace.push_back(userWord[i]);
            parsedWord.correctLetter.push_back(userWord[i]); // add letter to correct
        }
        else
        {
            parsedWord.correctPlace.push_back('-'); // fill places
            parsedWord.incorrectPlace.push_back('-');
            parsedWord.incorrectLetter.push_back(userWord[i]); // add letter to incorrect
        }
    }

    // test for duplicates in correct and incorrect
    for (int i = 0; i < parsedWord.correctLetter.size(); i++)
    {
        for (int j = 0; j < parsedWord.incorrectLetter.size(); j++)
        {
            // replace duplicates in incorrect
            if (parsedWord.correctLetter[i] == parsedWord.incorrectLetter[j])
            {
                parsedWord.incorrectLetter[j] = '-';
            }
        }
    }

    return parsedWord;
}

void filterIncorrectLetters(vector<string>& wordList, wordPackage parsedWord)
{
    vector<string> tempWordList;
    string word;
    bool incorrectFlag;
    int wordListSize = wordList.size();

    // eliminate words with incorrect letters
    for (int i = 0; i < wordListSize; i++)
    {
        word = wordList[i];
        incorrectFlag = false;

        // test for every letter in the word
        for (int j = 0; j < word.size(); j++)
        {
            // test for every incorrect letter
            for (int k = 0; k < parsedWord.incorrectLetter.size(); k++)
            {
                // test if current character is an incorrect letter
                if (word[j] == parsedWord.incorrectLetter[k])
                {
                    incorrectFlag = true;
                }
            }
        }

        if (!incorrectFlag)
        {
            tempWordList.push_back(word);
        }
    }

    wordList = tempWordList;
}

void filterCorrectLetters(vector<string>& wordList, wordPackage parsedWord)
{
    vector<string> tempWordList;
    string word;
    string tempWord;
    int wordListSize = wordList.size();
    int correctCount;
    bool correctFlag;

    // runs for every word in file
    for (int i = 0; i < wordListSize; i++)
    {
        word = wordList[i];
        tempWord = wordList[i];
        correctCount = 0;

        // runs for each correct letter
        for (int j = 0; j < parsedWord.correctLetter.size(); j++)
        {
            correctFlag = false;

            // runs for each character in the word or until it has found a correct letter
            for (int k = 0; k < word.size() && !correctFlag; k++)
            {
                // tests if letter is correct and increments the number of correct letters found
                if (parsedWord.correctLetter[j] == tempWord[k])
                {
                    correctCount++;
                    tempWord[k] = '-';
                    correctFlag = true;
                }
            }
        }

        // tests if number of correct letters found = the number of correct letters
        if (correctCount == parsedWord.correctLetter.size())
        {
            tempWordList.push_back(word);
        }
    }

    wordList = tempWordList;
}

void filterIncorrectPlacement(vector<string>& wordList, wordPackage parsedWord)
{
    vector<string> tempWordList;
    string word;
    bool incorrectFlag;
    int wordListSize = wordList.size();

    // test for every character in word
    for (int i = 0; i < parsedWord.incorrectPlace.size(); i++)
    {
        // test if current character is an incorrectly placed character
        if (parsedWord.incorrectPlace[i] == '-')
        {
            continue;
        }
        // runs if current character is incorrectly placed
        else
        {
            // run for every word in wordList
            for (int j = 0; j < wordListSize; j++)
            {
                word = wordList[j];
                incorrectFlag = false;

                // test if a character in word is incorretly placed
                if (word[i] == parsedWord.incorrectPlace[i])
                {
                    incorrectFlag = true;
                }

                if (!incorrectFlag)
                {
                    tempWordList.push_back(word);
                }
            }
        }

        wordList = tempWordList;
        tempWordList.clear();
    }
}

void filterCorrectPlacement(vector<string>& wordList, wordPackage parsedWord)
{
    vector<string> tempWordList;
    string word;
    bool correctFlag;
    int wordListSize = wordList.size();

    // test for every character in word
    for (int i = 0; i < parsedWord.correctPlace.size(); i++)
    {
        // test if current character is correctly placed
        if (parsedWord.correctPlace[i] == '-')
        {
            continue;
        }
        // runs if current character is correctly placed
        else
        {
            // runs for every word in wordList
            for (int j = 0; j < wordListSize; j++)
            {
                word = wordList[j];
                correctFlag = false;

                // testing if current character in word is correctly placed
                if (word[i] == parsedWord.correctPlace[i])
                {
                    correctFlag = true;
                }

                if (correctFlag)
                {
                    tempWordList.push_back(word);
                }
            }
        }

        wordList = tempWordList;
        tempWordList.clear();
    }
}

void printWords(vector<string> wordList)
{
    int wordListSize = wordList.size();

    // display header
    cout << "\n                       Correct Words" << endl;
    cout << "------------------------------------------------------------" << endl;

    // display all words in wordList
    for (int i = 0; i < wordListSize; i++)
    {
        cout << wordList[i] << " ";

        if ((i + 1) % 10 == 0)
        {
            cout << endl;
        }
    }

    // display footer
    cout << "\n------------------------------------------------------------" << endl;
}
