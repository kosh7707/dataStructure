#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

template <class T> class Node;
template <class T> class CircularList;
template <class T> class ListIterator;

/* Node ---------------------------------------------------------- */
template <class T>
class Node {
    friend class CircularList<T>;
    friend class ListIterator<T>;
public:
    Node(T data) : data(data) {
        link = nullptr;
    }
private:
    T data;
    Node* link;
};
/* --------------------------------------------------------------- */

/* CircularList -------------------------------------------------- */
template <class T>
class CircularList {
    friend class ListIterator<T>;
public:
    CircularList() {
        first = last = nullptr;
    }
    void Add(T x) {
        Node<T>* newNode = new Node<T>(x);
        if (first == nullptr) {
            first = last = newNode;
            first->link = first;
        }
        else if (first->data > x) {
            newNode->link = first;
            first = newNode;
            last->link = first;
        }
        else {
            Node<T>* prev = first;
            Node<T>* next = first->link;
            while (prev != last and next->data < x) {
                prev = next;
                next = next->link;
            }
            newNode->link = prev->link;
            prev->link = newNode;
            if (prev == last) last = newNode;
        }
    }
    void Delete(T x) {
        if (first == nullptr)
            cout << "Error - List is empty. not deleted" << endl;
        else if (first->data == x and first->link == first) {
            delete first;
            first = last = nullptr;
        }
        else if (first->data == x and first->link != first) {
            Node<T>* temp = new Node(x);
            temp->link = first->link;
            delete first;
            first = temp->link;
            last->link = first;
        }
        else {
            Node<T>* prev = first;
            Node<T>* next = first->link;
            while (prev != last and next->data != x) {
                prev = next;
                next = next->link;
            }
            if (prev == last);
                // cout << "Error - Can't find element " << x << ". not deleted" << endl;
            else {
                prev->link = next->link;
                if (next == last) last = prev;
                delete next;
            }
        }
    }
    void Merge(const CircularList<T> ls) {
        Node<T>* prev = ls.first;
        Node<T>* next = ls.first->link;
        do {
            Add(prev->data);
            prev = next;
            next = next->link;
        } while (prev != ls.first);
    }
    friend ostream& operator << (ostream& os, const CircularList<T>& cq) {
        ListIterator<T> listIterator(cq);
        if (listIterator.Null()) os << "List is empty";
        else {
            os << listIterator.First();
            while (listIterator.Current() != cq.last)
                os << " -> " << listIterator.Next();
        }
        return os;
    }
private:
    Node<T>* first;
    Node<T>* last;
};
/* --------------------------------------------------------------- */

/* ListIterator -------------------------------------------------- */
template <class T>
class ListIterator {
public:
    ListIterator(const CircularList<T>& ls) : list(ls), current(ls.first) {
        cout << endl << "List Iterator is constructed" << endl;
    }
    bool Null() {
        return list.first == nullptr;
    }
    bool NextNull() {
        return current->link == nullptr;
    }
    T First() {
        return list.first->data;
    }
    T Next() {
        current = current->link;
        return current->data;
    }
    Node<T>* Current() {
        return current;
    }
    T& operator * () const {
        return *(current->data);
    }
    T* operator -> () const {
        return current->data;
    }
    ListIterator& operator ++ () {
        current = current->link;
        return *this;
    }
    ListIterator operator ++ (int) {
        ListIterator<T> old = *this;
        current = current->link;
        return old;
    }
    bool operator != (const ListIterator<T> b) const {
        return current != b.current;
    }
    bool operator == (const ListIterator<T> b) const {
        return current == b.current;
    }
private:
    const CircularList<T>& list;
    Node<T>* current;
};

/* --------------------------------------------------------------- */

int main() {
    srand(time(nullptr));

    CircularList<int> cq1, cq2;
    for (int i=0; i<15; i++) {
        cq1.Add(rand()%50+1);
        cq2.Add(rand()%50+1);
    }

    cout << "----- Add cq1 -----"
         << cq1 << endl
         << "----- Add cq2 -----"
         << cq2 << endl;

    for (int i=0; i<30; i++)
        cq1.Delete(i);
    cout << "----- Delete cq1 -----"
         << cq1 << endl;

    cq1.Merge(cq2);
    cout << "----- Merge cq1, cq2 -----"
         << cq1 << endl;

    return 0;
}












