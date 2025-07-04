#ifdef DONOTDEFINE
    g++ -std=c++11 -O2 -o sped SPEED.cpp && chmod +x sped && ./sped
    exit
#endif

#include <atomic>
#include <chrono>
#include <cmath>
#include <iostream>
#include <numeric>
#include <thread>
#include <unistd.h>
#include <vector>

const int WAIT_SECS = 3;
const int SUBT = 20000;
const int64_t ITERS = 400000000;
const int64_t MULTI = 1000000000;
const double ANCHOR = 10000000;
const double MAGICF = 999352;
const int64_t MAGIC = 4225772;
using namespace std;
int64_t MTT;
atomic<int64_t> cntN;

int64_t truncF(double x) {
    double y = x / MAGICF;
    double z = y * MAGICF;
    return (x - z) * MULTI;
}

int64_t truncI(int64_t x) {
    return x % MAGIC;
}

int64_t fpu(int64_t len) {
    double ret = 0;
    for (int64_t i = 0; i < len; ++i)
        ret += sqrt(i * (double) 3.14159) + sin(i * (float) 0.001);
    return truncF(ret);
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
    return truncI(ret);
}

void workerF(int64_t &val) {
    int cnt = 0;
    double ret = 0;
    while (cntN < MTT) {
        ret += sqrt(cnt * (double) 3.14159) + sin(cnt * (float) 0.001);
        if (++cnt >= SUBT) {
            cntN += SUBT;
            cnt = 0;
        }
    }
    val = truncF(ret);
}

void workerI(int64_t &val) {
    volatile int32_t a = 123, b = 456, c = 7890123;
    int cnt = 0;
    int64_t ret = 0;
    while (cntN < MTT) {
        ret += a + b;
        ret -= b - c;
        ret *= (a + 42);
        ret %= c;
        ret |= (b >> 1);
        ret &= 0xC0FFEE;
        if (++cnt >= SUBT) {
            cntN += SUBT;
            cnt = 0;
        }
    }
    val = truncI(ret);
}

void block(int len = WAIT_SECS) {
    while (--len >= 0) {
        cout << ".";
        cout.flush();
        sleep(1);
    }
}

int main() {
    uint16_t NT = thread::hardware_concurrency();
    MTT = ITERS * static_cast<int64_t>(NT);
{
    block();
    cout << "Single thread ";
    cout.flush();
    auto start1 = chrono::high_resolution_clock::now();
    auto ret1 = fpu(ITERS);
    auto stop1 = chrono::high_resolution_clock::now();
    auto dur1 = chrono::duration_cast<chrono::microseconds>(stop1 - start1).count();
    cout << ret1;

    vector<thread> threads;
    vector<int64_t> retN(NT);
    cntN = 0;
    block();
    cout << "Multi thread ";
    cout.flush();

    auto startN = chrono::high_resolution_clock::now();
    for (uint16_t i = 0; i < NT; ++i)
        threads.emplace_back(workerF, ref(retN[i]));
    for (auto &t : threads) t.join();
    auto stopN = chrono::high_resolution_clock::now();
    auto durN = chrono::duration_cast<chrono::microseconds>(stopN - startN).count();
    int64_t retMT = 0;
    for (auto r : retN) retMT += r;
    cout << retMT << endl;

    cout << "=== FLOATING POINT RESULT ===" << endl;
    cout << "Single: " << dur1 / 1000.0 << " ms = " << ANCHOR / dur1 << endl;
    cout << "Multi:  " << durN / 1000.0 << " ms = " << ANCHOR / durN * NT << endl;
    cout << "Speedup: " << (double) dur1 * NT / durN << "x  Threads: " << NT << endl;
}{
    block();
    cout << "Single thread ";
    cout.flush();
    auto start1 = chrono::high_resolution_clock::now();
    auto ret1 = ipu(ITERS);
    auto stop1 = chrono::high_resolution_clock::now();
    auto dur1 = chrono::duration_cast<chrono::microseconds>(stop1 - start1).count();
    cout << ret1;

    vector<thread> threads;
    vector<int64_t> retN(NT);
    cntN = 0;
    block();
    cout << "Multi thread ";
    cout.flush();

    auto startN = chrono::high_resolution_clock::now();
    for (uint16_t i = 0; i < NT; ++i)
        threads.emplace_back(workerI, ref(retN[i]));
    for (auto &t : threads) t.join();
    auto stopN = chrono::high_resolution_clock::now();
    auto durN = chrono::duration_cast<chrono::microseconds>(stopN - startN).count();
    int64_t retMT = 0;
    for (auto r : retN) retMT += r;
    cout << retMT << endl;

    cout << "=== INTEGER RESULT ===" << endl;
    cout << "Single: " << dur1 / 1000.0 << " ms = " << ANCHOR / dur1 << endl;
    cout << "Multi:  " << durN / 1000.0 << " ms = " << ANCHOR / durN * NT << endl;
    cout << "Speedup: " << (double) dur1 * NT / durN << "x  Threads: " << NT << endl;
}
    return 0;
}

