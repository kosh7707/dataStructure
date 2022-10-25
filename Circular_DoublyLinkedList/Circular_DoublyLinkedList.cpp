#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

template<class T> class DoublyListNode;
template<class T> class CircularDoublyList;
template<class T> class CircularDoublyListIterator;

/* DoublyListNode ------------------------------------------------ */
template <class T>
class DoublyListNode {
    friend class CircularDoublyList<T>;
    friend class CircularDoublyListIterator<T>;
private:
    T data;
    DoublyListNode<T>* llink, * rlink;
};
/* --------------------------------------------------------------- */

/* CircularDoublyListIterator ------------------------------------ */
template<class T>
class CircularDoublyListIterator {
public:
    CircularDoublyListIterator(const CircularDoublyList<T>& l) : list(l) {
        current = l.first->rlink;
    }
    T* Next() {
        current = current->rlink;
        if (current != list.first) return &current->data;
        else return nullptr;
    }
    T* First() {
        if (current != list.first) return &current->data;
        else return nullptr;
    }
    bool NotNull() {
        if (current != list.first) return true;
        else return false;
    }
    bool NextNotNull() {
        if (current->rlink != list.first) return true;
        else return false;
    }
private:
    const CircularDoublyList<T>& list;
    DoublyListNode<T>* current;
};
/* --------------------------------------------------------------- */

/* CircularDoublyList -------------------------------------------- */
template <class T>
class CircularDoublyList {
    friend class CircularDoublyListIterator<T>;
public:
    CircularDoublyList() {
        first = new DoublyListNode<T>; // 이게 바로 headNode가 있다는 소리임.
        first->llink = first;
        first->rlink = first;
        first->data = -1;
    }
    void Insert(T t) {
        DoublyListNode<T>* p;
        auto* temp = new DoublyListNode<T>();
        p = first; temp->data = t;
        while (!(p->rlink->data == -1 or t < p->rlink->data))
            p = p->rlink;
        temp->llink = p; temp->rlink = p->rlink;
        p->rlink->llink = temp; p->rlink = temp;
    }
    bool Delete(T t) {
        auto* current = first;
        do {
            if (current->data == t) {
                current->llink->rlink = current->rlink;
                current->rlink->llink = current->llink;
                delete current;
                return true;
            }
            else current = current->rlink;
        } while (current != first);
        return false;
    }
    CircularDoublyList<T> Merge(CircularDoublyList<T> b) {
        CircularDoublyList<T> c;
        CircularDoublyListIterator<T> la(*this), lb(b);
        if (la.NotNull()) c.Insert(*la.First());
        while (la.NextNotNull()) c.Insert(*la.Next());
        if (lb.NotNull()) c.Insert(*lb.First());
        while (lb.NextNotNull()) c.Insert(*lb.Next());
        return c;
    }
    friend ostream& operator << (ostream& os, CircularDoublyList<T> circularDoublyList) {
        CircularDoublyListIterator<T> li(circularDoublyList);
        if (!li.NotNull()) return os;
        os << *li.First();
        while (li.NextNotNull())
            os << " + " << *li.Next();
        os << endl;
        return os;
    }
private:
    DoublyListNode<T>* first;
};
/* --------------------------------------------------------------- */


int main() {
    srand(time(nullptr));
    char select = '0';
    int weight;

    CircularDoublyList<int> a, b, c;

    while (select != 'q' || select != 'Q') {
        cout << endl << "Select command\n[a]: Add to List a\n[b]: Add to List b\n"
                        "[d]: Delete\n[m]: Merge\n[p]: PrintAll\n[q]: exit" << endl;
        cout << ">>";
        cin >> select;

        switch (select) {
            case 'a':
                cout << "Random value is inserted in List a" << endl;
                for (int i=0; i<10; i++)
                    a.Insert(rand()%100);
                break;
            case 'b':
                cout << "Random value is inserted in List b" << endl;
                for (int i=0; i<10; i++)
                    b.Insert(rand()%100);
                break;
            case 'd':
                cout << "Input value to delete :";
                cin >> weight;
                if (a.Delete(weight))
                    cout << "List a, " << weight << " is deleted." << endl;
                else
                    cout << "List a has no value, " << weight << "." << endl;
                if (b.Delete(weight))
                    cout << "List b, " << weight << " is deleted." << endl;
                else
                    cout << "List b has no value, " << weight << "." << endl;
                break;
            case 'm':
                cout << "--- a.Merge(b) ---" << endl;
                c = a.Merge(b);
                cout << c;
                break;
            case 'p':
                cout << "--- List a ---" << endl;
                cout << a << endl;
                cout << "--- List b ---" << endl;
                cout << b << endl;
                break;
            case 'q':
            case 'Q':
                cout << "Exit program.." << endl;
                break;
            default:
                cout << "WRONG INPUT  " << endl;
                cout << "Re-Enter" << endl;
        }
    }
    system("pause");
    return 0;
}
