#include <iostream>
#include <vector>
#include <array>
#include <string>

using namespace std;

class Permutation {
private:
    array<int, 6> mapping;

public:
    Permutation() {
        for (int i = 0; i < 6; i++) mapping[i] = i;
    }

    Permutation(const array<int, 6>& m) : mapping(m) {}

    int apply(int x) const {
        return mapping[x];
    }

    Permutation compose(const Permutation& other) const {
        array<int, 6> result;
        for (int i = 0; i < 6; i++) {
            result[i] = other.apply(this->apply(i));
        }
        return Permutation(result);
    }

    bool operator==(const Permutation& other) const {
        return mapping == other.mapping;
    }

    bool operator!=(const Permutation& other) const {
        return !(*this == other);
    }

    string toString() const {
        string result = "(";
        for (int i = 0; i < 6; i++) {
            result += to_string(mapping[i] + 1);
            if (i < 5) result += " ";
        }
        result += ")";
        return result;
    }

    string cycleString() const {
        vector<bool> visited(6, false);
        string result;

        for (int i = 0; i < 6; i++) {
            if (!visited[i]) {
                if (mapping[i] != i) {
                    result += "(";
                    int current = i;
                    do {
                        visited[current] = true;
                        result += to_string(current + 1);
                        current = mapping[current];
                        if (current != i) result += " ";
                    } while (current != i);
                    result += ")";
                }
                else {
                    visited[i] = true;
                }
            }
        }

        if (result.empty()) result = "e";
        return result;
    }

    int order() const {
        Permutation current = *this;
        Permutation identity;
        int ord = 1;

        while (current != identity) {
            current = current.compose(*this);
            ord++;
            if (ord > 1000) return -1;
        }
        return ord;
    }
};

class DihedralGroup {
private:
    vector<Permutation> elements;
    vector<vector<int>> multiplicationTable;

public:
    DihedralGroup() {
        generateElements();
        buildMultiplicationTable();
    }

    void generateElements() {
        for (int r = 0; r < 6; r++) {
            array<int, 6> rot;
            for (int i = 0; i < 6; i++) {
                rot[i] = (i + r) % 6;
            }
            elements.push_back(Permutation(rot));
        }

        for (int axis = 0; axis < 6; axis++) {
            array<int, 6> refl;
            if (axis % 2 == 0) {
                for (int i = 0; i < 6; i++) {
                    refl[i] = (axis - i + 6) % 6;
                }
            }
            else {
                for (int i = 0; i < 6; i++) {
                    refl[i] = (axis - 1 - i + 6) % 6;
                }
            }
            elements.push_back(Permutation(refl));
        }
    }

