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
#include <cassert>

using namespace std;

namespace gen {
  // 函数的声明（实现在后面）
  void gen_warn(const string&);
  void gen_assert(bool, const string&);
  template<class T> vector<int> split(T n);

  // 随机数生成器
  mt19937_64 gen(chrono::steady_clock::now().time_since_epoch().count());

  // 随机整数
  struct Int {
    uniform_int_distribution<int64_t> ud;
    int64_t operator()() { return ud(gen); }
    int32_t operator()(int32_t& var) { return var = (int32_t)ud(gen); }
    int64_t operator()(int64_t& var) { return var = ud(gen); }
    Int(int64_t lo, int64_t hi): ud(lo, hi) {
      gen_assert(lo <= hi, "Using (lo, hi) = (" + to_string(lo) + ", " + to_string(hi) + "), where lo is greater than hi for constructing uniform_int_distribution.");
    }
  };

  // 随机数组
  struct Ints {
    uniform_int_distribution<int64_t> ud;
    vector<int64_t> operator()(int size, int pattern = 0) {
      vector<int64_t> result(size);
      switch (pattern) {
        case 0: // 0. 简单随机的数组SimpleRandom
        {
          generate(result.begin(), result.end(), [&] () { return ud(gen); });
          break;
        }
        case 1: case 5: // 1. 随机但是非递减的数组NonDecreasingRandom | 5. 随机但是非递增的数组NonIncreasingRandom
        {
          generate(result.begin(), result.end(), [&] () { return ud(gen); });
          sort(result.begin(), result.end());
          if (pattern == 5) {
            reverse(result.begin(), result.end());
          }
          break;
        }
        case 2: case 4: // 2. 随机但是递增的数组IncreasingRandom | 4. 随机但是递减的数组DecreasingRandom
        {
          unordered_set<int64_t> st;
          int count = 0;
          while (st.size() < size) {
            st.insert(ud(gen));
            count++;
            if (count > 6*size) {
              gen_warn("Too many collisions while generating INC/DEC array: " + to_string(6) + " times larger than the requested size (size = " + to_string(size) + ").");
              for (int64_t num = ud.a(); st.size() < size && num <= ud.b(); num++) {
                st.insert(num);
              }
              gen_assert(st.size() == size, "Requested size (size = " + to_string(size) + ") exceeds the available range (hi - lo + 1 = " + to_string(ud.b() - ud.a() + 1) + ").");
            }
          }
          result.assign(st.begin(), st.end());
          sort(result.begin(), result.end());
          if (pattern == 4) {
            reverse(result.begin(), result.end());
          }
          break;
        }
        case 3: // 3. 全相等的某个随机值数组EqualArray
        {
          fill(result.begin(), result.end(), ud(gen));
          break;
        }
        case 8: case 9: // 8. 从零开始的随机排列RandomPermutation0 | 9. 从一开始的随机排列RandomPermutation1
        {
          iota(result.begin(), result.end(), (int)(pattern == 9));
          shuffle(result.begin(), result.end(), gen);
          break;
        }
        case 6: case 7: // 6. 结点下标从零开始的随机树 RandomTreeParent0 | 7. 结点下标从一开始的随机树 RandomTreeParent1
                        // 用长度为 n 的 parent 数组来表示表示随机树，生成时 parent[0] == -1，即第一个节点将成为根节点。
        {
          vector<int> branchesLen = split(size);
          for (int i = 0, spent = 0; i < (int)branchesLen.size(); i++, spent += branchesLen[i]) {
            result[spent] = (i == 0 ? -1 : Int(0, spent-1)()+(pattern == 7 ? 1 : 0));
            iota(result.begin()+spent+1, result.begin()+spent+branchesLen[i], spent+(pattern == 7 ? 1 : 0));
          }
        }
        default:
        {
          gen_assert(false, "Using an undefined pattern (pattern = " + to_string(pattern) + ").");
        }
      }
      return result;
    }
    explicit Ints(int64_t lo = -1, int64_t hi = -1): ud(lo, hi) {
      gen_assert(lo <= hi, "Using (lo, hi) = (" + to_string(lo) + ", " + to_string(hi) + "), where lo is greater than hi for constructing uniform_int_distribution.");
    }
  };

  // 字符串包装类
  struct Str {
    char offset;
    string operator() (const vector<int64_t>& numbers) const {
      string result(numbers.size(), '?');
      std::transform(numbers.begin(), numbers.end(), result.begin(), [&](int64_t num) { return offset + num; });
      return result;
    }
    explicit Str(char offset): offset(offset) {}
  };

  // 函数的实现

  // description: 打印警告信息
  void gen_warn(const string& message) {
    cerr << "[Source: gen.h] Warning: " << message << endl;
  }

  // description：断言并打印错误信息
  void gen_assert(bool condition, const string& reason) {
    if (!condition) {
      cerr << "[Source: gen.h] Assertion failed. Reason: " << reason << endl;
      abort();
    }
  }

  // description: 将整数 `n` 逐步分成若干份，每份最多为当前剩余数值的 80%（向上取整），同时，让结果最多允许一个一，将多余一的合并。
  template<class T> vector<int> split(T n) {
    int remaining = static_cast<int>(n);
    vector<int> result;
    int countOfOnes = 0;
    while (remaining > 0) {
      int piece = (int)gen::Int(1, (remaining*4+4)/5)();
      if (piece == 1) {
        if (countOfOnes++ == 0) {
          result.push_back(1);
        }
      } else {
        result.push_back(piece);
      }
      remaining -= piece;
    }
    if (countOfOnes > 1) {
      result.push_back(countOfOnes-1);
    }
    sort(result.begin(), result.end());
    return result;
  }

  // description: 将一个用例格式写N次到指定文件中，通常是只写一次。
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
}

// description: 重载顺序输出 vector 的元素，每个元素之间用空格分隔，末尾也会有一个空格。如果你希望更方便地输出 vector，可以像以下方式重载 ostream 的 << 运算符。
//template<class T> ostream& operator<<(ostream& os, const vector<T>& a) {
//  for (const T& x : a) {
//    os << x << ' ';
//  }
//  return os;
//}

#endif //SOLUTION_GEN_H

