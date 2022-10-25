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
    void Insert(T x) {
        DoublyListNode<T>* p;
        DoublyListNode<T>* temp = new DoublyListNode<T>();
        p = first; temp->data = x;
        while (!(p->rlink->data == -1 or x < p->rlink->data))
            p = p->rlink;
        temp->llink = p; temp->rlink = p->rlink;
        p->rlink->llink = temp; p->rlink = temp;
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
        else return 0;
    }
    T* First() {
        if (current != list.first) return &current->data;
        else return 0;
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


int main() {
    srand(time(nullptr));
    char select = '0';
    int weight;

    CircularDoublyList<int> a, b, c;

    while (select != 'q' || select != 'Q') {
        cout << endl << "Select command: a: Add to List a, b: Add to List b, "
                        "d: Delete, m: Merge, p: PrintAll, q: exit" << endl;
        cin >> select;

        switch (select) {
            case 'a':
                cout << "List a 값 자동 추가 : " << endl;
                for (int i=0; i<10; i++)
                    a.Insert(rand()%100);
                break;
            case 'b':
                cout << "List b 값 자동 추가 : " << endl;
                for (int i=0; i<10; i++)
                    b.Insert(rand()%100);
                break;
            case 'd':
                cout << "List a와 b에서 삭제할 값을 입력 : " << endl;
                cin >> weight;
                if (a.Delete(weight))
                    cout << "List a에서" << weight << "값이 삭제됨." << endl;
                else
                    cout << "List a에" << weight << "값이 없음." << endl;
                if (b.Delete(weight))
                    cout << "List b에서" << weight << "값이 삭제됨." << endl;
                else
                    cout << "List b에" << weight << "값이 없음." << endl;
                break;
            case 'm':
                c = a.Merge(b);
                cout << c;
                break;
            case 'p':
                cout << "List a의 출력: " << endl;
                cout << a << endl;
                cout << "List b의 출력: " << endl;
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