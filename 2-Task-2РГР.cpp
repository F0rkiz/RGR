#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <cmath>

using namespace std;

bool is_prime(int n) {
    if (n < 2) return false;
    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) return false;
    }
    return true;
}

int generate_prime(int start, int end) {
    vector<int> primes;
    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            primes.push_back(i);
        }
    }
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, primes.size() - 1);
    return primes[dis(gen)];
}

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

long long mod_pow(long long base, long long exp, long long mod) {
    long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

vector<int> text_to_numbers(const string& text) {
    vector<int> result;
    for (unsigned char c : text) {
        result.push_back(static_cast<int>(c));
    }
    return result;
}

string numbers_to_text(const vector<int>& numbers) {
    string result;
    for (int num : numbers) {
        result.push_back(static_cast<char>(num));
    }
    return result;
}

vector<long long> rsa_encrypt(const string& text, int e, int n) {
    vector<int> numbers = text_to_numbers(text);
    vector<long long> encrypted;
    for (int num : numbers) {
        encrypted.push_back(mod_pow(num, e, n));
    }
    return encrypted;
}

string rsa_decrypt(const vector<long long>& encrypted, int d, int n) {
    vector<int> decrypted_numbers;
    for (long long num : encrypted) {
        decrypted_numbers.push_back(static_cast<int>(mod_pow(num, d, n)));
    }
    return numbers_to_text(decrypted_numbers);
}

int main() {
    setlocale(LC_ALL, "Russian");
    int p = 61;
    int q = 53;
    int n = p * q;
    int phi_n = (p - 1) * (q - 1);

    int e = 17;
    int d = mod_inverse(e, phi_n);

    cout << "\nПараметры RSA:" << endl;
    cout << "p = " << p << ", q = " << q << endl; 
    cout << "n = p * q = " << n << endl;
    cout << "f(n) = " << phi_n << endl;
    cout << "Открытая экспонента e = " << e << endl;
    cout << "Секретная экспонента d = " << d << endl; // В примере достаточно маленького значения d, чтобы наглядно 
                                                      // продемонстрировать работу RSA, хотя в реальных системах d
                                                      // берут огромным — порядка 2^2048 — чтобы исключить возможность коллизий и обеспечить криптостойкость.

    string original_text = "Четные числа - питательны, а нечетные - просто вкусные";
    cout << "\nИсходная фраза:" << endl;
    cout << original_text << endl;

    vector<long long> encrypted = rsa_encrypt(original_text, e, n);
    cout << "\nЗашифрованное сообщение:" << endl;
    for (size_t i = 0; i < encrypted.size(); i++) {
        cout << encrypted[i];
        if (i < encrypted.size() - 1) cout << ", ";
    }
    cout << endl;

    string decrypted = rsa_decrypt(encrypted, d, n);
    cout << "\nРасшифрованное сообщение:" << endl;
    cout << decrypted << endl;

    cout << "\nПроверка: " << (original_text == decrypted ? "УСПЕШНО" : "ОШИБКА") << endl;

    return 0;
}