#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

template <class T>
class LinearDeque {
private:
    T* _array;
    int _front, _rear;
    int _capacity;
public:
    LinearDeque(int capacity = 10) : _capacity(capacity) {
        _front = _rear = -1;
        _array = new T[capacity];
    }
    bool empty();
    void ChangeSizeID();
    void push_front(T t);
    void push_rear(T t);
    void pop_front();
    void pop_rear();
    T front();
    T rear();
    friend ostream& operator << (ostream& os, LinearDeque<T> ld) {
        os << "_front: " << ld._front << " | _rear: " << ld._rear << " | _capacity: " << ld._capacity << endl;
        if (!ld.empty()) {
            for (int i=ld._front; i<=ld._rear; i++) {
                if (i == ld._front) os << "| ";
                os << "[" << i << "]:" << ld._array[i] << " | ";
            }
        }
        os << endl;
        return os;
    }
};
template <class T>
void LinearDeque<T>::ChangeSizeID() {
    T* newQueue = new T[_capacity*2];
    int prev_count = _rear-_front+1;
    int new_front = _capacity*2/3; int new_rear = new_front+prev_count-1;
    memcpy(newQueue + new_front, _array+_front, sizeof(T) * prev_count);
    delete[] _array;
    _array = newQueue;
    _front = new_front; _rear = new_rear;
    _capacity *= 2;
}

template <class T>
bool LinearDeque<T>::empty() {
    return _front == -1 and _rear == -1;
}

template <class T>
void LinearDeque<T>::push_front(T t) {
    if (_rear == -1 and _front == -1) {
        _rear = _front = _capacity/3; // 시작점을 용량의 1/3 지점으로 함.
    }
    else {
        if (_front == 0)  // index[0]에 원소가 존재할 시 expand capacity
            ChangeSizeID();
        _front--;
    }
    _array[_front] = t;
}

template <class T>
void LinearDeque<T>::push_rear(T t) {
    if (_rear == -1 and _front == -1) {
        _rear = _front = _capacity/3; // 시작점을 용량의 1/3 지점으로 함.
    }
    else {
        if (_rear == _capacity-1)  // index[0]에 원소가 존재할 시 expand capacity
            ChangeSizeID();
        _rear++;
    }
    _array[_rear] = t;
}

template <class T>
void LinearDeque<T>::pop_front() {
    if (empty()) throw "Queue is empty";
    if (_front == _rear) {
        _array[_front].~T();
        _front = _rear = -1;
    }
    else
        _array[_front++].~T();
}

template <class T>
void LinearDeque<T>::pop_rear() {
    if (empty()) throw "Queue is empty";
    if (_rear == _front) {
        _array[_rear].~T();
        _front = _rear = -1;
    }
    else
        _array[_rear--].~T();
}

template<class T>
T LinearDeque<T>::front() {
    return _array[_front];
}

template<class T>
T LinearDeque<T>::rear() {
    return _array[_rear];
}


int main() {
    srand(time(nullptr));

    while (true) {
        int input;
        cout << "[1]: push test" << endl;
        cout << "[2]: pop/empty test" << endl;
        cout << "[0]: exit" << endl;
        cout << ">>"; cin >> input;
        switch (input) {
            case 0:
                cout << "Bye"; exit(0);
            case 1: {
                LinearDeque<int> ld(2);
                for (int i=0; i<10; i++) {
                    int push_front_count = rand()%3+1;
                    int push_rear_count = rand()%3+1;
                    for (int j=0; j<push_front_count; j++) {
                        int temp = rand()%20+1;
                        cout << "push front " << temp << endl;
                        ld.push_front(temp); cout << ld;
                    }
                    for (int j=0; j<push_rear_count; j++) {
                        int temp = rand()%20+1;
                        cout << "push rear " << temp << endl;
                        ld.push_rear(temp); cout << ld;
                    }
                }
            }
            break;
            case 2: {
                LinearDeque<int> ld(2);
                cout << "\npush_front ten times" << endl;
                for (int i=0; i<10; i++) {
                    int temp = rand()%20+1;
                    ld.push_front(temp);
                }
                cout << ld;
                cout << "\npop_front and pop_rear each five times" << endl;
                for (int i=0; i<5; i++) {
                    ld.pop_front();
                    ld.pop_rear();
                }
                cout << ld;
                if (ld.empty()) cout << "deque is empty!" << endl;
            }
            break;
        }
    }
}