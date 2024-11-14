## 本仓库中一些头文件的`名称`、`功能`和`用法`说明：

## **一、"gen.h"** 
#### 1. `名称`："gen.h" 即 "generator.h"
#### 2. `功能`：生成随机测试用例（Generating Random Test Cases）
#### 3. `用法`：将以下静态执行的代码放入 `main.cpp`
```cpp
#ifdef LOCAL_DBG  
#include "dbg/gen.h" // https://github.com/visitpage/ONLINE_JUDGE_LOCAL_DBG/blob/main/gen.h
struct GEN {  
  GEN() {  
    gen::testcases("../input.txt", 1, [&] (ofstream& file) {  
      // write a single test case generation function here...
    });  
  }  
} GEN;  
#endif
```
#### 使用时在 gen::testcases() 的第三个参数中实现单个测试用例的工作逻辑。


## 二、"che.h"
#### 1. `名称`："che.h" 即 "checker.h"
#### 2. `功能`：检查当前解法和暴力解法输出是否一致（Check Solution Correctness）
#### 3. `用法`：将以下静态执行的放入 `main.cpp`
```cpp
#ifdef LOCAL_DBG
#include "dbg/che.h" // https://github.com/visitpage/ONLINE_JUDGE_LOCAL_DBG/blob/main/che.h 
struct CHE {
  CHE() {
    che::program("../input.txt", "../outputA.txt", solve).run();
    che::program("../input.txt", "../outputB.txt", [&] () {
      // write a brute force solution here...
    }).run();
    assert(che::isContentConsistent("../outputA.txt", "../outputB.txt"));
  }
} CHE;
#endif
```
#### 使用时在两个中的一个 che::program() 的第三个参数中实现暴力解法的工作逻辑。

---
## Examples: 
# 实战，DEBUG，LET'S START。
###  Problem #1: [E - Sum of All Substrings](https://atcoder.jp/contests/abc379/submissions/59613160) (Atcoder_abc379)
```cpp
#ifdef LOCAL_DBG
#include "dbg/che.h" // https://github.com/visitpage/ONLINE_JUDGE_LOCAL_DBG/blob/main/che.h
#include "dbg/gen.h" // https://github.com/visitpage/ONLINE_JUDGE_LOCAL_DBG/blob/main/gen.h
struct DBG {
  DBG() {
    while (true) { // 0. 无限循环直到出错。
      // 1. 编写代码来生成随机测试用例，并实现暴力解法与错误解法进行对比。
      // 2. 两个解法被调用时使用相同的输入（就是本次测试生成的用例），当解法输出内容不一致时，断言会终止程序。
      // 3. 首次出错的随机测试用例及其对应输出将保留在生成时使用的文件中，之后可以用于笔调或者在IDE中进行调试。
      vector<int64_t> a = gen::Ints(1, 10)(5);
      int sum = 0;
      for (int x : a) sum += x;
      if (sum != 10) continue;
      gen::testcases("../input.txt", 1, [&] (ofstream& file) {
        file << 10 << ' ' << 5 << '\n';
        file << gen::Ints(1, 10)(5, 2) << '\n';
        file << a << '\n';
      });
      
      che::program("../input.txt", "../outputA.txt", solve).run();
      che::program("../input.txt", "../outputB.txt", [&] () {
        int n, m;
        cin >> n >> m;
        vector<int> pos(m), cnt(m);
        vector<int> X(n);
        for (int i = 0; i < m; i++) {
          cin >> pos[i];
          --pos[i];
        }
        for (int i = 0; i < m; i++) {
          cin >> cnt[i];
        }
        for (int i = 0; i < m; i++) {
          X[pos[i]] = cnt[i];
        }
        int64_t count = 0;
        for (int i = 0; i < n; i++) {
          if (X[i] == 0) {
            for (int j = i-1; j >= 0; j--) {
              if (X[j] > 1) {
                X[j]--;
                count += i-j;
                X[i]++;
              }
            }
            if (X[i] == 0) {
              cout << -1 << '\n';
              return;
            }
          }
        }
        for (int i = 0; i < n; i++) {
          if (X[i] != 1) {
            cout << -1 << '\n';
            return;
          }
        }
        cout << count << '\n';
      }).run();
      assert(che::isContentConsistent("../outputA.txt", "../outputB.txt"));
    }
  }
} DBG;
#endif
```

### Problem #2: (cf.) [E. XOR and Favorite Number](https://codeforces.com/problemset/problem/617/E) (Codeforces_round_340_div2)
```cpp
#ifdef LOCAL_DBG1
#include "dbg/che.h" // https://github.com/visitpage/ONLINE_JUDGE_LOCAL_DBG/blob/main/che.h
#include "dbg/gen.h" // https://github.com/visitpage/ONLINE_JUDGE_LOCAL_DBG/blob/main/gen.h
struct DBG {
  DBG() {
    while (true) {
       gen::testcases("../input.txt", 1, [&] (ofstream& file) {
        int n = 10, q = 10, k = gen::Int(0, 10)();
        file << n << ' ' << q << ' ' << k << '\n';
        file << gen::Ints(0, 10)(n) << '\n';
        for (int i = 0; i < q; i++) {
          file << gen::Ints(1, n)(2, 1) << '\n';
        }
      });
      che::program("../input.txt", "../outputA.txt", solve).run();
      che::program("../input.txt", "../outputB.txt", [&] () {
        int n, q, k;
        cin >> n >> q >> k;
        vector<int> a(n);
        for (int& x : a) cin >> x;
        for (int i = 0; i < q; i++) {
          int l, r;
          cin >> l >> r;
          --l; --r;
          int count = 0;
          for (int i = l; i <= r; i++) {
            int sum = 0;
            for (int j = i; j <= r; j++) {
              sum ^= a[j];
              if (sum == k) count++;
            }
          }
          cout << count << '\n';
        }
      }).run();
      assert(che::isContentConsistent("../outputA.txt", "../outputB.txt"));
    }
  }
} DBG;
#endif
```
note：`MO's  algortithm`的**add**和**remove** lambda都分别写会比较好，我第一次只写了一个add(±1)想一个顶俩，会因为add和remove计数答案和维护数据结构的代码执行顺序不同导致出错了。

#### Problem #3: 