#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

struct ExtendedGCD {
    int gcd;
    int x;
    int y;
};

ExtendedGCD extended_gcd(int a, int b) {
    if (a == 0) {
        return { b, 0, 1 };
    }
    ExtendedGCD prev = extended_gcd(b % a, a);
    int x = prev.y - (b / a) * prev.x;
    int y = prev.x;
    return { prev.gcd, x, y };
}

int mod_inverse(int a, int m) {
    ExtendedGCD egcd = extended_gcd(a, m);
    return (egcd.x % m + m) % m;
}

pair<int, int> chinese_remainder(const vector<int>& remainders, const vector<int>& moduli) {
    int M = 1;
    for (int m : moduli) {
        M *= m;
    }

    int result = 0;
    for (size_t i = 0; i < remainders.size(); i++) {
        int Mi = M / moduli[i];
        int inv = mod_inverse(Mi, moduli[i]);
        result += remainders[i] * Mi * inv;
    }

    return { result % M, M };
}

int main() {
    setlocale(LC_ALL, "Russian");
    vector<int> remainders = { 2, 3, 2 };
    vector<int> moduli = { 3, 5, 7 };

    pair<int, int> solution = chinese_remainder(remainders, moduli);
    int x = solution.first;
    int M = solution.second;
    cout << "== это сравнение по модулю" << endl;

    cout << "\nСистема уравнений:" << endl;
    for (size_t i = 0; i < remainders.size(); i++) {
        cout << "x == " << remainders[i] << " (mod " << moduli[i] << ")" << endl;
    }

    cout << "\nРешение: x = " << x << " (mod " << M << ")" << endl;
    cout << "Проверка:" << endl;
    for (size_t i = 0; i < moduli.size(); i++) {
        cout << "  " << x << " mod " << moduli[i] << " = " << (x % moduli[i])
            << " (должно быть " << remainders[i] << ")" << endl;
    }

    return 0;
}