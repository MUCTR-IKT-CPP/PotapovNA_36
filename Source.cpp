#include <iostream>

using namespace std;

template <typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(T value) : data(value), next(nullptr) {}
    };

    Node* front;
    Node* back;
    size_t size;

public:
    Queue() : front(nullptr), back(nullptr), size(0) {}

    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    /**
    * ��������, ����� �� �������.
    * @return true, ���� ������� �����, ����� false.
    */
    bool isEmpty() const {
        return front == nullptr;
    }

    /**
    * @brief ��������� �������� ������� �������.
    * @return ���������� ��������� � �������.
    */
    size_t getSize() const {
        return size;
    }

    /*
    * ���������� �������� � ����� �������.
    * @param value ��������, ������� ����� �������� � �������.
    */
    void enqueue(const T& value) {
        Node* newNode = new Node(value);
        if (isEmpty()) {
            front = back = newNode;
        }
        else {
            back->next = newNode;
            back = newNode;
        }
        size++;
    }

    /**
    * ��������� �������� �� ������ �������
    * 
    * @return ��������� �� ������ �������� � ������ �������.
    */
    T frontElement() const {
        if (isEmpty()) {
            throw runtime_error("Queue is empty");
        }
        return front->data;
    }

    /*
     * �������� �������� �� ������ �������.
     */
    void dequeue() {
        if (isEmpty()) {
            throw runtime_error("Queue is empty");
        }
        Node* temp = front;            // ��������� ��������� �� ������ �������
        front = front->next;           // ���������� ��������� front �� ��������� �������
        delete temp;                   // ����������� ������ ��� ������� ������� ��������
        size--;
        if (isEmpty()) {
            back = nullptr;
        }
    }

      

    class Iterator {
    private:
        Node* current;

    public:
        Iterator(Node* node) : current(node) {}

        T& operator*() {
            return current->data;
        }

        Iterator& operator++() {
            if (current) current = current->next;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    /*
    * ��������� ���������, ������������ �� ������ ������� �������.
    * 
    * @return �������� ������ �������.
    */
    Iterator begin() {
        return Iterator(front);
    }

    /*
    * ��������� ���������, ������������ �� ����� ������� (nullptr).
    * 
    * @return �������� ����� �������.
    */
    Iterator end() {
        return Iterator(nullptr);
    }


    friend ostream& operator<<(ostream& os, Queue<T>& q) {
        os << "Queue {";
        bool first = true;  // ���� ��� ������ ��������
        for (auto el : q) {
            if (!first) {
                os << ", ";  // ��������� ������� ����� ������������ ����������
            }
            os << el;  // ������� ������� �������
            first = false;  // ������������� ���� � false ����� ������� ��������
        }
        os << "}";
        return os;
      // ��� ��� ��������
      // for (Queue<int>::Iterator i = q.begin(); i != q.end(); ++i) {
      //     int el = *i;  // �������� �������� �������� ��������
      //     cout << el << " ";
      // }
    }

    void display() {
        cout << *this << endl;
    }

};


int main() {
    Queue<int> q;

    int n;
    do {
        cout << "\n Operation:\n";
        cout << "1. Add element to end\n";
        cout << "2. Get element from beginning\n";
        cout << "3. Remove element from beginning\n";
        cout << "4. Get size of queue\n";
        cout << "9. Exit the programm\n";

        cin >> n;

        switch (n) {
        
        case 1:
            cout << "Enter value: ";
            int value;
            cin >> value;
            q.enqueue(value);
            cout << endl;
            q.display();
            break;
        case 2:
            q.display();
            if (!q.isEmpty()) {
                cout << "Element from start: " << q.frontElement() << endl;
            }
            else {
                cout << "Queue is empty!" << endl;
            }
            break;
        case 3:
            if (!q.isEmpty()) {
                q.dequeue();
                cout << "\n Updated queue: ";
                q.display();
            }
            else {
                cout << "Queue is empty!" << endl;
            }
            break;

        case 4:
            q.display();
            cout << "Queue size: " << q.getSize() << "\n";
            break;

        case 9:
            break;

        default:
            cout << "Incorret input\n";
        }
    } while (n != 9);

    return 0;
}

