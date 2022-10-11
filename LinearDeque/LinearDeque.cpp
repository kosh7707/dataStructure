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
        _front = _rear = 0;
        _array = new T[capacity];
    }
    bool empty();
    void push_front(T t);
    void push_rear(T t);
    void pop_front();
    void pop_rear();
    T front();
    T rear();
    friend ostream& operator << (ostream& os, LinearDeque<T> ld) {
        os << "_front: " << ld._front << " | _rear: " << ld._rear << " | _capacity: " << ld._capacity << endl;
        for (int i=ld._front; i<ld._rear; i++) {
            os << "[" << i << "]:" << ld._array[i] << " | ";
        }
        return os;
    }
};

template <class T>
bool LinearDeque<T>::empty() {
    return _front == _rear;
}

template <class T>
void LinearDeque<T>::push_front(T t) {
    if (_rear + 1 == _capacity) {
        cout << "expand capacity" << endl;
        T* newQueue = new T[_capacity*2];
        int prev_count = _rear - _front;
        int new_front = _capacity*2/3; int new_rear = new_front + prev_count;
        memcpy(newQueue + new_front, _array + _front, sizeof(T) * prev_count);
        delete[] _array;
        _array = newQueue;
        _front = new_front; _rear = new_rear;
        _capacity *= 2;
    }
    T* temp = new T[_capacity];
    temp[_front] = t;
    memcpy(temp+_front+1, _array + _front, sizeof(T)*(_rear - _front));
    _rear += 1;
    delete[] _array;
    _array = temp;
}

template <class T>
void LinearDeque<T>::push_rear(T t) {
    if (_rear + 1 == _capacity) {
        cout << "expand capacity" << endl;
        T* newQueue = new T[_capacity*2];
        int prev_count = _rear - _front;
        int new_front = _capacity*2/3; int new_rear = new_front + prev_count;
        memcpy(newQueue + new_front, _array + _front, sizeof(T) * prev_count);
        delete[] _array;
        _array = newQueue;
        _front = new_front; _rear = new_rear;
        _capacity *= 2;
    }
    _array[_rear++] = t;
}

template <class T>
void LinearDeque<T>::pop_front() {
    if (empty()) throw "Queue is empty";
    _array[_front++].~T();
}

template <class T>
void LinearDeque<T>::pop_rear() {
    if (empty()) throw "Queue is empty";
    _array[--_rear].~T();
}

template<class T>
T LinearDeque<T>::front() {
    return _array[_front];
}

template<class T>
T LinearDeque<T>::rear() {
    return _array[_rear-1];
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    srand(time(nullptr));

    LinearDeque<int> ld(2);
    for (int i=0; i<rand()%10+10; i++) {
        int temp; temp = rand()%20+1;
        cout << "push front " << temp << endl;
        ld.push_front(temp);
        cout << ld << endl;
    }
    for (int i=0; i<rand()%10+10; i++) {
        int temp; temp = rand()%20+1;
        cout << "push rear " << temp << endl;
        ld.push_rear(temp);
        cout << ld << endl;
    }
    for (int i=0; i<10; i++) {
        cout << "pop rear " << endl;
        ld.pop_rear();
        cout << ld << endl;
    }
    for (int i=0; i<10; i++) {
        cout << "pop front " << endl;
        ld.pop_front();
        cout << ld << endl;
    }
}