    void buildMultiplicationTable() {
        int n = elements.size();
        multiplicationTable.resize(n, vector<int>(n));

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                Permutation result = elements[i].compose(elements[j]);
                for (int k = 0; k < n; k++) {
                    if (result == elements[k]) {
                        multiplicationTable[i][j] = k;
                        break;
                    }
                }
            }
        }
    }

    bool isGroup() const {
        int n = elements.size();

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (multiplicationTable[i][j] < 0 || multiplicationTable[i][j] >= n) {
                    return false;
                }
            }
        }

        for (int a = 0; a < n; a++) {
            for (int b = 0; b < n; b++) {
                for (int c = 0; c < n; c++) {
                    int ab_c = multiplicationTable[multiplicationTable[a][b]][c];
                    int a_bc = multiplicationTable[a][multiplicationTable[b][c]];
                    if (ab_c != a_bc) {
                        return false;
                    }
                }
            }
        }

        int identityIndex = -1;
        for (int i = 0; i < n; i++) {
            bool isIdentity = true;
            for (int j = 0; j < n; j++) {
                if (multiplicationTable[i][j] != j || multiplicationTable[j][i] != j) {
                    isIdentity = false;
                    break;
                }
            }
            if (isIdentity) {
                identityIndex = i;
                break;
            }
        }
        if (identityIndex == -1) return false;

        for (int i = 0; i < n; i++) {
            bool hasInverse = false;
            for (int j = 0; j < n; j++) {
                if (multiplicationTable[i][j] == identityIndex &&
                    multiplicationTable[j][i] == identityIndex) {
                    hasInverse = true;
                    break;
                }
            }
            if (!hasInverse) return false;
        }

        return true;
    }

    bool isAbelian() const {
        int n = elements.size();
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (multiplicationTable[i][j] != multiplicationTable[j][i]) {
                    return false;
                }
            }
        }
        return true;
    }

    void printElements() const {
        cout << "\n=== ВСЕ ЭЛЕМЕНТЫ ГРУППЫ ===" << endl;
        cout << "Всего элементов: " << elements.size() << endl;
        cout << "Индекс | Подстановка | Циклы | Порядок" << endl;
        cout << "-------+--------------+-------+--------" << endl;

        for (size_t i = 0; i < elements.size(); i++) {
            printf("  %2zu   | %12s | %5s |    %2d\n",
                i,
                elements[i].toString().c_str(),
                elements[i].cycleString().c_str(),
                elements[i].order());
        }
    }

    void printMultiplicationTable() const {
        cout << "\n=== ТАБЛИЦА УМНОЖЕНИЯ (индексы элементов) ===" << endl;
        cout << "    ";
        for (size_t j = 0; j < elements.size(); j++) {
            printf("%3zu", j);
        }
        cout << endl;
        cout << "    ";
        for (size_t j = 0; j < elements.size(); j++) {
            cout << "---";
        }
        cout << endl;

        for (size_t i = 0; i < elements.size(); i++) {
            printf("%2zu |", i);
            for (size_t j = 0; j < elements.size(); j++) {
                printf("%3d", multiplicationTable[i][j]);
            }
            cout << endl;
        }
    }

    void printMultiplicationTableWithPermutations() const {
        cout << "\n=== ТАБЛИЦА УМНОЖЕНИЯ (подстановки) ===" << endl;
        cout << "Размер таблицы: " << elements.size() << "x" << elements.size() << endl << endl;

        cout << "      ";
        for (size_t j = 0; j < elements.size(); j++) {
            printf("%12s", elements[j].toString().c_str());
        }
        cout << endl;

        cout << "      ";
        for (size_t j = 0; j < elements.size(); j++) {
            cout << "------------";
        }
        cout << endl;

        for (size_t i = 0; i < elements.size(); i++) {
            printf("%4s |", elements[i].toString().c_str());
            for (size_t j = 0; j < elements.size(); j++) {
                cout << " ";
                printf("%11s", elements[multiplicationTable[i][j]].toString().c_str());
            }
            cout << endl;
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");

    cout << "========================================" << endl;
    cout << "  ГРУППА САМОСОВМЕЩЕНИЙ ПРАВИЛЬНОГО" << endl;
    cout << "         ШЕСТИУГОЛЬНИКА (D6)" << endl;
    cout << "========================================" << endl;

    DihedralGroup D6;

    D6.printElements();
    D6.printMultiplicationTable();

    cout << "\n=== ПРОВЕРКА АКСИОМ ГРУППЫ ===" << endl;
    if (D6.isGroup()) {
        cout << "Все аксиомы группы выполняются!" << endl;
        cout << "Полученная структура ЯВЛЯЕТСЯ группой." << endl;
    }
    else {
        cout << "Аксиомы группы НЕ выполняются." << endl;
    }

    cout << "\n=== ПРОВЕРКА КОММУТАТИВНОСТИ ===" << endl;
    if (D6.isAbelian()) {
        cout << "Группа является АБЕЛЕВОЙ (коммутативной)." << endl;
    }
    else {
        cout << "Группа НЕ является абелевой (некоммутативная)." << endl;
    }

    D6.printMultiplicationTableWithPermutations();

    cout << "\n========================================" << endl;
    cout << "ВЫВОДЫ:" << endl;
    cout << "1. Группа состоит из 12 элементов:" << endl;
    cout << "   - 6 поворотов (включая тождественный)" << endl;
    cout << "   - 6 отражений" << endl;
    cout << "2. Группа является диэдральной группой D6" << endl;
    cout << "3. Группа НЕ абелева" << endl;
    cout << "========================================" << endl;

    return 0;
}