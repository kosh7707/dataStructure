#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;
enum Mode { LEFT = 0, RIGHT = 1, };
const int DefaultSize = 100;

template <class T> class Stack;
template <class T> class Queue;
template <class T> class TreeNode;
template <class T> class Tree;
template <class T> class IIterator;
template <class T> class InorderIterator;
template <class T> class LevelIterator;
template <class T> class PostIterator;

//<editor-fold desc="Implement Stack">
template <class T>
class Stack {
private:
    int top;
    T* stack;
    int maxSize;
public:
    Stack(int maxSize = DefaultSize) {
        this->maxSize = maxSize;
        stack = new T[maxSize];
        top = -1;
    }
    bool isEmpty() {
        return top == -1;
    }
    bool isFull() {
        return top == maxSize - 1;
    }
    void push(const T& item) {
        if (isFull()) printStackIsFull();
        else stack[++top] = item;
    }
    T* pop() {
        if (isEmpty()) {
            printStackIsEmpty();
            return nullptr;
        }
        T* item = &stack[top--];
        return item;
    }
    void printStackIsFull() {
        cout << "empty" << endl;
    }
    void printStackIsEmpty() {
        cout << "full" << endl;
    }
    friend ostream& operator << (ostream& os, Stack<T> st) {
        os << "top = " << st.top << endl;
        for (int i=0; i<=st.top; i++)
            os << i << ":" << st.stack[i] << endl;
        return os;
    }
};
//</editor-fold>

//<editor-fold desc="Implement Queue">
template <class T>
class Queue {
private:
    int front;
    int rear;
    T* queue;
    int maxSize;
public:
    Queue(int maxSize = DefaultSize) {
        this->maxSize = maxSize;
        front = rear = -1;
        queue = new T[maxSize];
    }
    bool isEmpty() {
        return rear == front;
    }
    bool isFull() {
        return rear == maxSize - 1;
    }
    void push(const T& item) {
        if (isFull()) printQueueIsFull();
        else queue[++rear] = item;
    }
    T* pop() {
        if (isEmpty()) {
            printQueueIsEmpty();
            return nullptr;
        }
        T* item = &queue[++front];
        return item;
    }
    void printQueueIsFull() {
        cout << "empty" << endl;
    }
    void printQueueIsEmpty() {
        cout << "full" << endl;
    }
    friend ostream& operator << (ostream& os, Queue<T> qu) {
        os << "front = " << qu.front << "rear = " << qu.rear << endl;
        for (int i=qu.front+1; i<=qu.rear; i++)
            os << i << ":" << qu.queue[i] << endl;
        return os;
    }
};
//</editor-fold>

//<editor-fold desc="Implement TreeNode">
template <class T>
class TreeNode {
    friend class Tree<T>;
    friend class InorderIterator<T>;
    friend class LevelIterator<T>;
    friend class PostIterator<T>;
private:
    TreeNode* leftChild;
    TreeNode* rightChild;
    T data;
    int balanceFactor;
    TreeNode() {
        leftChild = rightChild = nullptr;
    }
    TreeNode(int data, TreeNode<T>* leftChild, TreeNode<T>* rightChild) {
        this->data = data;
        this->leftChild = leftChild;
        this->rightChild = rightChild;
    }
public:
    bool static Compare(const TreeNode<T>* a, const TreeNode<T>* b) {
        if (!(a or b)) return true;
        if (a and b and (a->data == b->data)
            and Compare(a->leftChild, b->leftChild)
            and Compare(a->rightChild, b->rightChild))
            return true;
        return false;
    }
    friend ostream& operator << (ostream& os, TreeNode<T> treeNode) {
        os << "{data:" << treeNode.data << ", bF:" << treeNode.balanceFactor << "}";
        return os;
    }
};
//</editor-fold>

