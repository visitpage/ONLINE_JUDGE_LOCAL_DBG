//
// Created by sion on 2024/11/8.
//

#ifndef SOLUTION_GEN_H
#define SOLUTION_GEN_H

#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <algorithm>
#include <unordered_set>

using namespace std;

namespace gen {
  mt19937_64 gen(chrono::steady_clock::now().time_since_epoch().count());
  struct Int {
    uniform_int_distribution<int64_t> ud;
    Int(int64_t lo, int64_t hi): ud(lo, hi) {}
    int64_t operator()() { return ud(gen); }
    int32_t operator()(int32_t& var) { return var = (int32_t)ud(gen); }
    int64_t operator()(int64_t& var) { return var = ud(gen); }
  };
  struct Ints {
    uniform_int_distribution<int64_t> ud;
    Ints(int64_t lo, int64_t hi): ud(lo, hi) {}
    vector<int64_t> operator()(int size, int pattern = 0) {
      vector<int64_t> result(size);
      if (pattern == 0) { // 0. 简单随机的数组SimpleRandom
        generate(result.begin(), result.end(), [&] () { return ud(gen); });
      }
      else if (pattern == 1 || pattern == 5) { // 1. 随机但是非递减NonDecreasingRandom | 5. 随机但是非递增NonIncreasingRandom
        generate(result.begin(), result.end(), [&] () { return ud(gen); });
        sort(result.begin(), result.end());
        if (pattern == 5) {
          reverse(result.begin(), result.end());
        }
      }
      else if (pattern == 2 || pattern == 4) { // 2. 随机但是递增的数组IncreasingRandom | 4. 随机但是递减的数组DecreasingRandom
        unordered_set<int64_t> st;
        int count = 0;
        while (st.size() < size) {
          st.insert(ud(gen));
          count++;
          if (count > 10*size) {
            cerr << "gen[warning]: \"too many collisions while generating INC/DEC array: count = " << count << " > 10*size(" << size << ")\"" << endl;
            for (int64_t num = ud.min(); st.size() < size && num <= ud.max(); num++) {
              st.insert(num);
            }
            if (st.size() < size) {
              cerr << "gen[fatal-error]: \"hi-lo+1 < n\"" << endl;
              assert(false);
            }
          }
        }
        result.assign(st.begin(), st.end());
        sort(result.begin(), result.end());
        if (pattern == 4) {
          reverse(result.begin(), result.end());
        }
      }
      else if (pattern == 3) { // 3. 全相等的某个随机值数组EqualArray
        fill(result.begin(), result.end(), ud(gen));
      }
      else if (pattern == 8 || pattern == 9) { // 8. 从零开始的随机排列RandomPermutation0 | 9. 从一开始的随机排列RandomPermutation1
        iota(result.begin(), result.end(), (int)(pattern == 9));
        shuffle(result.begin(), result.end(), gen);
      }
      return result;
    }
  };
  struct Str {
    char offset;
    explicit Str(char offset): offset(offset) {}
    string operator() (const vector<int64_t>& numbers) const {
      string result(numbers.size(), '?');
      std::transform(numbers.begin(), numbers.end(), result.begin(), [&](int64_t num) { return offset + num; });
      return result;
    }
  };
  template<class F> void testcases(const string& filename, int N, F f) {
    cout << "gen::testcases" << endl;
    ofstream file(filename);
#ifdef TEST_SOLUTION_N_TIMES
    file << N << "\n\n";
#endif
    for (int i = 0; i < N; i++) {
      f(file);
      file << "\n\n";
    }
  }
  // description: 将总数 `n` 分配为若干份，每份最多为当前剩余数值的 80%（向上取整）。让最终结果中最多只允许出现一个 1。
  template<class T> vector<int> split(T n) {
    vector<int> result;
    int count1 = 0;
    for (int piece; n > 0; ) {
      piece = (int)gen::Int(1, (n*4+4)/5)();
      if (piece == 1) {
        if (count1++ == 0) {
          result.push_back(1);
        }
      } else {
        result.push_back(piece);
      }
      n -= piece;
    }
    if (count1 > 1) result.push_back(count1-1);
    sort(result.begin(), result.end());
    return result;
  }
}

// description: 想要更方便地输出 vector 时，考虑启用以下ostream的<<运算符重载。
//template<class T> ostream& operator<<(ostream& os, const vector<T>& a) {
//  for (const T& x : a) {
//    os << x << ' ';
//  }
//  return os;
//}

#endif //SOLUTION_GEN_H

