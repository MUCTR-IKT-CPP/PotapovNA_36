#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <map>
#include <tuple>
#include <algorithm>

using namespace std;

// �������� �������
enum Denomination {
    ONE = 1,
    FIVE = 5,
    TEN = 10,
    TWENTY = 20,
    FIFTY = 50,
    HUNDRED = 100,
    THOUSAND = 1000
};

// ��� ������
enum CurrencyType {
    RUB,
    USD,
    EUR
};

// ��������� ��������
struct Banknote {
    CurrencyType type;  // ��� ������
    double rate;    // ���� � �����
    Denomination denomination; // �������
};

/*
 * ������� ����� ����� � ������
 *
 * @param vector<Banknote>& wallet ������ �� ����������� ������ �������� ���� `Banknote`
 *
 * @return ���������� ������������ ����� � ������
 */
double calculateTotalInRoubles(const vector<Banknote>& wallet) {
    double sum = 0.0;
    for (const Banknote& note : wallet) {
        sum += note.denomination * note.rate;
    }
    return sum;
}

/*
 * ����� ��������������� �������
 *
 * @param vector<Banknote>& wallet ������ �� ����������� ������ �������� ���� `Banknote`
 */
void printBanknotes(const vector<Banknote>& wallet) {
    cout << "Banknotes:" << endl;
    for (const Banknote& note : wallet) {
        string currencyName;
        switch (note.type) {
         case RUB:
            currencyName = "|RUB|";
            break;
         case USD:
            currencyName = "|USD|";
            break;
         case EUR: currencyName = "|EUR|";
            break;
        }
        cout << "Nominal: " << note.denomination << ", type: " << currencyName
            << ", rate: " << fixed << setprecision(2) << note.rate << endl;
    }
}

/*
 * ����� ��������������� �������
 *
 * @param vector<Banknote>& wallet ������ �� ����������� ������ �������� ���� `Banknote`
 * @param Denomination filterValue �������� �� �������� ���������� ����
 */
void filterBanknotesByDenomination(const vector<Banknote>& wallet, Denomination filterValue) {
    int countRUB = 0, countUSD = 0, countEUR = 0;
    for (const Banknote& note : wallet) {
        if (note.denomination > filterValue) {
            switch (note.type) {
             case RUB:
                countRUB++;
                break;
             case USD:
                countUSD++;
                break;
             case EUR: countEUR++;
                break;
            }
        }
    }
    cout << "RUB: " << countRUB << endl;
    cout << "USD: " << countUSD << endl;
    cout << "EUR: " << countEUR << endl;
}

/*
 * ����� ������� ��������, ���������������� �� ������� � ���������
 *
 * @param vector<Banknote>& wallet ������ �� ����������� ������ �������� ���� Banknote
 */
void printWalletComposition(const vector<Banknote>& wallet) {
    map<pair<string, Denomination>, int> walletComposition;
    for (const Banknote& note : wallet) {
        string currencyName;
        switch (note.type) {
            case RUB: 
                currencyName = "RUB"; 
                break;
            case USD: 
                currencyName = "USD"; 
                break;
            case EUR: 
                currencyName = "EUR"; 
                break;
        }
        walletComposition[{currencyName, note.denomination}]++;
    }

    cout << "\nCurrent wallet:" << endl;
    for (const auto& entry : walletComposition) {
        cout << entry.first.first << ": " << entry.first.second << " - " << entry.second << endl;
    }
}

/*
 * ����������� ���� ������� � �����, ��������� �������� � ������������ ���������
 *
 * @param vector<Banknote>& wallet ������ �� ����������� ������ �������� ���� Banknote
 */
void convertToRoublesUsingMaxDenomination(const vector<Banknote>& wallet) {
    double totalInRoubles = calculateTotalInRoubles(wallet);

    //����� ������������� ��������
    Denomination maxDenomination = ONE; 
    for (const Banknote& note : wallet) {
        if (note.denomination > maxDenomination) {
            maxDenomination = note.denomination;
        }
    }

    //���������� ����������� ������� � ������������ ���������
    int numOfNotes = static_cast<int>(ceil(totalInRoubles / static_cast<double>(maxDenomination)));

    cout << "\nTotal sum: " << fixed << setprecision(2) << totalInRoubles << " rub" << endl;
    cout << "Max nominal: " << maxDenomination << endl;
    cout << "Banknotes required: " << numOfNotes << endl;
}


int main() {
    srand(time(0));

    int N;
    cout << "Enter the N value: ";
    cin >> N;

    vector<Banknote> wallet(N);

    // ������ �����
    Banknote currencies[] = {
        {RUB, 1.0, ONE},  
        {USD, 75.0, ONE}, 
        {EUR, 85.0, ONE}  
    };

    // ������ ���������
    Denomination denominations[] = {ONE, FIVE, TEN, TWENTY, FIFTY, HUNDRED, THOUSAND};

    // ��������� �������
    for (int i = 0; i < N; i++) {
        wallet[i].denomination = denominations[rand() % (sizeof(denominations) / sizeof(denominations[0]))];
        int k = rand() % (sizeof(currencies) / sizeof(currencies[0]));
        wallet[i].type = currencies[k].type;
        wallet[i].rate = currencies[k].rate;
    }

    printBanknotes(wallet);

    int filterValue;
    cout << "\nNominal filter: ";
    cin >> filterValue;

    // ���������� � ����� ����������
    filterBanknotesByDenomination(wallet, Denomination(filterValue));

    // ����� ������� ��������
    printWalletComposition(wallet);

    // ����������� � ����� � �������������� ������������� ��������
    convertToRoublesUsingMaxDenomination(wallet);

    return 0;
}