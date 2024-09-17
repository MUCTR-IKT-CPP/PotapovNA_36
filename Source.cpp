#include <iostream>
#include <algorithm> 
#include <vector>

using namespace std;

/*
 * ����� ������� �� �����
 */
void printArray(int** array, int n) {
    cout << "Array:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << array[i][j] << "\t";
        }
        cout << endl;
    }
}

/*
 * ������� ��� ��������� ������ � ���������� �������
 *
 * @return ���������� ������ ����� �� 0 �� n.
 */
int** createAndFillArray(int n) {
    int** array = new int* [n];
    for (int i = 0; i < n; i++) {
        array[i] = new int[n];
    }

    int k = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            array[i][j] = k++;
        }
    }

    printArray(array, n);
    return array;
}

/*
 * ������� ��� ������������ ������
 */
void deleteArray(int** array, int n) {
    for (int i = 0; i < n; i++) {
        delete[] array[i];
    }
    delete[] array;
}

/*
 * ������� ��� ���������� ��������� �������� ���������
 *
 * @return ���������� ������ ����� c �������� ��������� ���������
 */
int** reverseArray(int** array, int n) {
    for (int i = 0; i < n; i++) {
        reverse(array[i], array[i] + n); // Reverse each row
    }

    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n / 2; i++) {
            swap(array[i][j], array[n - 1 - i][j]);
        }
    }

    printArray(array, n);
    return array;
}


/*
 * ���������� ���������, ��� �� ��� ���� �������� ����� ������� ���������.
 *
 * @return ���������� ��������������� ������
 */
int** mainDiagonal(int** array, int n) {
    vector<int> elements;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            elements.push_back(array[i][j]);
        }
    }

    // ���������� ���������
    sort(elements.begin(), elements.end());

    int index = 0;
    for (int d = 0; d < 2 * n - 1; d++) {
        // �������� �� ��������� ������� ���������
        for (int i = max(0, d - (n - 1)); i <= min(d, n - 1); i++) {
            int j = d - i;
            array[i][j] = elements[index++];
        }
    }

    printArray(array, n);
    return array;
}

/*
 * ���������� ���������, ��� �� ��� ���� �������� ����� �������� ���������.
 *
 * @return ���������� ��������������� ������
 */
int** subMainDiagonal(int** array, int n) {
    vector<int> elements;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            elements.push_back(array[i][j]);
        }
    }

    // ���������� ���������
    sort(elements.begin(), elements.end());

    int index = 0;
    for (int d = 0; d < 2 * n - 1; d++) {
        // �������� �� ��������� �������� ���������
        for (int i = max(0, d - (n - 1)); i <= min(d, n - 1); i++) {
                int j = n - 1 - (d - i);
            array[i][j] = elements[index++];
        }
    }

    printArray(array, n);
    return array;
}

/*
 * ���������� ���������, ����� ��� �������� ������������� �� ������� ������� �������� �� �������� [0,0].
 *
 * @return ���������� ��������������� ������
 */
int** spiralOrder(int** array, int n) {
    int top = 0, bottom = n - 1, left = 0, right = n - 1;
    int num = 1;

    while (top <= bottom && left <= right) {
        for (int i = left; i <= right; i++) {
            array[top][i] = num++;
        }
        top++;

        for (int i = top; i <= bottom; i++) {
            array[i][right] = num++;
        }
        right--;

        if (top <= bottom) {
            for (int i = right; i >= left; i--) {
                array[bottom][i] = num++;
            }
            bottom--;
        }

        if (left <= right) {
            for (int i = bottom; i >= top; i--) {
                array[i][left] = num++;
            }
            left++;
        }
    }
    printArray(array, n);
    return array;
}


int main() {
    int n = 0;
    int x = 0;
    int flag = 0;

    cout << "Enter the value N: ";
    cin >> n;

    int** array = createAndFillArray(n);

    do {
        cout << "What should be done with the array: " << endl;
        cout << "1. Restore original order" << endl;
        cout << "2. Reverse order" << endl;
        cout << "3. Elements along main diagonal" << endl;
        cout << "4. Elements along reverse diagonal" << endl;
        cout << "5. Elements in spiral order" << endl;
        cout << "6. Close programm" << endl;
        cin >> x;

        switch (x) {
        case 1:
            deleteArray(array, n);
            array = createAndFillArray(n);
            break;
        case 2:
            reverseArray(array, n);
            break;
        case 3:
            mainDiagonal(array, n);
            break;
        case 4:
            subMainDiagonal(array, n);
            break;
        case 5:
            spiralOrder(array, n);
            break;
        case 6:
            flag = 1;
            deleteArray(array, n);
            break;
        default:
            cout << "Incorrect input" << endl;
            flag = 1;
            break;
        }

    } while (flag == 0);

    return 0;
}