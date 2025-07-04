#ifdef DONOTDEFINE
    g++ -std=c++11 -O2 -o sped SPEED.cpp && chmod +x sped && ./sped
    exit
#endif

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>

const int64_t ITERS = 400000000;
const int64_t MULTI = 1000000000;
const double ANCHOR = 10000000;
const double MAGICF = 999352;
const int64_t MAGIC = 4225772;
using namespace std;

double fpu(int64_t len) {
    double ret = 0;
    for (int64_t i = 0; i < len; ++i)
        ret += sqrt(i * (double) 3.14159) + sin(i * (float) 0.001);
    return ret;
}

int64_t ipu(int64_t len) {
    volatile int32_t a = 123, b = 456, c = 7890123;
    int64_t ret = 0;
    for (int64_t i = 0; i < len; ++i) {
        ret += a + b;
        ret -= b - c;
        ret *= (a + 42);
        ret %= c;
        ret |= (b >> 1);
        ret &= 0xC0FFEE;
    }
    return ret;
}

void workerF(int64_t len, double &ret) {
    ret = fpu(len);
}

void workerI(int64_t len, int64_t &ret) {
    ret = ipu(len);
}

int64_t _truncF(double x) {
    double y = x / MAGICF;
    double z = y * MAGICF;
    return (x - z) * MULTI;
}

int64_t _truncI(int64_t x) {
    return x % MAGIC;
}

void block(int len = 7) {
    while (--len >= 0) {
        cout << ".";
        cout.flush();
        sleep(1);
    }
}

int main() {
{
    block();
    cout << "Single thread ";
    cout.flush();
    auto start1 = chrono::high_resolution_clock::now();
    double ret1 = fpu(ITERS);
    auto stop1 = chrono::high_resolution_clock::now();
    auto dur1 = chrono::duration_cast<chrono::microseconds>(stop1 - start1);
    cout << _truncF(ret1);

    uint16_t NT = thread::hardware_concurrency();
    vector<thread> threads;
    vector<double> retN(NT);

    block();
    cout << endl << "Multi thread";
    cout.flush();
    auto startN = chrono::high_resolution_clock::now();
    for (uint16_t i = 0; i < NT; ++i)
        threads.emplace_back(workerF, ITERS, ref(retN[i]));
    for (auto &t : threads) t.join();
    auto stopN = chrono::high_resolution_clock::now();
    auto durN = chrono::duration_cast<chrono::microseconds>(stopN - startN);
    for (auto r : retN) cout << " " << _truncF(r);

    cout << endl << "=== FPU RESULT ===" << endl;
    cout << "Single: " << dur1.count() / 1000.0 << " ms = " << ANCHOR / dur1.count() << endl;
    cout << "Multi:  " << durN.count() / 1000.0 << " ms = " << ANCHOR / durN.count() * NT << endl;
    cout << "Speedup: " << (double) dur1.count() * NT / durN.count() << "x  Threads: " << NT << endl;
}{
    block();
    cout << "Single thread ";
    cout.flush();
    auto start1 = chrono::high_resolution_clock::now();
    double ret1 = ipu(ITERS);
    auto stop1 = chrono::high_resolution_clock::now();
    auto dur1 = chrono::duration_cast<chrono::microseconds>(stop1 - start1);
    cout << _truncI(ret1);

    uint16_t NT = thread::hardware_concurrency();
    vector<thread> threads;
    vector<int64_t> retN(NT);

    block();
    cout << endl << "Multi thread";
    cout.flush();
    auto startN = chrono::high_resolution_clock::now();
    for (uint16_t i = 0; i < NT; ++i)
        threads.emplace_back(workerI, ITERS, ref(retN[i]));
    for (auto &t : threads) t.join();
    auto stopN = chrono::high_resolution_clock::now();
    auto durN = chrono::duration_cast<chrono::microseconds>(stopN - startN);
    for (auto r : retN) cout << " " << _truncI(r);

    cout << endl << "=== INT RESULT ===" << endl;
    cout << "Single: " << dur1.count() / 1000.0 << " ms = " << ANCHOR / dur1.count() << endl;
    cout << "Multi:  " << durN.count() / 1000.0 << " ms = " << ANCHOR / durN.count() * NT << endl;
    cout << "Speedup: " << (double) dur1.count() * NT / durN.count() << "x  Threads: " << NT << endl;
}
    return 0;
}

