#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

template <class T>
class CircularQueue {
private:
    int _front, _rear;
    int _capacity;
    T* _array;
public:
    CircularQueue(int capacity) : _capacity(capacity) {
        _front = 0; _rear = 0;
        _array = new T[_capacity];
    }
    bool empty() {
        return _front == _rear;
    }
    void push(T t) {
        if (_front == (_rear+1)%_capacity) {
            cout << "expand capacity\n";
            T* newQueue = new T[_capacity*2];
            int newFront = _capacity*2/3; int newRear = newFront + _capacity - 1;
            memcpy(newQueue+newFront, _array+_front, sizeof(T)*(_capacity-_front));
            memcpy(newQueue+newFront+_capacity-_front, _array, sizeof(T)*_rear);
            delete[] _array;
            _array = newQueue; _capacity = _capacity*2;
            _front = newFront; _rear = newRear;
        }
        _array[_rear] = t;
        _rear = (_rear + 1) % _capacity;
    }
    void pop() {
        if (empty()) throw "Queue is empty\n";
        _array[_front].~T();
        _front = (_front + 1) % _capacity;
    }
    T front() {
        if (empty()) throw "Queue is empty\n";
        return _array[_front];
    }
    T rear() {
        if (empty()) throw "Queue is empty\n";
        return _array[_rear-1];
    }
    friend ostream& operator << (ostream& os, CircularQueue cq) {
        cout << "_front: " << cq._front << " | _rear: " << cq._rear << " | _capacity: " << cq._capacity << endl;
        if (cq._front > cq._rear) {
            for (int i=cq._front; i<cq._rear+cq._capacity; i++)
                cout << "[" << i%cq._capacity << "]:" << cq._array[i%cq._capacity] << " | ";
        }
        else {
            for (int i=cq._front; i<cq._rear; i++)
                cout << "[" << i << "]:" << cq._array[i] << " | ";
        }
        return os;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    srand(time(nullptr));

    CircularQueue<int> cq(2);
    cout << "------------------------push------------------------" << endl;
    for (int i=0; i<rand()%10+10; i++) {
        int temp; temp = rand()%20+5;
        cout << "push " << temp << endl;
        cq.push(temp); cout << cq << endl;
    }
    cout << "------------------------pop------------------------" << endl;
    for (int i=0; i<10; i++) {
        cout << "pop" << endl;
        cq.pop(); cout << cq << endl;
    }
    cout << "------------------------push------------------------" << endl;
    for (int i=0; i<rand()%10+10; i++) {
        int temp; temp = rand()%20+5;
        cout << "push " << temp << endl;
        cq.push(temp); cout << cq << endl;
    }
    cout << "------------------------pop------------------------" << endl;
    for (int i=0; i<10; i++) {
        cout << "pop" << endl;
        cq.pop(); cout << cq << endl;
    }
}