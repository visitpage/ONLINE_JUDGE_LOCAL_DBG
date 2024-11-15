## 本仓库一些头文件的`名称`、`功能`以及`用法`说明

### 一、"gen.h"
#### 1. `名称`：generator
#### 2. `功能`：该头文件帮助生成随机测试用例。
#### 3. `用法`：通过使用其中的gen::testcases()方法，可以生成 N 组随机测试用例，并将其保存到指定的文本文件中。每组测试用例的格式需在gen::testcases()的第三个参数进行设计。以下是静态使用例：

```cpp
#include "dbg/gen.h" // https://github.com/visitpage/ONLINE_JUDGE_LOCAL_DBG/blob/main/gen.h
struct GEN {  
  GEN() {  
    gen::testcases("../input.txt", 1, [&] (ofstream& file) {  
      // one single testcase generating...
    });  
  }  
} GEN;  
```

---

### 二、"che.h"
#### 1. 名称：checker
#### 2. 功能：该头文件用于验证当前解法与暴力解法输出的一致性。
#### 3. 用法：通过使用其中che::program结构体和che::isContentConsistent方法，可以对两个解题代码使用相同的输入、运行、各自输出，并检查输出的一致性。che::program结构体需要运行的代码由构造时的第三个参数指定。che::program结构体的前两个参数则用于指定代码运行输入输出将使用的文件。以下是静态使用例：

```cpp
#include "dbg/che.h" // 更多信息请访问：https://github.com/visitpage/ONLINE_JUDGE_LOCAL_DBG/blob/main/che.h
struct CHE {
    CHE() {
        che::program("../input.txt", "../outputA.txt", solve).run();
        che::program("../input.txt", "../outputB.txt", [&] () {
            // implemented brute force solution...
        }).run();
        assert(che::isContentConsistent("../outputA.txt", "../outputB.txt"));
    }
} CHE;
```

---
### 三、Examples:
以下是一些使用历史，和当时记录的日志。
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
      gen::testcases("../input.txt", 1, [&] (ofstream& file) { // formatted content of each testcase
        file << 10 << ' ' << 5 << '\n';
        file << gen::Ints(1, 10)(5, 2) << '\n';
        file << a << '\n';
      });
      
      che::program("../input.txt", "../outputA.txt", solve).run(); // my current solution
      che::program("../input.txt", "../outputB.txt", [&] () { // another solution using brute force
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
24B09：这种基于 while(true) 的调试方案是我在这场比赛中首次尝试的，用于处理 WA。对于那些可以轻松编写暴力程序进行验证的问题，这种方法让我能够将检查时间转化为完成代码的时间。同时，这种方法的一个优点是，由于代码通过了小例子的随机测试，第二次提交时出现 WA 的概率较低。
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
24B14；我发现将 MO's algorithm 的 add 和 remove 分别实现为独立的 lambda 函数会更好。第一次解这道题时，我只实现了一个 add（±1），试图用一个函数完成两个操作，结果由于 add 和 remove 在计数答案及维护数据结构时的执行顺序需要反转，导致出错。
#### Problem #3: 