//<editor-fold desc="Implement Tree">
template <class T>
class Tree {
    friend class InorderIterator<T>;
    friend class LevelIterator<T>;
    friend class PostIterator<T>;
private:
    TreeNode<T>* root;
    void printInorder(TreeNode<T>* currentNode) {
        if (currentNode) {
            printInorder(currentNode->leftChild);
            cout << " " << *currentNode;
            printInorder(currentNode->rightChild);
        }
    }
    void printPostorder(TreeNode<T>* currentNode) {
        if (currentNode) {
            printPostorder(currentNode->leftChild);
            printPostorder(currentNode->rightChild);
            cout << " " << *currentNode;
        }
    }
    void printPreorder(TreeNode<T>* currentNode) {
        if (currentNode) {
            cout << " " << *currentNode;
            printPreorder(currentNode->leftChild);
            printPreorder(currentNode->rightChild);
        }
    }
    TreeNode<T>* copy(TreeNode<T>* originalNode) {
        if (!originalNode) return nullptr;
        TreeNode<T>* root_temp = new TreeNode<T>();
        root_temp->data = originalNode->data;
        root_temp->leftChild = copy(originalNode->leftChild);
        root_temp->rightChild = copy(originalNode->rightChild);
        return root_temp;
    }
    int calculateBalanceFactor(TreeNode<T>* t) {
        if (!t) return 0;
        int left = calculateBalanceFactor(t->leftChild);
        int right = calculateBalanceFactor(t->rightChild);
        t->balanceFactor = left - right;
        return 1 + std::max(left, right);
    }
    bool isBalanced(TreeNode<T>* t) {
        if (!t) return true;
        return (t->balanceFactor < 3 && t->balanceFactor > -3)
                && (isBalanced(t->leftChild)) && (isBalanced(t->rightChild));
    }
public:
    Tree() {
        root = nullptr;
    }
    Tree(const Tree<T>& tree) {
        root = copy(tree.root);
    }
    Tree<T>* copy() {
        Tree<T> newTree = new Tree<T>(this);
        return *newTree;
    }
    void printInorder() {
        printInorder(root);
    }
    void printPreorder() {
        printPreorder(root);
    }
    void printPostorder() {
        printPostorder(root);
    }
    void printNonRecursiveInorder() {
        Stack<TreeNode<T>*> stack;
        TreeNode<T>* currentNode = root;
        while (true) {
            while (currentNode) {
                stack.push(currentNode);
                currentNode = currentNode->leftChild;
            }
            if (!stack.isEmpty()) {
                currentNode = *stack.pop();
                cout << *currentNode << " ";
                currentNode = currentNode->rightChild;
            }
            else break;
        }
        cout << endl;
    }
    void printLevelorder() {
        Queue<TreeNode<T>*> queue;
        TreeNode<T>* currentNode = root;
        if (currentNode) queue.push(currentNode);
        while (true) {
            if (!queue.isEmpty()) {
                currentNode = *queue.pop();
                cout << *currentNode << " ";
                if (currentNode->leftChild) queue.push(currentNode->leftChild);
                if (currentNode->rightChild) queue.push(currentNode->rightChild);
            }
            else break;
        }
    }
    void calculateBalanceFactor() {
        calculateBalanceFactor(root);
    }
    bool isEmpty() {
        return root == nullptr;
    }
    bool isExist(T t) {
        InorderIterator<T> inorderIterator(*this);
        T* temp_element = inorderIterator.Next();
        while (temp_element) {
            if (t == *temp_element) return true;
            temp_element = inorderIterator.Next();
        }
        return false;
    }
    bool Insert(T data) {
        if (isExist(data)) {
            cout << "Tree has data " << data << "." << endl;
            return false;
        }
        TreeNode<T>* insertNode = new TreeNode<T>();
        insertNode->data = data;
        if (isEmpty()) {
            root = insertNode;
            return true;
        }
        TreeNode<T>* leafNode = nullptr;
        TreeNode<T>* tempNode = root;
        while (tempNode) {
            leafNode = tempNode;
            if (data < tempNode->data) tempNode = tempNode->leftChild;
            else tempNode = tempNode->rightChild;
        }
        if (data < leafNode->data) leafNode->leftChild = insertNode;
        else leafNode->rightChild = insertNode;
        return true;
    }
    bool Delete(T data) {
        if (isEmpty()) {
            cout << "empty root" << endl;
            return false;
        }
        if (!isExist(data)) {
            cout << "Tree has no data " << data << "." << endl;
            return false;
        }
        TreeNode<T>* PrevNode = nullptr;
        TreeNode<T>* DeletedNode = root;
        while (DeletedNode and DeletedNode->data != data) {
            PrevNode = DeletedNode;
            if (data > DeletedNode->data)
                DeletedNode = DeletedNode->rightChild;
            else
                DeletedNode = DeletedNode->leftChild;
        }
        if (!DeletedNode->leftChild and !DeletedNode->rightChild) {
            if (DeletedNode == root)
                root = nullptr;
            else {
                if (data < PrevNode->data)
                    PrevNode->leftChild = nullptr;
                else
                    PrevNode->rightChild = nullptr;
            }
            free(DeletedNode);
        }
        else if (DeletedNode->leftChild and !DeletedNode->rightChild) {
            if (DeletedNode == root)
                root = DeletedNode->leftChild;
            else {
                TreeNode<T>* tempNode = DeletedNode->leftChild;
                if (data < PrevNode->data)
                    PrevNode->leftChild = tempNode;
                else
                    PrevNode->rightChild = tempNode;
            }
            free(DeletedNode);
        }
        else if (!DeletedNode->leftChild and DeletedNode->rightChild) {
            if (DeletedNode == root)
                root = DeletedNode->rightChild;
            else {
                TreeNode<T>* tempNode = DeletedNode->rightChild;
                if (data < PrevNode->data)
                    PrevNode->leftChild = tempNode;
                else
                    PrevNode->rightChild = tempNode;
            }
            free(DeletedNode);
        }
        else {
            InorderIterator<T> inorderSuccessor(*this);
            T tempData = *inorderSuccessor.Next();
            while (tempData != data)
                tempData = *inorderSuccessor.Next();
            tempData = *inorderSuccessor.Next();
            Delete(tempData);
            DeletedNode->data = tempData;
        }
        return true;
    }
    bool isBalanced() {
        return isBalanced(root);
    }
    T DeleteLargestElement() {
        InorderIterator<T> inorderIterator(*this);
        T* temp_element = inorderIterator.Next();
        T max_element = 0;
        while (temp_element) {
            if (max_element < *temp_element) max_element = *temp_element;
            temp_element = inorderIterator.Next();
        }
        Delete(max_element);
        return max_element;
    }
    void Split(T i, Tree<T>& B, Tree<T>& C) {
        if (isEmpty()) {
            B.root = C.root = nullptr;
            return ;
        }
        if (!isExist(i)) {
            cout << "Tree has no data " << i << "." << endl;
            return;
        }
        if (i == root->data) {
            B.root = root->leftChild;
            C.root = root->rightChild;
            return ;
        }
        Mode mode = LEFT;
        TreeNode<T>* PrevNode = nullptr;
        TreeNode<T>* CurrentNode = root;
        while (CurrentNode) {
            if (i < CurrentNode->data) {
                PrevNode = CurrentNode;
                CurrentNode = CurrentNode->leftChild;
                mode = LEFT;
            }
            else if (i > CurrentNode->data) {
                PrevNode = CurrentNode;
                CurrentNode = CurrentNode->rightChild;
                mode = RIGHT;
            }
            else {
                if (mode == LEFT) {
                    PrevNode->leftChild = CurrentNode->rightChild;
                    B.root = CurrentNode->leftChild;
                }
                else {
                    PrevNode->rightChild = CurrentNode->leftChild;
                    B.root = CurrentNode->rightChild;
                }
                C.root = root;
                return;
            }
        }
    }
    static Tree<T>* ThreeWayJoin(Tree<T>* A, T root_data, Tree<T>* B) {
        TreeNode<T>* newRootNode = new TreeNode<T>();
        newRootNode->data = root_data;
        newRootNode->leftChild = A->root;
        newRootNode->rightChild = B->root;
        Tree<T>* newTree = new Tree<T>();
        newTree->root = newRootNode;
        return newTree;
    }
    static Tree<T>* TwoWayJoin(Tree<T>* A, Tree<T>* B) {
        if (!A and !B) return nullptr;
        if (!A) return B;
        if (!B) return A;
        T LargestData = A->DeleteLargestElement();
        return ThreeWayJoin(A, LargestData, B);
    }
    friend bool operator == (const Tree<T>& a, const Tree<T>& b) {
        return TreeNode<T>::Compare(a.root, b.root);
    }
};
//</editor-fold>

