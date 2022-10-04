#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

template <class T>
class LinearQueue {
private:
    int _front, _rear;
    int _capacity;
    T* _array;
public:
    LinearQueue(int capacity) : _capacity(capacity) {
        _front = 0; _rear = 0;
        _array = new T[_capacity];
    }
    bool empty() {
        return _front == _rear;
    }
    void push(T t) {
        if (_rear+1 == _capacity) {
            T* temp = new T[_capacity*2];
            memcpy(temp, _array, sizeof(T)*_capacity);
            _capacity *= 2;
            delete[] _array;
            _array = temp;
        }
        _array[_rear++] = t;
    }
    void pop() {
        if (empty()) throw "Queue is empty.\n";
        _array[_front++].~T();
    }
    T front() {
        if (empty()) throw "Queue is empty.\n";
        return _array[_front];
    }
    T rear() {
        if (empty()) throw "Queue is empty.\n";
        return _array[_rear-1];
    }
    int size() {
        return _rear - _front;
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    srand(time(nullptr));

    LinearQueue<int> lq(10);
    cout << "|";
    for (int i=0; i<20; i++) {
        int temp = rand() % 30 + 1;
        cout << temp << "|";
        lq.push(temp);
    }
    cout << "\n";
    for (int i=0; i<20; i++) {
        cout << "lq.front(): " << lq.front() << endl;
        cout << "lq.rear(): " << lq.rear() << endl;
        cout << "lq.size(): " << lq.size() << endl;
        cout << "---pop---" << endl; lq.pop();
    }

    cout << "lq.empty(): " << lq.empty() << endl;
}