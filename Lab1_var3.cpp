#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

/*
 * Генерация случайной буквы от 'A' до 'Z'
 * 
 * @return Возвращает случайную букву от 'A' до 'Z'.
 */
char get_RandChar() {
    return 'A' + rand() % 26;
}

/*
 * Генерация строки состоящей из n случайных символов английского алфавита
 * 
 * @param length длина генерируемой строки
 * @return строка из n случайных символов английского алфавита
 */
string gen_RandString(int const length) {
    string rndString;
    for (int i = 0; i < length; i++) {
        char rndChar = 'A' + rand() % 26;
        rndString += rndChar;
    }
    return rndString;
}

/*
 * Игра в угадай слово (слово угадывается выбирая буквы случайно), программа пытается подобрать аналогичную строку посимвольно
 * 
 * @param length Длина подбираемой строки
 */
void wordGuessingGame(int const length) {
    srand(time(0));
    string targetWord = gen_RandString(length);
    string guessedWord;
    int iter = 0;
    int maxK = 0;
    int k = 0;
    int i = 0;
    while (guessedWord != targetWord) {
        char rndChar = get_RandChar();
        k++;
        if (targetWord[i] == rndChar) {
            guessedWord += rndChar;
            maxK = max(maxK, k);
            k = 0;
            i++;
        }
        iter++;
        cout << "Current guessed word: " << guessedWord << endl;
    }
    cout << "Total iter required: " << iter << endl;
    cout << "Maximum attempts to guess a single character: " << maxK << endl;
}

int main() {
    int length = 1000;
    wordGuessingGame(length);
    return 0;
}
