#include <iostream>
#include <vector>
#include <cassert>
#include <clocale>

using namespace std;

using BooleanMatrix = vector<vector<int>>;

void printMatrix(const BooleanMatrix& matrix) {
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

BooleanMatrix disjunctio(const BooleanMatrix& A, const BooleanMatrix& B) {
    assert(A.size() == B.size() && A[0].size() == B[0].size());

    int rows = A.size();
    int cols = A[0].size();
    BooleanMatrix result(rows, vector<int>(cols));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = A[i][j] || B[i][j];
        }
    }
    return result;
}

BooleanMatrix transpose(const BooleanMatrix& A) {
    int rows = A.size();
    int cols = A[0].size();
    BooleanMatrix result(cols, vector<int>(rows));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = A[i][j];
        }
    }
    return result;
}

BooleanMatrix invert(const BooleanMatrix& A) {
    int rows = A.size();
    int cols = A[0].size();
    BooleanMatrix result(rows, vector<int>(cols));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = 1 - A[i][j];
        }
    }
    return result;
}

BooleanMatrix subtraction(const BooleanMatrix& A, const BooleanMatrix& B) {
    assert(A.size() == B.size() && A[0].size() == B[0].size());

    int rows = A.size();
    int cols = A[0].size();
    BooleanMatrix result(rows, vector<int>(cols));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = A[i][j] && (1 - B[i][j]);
        }
    }
    return result;
}

BooleanMatrix multiplication(const BooleanMatrix& A, const BooleanMatrix& B) {
    assert(A[0].size() == B.size());

    int rows = A.size();
    int cols = B[0].size();
    int n = A[0].size();

    BooleanMatrix result(rows, vector<int>(cols, 0));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum += (A[i][k] && B[k][j]);
            }
            result[i][j] = (sum > 0) ? 1 : 0;
        }
    }
    return result;
}

int main() {

    setlocale(LC_ALL, "Russian");
    BooleanMatrix A = {
        {1, 0, 1},
        {0, 1, 0},
        {1, 1, 0}
    };

    BooleanMatrix B = {
        {0, 1, 0},
        {1, 0, 1},
        {0, 1, 1}
    };

    cout << "Матрица A:" << endl;
    printMatrix(A);

    cout << "\nМатрица B:" << endl;
    printMatrix(B);

    cout << "\n1. Дизъюнкция (A OR B):" << endl;
    BooleanMatrix or_result = disjunctio(A, B);
    printMatrix(or_result);

    cout << "\n2. Транспонирование A^T:" << endl;
    BooleanMatrix trans_result = transpose(A);
    printMatrix(trans_result);

    cout << "\n3. Инвертирование !A:" << endl;
    BooleanMatrix inv_result = invert(A);
    printMatrix(inv_result);

    cout << "\n4. Вычитание (A - B):" << endl;
    BooleanMatrix sub_result = subtraction(A, B);
    printMatrix(sub_result);

    cout << "\n5. Умножение (A x B):" << endl;
    BooleanMatrix mul_result = multiplication(A, B);
    printMatrix(mul_result);

    return 0;
}