/*
        Analysis of Algorithms - Assignment 01

    Name: Jose Ignacio Cabascango Fernandez

    3. Code the Middle-school procedure algorithm from the textbook (You will need to code the Sieve of Eratosthenes algorithm too)
*/

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

vector<int> sieve(int n) {
    vector<bool> is_prime(n + 1, true);

    is_prime[0] = false;
    is_prime[1] = false;

    for (int p = 2; p <= sqrt(n); p++) {
        if (is_prime[p]) {
            for (int j = p * p; j <= n; j += p) {
                is_prime[j] = false;
            }
        }
    }

    vector<int> primes;

    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
        }
    }
    return primes;
}

int middle_school(int m, int n) {
    int limit = max(m, n);

    vector<int> primes = sieve(limit);

    int gcd = 1;

    for (int p : primes) {
        int count_m = 0;
        int count_n = 0;

        while (m % p == 0) {
            m = m / p;
            count_m++;
        }

        while (n % p == 0) {
            n = n / p;
            count_n++;
        }

        int common = min(count_m, count_n);

        for (int i = 0; i < common; i++) {
            gcd = gcd * p;
        }
    }
    return gcd;
}

int main() {
    int m, n;

    cout << "Enter m: ";
    cin >> m;

    cout << "Enter n: ";
    cin >> n;

    cout << "GCD = " << middle_school(m, n) << endl;
    return 0;
}