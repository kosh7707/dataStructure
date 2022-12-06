#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

const int HeapSize = 100;

template <class T> class MinHeap;
template <class T> class ListNode;
template <class T> class List;
template <class T> class ListIterator;

template <class T>
class MinHeap {
private:
    int capacity = HeapSize;
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
class ListNode {
    friend class List<T>;
    friend class ListIterator<T>;
private:
    pair<T, T> data;
    ListNode* link;
    ListNode(T dest, T weight) {
        data = pair<T, T>(dest, weight);
        link = nullptr;
    }
};

template <class T>
class List {
    friend class ListIterator<T>;
private:
    ListNode<T>* first;
public:
    List() {
        first = nullptr;
    }
    void Insert(T dest, T weight) {
        auto* newNode = new ListNode<T>(dest, weight);
        newNode->link = first;
        first = newNode;
    }
    void Delete(T t) {
        ListNode<T>* prev;
        ListNode<T>* current = first;
        while (current and current->data != t) {
            prev = current;
            current = current->link;
        }
        if (current) {
            if (prev) prev->link = current->link;
            else first = first->link;
            delete current;
        }
    }
    friend ostream& operator << (ostream& os, List<T>& l) {
        ListIterator<T> li(l);
        if (!li.NotNull()) return os;
        os << *li.First() << endl;
        while (li.NextNotNull())
            os << *li.Next() << endl;
        return os;
    }
};

template <class T>
class ListIterator {
private:
    ListNode<T>* current;
public:
    ListIterator(const List<T>& l) { current = l.first; }
    pair<T, T>* First() {
        if (current) return &current->data;
        return nullptr;
    }
    pair<T, T>* Next() {
        current = current->link;
        return &current->data;
    }
    bool NotNull() {
        return current;
    }
    bool NextNotNull() {
        return current->link;
    }
};

class Sets {
private:
    int* parent;
    int size;
public:
    Sets(int sz = HeapSize) {
        size = sz;
        parent = new int[sz];
        for (int i=0; i<size; i++)
            parent[i] = -1;
    }
    void SimpleUnion(int i, int j) {
        parent[j] = i;
    }
    int SimpleFind(int i) {
        while (parent[i] >= 0) i = parent[i];
        return i;
    }
    void WeightedUnion(int i, int j) {
        int temp = parent[i] + parent[j];
        if (parent[i] > parent[j]) {
            parent[i] = j;
            parent[j] = temp;
        }
        else {
            parent[j] = i;
            parent[i] = temp;
        }
    }
    int CollapsingFind(int i) {
        int r;
        for (r=i; parent[r] >= 0; r=parent[r]) ;
        while (i != r) {
            int s = parent[i];
            parent[i] = r;
            i = s;
        }
        return r;
    }
    friend ostream& operator << (ostream& os, Sets sets) {
        os << "display:index= ";
        for (int i=1; i<=sets.size; i++)
            os << " " << i;
        os << "\ndisplay: value= ";
        for (int i=1; i<=sets.size; i++)
            os << " " << sets.parent[i];
        os << "\n";
        return os;
    }
};

class Graph {
private:
    List<int>* HeadNodes;
    vector<pair<int, pair<int, int>>> Edges;
    int n;
public:
    Graph(int vertices = 10) : n(vertices) {
        HeadNodes = new List<int>[n];
    }
    int getVerticesCount() {
        return n;
    }
    void displayAdjacencyLists() {
        for (int i=0; i<n; i++) {
            ListIterator<int> iter(HeadNodes[i]);
            if (!iter.NotNull()) {
                cout << i << " -> null" << endl;
                continue;
            }
            cout << i;
            for (auto first = iter.First(); iter.NotNull(); first = iter.Next())
                cout << " -> (" << (*first).first << ", " << (*first).second << ")";
            cout << endl;
        }
    }
    void InsertVertex(int start, int end, int weight) {
        if (start < 0 or start >= n or end < 0 or end >= n) throw;
        ListIterator<int> iter(HeadNodes[start]);
        for (auto first = iter.First(); iter.NotNull(); first=iter.Next())
            if ((*first).first == end) return;
        HeadNodes[start].Insert(end, weight);
        HeadNodes[end].Insert(start, weight);
        Edges.push_back({weight, {start, end}});
    }

