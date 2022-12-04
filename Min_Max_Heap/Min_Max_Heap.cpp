#include <iostream>

using namespace std;
const int DEFAULT_CAPACITY = (1<<8) + 1;

template <class T>
class MinHeap {
private:
    int capacity = DEFAULT_CAPACITY;
    int size;
    T* arr;
public:
    MinHeap() {
        size = 0;
        arr = new T[capacity];
    }
    bool isFull() {
        return size == capacity;
    }
    bool isEmpty() {
        return size == 0;
    }
    int getSize() {
        return size;
    }
    void push(T data) {
        if (isFull()) return;
        int current_index = ++size;
        while (true) {
            if (current_index == 1) break;
            if (data >= arr[current_index/2]) break;
            arr[current_index] = arr[current_index/2];
            current_index /= 2;
        }
        arr[current_index] = data;
    }
    T pop() {
        if (isEmpty()) return T();
        int i, j;
        T retValue = arr[1];
        T k = arr[size--];
        for (i=1, j=2; j<=size; ) {
            if (j<size and arr[j] >= arr[j+1]) j++;
            if (k < arr[j]) break;
            arr[i] = arr[j];
            i = j; j *= 2;
        }
        arr[i] = k;
        return retValue;
    }
    T top() {
        return arr[1];
    }
    friend ostream& operator << (ostream& os, MinHeap<T> heap) {
        os << "MinHeap [(key, value)]\n";
        for (int i=1; i<=heap.size; i++)
            os << "--> (" << i << ", " << heap.arr[i] << ")\n";
        return os;
    }
};

template <class T>
class MaxHeap {
private:
    int capacity = DEFAULT_CAPACITY;
    int size;
    T* arr;
public:
    MaxHeap() {
        size = 0;
        arr = new T[capacity];
    }
    bool isFull() {
        return size == capacity;
    }
    bool isEmpty() {
        return size == 0;
    }
    int getSize() {
        return size;
    }
    void push(T data) {
        if (isFull()) return;
        int current_index = ++size;
        while (true) {
            if (current_index == 1) break;
            if (data < arr[current_index/2]) break;
            arr[current_index] = arr[current_index/2];
            current_index /= 2;
        }
        arr[current_index] = data;
    }
    T pop() {
        if (isEmpty()) return T();
        int i, j;
        T retValue = arr[1];
        T k = arr[size--];
        for (i=1, j=2; j<=size; ) {
            if (j<size and arr[j] < arr[j+1]) j++;
            if (k >= arr[j]) break;
            arr[i] = arr[j];
            i = j; j *= 2;
        }
        arr[i] = k;
        return retValue;
    }
    T top() {
        return arr[1];
    }
    friend ostream& operator << (ostream& os, MaxHeap<T> heap) {
        os << "MaxHeap [(key, value)]\n";
        for (int i=1; i<=heap.size; i++)
            os << "--> (" << i << ", " << heap.arr[i] << ")\n";
        return os;
    }
};
