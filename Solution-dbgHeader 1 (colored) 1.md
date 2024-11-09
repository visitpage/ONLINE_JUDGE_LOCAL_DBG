# RandomTestcaseGeneratingKit （随机测试用例生成工具箱）
```cpp
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
              cerr << "gen[fatal-error]: hi-lo+1 < n" << endl;  
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
    for (T piece; n > 0; ) {  
      piece = gen::Int(1, (n*4+4)/5)();  
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
  
#endif //SOLUTION_GEN_H 
```
## Example: RandomTestcaseGenerating（随机测试用例生成例）
```cpp
#ifdef LOCAL_DBG  
#include "dbg/gen.h" // [网址还没有，还没有上传]()  
struct GEN {  
  GEN() {  
    gen::testcases("../input.txt", 1, [&] (ofstream& file) {  
      auto list = gen::split(1e2);  
      file << list.size() << '\n';  
      for (int n : list) {  
        file << n << '\n';  
        for (int i = 0; i < n; ++i) {  
          file << gen::Ints(1, 100)(n) << '\n';  
        }  
        file << '\n';  
      }  
    });  
  }  
} GEN;  
#endif
```

# SolutionCorrectnessChecker （解决方案正确性检查类）
```cpp
//  
// Created by sion on 2024/11/8.  
//  
  
#ifndef SOLUTION_CHE_H  
#define SOLUTION_CHE_H  
  
#include <iostream>  
#include <functional>  
#include <fstream>  
  
using namespace std;  
  
namespace che {  
  struct program {  
    ifstream in;  
    ofstream out;  
    function<void()> solve;  
    program(const string& in, const string& out, const function<void()>& f): in(in), out(out), solve(f) {}  
    void run() const {  
      cout << "che::program.run" << endl;  
      streambuf *oldCinBuf = cin.rdbuf(), *oldCoutBuf = cout.rdbuf();  
      cin.rdbuf(in.rdbuf()); cout.rdbuf(out.rdbuf());  
#ifdef TEST_SOLUTION_N_TIMES  
      int N;  
      cin >> N;  
      for (int i = 0; i < N; i++)  
#endif  
        solve();  
      cin.rdbuf(oldCinBuf); cout.rdbuf(oldCoutBuf);  
    }  
  };  
  bool isContentConsistent(const string& filenameA, const string& filenameB) {  
    cout << "che::isContentConsistent" << endl;  
    std::ifstream fileA(filenameA), fileB(filenameB);  
    for (std::string a, b; a == b && !fileA.eof() && !fileB.eof(); fileA >> a, fileB >> b) {}  
    return fileA.eof() && fileB.eof();  
  }  
}  
  
#endif //SOLUTION_CHE_H
```
## Example: SolutionCorrectnessCheck （解决方案正确性检查）
```cpp
#ifdef LOCAL_DBG  
#include "dbg/che.h" // [网址还没有，还没有上传]()  
struct CHE {  
  CHE() {  
    che::program("../input.txt", "../outputA.txt", solve).run();  
    che::program("../input.txt", "../outputB.txt", [&] () {  
    }).run();  
    assert(che::isContentConsistent("../outputA.txt", "../outputB.txt"));  
  }  
} CHE;  
#endif
```