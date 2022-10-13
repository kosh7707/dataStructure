#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

template <class T> class Term;
template <class T> class Chain;
template <class T> class ChainNode;
template <class T> class ChainIterator;
template <class T> class Polynomial;

/* Term begin ---------------------------------------------------------- */
template <class T>
class Term {
public:
    T coef;
    int exp;
    Term() { coef = exp = 0; }
    Term(T c, int e) : coef(c), exp(e) {}
    Term Set(T c, int e) { coef = c; exp = e; return *this; }
    friend ostream& operator << (ostream& os, const Term<T>& term) {
        os << term.coef << "^" << term.exp;
        return os;
    }
    bool operator < (Term<T> t) {
        return exp < t.exp;
    }
    bool operator > (Term<T> t) {
        return exp > t.exp;
    }
    bool operator == (Term<T> t) {
        return exp == t.exp;
    }
};
/* --------------------------------------------------------------------- */

/* ChainNode begin ----------------------------------------------------- */
template <class T>
class ChainNode {
    friend class Chain<T>;
    friend class ChainIterator<T>;
public:
    ChainNode(const T& element) {
        data = element;
        link = 0;
    }
    T data;
    ChainNode<T>* link;
};
/* --------------------------------------------------------------------- */

/* ChainIterator begin ------------------------------------------------- */
template <class T>
class ChainIterator {
public:
    ChainIterator(ChainNode<T>* node) : current(node) {}
    T& operator * () const { return &current->data; }
    T* operator -> () const { return &current->data; }
    bool operator && (ChainIterator<T> iter) const {
        return current != nullptr and iter.current != nullptr;
    }
    bool isEmpty() const { return current == nullptr; }
    ChainIterator<T>& operator ++ () {
        current = current->link;
        return *this;
    }
    ChainIterator<T> operator ++ (int) {
        ChainIterator<T> old = *this;
        current = current->link;
        return old;
    }
    bool NotNull() {
        return current != nullptr;
    }
    bool NextNotNull() {
        return current != nullptr and current->link != nullptr;
    }
    T* First() {
        if (NotNull())
            return &current->data;
        else return 0;
    }
    T* Next() {
        if (NextNotNull()) {
            current = current->link;
            return &current->data;
        }
        else return 0;
    }
private:
    ChainNode<T>* current;
};
/* --------------------------------------------------------------------- */

/* Chain begin --------------------------------------------------------- */
template <class T>
class Chain {
    friend class Polynomial<T>;
public:
    Chain() { first = 0; }
    void Add(const T& element) { //add a new node after first
        ChainNode<T>* newNode = new ChainNode<T>(element);
        if (first == nullptr) {
            newNode->link = first;
            first = newNode;
        }
        else if (first->data < element) {
            newNode->link = first;
            first = newNode;
        }
        else {
            ChainNode<T>* prev = first;
            ChainNode<T>* next = first->link;
            while (next != nullptr and next->data > element) {
                prev = next;
                next = next->link;
            }
            newNode->link = prev->link;
            prev->link = newNode;
        }
    }
    void Delete() { //delete the first element after first
        ChainNode<T> *next;
        next = first->link;
        if (first != nullptr) {
            ChainNode<T>* temp = first;
            first = next;
            delete temp;
        }
        else cout << "Empty List: Not deleted" << endl;
    }
    void Invert() {
        ChainNode<T>* prev = first;
        ChainNode<T>* next = 0;
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
    void InsertBack(const T& element) {
        ChainNode<T>* newNode = new ChainNode<T>(element);
        if (first != nullptr) {
            ChainNode<T>* temp = first;
            while (temp->link != nullptr)
                temp = temp->link;
            temp->link = newNode;
        }
        else first = newNode;
    }
    int Length() {
        int length = 0;
        if (first != nullptr) {
            ChainNode<T>* temp = first;
            while (temp != nullptr) {
                length += 1;
                temp = temp->link;
            }
        }
        return length;
    }
    friend ostream& operator << (ostream& os, const Chain<T> chain) {
        ChainIterator<T> chainIterator(chain.first);
        os << *chainIterator.First();
        while (chainIterator.NextNotNull()) {
            os << " + " << *chainIterator.Next() ;
        }
        return os;
    }
    ChainIterator<T> begin() const {
        return ChainIterator<T>(first);
    }
    ChainIterator<T> end() const {
        return ChainIterator<T>(nullptr);
    }
private:
    ChainNode<T>* first;
};
/* --------------------------------------------------------------------- */

/* Polynomial begin ---------------------------------------------------- */
template <class T>
class Polynomial {
public:
    Polynomial() {}
    Polynomial(Chain<Term<T>>* terms) : poly(terms) {}
    Polynomial<T> operator + (const Polynomial<T>& b) const {
        Term<T> temp;
        ChainIterator<Term<T>> ai = poly.begin();
        ChainIterator<Term<T>> bi = b.poly.begin();
        Polynomial<T> c;

        while ( (!ai.isEmpty()) and (!bi.isEmpty()) ) {
            if (ai->exp == bi->exp) {
                T sum = ai->coef + bi->coef;
                if (sum) c.poly.InsertBack(temp.Set(sum, ai->exp));
                ai++; bi++;
            }
            else if (ai->exp < bi->exp) {
                c.poly.InsertBack(temp.Set(bi->coef, bi->exp));
                bi++;
            }
        }
        while (!ai.isEmpty()) { //copy rest of a
            c.poly.InsertBack(temp.Set(ai->coef, ai->exp));
            ai++;
        }
        while (!bi.isEmpty()) { //copy rest of b
            c.poly.InsertBack(temp.Set(bi->coef, bi->exp));
            bi++;
        }
        return c;
    }
    void add(T coef, int exp) {
        Term<T>* newTerm = new Term<T>(coef, exp);
        poly.Add(*newTerm);
    }
    void addAll(Polynomial<T>* b) {
        ChainIterator<Term<T>> iterB = b->poly.begin();
        while (iterB.NotNull()) {
            Term<T> dataB = *iterB;
            add(dataB.coef, dataB.exp);
            iterB.next();
        }
    }
    friend ostream& operator << (ostream& os, Polynomial<T> polynomial) {
        os << polynomial.poly;
        return os;
    }
private:
    Chain<Term<T>> poly;
};
/* --------------------------------------------------------------------- */

int main() {
    srand(time(nullptr));
    cout.precision(2);
    cout << fixed; // 자리수 2자리 제한

    Polynomial<double> polynomial1, polynomial2, polynomial3;
    double coef1, coef2;
    for (int i=0; i<=5; i++) {
        coef1 = (double)rand()/RAND_MAX*9.0;
        coef2 = (double)rand()/RAND_MAX*9.0;
        if (rand()%2) coef1 *= -1;
        if (rand()%2) coef2 *= -1;
        polynomial1.add(coef1, i);
        polynomial2.add(coef2, i);
    }

    cout << "\npoly 1---------------------\n"
         << polynomial1
         << "\npoly 2---------------------\n"
         << polynomial2 << endl;

    polynomial3 = polynomial1 + polynomial2;

    cout << "\npoly3 = poly1 + poly2--------------------\n"
         << polynomial3 << endl;
}





