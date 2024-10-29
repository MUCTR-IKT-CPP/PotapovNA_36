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
    * Проверка, пуста ли очередь.
    * @return true, если очередь пуста, иначе false.
    */
    bool isEmpty() const {
        return front == nullptr;
    }

    /**
    * @brief Получение текущего размера очереди.
    * @return Количество элементов в очереди.
    */
    size_t getSize() const {
        return size;
    }

    /*
    * Добавление элемента в конец очереди.
    * @param value Значение, которое нужно добавить в очередь.
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
    * Получение элемента из начала очереди
    * 
    * @return Указатель на данные элемента в начале очереди.
    */
    T frontElement() const {
        if (isEmpty()) {
            throw runtime_error("Queue is empty");
        }
        return front->data;
    }

    /*
     * Удаление элемента из начала очереди.
     */
    void dequeue() {
        if (isEmpty()) {
            throw runtime_error("Queue is empty");
        }
        Node* temp = front;            // Сохраняем указатель на первый элемент
        front = front->next;           // Перемещаем указатель front на следующий элемент
        delete temp;                   // Освобождаем память для старого первого элемента
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
    * Получение итератора, указывающего на первый элемент очереди.
    * 
    * @return Итератор начала очереди.
    */
    Iterator begin() {
        return Iterator(front);
    }

    /*
    * Получение итератора, указывающего на конец очереди (nullptr).
    * 
    * @return Итератор конца очереди.
    */
    Iterator end() {
        return Iterator(nullptr);
    }


    friend ostream& operator<<(ostream& os, Queue<T>& q) {
        os << "Queue {";
        bool first = true;  // Флаг для первой итерации
        for (auto el : q) {
            if (!first) {
                os << ", ";  // Добавляем запятую перед последующими элементами
            }
            os << el;  // Выводим текущий элемент
            first = false;  // Устанавливаем флаг в false после первого элемента
        }
        os << "}";
        return os;
      // как оно работает
      // for (Queue<int>::Iterator i = q.begin(); i != q.end(); ++i) {
      //     int el = *i;  // Получаем значение текущего элемента
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

