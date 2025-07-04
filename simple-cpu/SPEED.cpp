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

const int WAIT_SECS = 5;
const int64_t ITERS = 400000000;
const int64_t MULTI = 1000000000;
const double ANCHOR = 10000000;
const double MAGICF = 999352;
const int64_t MAGIC = 4225772;
using namespace std;

#ifdef __APPLE__
using CTP = chrono::time_point<chrono::steady_clock>;
#else
using CTP = chrono::time_point<chrono::system_clock>;
#endif

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

void workerF(int64_t len, pair<int64_t, CTP> &ret) {
    ret.first = fpu(len);
    ret.second = chrono::high_resolution_clock::now();
}

void workerI(int64_t len, pair<int64_t, CTP> &ret) {
    ret.first = ipu(len);
    ret.second = chrono::high_resolution_clock::now();
}

void block(int len = WAIT_SECS) {
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
    auto ret1 = fpu(ITERS);
    auto stop1 = chrono::high_resolution_clock::now();
    auto dur1 = chrono::duration_cast<chrono::microseconds>(stop1 - start1);
    cout << ret1;

    uint16_t NT = thread::hardware_concurrency();
    vector<thread> threads;
    vector<pair<int64_t, CTP>> retN(NT);

    block();
    cout << endl << "Multi thread";
    cout.flush();
    auto startN = chrono::high_resolution_clock::now();
    for (uint16_t i = 0; i < NT; ++i)
        threads.emplace_back(workerF, ITERS, ref(retN[i]));
    for (auto &t : threads) t.join();
    int64_t durN = 0;
    for (auto &r : retN) {
        cout << " " << r.first;
        durN += chrono::duration_cast<chrono::microseconds>(r.second - startN).count();
    }

    cout << endl << "=== FPU RESULT ===" << endl;
    cout << "Single: " << dur1.count() / 1000.0 << " ms = " << ANCHOR / dur1.count() << endl;
    cout << "Multi:  " << durN / 1000.0 << " ms = " << ANCHOR / durN * NT * NT << endl;
    cout << "Speedup: " << (double) dur1.count() * NT * NT / durN << "x  Threads: " << NT << endl;
}{
    block();
    cout << "Single thread ";
    cout.flush();
    auto start1 = chrono::high_resolution_clock::now();
    auto ret1 = ipu(ITERS);
    auto stop1 = chrono::high_resolution_clock::now();
    auto dur1 = chrono::duration_cast<chrono::microseconds>(stop1 - start1);
    cout << ret1;

    uint16_t NT = thread::hardware_concurrency();
    vector<thread> threads;
    vector<pair<int64_t, CTP>> retN(NT);

    block();
    cout << endl << "Multi thread";
    cout.flush();
    auto startN = chrono::high_resolution_clock::now();
    for (uint16_t i = 0; i < NT; ++i)
        threads.emplace_back(workerI, ITERS, ref(retN[i]));
    for (auto &t : threads) t.join();
    int64_t durN = 0;
    for (auto &r : retN) {
        cout << " " << r.first;
        durN += chrono::duration_cast<chrono::microseconds>(r.second - startN).count();
    }

    cout << endl << "=== INT RESULT ===" << endl;
    cout << "Single: " << dur1.count() / 1000.0 << " ms = " << ANCHOR / dur1.count() << endl;
    cout << "Multi:  " << durN / 1000.0 << " ms = " << ANCHOR / durN * NT * NT << endl;
    cout << "Speedup: " << (double) dur1.count() * NT * NT / durN << "x  Threads: " << NT << endl;
}
    return 0;
}

