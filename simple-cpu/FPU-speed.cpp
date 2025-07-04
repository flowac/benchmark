#ifdef DONOTDEFINE
    g++ -std=c++11 -O2 -o fpu FPU-speed.cpp
    chmod +x fpu
    sleep 1
    ./fpu
    exit
#endif

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>
#include <vector>

const int64_t ITERS = 400000000;
const int64_t MULTI = 1000000000;
const double ANCHOR = 10000000;
const double MAGIC  = 999352;
using namespace std;

double fpu(int64_t len) {
    double ret = 0;
    for (int64_t i = 0; i < len; ++i)
        ret += sqrt(i * 3.14159) + sin(i * 0.001);
    return ret;
}

void worker(int64_t len, double &ret) {
    ret = fpu(len);
}

int64_t _trunc(double x) {
    double y = x / MAGIC;
    double z = y * MAGIC;
    return (x - z) * MULTI;
}

int main() {
    cout << "Single thread ";
    auto start1 = chrono::high_resolution_clock::now();
    double ret1 = fpu(ITERS);
    auto stop1 = chrono::high_resolution_clock::now();
    auto dur1 = chrono::duration_cast<chrono::microseconds>(stop1 - start1);
    cout << _trunc(ret1) << endl;

    uint16_t NT = thread::hardware_concurrency();
    vector<thread> threads;
    vector<double> retN(NT);

    cout << "Multi thread";
    auto startN = chrono::high_resolution_clock::now();
    for (uint16_t i = 0; i < NT; ++i)
        threads.emplace_back(worker, ITERS, ref(retN[i]));
    for (auto &t : threads) t.join();
    auto stopN = chrono::high_resolution_clock::now();
    auto durN = chrono::duration_cast<chrono::microseconds>(stopN - startN);
    for (auto r : retN) cout << " " << _trunc(r);

    cout << endl << "=== RESULT ===" << endl;
    cout << "Single: " << dur1.count() / 1000.0 << " ms = " << ANCHOR / dur1.count() << endl;
    cout << "Multi:  " << durN.count() / 1000.0 << " ms = " << ANCHOR / durN.count() * NT << endl;
    cout << "Speedup: " << (double) dur1.count() * NT / durN.count() << "x  Threads: " << NT << endl;

    return 0;
}

