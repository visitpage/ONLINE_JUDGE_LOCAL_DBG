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
#### Problem #3: [E - 1D Bucket Tool](https://atcoder.jp/contests/abc380/tasks/abc380_e) (Atcoder_abc380)

```cpp
#ifdef LOCAL_DBG
#include "dbg/che.h" // https://github.com/visitpage/ONLINE_JUDGE_LOCAL_DBG/blob/main/che.h
#include "dbg/gen.h" // https://github.com/visitpage/ONLINE_JUDGE_LOCAL_DBG/blob/main/gen.h
struct DBG {
  DBG() {
    while (true) {
      gen::testcases("../input.txt", 1, [&] (ofstream& file) { // formatted content of each testcase
        int n = 100, q = 10000;
        file << n << ' ' << q << '\n';
        int c = 1;
        for (int i = 0; i < q; i++) {
          int t = gen::Int(1, 2)();
          if (t == 1) {
            c = gen::Int(1, n)();
            file << t << ' ' << gen::Int(1, n)() << ' ' << c << '\n';
          } else {
            file << t << ' ' << c << '\n';
          }
        }
      });
      che::program("../input.txt", "../outputA.txt", solve).run(); // my current solution
      che::program("../input.txt", "../outputB.txt", [&] () { // another solution using brute force
        int n, q;
        cin >> n >> q;
        vector<int> color(n);
        iota(color.begin(), color.end(), 0);
        for (int i = 0; i < q; i++) {
          int t;
          cin >> t;
          if (t == 1) {
            int x, c;
            cin >> x >> c;
            --x; --c;
            int l = x, r = x;
            while (l-1 >= 0 && color[l-1] == color[l]) l--;
            while (r+1 < n && color[r+1] == color[r]) r++;
            for (int j = l; j <= r; j++) color[j] = c;
          } else {
            int c;
            cin >> c;
            --c;
            int count = 0;
            for (int j = 0; j < n; j++) {
              count += (color[j] == c);
            }
            cout << count << '\n';
          }
        }
      }).run();
      assert(che::isContentConsistent("../outputA.txt", "../outputB.txt"));
    }

    exit(0);
  }
} DBG;
#endif
```
这道题目使用并查集来维护数组LR，但我遇到了错误。问题出在合并数组的过程中。当需要合并三个相邻区间[l, a-1]、[a, b]和[b+1, r]时，我在合并[l, a-1]和[a, b]后，忘记再使用root更新根节点变量rt，使其指向新的根。结果我用了可能是旧的根与右侧的[b+1, r]进行合并，导致了错误。 我在使用并查集时出的错是我常常忽视正确处理根节点，因此我深感忏悔，我忏悔着地修改了并查集的代码，提醒自己直接使用并查集提供的方法获取值。
此外，这次错误让我意外的是，尽管我使用了while(true)循环与暴力方式进行调试，while(true)经过上百轮却没有发现未更新根节点的问题。直到将q设置得远大于n时，才首次出现错误。这表明，不通过并查集提供的方法进行操作是非常隐蔽且危险的。
```cpp
// 新并查集样式
struct DSU {
  vector<int> par, sz;
  vector<int> L, R;
  vector<int> color;
  DSU(int n): par(n), sz(n, 1), L(n), R(n), color(n) {
    iota(par.begin(), par.end(), 0);
    iota(L.begin(), L.end(), 0);
    iota(R.begin(), R.end(), 0);
    iota(color.begin(), color.end(), 0);
  }
  int root(int v) {
    return par[v] == v ? v : par[v] = root(par[v]);
  }
  bool join(int v, int u) {
    int a = root(v), b = root(u);
    if (a == b) return false;
    if (sz[a] > sz[b]) swap(a, b);
    L[b] = min(L[a], L[b]);
    R[b] = max(R[a], R[b]);
    par[a] = b;
    sz[b] += sz[a];
    return true;
  }
  // 在并查集中操作时，务必通过提供的方法访问数据，避免随意访问并查集内部的数组。
  // 每个节点的根节点是外部观察每个节点的代表，这样可以保持并查集的简单性。只有在合并操作时，才需要关心节点在并查集中的具体情况。  int getL(int v) { return L[root(v)]; }
  int getR(int v) { return R[root(v)]; }
  int getSz(int v) { return sz[root(v)]; }
  int getColor(int v) { return color[root(v)]; }
  void setColor(int v, int c) { color[root(v)] = c; }
};
```