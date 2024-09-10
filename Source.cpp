#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

/*
 * ��������� ��������� ����� �� 'A' �� 'Z'
 * 
 * @return ���������� ��������� ����� �� 'A' �� 'Z'.
 */
char get_RandChar() {
    return 'A' + rand() % 26;
}

/*
 * ��������� ������ ��������� �� n ��������� �������� ����������� ��������
 * 
 * @param length ����� ������������ ������
 * @return ������ �� n ��������� �������� ����������� ��������
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
 * ���� � ������ ����� (����� ����������� ������� ����� ��������), ��������� �������� ��������� ����������� ������ �����������
 * 
 * @param length ����� ����������� ������
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
