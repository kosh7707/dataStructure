#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

template <class T> class ChainNode;
template <class T> class Chain;
template <class T> class ChainIterator;

/* ChainNode 구현 begin ------------------------------------------------------------- */
template <class T>
class ChainNode {
    friend class Chain<T>;
    friend class ChainIterator<T>;
public:
    ChainNode(T element = nullptr) {
        data = element;
        link = nullptr;
    }
private:
    T data;
    ChainNode<T>* link;
};
/* ChainNode 구현 end ------------------------------------------------------------- */

/* Chain 구현 begin ------------------------------------------------------------- */
template <class T>
class Chain {
    friend class ChainIterator<T>;
public:
    Chain() {
        first = nullptr;
    }
    int Length() {
        int count = 0;
        if (first == nullptr) return count;
        else {
            ChainNode<T>* next = first;
            do {
                next = next->link;
                count++;
            } while(next != nullptr);
        }
        return count;
    }
    void Add(const T& element) {
        ChainNode<T>* newNode = new ChainNode<T>(element);
        if (first != nullptr) {
            newNode->link = first;
            first = newNode;
        }
        else first = newNode;
    }
    void Delete() {
        if (first != nullptr) {
            ChainNode<T>* next = first->link;
            delete first;
            first = next;
        }
        else cout << "Error - Empty List. not deleted" << endl;
    }
    void Invert() {
        ChainNode<T>* prev = first;
        ChainNode<T>* next = nullptr;
        while (prev != nullptr) {
            ChainNode<T>* temp = next;
            next = prev;
            prev = prev->link;
            next->link = temp;
        }
        first = next;
    }
    void Concatenate(Chain<T> b) {
        if (first != nullptr) {
            ChainNode<T>* temp = first;
            while (temp->link != nullptr)
                temp = temp->link;
            temp->link = b.first;
        }
        else first = b.first;
    }
    friend ostream& operator << (ostream& os, Chain<T> chain) {
        ChainIterator<T> chainIterator(chain);
        if (!chainIterator.Null()) {
            os << chainIterator.First();
            while (!chainIterator.NextNull())
                os << " -> " << chainIterator.Next();
        }
        return os;
    }
private:
    ChainNode<T>* first;
};
/* Chain 구현 end ------------------------------------------------------------- */

/* ChainIterator 구현 begin ------------------------------------------------------------- */
template <class T>
class ChainIterator {
public:
    ChainIterator(const Chain<T>& ls) : list(ls) {
        current = ls.first;
    }
    T& operator * () const {
        return current -> data;
    }
    T* operator -> () const {
        return &current -> data;
    }
    ChainIterator<T>& operator ++ () {
        current = current->link;
        return *this;
    }
    ChainIterator<T> operator ++ (int) {
        ChainIterator<T> old = *this;
        current = current->link;
        return old;
    }
    bool Null() {
        return current == nullptr;
    }
    bool NextNull() {
        return !Null() and current->link == nullptr;
    }
    T First() {
        if (list.first != nullptr)
            return list.first->data;
        else return 0;
    }
    T Next() {
        if (current != nullptr) {
            current = current->link;
            return current->data;
        }
        else return 0;
    }
private:
    const Chain<T> list;
    ChainNode<T>* current;
};
/* ChainIterator 구현 end ------------------------------------------------------------- */

int main() {
    srand(time(nullptr));

    Chain<int> A, B;
    for (int i=0; i<10; i++) {
        A.Add(rand()%50+1);
        B.Add(rand()%50+1);
    }
    cout << "-----Add A-----\n" << A << endl
         << "-----Add B-----\n" << B << endl;

    A.Delete(); B.Delete();
    cout << "-----Delete A-----\n" << A << endl
         << "-----Delete B-----\n" << B << endl;

    A.Invert(); B.Invert();
    cout << "-----Invert A-----\n" << A << endl
         << "-----Invert B-----\n" << B << endl;

    A.Concatenate(B);
    cout << "-----Concatenate A-----\n" << A << endl;

    int sum = 0;
    ChainIterator<int> chainIterator(A);
    sum += chainIterator.First();
    while (!chainIterator.NextNull())
        sum += chainIterator.Next();
    cout << "-----Sum A-----\n" << sum << endl;

    int length = A.Length();
    cout << "-----Length A-----\n" << length << endl;
}



















