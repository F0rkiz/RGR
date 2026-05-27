#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <algorithm>
#include <set>
#include <clocale>
using namespace std;

class PartialOrder {
private:
    int n;
    vector<set<int>> graph;
    vector<set<int>> transitiveClosure;

public:
    PartialOrder(int size) : n(size) {
        graph.resize(n);
        transitiveClosure.resize(n);
        for (int i = 0; i < n; i++) {
            graph[i].insert(i);
            transitiveClosure[i].insert(i);
        }
    }

    void addRelation(int a, int b) {
        if (a == b) return;

        if (hasPath(b, a)) {
            cout << "Ошибка: добавление отношения (" << a << ", " << b
                << ") нарушает антисимметричность" << endl;
            return;
        }

        graph[a].insert(b);
        updateTransitiveClosure(a, b);
    }

    bool hasPath(int a, int b) {
        vector<bool> visited(n, false);
        return dfs(a, b, visited);
    }

    bool dfs(int current, int target, vector<bool>& visited) {
        if (current == target) return true;
        visited[current] = true;

        for (int next : graph[current]) {
            if (!visited[next]) {
                if (dfs(next, target, visited)) return true;
            }
        }
        return false;
    }

    void updateTransitiveClosure(int a, int b) {
        vector<int> predecessors;
        for (int i = 0; i < n; i++) {
            if (transitiveClosure[i].count(a)) {
                predecessors.push_back(i);
            }
        }

        vector<int> successors;
        for (int i = 0; i < n; i++) {
            if (transitiveClosure[b].count(i)) {
                successors.push_back(i);
            }
        }

        for (int pred : predecessors) {
            for (int succ : successors) {
                transitiveClosure[pred].insert(succ);
            }
        }
    }

    vector<int> topologicalSort() {
        vector<int> result;
        vector<int> inDegree(n, 0);

        for (int i = 0; i < n; i++) {
            for (int j : graph[i]) {
                if (i != j) {
                    inDegree[j]++;
                }
            }
        }

        stack<int> zeroDegree;
        for (int i = 0; i < n; i++) {
            if (inDegree[i] == 0) {
                zeroDegree.push(i);
            }
        }

        while (!zeroDegree.empty()) {
            int node = zeroDegree.top();
            zeroDegree.pop();
            result.push_back(node);

            for (int neighbor : graph[node]) {
                if (node != neighbor) {
                    inDegree[neighbor]--;
                    if (inDegree[neighbor] == 0) {
                        zeroDegree.push(neighbor);
                    }
                }
            }
        }

        if (result.size() != n) {
            cout << "Предупреждение: граф содержит циклы!" << endl;
        }

        return result;
    }

    vector<int> extendToLinearOrder() {
        vector<int> linearOrder;
        vector<bool> used(n, false);

        for (int step = 0; step < n; step++) {
            int minElement = -1;

            for (int i = 0; i < n; i++) {
                if (!used[i]) {
                    bool isMinimal = true;

                    for (int j = 0; j < n; j++) {
                        if (!used[j] && j != i && transitiveClosure[j].count(i)) {
                            isMinimal = false;
                            break;
                        }
                    }

                    if (isMinimal) {
                        minElement = i;
                        break;
                    }
                }
            }

            if (minElement == -1) {
                cout << "Ошибка: невозможно построить линейный порядок" << endl;
                return {};
            }

            linearOrder.push_back(minElement);
            used[minElement] = true;
        }

        return linearOrder;
    }

    void printMatrix() {
        cout << "Матрица частичного порядка:" << endl;
        cout << "   ";
        for (int i = 0; i < n; i++) {
            cout << i << " ";
        }
        cout << endl;

        for (int i = 0; i < n; i++) {
            cout << i << ": ";
            for (int j = 0; j < n; j++) {
                if (graph[i].count(j)) {
                    cout << "1 ";
                }
                else {
                    cout << "0 ";
                }
            }
            cout << endl;
        }
    }

    void printLinearOrder(const vector<int>& order) {
        cout << "Линейный порядок: ";
        for (int i = 0; i < order.size(); i++) {
            cout << order[i];
            if (i < order.size() - 1) cout << " < ";
        }
        cout << endl;

        cout << "Проверка сохранения исходных отношений:" << endl;
        bool allPreserved = true;
        for (int i = 0; i < n; i++) {
            for (int j : graph[i]) {
                if (i != j) {
                    int posI = -1, posJ = -1;
                    for (int k = 0; k < order.size(); k++) {
                        if (order[k] == i) posI = k;
                        if (order[k] == j) posJ = k;
                    }
                    if (posI < posJ) {
                        cout << "  " << i << " < " << j << " сохранено" << endl;
                    }
                    else {
                        cout << "  " << i << " < " << j << " нарушено" << endl;
                        allPreserved = false;
                    }
                }
            }
        }

        if (allPreserved) {
            cout << "Все исходные отношения сохранены!" << endl;
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");

    int n = 5;
    PartialOrder po(n);

    cout << "Исходный частичный порядок:" << endl;
    po.addRelation(0, 1);
    po.addRelation(0, 2);
    po.addRelation(1, 3);
    po.addRelation(2, 3);
    po.addRelation(3, 4);

    po.printMatrix();
    cout << endl;

    vector<int> linearOrder = po.extendToLinearOrder();
    po.printLinearOrder(linearOrder);

    cout << "\n--- Дополнительный пример ---" << endl;

    PartialOrder po2(4);
    po2.addRelation(0, 1);
    po2.addRelation(0, 2);
    po2.addRelation(2, 3);

    po2.printMatrix();
    cout << endl;

    vector<int> order2 = po2.extendToLinearOrder();
    po2.printLinearOrder(order2);

    return 0;
}