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
  template<class T> vector<int> split(T, double allocationLimit = 0.8);

  // 随机数生成器
  mt19937_64 gen(chrono::steady_clock::now().time_since_epoch().count());

  // 随机整数
  template<class intX_t = int32_t> struct Int {
    uniform_int_distribution<intX_t> ud;
    intX_t operator()() { return ud(gen); }
    intX_t operator()(intX_t& var) { return var = ud(gen); }
    Int(intX_t lo, intX_t hi): ud(lo, hi) {
      gen_assert(lo <= hi, "Using (lo, hi) = (" + to_string(lo) + ", " + to_string(hi) + "), where lo is greater than hi for constructing uniform_int_distribution.");
    }
  };

  // 随机数组
  template<class intX_t = int32_t> struct Ints {
    uniform_int_distribution<intX_t> ud;
    vector<intX_t> operator()(int size, int pattern = 0) {
      vector<intX_t> result(size);
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
          vector<int> branchesLen = split(size, log(size)/size);
          reverse(branchesLen.begin(), branchesLen.end());
          for (int i = 0, spent = 0; i < (int)branchesLen.size(); spent += branchesLen[i++]) {
            result[spent] = (i == 0 ? -1 : Int(0, spent-1)()+(pattern == 7 ? 1 : 0));
            iota(result.begin()+spent+1, result.begin()+spent+branchesLen[i], spent+(pattern == 7 ? 1 : 0));
          }
          break;
        }
        default:
        {
          gen_assert(false, "Using an undefined pattern (pattern = " + to_string(pattern) + ").");
          break;
        }
      }
      return result;
    }
    explicit Ints(intX_t lo = -1, intX_t hi = -1): ud(lo, hi) {
      gen_assert(lo <= hi, "Using (lo, hi) = (" + to_string(lo) + ", " + to_string(hi) + "), where lo is greater than hi for constructing uniform_int_distribution.");
    }
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

  // description：将值域在 [0-25] 或 [0-9] 范围内的整数数组转换为对应字符的字符串表示。
  template<char offset>
  std::string stringify(const std::vector<int>& numbers) {
    std::string result(numbers.size(), '?');
    std::transform(numbers.begin(), numbers.end(), result.begin(),
                   [&](int num) { return (char)(offset + num); });
    return result;
  }

  /**
   * 描述：
   * 将表示树结构的父节点数组转换为边列表，以符合题目要求的输入格式。
   * - 如果 treeArr 的节点下标从零开始，则返回零开始的边列表。
   * - 如果 treeArr 的节点下标从一开始，则返回一开始的边列表。
   */
  std::vector<std::pair<int, int>> edges(const std::vector<int>& treeArr) {
    std::vector<std::pair<int, int>> edges;
    for (int v = 1; v < (int)treeArr.size(); v++) {
      edges.emplace_back(treeArr[v], v+(treeArr[1] == 1));
    }
    return edges;
  }

  /**
   * 描述：
   * 将整数 n 逐步分成若干份。
   * 每份大小受限于当前剩余值的 allocationLimit 倍（向上取整）。
   * allocationLimit 范围为 (0.0, 1.0]，默认值为 0.8，值越低每份越小且均匀。
   */
  template<class T> vector<int> split(T n, double allocationLimit) {
    int remaining = static_cast<int>(n);
    vector<int> result;
    while (remaining > 0) {
      int piece = (int)gen::Int(1, max<int>(ceil(remaining * allocationLimit), 1))();
      result.push_back(piece);
      remaining -= piece;
    }
    sort(result.begin(), result.end());
    return result;
  }
}

// 描述: 来顺序输出 vector 的元素，每个元素之间用空格分隔，末尾也会有一个空格。如果你希望更方便地将 vector 写入文件中，可以以以下方式重载 ostream 的 << 运算符。
//template<class T> ostream& operator<<(ostream& os, const vector<T>& a) {
//  for (const T& x : a) {
//    os << x << ' ';
//  }
//  return os;
//}

#endif //SOLUTION_GEN_H

