#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <functional>

using namespace std;

class NaturalDivisors {
private:
    const int number;
    vector<int> divisors;

    // ������ �����������
    NaturalDivisors(const NaturalDivisors&) = delete; // ����������� �����������
    NaturalDivisors& operator=(const NaturalDivisors&) = delete; // �������� ������������

    /*
    * ���������� ���������
    */
    void calculateDivisors() {
        for (int i = 1; i <= number; ++i) {
            if (number % i == 0) {
                divisors.push_back(i);
            }
        }
    }

public:
    NaturalDivisors(int num) : number(num) {
        if (num <= 0) throw invalid_argument("Number must be natural.");
        calculateDivisors();
    }

    /*
    * ����� ��� ��������� ���������
    * 
    * @return ��������
    */
    const vector<int>& getDivisors() const {
        return divisors;
    }

    /*
    * ����� ��� ��������� �������� ��������������� ���������
    * 
    * @return ������� �������� ���������
    */
    double averageOfDivisors() const {
        if (divisors.empty()) return 0.0;
        int sum = accumulate(divisors.begin(), divisors.end(), 0);
        return static_cast<double>(sum) / divisors.size();
    }

    /*
    * ����� ��� ��������� ������ �����
    * 
    * @return �����
    */
    int getNumber() const {
        return number;
    }

    /*
    * ����� ��� "�����������" �������
    * 
    * @return ������������� ������
    */
    unique_ptr<NaturalDivisors> copy_new() const {
        return make_unique<NaturalDivisors>(number);
    }

    // ������������� ������� ��� ���������� ��������� ������
    friend ostream& operator<<(ostream& os, const NaturalDivisors& nd) {
        os << "Divisors of " << nd.number << ": ";
        copy(nd.divisors.begin(), nd.divisors.end(), ostream_iterator<int>(os, " "));
        return os;
    }

    /*
    * ������������� ������� ��� ������ ��� � �������������� std::bind
    * 
    * @param a ������ �����
    * @param b ������ �����
    */
    friend int nod_with_bind(int a, int b);

    /*
    * ������������� ������� ��� ������ ��� � �������������� ������-���������
    * 
    * @param a ������ �����
    * @param b ������ �����
    */
    friend int nod_with_lambda(int a, int b);

};


/*
* ������� ��� ���������� ���
*
* @param a ������ �����
* @param b ������ �����
* 
* @return ���
*/
int nod(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// ������� ��� ���������� ��� � �������������� std::bind
int nod_with_bind(int a, int b) {
    // �������� �������, ������� ���� ��� � a � ����������
    auto nod_function = bind(nod, a, placeholders::_1);

    // ���������� find_if ��� ������ ������� ��������� b
    int result = -1; // ��������� ��������
    vector<int> divisors = { b };
    auto it = find_if(divisors.begin(), divisors.end(), nod_function);
    int index = abs(divisors.begin() - it);
    return nod_function(divisors[index]);
}

// ������� ��� ���������� ��� � �������������� ������-���������
int nod_with_lambda(int a, int b) {
    auto find_if_nod = [&a](int b) {
        return nod(a, b); // �������� a � ������-���������
        };

    int result = 0;
    vector<int> divisors = { b }; // ������ ���������
    auto it = find_if(divisors.begin(), divisors.end(),
        [&](int div) {
            result = find_if_nod(div);
            return result > 1;  // ���������� ��� ������ ������ ��� ������ 1
        });

    return result; // ���������� ���������
}

int main() {
    int N = 5; // ���������� ��������
    vector<unique_ptr<NaturalDivisors>> divisorsList;

    // �������� N ��������
    for (int i = 1; i <= N; ++i) {
        divisorsList.push_back(make_unique<NaturalDivisors>(i * 10));
    }

    // ����� ��������� � �� �������� ���������������
    for (const auto& divisorObj : divisorsList) {
        cout << *divisorObj << endl;
        cout << "Average of divisors: " << divisorObj->averageOfDivisors() << "\n\n";
    }

    // ���� �������� �� ������������
    int userDivisor;
    cout << "Enter a divisor to count: ";
    cin >> userDivisor;

    // ������� �������� � ��������� ���������
    int count = count_if(divisorsList.begin(), divisorsList.end(),
        [userDivisor](const unique_ptr<NaturalDivisors>& obj) {
            return obj->getNumber() % userDivisor == 0;
        });

    cout << "Count of numbers divisible by " << userDivisor << ": " << count << endl;

    // ������ ����������� ������� � �������������� ������ copy_new
    auto copiedObject = divisorsList[0]->copy_new();
    cout << "\nCopied object: " << *copiedObject << endl;

    cout << "\nExample: 36 & 60" << endl;
    NaturalDivisors num1(36);
    NaturalDivisors num2(60);

    cout << "\nNOD using std::bind: " << nod_with_bind(num1.getNumber(), num2.getNumber()) << endl;
    cout << "NOD using lambda: " << nod_with_lambda(num1.getNumber(), num2.getNumber()) << endl;

    return 0;
}