    vector<pair<int, pair<int, int>>> getEdges() {
        return Edges;
    }

    List<int>* getHeadNodes() const {
        return HeadNodes;
    }
};

void insertTestVertex(Graph& graph) {
    graph.InsertVertex(0, 1, 28);
    graph.InsertVertex(0, 5, 10);
    graph.InsertVertex(1, 6, 14);
    graph.InsertVertex(1, 2, 16);
    graph.InsertVertex(5, 4, 25);
    graph.InsertVertex(6, 4, 24);
    graph.InsertVertex(6, 3, 18);
    graph.InsertVertex(2, 3, 12);
    graph.InsertVertex(4, 3, 22);
}

Graph KruskalMST(Graph graph) {
    Graph kruskalMST(graph.getVerticesCount());
    Sets set;
    auto Edges = graph.getEdges();
    sort(Edges.begin(), Edges.end());
    for (int i=0; i<Edges.size(); i++) {
        int start = Edges[i].second.first;
        int end = Edges[i].second.second;
        int weight = Edges[i].first;
        if (set.SimpleFind(start) != set.SimpleFind(end)) {
            kruskalMST.InsertVertex(start, end, weight);
            set.SimpleUnion(set.SimpleFind(start), set.SimpleFind(end));
        }
    }
    return kruskalMST;
}

Graph PrimMST(Graph graph) {
    int n = graph.getVerticesCount();
    Graph primMST(n);
    bool visited[HeapSize] = {false};
    // pair --> (weight, start, end)
    MinHeap<pair<int, pair<int, int>>> minHeap;
    minHeap.push({0, {0, 0}});
    while(!minHeap.isEmpty()) {
        auto top = minHeap.pop();
        int start = top.second.first;
        int end = top.second.second;
        if (visited[start])
            continue;
        if (start != end) {
            visited[start] = true;
            primMST.InsertVertex(start, end, top.first);
        }
        ListIterator<int> iter(graph.getHeadNodes()[end]);
        for (auto first = iter.First(); iter.NotNull(); first = iter.Next()) {
            int newStart = end;
            int newEnd = first->first;
            int weight = first->second;
            if (!visited[newEnd])
                minHeap.push({weight, {newStart, newEnd}});
        }
    }
    return primMST;
}

int main() {
    Graph graph(10);
    int select;
    do {
        cout << "\nSelect Command"
             << "\n[1]: Add edges and Weight"
             << "\n[2]: Display Adjacency Lists"
             << "\n[3]: Insert Vertex for Testing MST"
             << "\n[4]: Kruskal MST"
             << "\n[5]: Prim MST"
             << "\n[0]: quit"
             << "\n>>";
        cin >> select;
        switch (select) {
            case 1: {
                cout << "\n### Add an edge ###" << endl;
                cout << "\tInput Start Node:";
                int start; cin >> start;
                cout << "\tInput Destination Node:";
                int end; cin >> end;
                cout << "\tInput Weight:";
                int weight; cin >> weight;
                graph.InsertVertex(start, end, weight);
                break;
            }
            case 2:
                graph.displayAdjacencyLists();
                break;
            case 3:
                insertTestVertex(graph);
                break;
            case 4: {
                Graph kruskalMST = KruskalMST(graph);
                kruskalMST.displayAdjacencyLists();
                break;
            }
            case 5: {
                Graph primMST = PrimMST(graph);
                primMST.displayAdjacencyLists();
                break;
            }
            default:
                break;
        }
    } while (select != 0);

    return 0;
}