//<editor-fold desc="Implement IIterator">
template <class T>
class IIterator {
public:
    virtual T* Next() = 0;
    void print() {
        T* nextValue = this->Next();
        while (nextValue) {
            cout << *nextValue << " ";
            nextValue = this->Next();
        }
        cout << endl;
    }
};
//</editor-fold>

//<editor-fold desc="Implement InorderIterator">
template <class T>
class InorderIterator : public IIterator<T> {
private:
    Tree<T> t;
    Stack<TreeNode<T>*> stack;
    TreeNode<T>* currentNode;
public:
    InorderIterator(Tree<T> tree) : t(tree) {
        currentNode = t.root;
    };
    T* Next() {
        while (currentNode) {
            stack.push(currentNode);
            currentNode = currentNode->leftChild;
        }
        if (!stack.isEmpty()) {
            currentNode = *stack.pop();
            T* retValue = &currentNode->data;
            currentNode = currentNode->rightChild;
            return retValue;
        }
        else return nullptr;
    }
};
//</editor-fold>

//<editor-fold desc="Implement LevelIterator">
template <class T>
class LevelIterator : public IIterator<T> {
private:
    Tree<T> t;
    TreeNode<T>* currentNode;
    Queue<TreeNode<T>*> queue;
public:
    LevelIterator(Tree<T> tree) : t(tree) {
        currentNode = t.root;
        if (currentNode) queue.push(currentNode);
    }
    T* Next() {
        if (!queue.isEmpty()) {
            currentNode = *queue.pop();
            T* retValue = &currentNode->data;
            if (currentNode->leftChild) queue.push(currentNode->leftChild);
            if (currentNode->rightChild) queue.push(currentNode->rightChild);
            return retValue;
        }
        else return nullptr;
    }
};
//</editor-fold>

