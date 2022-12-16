#include <iostream>
#include <string>

const int NMAX = 100;
const int MAX_WEIGHT = 9999999;
using namespace std;

class Pair;
class Graph;
template <class T> class List;
template <class T> class ListNode;
template <class T> class ListIterator;

class Pair {
public:
    int node, weight;
    Pair() {}
    Pair(int node, int weight) : node(node), weight(weight) {}
};

template <class T>
class ListNode {
    friend class List<T>;
    friend class ListIterator<T>;
private:
    T data;
    ListNode* link;
    ListNode(T t) {
        data = t;
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
    void Insert(T t) {
        ListNode<T>* newNode = new ListNode<T>(t);
        newNode->link = first;
        first = newNode;
    }
    friend ostream& operator << (ostream& os, List<T>& ls) {
        ListIterator<T> li(ls);
        if (li.NotNull()) {
            os << *li.First() << endl;
            while (li.NextNotNull())
                os << *li.Next() << endl;
        }
        return os;
    }
};

template <class T>
class ListIterator {
private:
    const List<T>& list;
    ListNode<T>* current;
public:
    ListIterator(const List<T>& list) : list(list) {
        current = list.first;
    }
    T* First() {
        if (current) return &current->data;
        else return nullptr;
    }
    T* Next() {
        current = current->link;
        return &current->data;
    }
    bool NotNull() {
        return current != nullptr;
    }
    bool NextNotNull() {
        return current->link != nullptr;
    }
};

class Graph {
private:
    // --------- common ---------
    int n;                  // total event number == node number
    List<Pair>* HeadNodes;  // adjacency list
    // --------- for AOE ---------
    int* count;             // incoming degree count
    int* ee;                // array for earliest event time
    int* le;                // array for latest event time
    // --------- for shortest path ---------
    int dist[NMAX];
    int a[NMAX][NMAX];
    bool s[NMAX];
    // --------- display tools ---------
    void displayArr(int* arr, int size) const {
        if (!arr) return;
        for (int i = 0; i < size; i++) {
            if (arr[i] < 10) cout << arr[i] << "  ";
            else cout << arr[i] << " ";
        }
    }
    void displayStack(int top, int* countArr) const {
        cout << "stack=[ ";
        for (int i = 0; i < n; i++) {
            cout << top << ", ";
            top = countArr[top];
            if (top == -1) break;
        }
        cout << " ]" << endl;
    }
    void displayS() {
        cout << "[";
        for (int i = 0; i < n; i++) {
            if (s[i]) cout << i << ",";
            else cout << "  ";
        }
        cout << "]; ";
    }
public:
    Graph(int n) : n(n) {
        HeadNodes = new List<Pair>[n];
        count = new int[n]();
        ee = new int[n]();
        le = new int[n]();
    }
    void addEdge(int start, int end, int weight) {
        if (start < 0 or end >= n) {
            cout << "out of bound" << endl;
            return;
        }
        ListIterator<Pair> li(HeadNodes[start]);
        for (auto first = li.First(); li.NotNull(); first = li.Next()) {
            if (first->node == end) {
                cout << "already exists" << endl;
                return;
            }
        }
        Pair pair(end, weight);
        HeadNodes[start].Insert(pair);
        count[end]++;
    }
    Graph* inverseGraph() {
        Graph* inversedGraph = new Graph(n);
        for (int i=0; i<n; i++) {
            List<Pair> ls = HeadNodes[i];
            ListIterator<Pair> li(ls);
            if (li.NotNull())
                for (auto first = li.First(); li.NotNull(); first = li.Next())
                    inversedGraph->addEdge(first->node, i, first->weight);
        }
        return inversedGraph;
    }
    void ealryActivity() {
        int* countArr = new int[n]();
        for (int i=0; i<n; i++)
            countArr[i] = count[i];

        int top = -1;
        for (int i=0; i<n; i++) {
            if (countArr[i] == 0) {
                countArr[i] = top;
                top = i;
            }
            ee[i] = 0;
        }

        for (int i=0; i<n; i++) {
            if (top == -1) {
                cout << "network has a cycle" << endl;
                return ;
            }
            int j = top;
            top = countArr[top];
            ListIterator<Pair> li(HeadNodes[j]);
            if (!li.NotNull()) continue;
            Pair pair = *li.First();
            while (true) {
                int k = pair.node;
                if (ee[k] < ee[j] + pair.weight)
                    ee[k] = ee[j] + pair.weight;
                countArr[k]--;
                if (countArr[k] == 0) {
                    countArr[k] = top;
                    top = k;
                }
                if (li.NextNotNull()) pair = *li.Next();
                else break;
            }
        }
        delete[] countArr;
    }
    void lateActivity() {
        for (int i = 0; i < n; i++)
            ee[i] = 0;
        ealryActivity();
        Graph* inversedGraph = this->inverseGraph();
        List<Pair>* inversedHeadNodes = inversedGraph->HeadNodes;
        int* inversedCount = inversedGraph->count;

        int max = -1;
        for (int i=0; i<n; i++)
            if (ee[n-1] > max) max = ee[n-1];
        for (int i = 0; i<n; i++)
            le[i] = max;
        int top = -1;

        for (int i=0; i<n; i++) {
            if (inversedCount[i] == 0) {
                inversedCount[i] = top;
                top = i;
            }
        }
        for (int i = 0; i < n; i++)
            if (top == -1) {
                cout << " network has a cycle" << endl;
                return;
            }
            else {
                int j = top;
                top = inversedCount[top];
                ListIterator<Pair> li(inversedHeadNodes[j]);
                if (!li.NotNull()) continue;
                Pair pair = *li.First();
                while (true) {
                    int k = pair.node;
                    if (le[k] > le[j] - pair.weight)
                        le[k] = le[j] - pair.weight;
                    inversedCount[k]--;
                    if (inversedCount[k] == 0) {
                        inversedCount[k] = top;
                        top = k;
                    }
                    if (li.NextNotNull()) pair = *li.Next();
                    else break;
                }
            }
        delete inversedGraph;
    }
    void printAdjacencyLists() {
        cout << "\nadjacency list: " << endl;
        for (int i=0; i<n; i++) {
            List<Pair> ls = HeadNodes[i];
            ListIterator<Pair> li(ls);
            cout << "in-degree count=" << count[i] << ", " << i << " ";

            if (!li.NotNull()) cout << "-> null" << endl;
            else {
                for (auto first = li.First(); li.NotNull(); first = li.Next())
                    cout << "-> (node " << first->node << ", weight=" << first->weight << ")";
                cout << endl;
            }
        }
    }
    void printEarlyActivity() {
        int* countArr = new int[n]();
        for (int i=0; i<n; i++)
            countArr[i] = count[i];

        int top = -1;
        for (int i=0; i<n; i++) {
            if (countArr[i] == 0) {
                countArr[i] = top;
                top = i;
            }
            ee[i] = 0;
        }

        cout << "Display ee (early event occurence time) " << endl;
        cout << "initial   ";
        displayArr(ee, n);
        displayStack(top, countArr);
        for (int i=0; i<n; i++) {
            if (top == -1) {
                cout << "network has a cycle" << endl;
                return ;
            }
            int j = top;
            top = countArr[top];
            cout << "output " << j << "; ";
            ListIterator<Pair> li(HeadNodes[j]);
            if (!li.NotNull()) continue;
            Pair pair = *li.First();
            while (true) {
                int k = pair.node;
                if (ee[k] < ee[j] + pair.weight)
                    ee[k] = ee[j] + pair.weight;
                countArr[k]--;
                if (countArr[k] == 0) {
                    countArr[k] = top;
                    top = k;
                }
                if (li.NextNotNull()) pair = *li.Next();
                else break;
            }
            displayArr(ee, n);
            displayStack(top, countArr);
        }
        displayArr(ee, n);
        delete[] countArr;
    }
    void printLateActivity() {
        for (int i = 0; i < n; i++)
            ee[i] = 0;
        ealryActivity();
        Graph* inversedGraph = this->inverseGraph();
        List<Pair>* inversedHeadNodes = inversedGraph->HeadNodes;
        int* inversedCount = inversedGraph->count;

        cout << endl;
        inversedGraph->printAdjacencyLists();

        int max = -1;
        for (int i=0; i<n; i++)
            if (ee[n-1] > max) max = ee[n-1];
        for (int i = 0; i<n; i++)
            le[i] = max;

        int top = -1;
        for (int i=0; i<n; i++) {
            if (inversedCount[i] == 0) {
                inversedCount[i] = top;
                top = i;
            }
        }
        cout << endl << "Display le (late event occurence time) " << endl;
        cout << "initial   ";
        displayArr(le, n);
        displayStack(top, inversedCount);
        for (int i = 0; i < n; i++)
            if (top == -1) {
                cout << " network has a cycle" << endl;
                return;
            }
            else {
                int j = top;
                top = inversedCount[top];
                cout << "output " << j << "; ";
                ListIterator<Pair> li(inversedHeadNodes[j]);
                if (!li.NotNull()) continue;
                Pair pair = *li.First();
                while (true) {
                    int k = pair.node;
                    if (le[k] > le[j] - pair.weight)
                        le[k] = le[j] - pair.weight;
                    inversedCount[k]--;
                    if (inversedCount[k] == 0) {
                        inversedCount[k] = top;
                        top = k;
                    }
                    if (li.NextNotNull()) pair = *li.Next();
                    else break;
                }
                displayArr(le, n);
                displayStack(top, inversedCount);
            }
        displayArr(le, n);
        delete inversedGraph;
    }
    void printCriticalActivity() {
        ealryActivity();
        lateActivity();
        cout << endl;
        for (int i=0; i<n; i++) {
            List<Pair> ls = HeadNodes[i];
            ListIterator<Pair> li(ls);
            if (li.NotNull()) {
                for (auto first = li.First(); li.NotNull(); first=li.Next()) {
                    int earlyTime = ee[i];
                    int lateTime = le[first->node] - first->weight;
                    int slack = lateTime - earlyTime;
                    string critical;
                    if (slack == 0) critical = "Yes";
                    else critical = "No";

                    cout << "edge(" << i << "->"
                         << first->node << "), early time="
                         << earlyTime << ", late time="
                         << lateTime
                         << ", slack=" << slack
                         << ", critical=" << critical << endl;
                }
            }
        }
    }
    void printTopologicalOrder() {
        int* countArr = new int[n]();
        for (int i=0; i<n; i++)
            countArr[i] = count[i];

        int top = -1;
        for (int i=0; i<n; i++) {
            if (countArr[i] == 0) {
                countArr[i] = top;
                top = i;
            }
        }

        for (int i=0; i<n; i++) {
            if (top == -1) {
                cout << "network has a cycle" << endl;
                return ;
            }
            int j = top;
            top = countArr[top];
            cout << j << " ";
            ListIterator<Pair> li(HeadNodes[j]);
            if (!li.NotNull()) continue;
            int k = (*li.First()).node;
            while (true) {
                countArr[k]--;
                if (countArr[k] == 0) {
                    countArr[k] = top;
                    top = k;
                }
                if (li.NextNotNull()) k = (*li.Next()).node;
                else break;
            }
        }
        delete[] countArr;
    }
    // --------- for shortest path ---------
    int choose() {
        int prevmax = -1;
        int index = -1;
        for (int i = 0; i < n; i++)
            if ((!s[i]) && ((prevmax == -1) || (dist[i] < prevmax))) {
                prevmax = dist[i];
                index = i;
            }
        return index;
    }
    bool hasNegativeWeight() {
        for (int i=0; i<n; i++) {
            List<Pair> ls = HeadNodes[i];
            ListIterator<Pair> li(ls);
            for (auto first = li.First(); li.NotNull(); first = li.Next())
                if (first->weight < 0) return true;
        }
        return false;
    }
    void out(int startNode) {
        for (int i = 0; i < n; i++) {
            if (i == startNode) cout << "0" << ", ";
            else if (dist[i] == MAX_WEIGHT) cout << "inf" << ", ";
            else cout << dist[i] << ", ";
        }
        cout << "\n";
    }
    void outA() {
        for (int i = 0; i < n; i++) {
            cout << "startNode = " << i << ": ";
            for (int j = 0; j < n; j++) {
                if (i == j) {
                    cout << "0 " << "  ";
                }
                else if (a[i][j] > MAX_WEIGHT - 100000)
                    cout << "inf" << "  ";
                else cout << a[i][j] << "  ";
            }
            cout << endl;
        }
    }
    void Dijkstra(const int v) {
        for (int i = 0; i < n; i++) {
            s[i] = false;
            ListIterator<Pair> li(HeadNodes[v]);
            for (auto first = li.First(); li.NotNull(); first = li.Next()) {
                if (i == first->node)
                    dist[i] = first->weight;
                else
                    dist[i] = MAX_WEIGHT;
            }
        }
        s[v] = true;
        dist[v] = 0;

        for (int i = 0; i < n - 2; i++) {
            out(i);
            int u = choose();
            s[u] = true;
            ListIterator<Pair> li(HeadNodes[u]);
            int len;
            for (int w = 0; w < n; w++) {
                for (auto first = li.First(); li.NotNull(); first = li.Next()) {
                    if (w == first->node)
                        len = first->weight;
                    else
                        len = MAX_WEIGHT;
                }
                if (!s[w] and (dist[u] + len < dist[w]))
                    dist[w] = dist[u] + len;
            }
        }
        out(v);
    }
    void printDijkstra(const int v) {
        for (int i = 0; i < n; i++) {
            s[i] = false;
            ListIterator<Pair> li(HeadNodes[v]);
            for (auto first = li.First(); li.NotNull(); first = li.Next()) {
                if (i == first->node)
                    dist[i] = first->weight;
                else
                    dist[i] = MAX_WEIGHT;
            }
        }

        cout << 1 << "; ";
        displayS();
        cout << "vertex selected: " << v << "; ";
        out(v);

        s[v] = true;
        dist[v] = 0;

        int i = 0;
        for (; i < n - 2; i++) {
            out(i);
            int u = choose();
            cout << endl << i + 2 << "; ";
            displayS();
            s[u] = true;
            cout << "vertex selected: " << u << "; ";
            out(v);
            ListIterator<Pair> li(HeadNodes[u]);
            int len;
            for (int w = 0; w < n; w++) {
                for (auto first = li.First(); li.NotNull(); first = li.Next()) {
                    if (w == first->node)
                        len = first->weight;
                    else
                        len = MAX_WEIGHT;
                }
                if (!s[w] and (dist[u] + len < dist[w]))
                    dist[w] = dist[u] + len;
            }
        }
        cout << endl << i + 2 << ", ";
        displayS();
        out(v);
    }
    void BellmanFord(const int v) {
        for (int i = 0; i < n; i++) {
            ListIterator<Pair> li(HeadNodes[v]);
            for (auto first = li.First(); li.NotNull(); first = li.Next()) {
                if (i == first->node)
                    dist[i] = first->weight;
                else
                    dist[i] = MAX_WEIGHT;
            }
        }
        cout << endl;
        int k = 0;
        for (k = 1; k <= n - 1; k++) {
            cout << "k = " << k << ", ";
            out(v);
            for (int u = 0; u < n; u++) {
                if (u != v) {
                    for (int i = 0; i < n; i++) {
                        ListIterator<Pair> li(HeadNodes[i]);
                        int len;
                        for (auto first = li.First(); li.NotNull(); first = li.Next()) {
                            if (u == first->node)
                                len = first->weight;
                            else
                                len = MAX_WEIGHT;
                        }
                        if ((u != i) && (len < MAX_WEIGHT)) {
                            if (dist[u] > dist[i] + len) {
                                dist[u] = dist[i] + len;
                            }
                        }
                    }
                }
            }
        }
    }
    void FloydWarshall() {
        for (int i = 0; i < n; i++) {
            ListIterator<Pair> li(HeadNodes[i]);
            for (auto first = li.First(); li.NotNull(); first = li.Next()) {
                if (i == first->node) dist[i] = first->weight;
                else dist[i] = MAX_WEIGHT;
            }
        }

        for (int i = 0; i < n; i++) {
            ListIterator<Pair> li(HeadNodes[i]);
            int len;
            for (int j = 0; j < n; j++) {
                for (auto first = li.First(); li.NotNull(); first = li.Next()) {
                    if (j == first->node) len = first->weight;
                    else len = MAX_WEIGHT;
                }
                a[i][j] = len;
            }
        }

        int k = 0;
        for (; k < n; k++) {
            cout << "\n A[" << k - 1 << "]" << endl;
            outA();
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    if ((a[i][k] + a[k][j]) < a[i][j])
                        a[i][j] = a[i][k] + a[k][j];
        }
        cout << "\n A[" << k - 1 << "]" << endl;
        outA();
    }
};

void DefaultSetup1(Graph* graph) {
    graph->addEdge(0, 3, 5);
    graph->addEdge(0, 2, 4);
    graph->addEdge(0, 1, 6);
    graph->addEdge(1, 4, 1);
    graph->addEdge(2, 4, 1);
    graph->addEdge(3, 5, 2);
    graph->addEdge(4, 7, 7);
    graph->addEdge(4, 6, 9);
    graph->addEdge(5, 7, 4);
    graph->addEdge(6, 8, 2);
    graph->addEdge(7, 8, 4);
}

void DefaultSetup2(Graph* graph) {
    graph->addEdge(0, 2, 6);
    graph->addEdge(0, 1, 5);
    graph->addEdge(1, 3, 3);
    graph->addEdge(2, 4, 3);
    graph->addEdge(2, 3, 6);
    graph->addEdge(3, 6, 4);
    graph->addEdge(3, 5, 4);
    graph->addEdge(3, 4, 3);
    graph->addEdge(4, 7, 4);
    graph->addEdge(4, 6, 1);
    graph->addEdge(5, 9, 4);
    graph->addEdge(6, 8, 5);
    graph->addEdge(7, 8, 2);
    graph->addEdge(8, 9, 2);
}

void DefaultSetup3(Graph* graph) {
    graph->addEdge(0, 3, 7);
    graph->addEdge(0, 2, 3);
    graph->addEdge(0, 1, 4);
    graph->addEdge(1, 4, 9);
    graph->addEdge(2, 4, 2);
    graph->addEdge(3, 4, 10);
    graph->addEdge(4, 6, 12);
    graph->addEdge(4, 5, 3);
    graph->addEdge(5, 8, 16);
    graph->addEdge(5, 7, 14);
    graph->addEdge(6, 9, 2);
    graph->addEdge(7, 10, 7);
    graph->addEdge(8, 10, 2);
    graph->addEdge(9, 10, 3);
}

void DefaultSetupForDijkstra(Graph* graph) {
    graph->addEdge(1, 0, 300);
    graph->addEdge(2, 1, 800);
    graph->addEdge(2, 0, 1000);
    graph->addEdge(3, 2, 1200);
    graph->addEdge(4, 3, 1500);
    graph->addEdge(4, 5, 250);
    graph->addEdge(5, 3, 1000);
    graph->addEdge(5, 7, 1400);
    graph->addEdge(5, 6, 900);
    graph->addEdge(6, 7, 1000);
    graph->addEdge(7, 0, 1700);
}

void DefaultSetupForBellmanFord(Graph* graph) {
    graph->addEdge(0, 1, 6);
    graph->addEdge(0, 2, 5);
    graph->addEdge(0, 3, 5);
    graph->addEdge(1, 4, -1);
    graph->addEdge(2, 1, -2);
    graph->addEdge(2, 4, 1);
    graph->addEdge(3, 2, -2);
    graph->addEdge(3, 5, -1);
    graph->addEdge(4, 6, 3);
    graph->addEdge(5, 6, 3);
}

void DefaultSetupForFloydWarshall(Graph* graph) {
    graph->addEdge(0, 1, 4);
    graph->addEdge(0, 2, 11);
    graph->addEdge(1, 0, 6);
    graph->addEdge(1, 2, 2);
    graph->addEdge(2, 0, 3);
}

int main() {
    Graph* graph = nullptr;
    int select = 0, n, start = -1, end = -1, weight = -1;
    cout << "[Setup]\n"
         << "[1]: default setup1 for AOE\n"
         << "[2]: default setup2 for AOE\n"
         << "[3]: default setup3 for AOE\n"
         << "[4]: default setup for Dijkstra\n"
         << "[5]: default setup for Bellman-Ford\n"
         << "[6]: default setup for Floyd-Warshall\n"
         << "[0]: custom setup\n>>";

    cin >> select;
    switch (select) {
        case 1:
            graph = new Graph(9);
            DefaultSetup1(graph);
            break;
        case 2:
            graph = new Graph(10);
            DefaultSetup2(graph);
            break;
        case 3:
            graph = new Graph(11);
            DefaultSetup3(graph);
            break;
        case 4:
            graph = new Graph(8);
            DefaultSetupForDijkstra(graph);
            break;
        case 5:
            graph = new Graph(7);
            DefaultSetupForBellmanFord(graph);
            break;
        case 6:
            graph = new Graph(3);
            DefaultSetupForFloydWarshall(graph);
            break;
        case 0:
            cout << "Input the total node number: ";
            cin >> n;
            graph = new Graph(n);
            break;
        default:
            cout << "WRONG INPUT  " << endl;
            exit(0);
    }

    while (true) {
        cout << endl
             << " [1]: Add edge\n"
             << " [2]: Print AdjacencyLists\n"
             << " [3]: Print Inversed AdjacencyList\n"
             << " [4]: Print Ealry Activity\n"
             << " [5]: Print Late Activity\n"
             << " [6]: Print Critical Activity\n"
             << " [7]: Print Topological Order\n"
             << " [8]: Test Dijkstra\n"
             << " [9]: Test Bellman-Ford\n"
             << "[10]: Test Floyd-Warshall\n"
             << " [0]: exit\n>>";
        cin >> select;
        switch (select) {
            case 1:
                cout << "\nAdd an edge: " << endl;
                cout << "Input start, dest, weight (separate space): ";
                cin >> start >> end >> weight;
                graph->addEdge(start, end, weight);
                break;
            case 2:
                graph->printAdjacencyLists();
                break;
            case 3: {
                Graph* inversedGraph = nullptr;
                inversedGraph = graph->inverseGraph();
                inversedGraph->printAdjacencyLists();
                delete inversedGraph;
                break;
            }
            case 4:
                graph->printEarlyActivity();
                break;
            case 5:
                graph->printLateActivity();
                break;
            case 6:
                graph->printCriticalActivity();
            case 7:
                graph->printTopologicalOrder();
                break;
            case 8:
                if (graph->hasNegativeWeight()) {
                    cout << "Negative weight node exists." << endl;
                    break;
                }
                cout << "\n ----------> Input start node:"; cin >> start;
                graph->printDijkstra(start);
                break;
            case 9:
                cout << "\n ----------> Input start node: "; cin >> start;
                graph->BellmanFord(start);
                break;
            case 10:
                cout << "\nAll-pairs shortest paths:" << endl;
                graph->FloydWarshall();
                break;
            case 0:
                delete graph;
                exit(0);
            default:
                cout << "WRONG INPUT  " << endl;
                cout << "Re-Enter" << endl;
                break;
        }
    }
}