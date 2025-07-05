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

const int RUN_SECS  = 4;
const int WAIT_SECS = 4;
const double MULTIPLIER = 0.01;
using namespace std;
atomic<bool> DO_RUN;
const uint16_t NT = thread::hardware_concurrency();

void run_float(int64_t &ret, int64_t &cnt) {
    int64_t _cnt = 0;
    double  _ret = 0;
    while (DO_RUN) {
        _ret += sqrt(_cnt * (double) 3.14159) + sin(_cnt * (float) 0.001);
        ++_cnt;
    }
    ret = _ret;
    cnt = _cnt;
}

void run_int(int64_t &ret, int64_t &cnt) {
    volatile int32_t a = 123, b = 456;
    volatile int64_t c = 7890123;
    int64_t _cnt = 0;
    int64_t _ret = 0;
    while (DO_RUN) {
        _ret += a + b;
        _ret -= b - c;
        _ret *= (a + 42);
        _ret %= c;
        _ret |= (b >> 1);
        _ret &= 0xC0FFEE;
        ++_cnt;
    }
    ret = _ret;
    cnt = _cnt;
}

void block(int len = WAIT_SECS) {
    while (--len >= 0) {
        cout << ".";
        cout.flush();
        sleep(1);
    }
}

double exec1(uint16_t nproc, void (*run)(int64_t &, int64_t &)) {
    vector<thread> thd;
    vector<int64_t> ret(nproc), cnt(nproc);

    block();
    cout << nproc << " thread" << endl;
    DO_RUN = true;
    auto start = chrono::high_resolution_clock::now();
    for (uint16_t i = 0; i < nproc; ++i)
        thd.emplace_back(run, ref(ret[i]), ref(cnt[i]));
    sleep(RUN_SECS);
    DO_RUN = false;
    for (auto &t : thd) t.join();
    auto stop = chrono::high_resolution_clock::now();
    auto dur = chrono::duration_cast<chrono::microseconds>(stop - start).count();
    int64_t sum = 0, cntSum = 0;
    for (auto r : ret) sum += r;
    for (auto c : cnt) cntSum += c;
    (void) sum;
    return MULTIPLIER * cntSum / dur;
}

int64_t exec2(const char *type, void (*run)(int64_t &, int64_t &)) {
    cout << type;
    double score1 = exec1(1, run);
    double scoreN = exec1(NT, run);
    cout << "=== " << type << " RESULT ===" << endl;
    cout << "Single Thread: " << score1 << endl;
    cout << "Multi  Thread: " << scoreN << endl;
    cout << "Speedup: " << scoreN / score1 << "x  Threads: " << NT << endl;
    return score1;
}

int main() {return exec2("FLOAT", &run_float) + exec2("INT", &run_int);}

