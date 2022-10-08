#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

template <typename T> class Node;
template <typename T> class List;
template <typename T> class CircularList;
template <typename T, class LISTCLASS> class ListIterator;

/* Node ---------------------------------------------------------- */
template <typename T>
class Node {
    friend class List<T>;
    friend class CircularList<T>;
    template <typename T1, class LISTCLASS> // partial specialized의 묘리가 여기 담겨있다.
    friend class ListIterator;
public:
    Node(T data) : data(data) {
        link = nullptr;
    }
private:
    T data;
    Node* link;
};
/* --------------------------------------------------------------- */

/* List ---------------------------------------------------------- */
template <typename T>
class List {
    friend class ListIterator<T, List>;
    friend class CircularList<T>;
public:
    List() {
        first = nullptr;
    }
    virtual void Add(T x) {
        Node<T>* newNode = new Node<T>(x);
        if (first == nullptr) first = newNode;
        else {
            newNode->link = first;
            first = newNode;
        }
    }
    virtual void Delete(T x) {
        if (first == nullptr)
            cout << "Error - List is empty. not deleted" << endl;
        else if (first->data == x) {
            Node<T>* temp = first;
            delete first;
            first = temp->link;
        }
        else {
            Node<T>* prev = first;
            Node<T>* next = first->link;
            while (next != nullptr and next->data != x) {
                prev = next;
                next = next->link;
            }
            if (next == nullptr) ;
                // cout << "Error - Can't find element. not deleted" << endl;
            else {
                prev->link = next->link;
                delete next;
            }
        }
    }
    friend ostream& operator << (ostream& os, const List<T>& ls) {
        ListIterator<T, List> listIterator(ls);
        if (listIterator.Null()) os << "List is empty";
        else {
            os << listIterator.First();
            while (!listIterator.NextNull())
                os << " -> " << listIterator.Next();
        }
        return os;
    }
private:
    Node<T>* first;
};
/* --------------------------------------------------------------- */

/* CircularList -------------------------------------------------- */
template <typename T>
class CircularList : public List<T> {
    friend class ListIterator<T, CircularList>;
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
        ListIterator<T, CircularList> listIterator(cq);
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
template <typename T, class LISTCLASS>
class ListIterator {
public:
    ListIterator(const LISTCLASS& ls) : list(ls), current(ls.first) {
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
        ListIterator<T, LISTCLASS> old = *this;
        current = current->link;
        return old;
    }
    bool operator != (const ListIterator<T, LISTCLASS> b) const {
        return current != b.current;
    }
    bool operator == (const ListIterator<T, LISTCLASS> b) const {
        return current == b.current;
    }
private:
    const LISTCLASS& list;
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
         << "----- Add cq1 -----"
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