//<editor-fold desc="Implement PostIterator">
template <class T>
class PostIterator : public IIterator<T> {
private:
    Tree<T> t;
    TreeNode<T>* currentNode;
    Stack<TreeNode<T>*> stack1;
    Stack<TreeNode<T>*> stack2;
public:
    PostIterator(Tree<T> tree) : t(tree) {
        currentNode = t.root;
        stack1.push(currentNode);
        while(!stack1.isEmpty()) {
            currentNode = *stack1.pop();
            stack2.push(currentNode);
            if (currentNode->leftChild) stack1.push(currentNode->leftChild);
            if (currentNode->rightChild) stack1.push(currentNode->rightChild);
        }
    }
    T* Next() {
        if (!stack2.isEmpty()) {
            currentNode = *stack2.pop();
            return &currentNode->data;
        }
        else return nullptr;
    }
};
//</editor-fold>

int main(void) {
    int item_count, remove_element, temp, split_data; char select;
    srand(time(nullptr));

    Tree<int> tree;
    do {
        cout << "\nBinarySearchTree Test" << endl
             << "[i]: Insert" << endl
             << "[r]: Remove" << endl
             << "[s]: split and join" << endl
             << "[d]: print inorder" << endl
             << "[k]: print levelorder" << endl
             << "[g]: print postorder" << endl
             << "[f]: print preorder" << endl
             << "[h]: Copy and Compare" << endl
             << "[q]: Quit" << endl
             << "=>";
        cin >> select;
        switch (select) {
            case 'i':
                cout << "The number of items =";
                cin >> item_count;
                for (int i = 0; i < item_count; i++) {
                    temp = rand() % 100;
                    if (!tree.Insert(temp))
                        cout << "Insert Duplicated data" << endl;
                    tree.calculateBalanceFactor();
                    if (!tree.isBalanced()) {
                        cout << "This tree is not balanced" << endl;
                    }
                }
                break;
            case 'r':
                cout << "removed element =";
                cin >> remove_element;
                if (tree.Delete(remove_element))
                    cout << "Successfully deleted " << remove_element << endl;
                else
                    cout << "Failed delete" << endl;
                break;
            case 's': {
                cout << "isBalanced: " << tree.isBalanced() << endl;
                break;
            }
            case 'd': {
                cout << "\nprint inorder" << endl;
                tree.printInorder();
                cout << "\nprint non-recursive inorder" << endl;
                tree.printNonRecursiveInorder();
                cout << "print order use iterator" << endl;
                InorderIterator<int> inorderIterator(tree);
                inorderIterator.print();
                break;
            }
            case 'k': {
                cout << "\nprint level order" << endl;
                tree.printLevelorder();
                cout << "\nprint order use iterator" << endl;
                LevelIterator<int> levelIterator(tree);
                levelIterator.print();
                break;
            }
            case 'g': {
                cout << "\nprint postorder" << endl;
                tree.printPostorder();
                cout << "\nprint order use iterator" << endl;
                PostIterator<int> postIterator(tree);
                postIterator.print();
                break;
            }
            case 'f': {
                tree.printPreorder();
                break;
            }
            case 'h':
                if (tree == Tree<int>(tree)) cout << "compare result: true";
                else cout << "compare result: false";
                break;
            case 'q':
                cout << "Quit" << endl;
                break;
            default:
                cout << "WRONG INPUT" << endl
                     << "Re-Enter";
                break;
        }
    } while(select != 'q');
    system("pause");
    return 0;
}