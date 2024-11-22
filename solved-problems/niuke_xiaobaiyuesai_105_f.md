# 题目链接： https://ac.nowcoder.com/acm/contest/95937/F

本题TAG：懒线段树，等比数列，MOD运算


    fsj-o：
        啊啊，
        **等比数列**，我一生的痛。高中时嫌它烦，如今在经过网络赛的没做出对勾函数那题的我，更是对它感到**PTSD**了。
        虽说如此，我今天运气不错，前天我刚裁剪出新的MOD运算整数类，昨天我折叠出LazySegtree的代码，今天我第一次打牛客小白月赛。
        有这样巧合的三件事凑在了一起，我今晚舒舒服服地过了这场比赛的F题。

    关于该懒线段树：
        懒线段树的模板化参考[AC-Library](https://github.com/atcoder/ac-library/blob/master/atcoder/lazysegtree.hpp)；
        懒线段树的构造函数和默认参数值写法参考[选手katatsugame（被炉游戏）的一次提交](https://atcoder.jp/contests/abc324/submissions/46551526)；
        懒线段树modify和query合二为一的写法是[fsj-o](https://leetcode.cn/u/fsj-o/)拍拍脑门想出来的；

```cpp
template<class T, T(*op)(T, T), T(*e)(), class P, T(*mapping)(T, P), P(*composition)(P, P), P(*id)()>
struct LazySegtree {
  int n;
  vector<T> t;
  vector<P> lazy;
  LazySegtree(vector<T> a): n(1<<(__lg(a.size())+1)), t(2*n, e()), lazy(n, id()) {
    copy(a.begin(), a.end(), t.begin()+n);
    for (int i = n-1; i >= 1; i--) t[i] = op(t[2*i], t[2*i+1]);
  }
  void apply(int v, P x) {
    if (x == id()) return;
    t[v] = mapping(t[v], x);
    if (v < n) lazy[v] = composition(lazy[v], x);
  }
  void push(int v) {
    if (lazy[v] == id()) return;
    apply(2*v, lazy[v]);
    apply(2*v+1, lazy[v]);
    lazy[v] = id();
  }
  T modifyQuery(int l, int r, P x = id(), int v = 1, int tl = 0, int tr = -1) {
    if (tr < 0) tr = n-1;
    if (r < tl || tr < l) {
      return e();
    } else if (l <= tl && tr <= r) {
      return apply(v, x), t[v];
    } else {
      push(v);
      int tm = (tl+tr)/2;
      T lResult = modifyQuery(l, r, x, 2*v, tl, tm);
      T rResult = modifyQuery(l, r, x, 2*v+1, tm+1, tr);
      return t[v] = op(t[2*v], t[2*v+1]), op(lResult, rResult);
    }
  }
};
```
    👆懒线段树模板。已被简短到手写相对OK的程度，同时保留了模板化。
    我个人很喜欢这个模板。我私人将modifyQuery合二为一的做法认为作是**利大于弊**的，这不止加快了默写速度，同时还降低默写出错率。

    于是乎，AC本题的收获：
        我发现在懒线段树模板的所有参数中，mapping的设计难度相对最高，在某些题需要写懒线段树那些“二级结论”的维护时，如何将模板参数P类型的值作用到**长度为n**的模板参数T类型的值上，在设计需要“二级结论”的维护的线段树时，是最重要的。
        我在线段树只有内部才使用的函数apply开头添加了`if (x == id()) return;`，今天说解决的就是这个bug，如果不加特判，合二为一的modifyQuery在query时就很难过，bug的检查用时为：30分钟，即时我用了全力去打印检查，这个bug还挺严重的。

# AC代码：
<details>
<summary> 👇 </summary>

```cpp
#include <bits/stdc++.h>
using namespace std;
template<class T> ostream& operator<<(ostream& os, const vector<T>& a) { for (const T& x : a) { os << x << ' '; } return os; }

int expMOD(int64_t a, int64_t b, int MOD) {
  a %= MOD;
  int64_t c = 1;
  while (b > 0) {
    if (b&1) c = c*a%MOD;
    a = a*a%MOD;
    b >>= 1;
  }
  return (int)c;
}

template<unsigned M> struct ModInt {
  int64_t x;
  ModInt(int64_t x_ = 0): x((x_%M+M)%M) {}
  ModInt operator+(const ModInt& other) const { return ModInt(this->x+other.x); }
  ModInt operator-(const ModInt& other) const { return ModInt(this->x-other.x); }
  ModInt operator*(const ModInt& other) const { return ModInt(this->x*other.x); }
  ModInt operator-() { return ModInt(-this->x); }
};

const int MOD = 1e9+7;
using mint = ModInt<MOD>;

mint geometric_sum(int a, int r, int n) {
  if (r == 1) {
    return mint(a)*mint(n);
  }
  // a*(1-r^n)/(1-r);
  return mint(a)*(mint(1)-mint(expMOD(r, n, MOD)))*mint(expMOD(mint(1-r).x, MOD-2, MOD));
}

// 模板化参考AC-Library
template<class T, T(*op)(T, T), T(*e)(), class P, T(*mapping)(T, P), P(*composition)(P, P), P(*id)()>
struct LazySegtree {
  int n;
  vector<T> t;
  vector<P> lazy;
  LazySegtree(vector<T> a): n(1<<(__lg(a.size())+1)), t(2*n, e()), lazy(n, id()) {
    copy(a.begin(), a.end(), t.begin()+n);
    for (int i = n-1; i >= 1; i--) t[i] = op(t[2*i], t[2*i+1]);
  }
  void apply(int v, P x) {
    if (x == id()) return;
    t[v] = mapping(t[v], x);
    if (v < n) lazy[v] = composition(lazy[v], x);
  }
  void push(int v) {
    if (lazy[v] == id()) return;
    apply(2*v, lazy[v]);
    apply(2*v+1, lazy[v]);
    lazy[v] = id();
  }
  T modifyQuery(int l, int r, P x = id(), int v = 1, int tl = 0, int tr = -1) {
    if (tr < 0) tr = n-1;
    if (r < tl || tr < l) {
      return e();
    } else if (l <= tl && tr <= r) {
      return apply(v, x), t[v];
    } else {
      push(v);
      int tm = (tl+tr)/2;
      T lResult = modifyQuery(l, r, x, 2*v, tl, tm);
      T rResult = modifyQuery(l, r, x, 2*v+1, tm+1, tr);
      return t[v] = op(t[2*v], t[2*v+1]), op(lResult, rResult);
    }
  }
};

using T = struct { mint prod0, prod1; int nodeLen; };
using P = int;
T e() { return T{1, 0, 0}; }
T op(T a, T b) { return T{a.prod0*b.prod0, a.prod1+b.prod1*a.prod0, a.nodeLen+b.nodeLen}; }
T mapping(T a, P x) { return T{mint(expMOD(x, a.nodeLen, MOD)), geometric_sum(x, x, a.nodeLen), a.nodeLen}; }
P composition(P a, P x) { return x; }
P id() { return -1; }

void solve() {
  int n, q;
  cin >> n >> q;
  vector<T> a(n);
  for (int i = 0; i < n; i++) {
    int x;
    cin >> x;
    a[i] = T{x, x, 1};
  }
  LazySegtree<T, op, e, P, mapping, composition, id> Tree(a);
  for (int i = 0; i < q; i++) {
    int op;
    cin >> op;
    if (op == 1) {
      int l, r, x;
      cin >> l >> r >> x;
      --l; --r;
      Tree.modifyQuery(l, r, x);
    } else {
      int l, r;
      cin >> l >> r;
      --l; --r;
      T result = Tree.modifyQuery(l, r);
      cout << result.prod1.x << '\n';
    }
  }
}


int32_t main() {
  ios::sync_with_stdio(false); cin.tie(nullptr);
#ifdef TEST_SOLUTION_N_TIMES
  freopen("../input.txt", "r", stdin);
  int N;
  cin >> N;
  for (int i = 0; i < N; i++)
#endif
          solve();
  return 0;
}

#ifdef LOCAL_DBG1
#include "dbg/che.h" // https://github.com/visitpage/ONLINE_JUDGE_LOCAL_DBG/blob/main/che.h
#include "dbg/gen.h" // https://github.com/visitpage/ONLINE_JUDGE_LOCAL_DBG/blob/main/gen.h
struct DBG {
  DBG() {
    while (true) {
      gen::testcases("../input.txt", 1, [&] (ofstream& file) {
        // generating testcase...
      });
      che::program("../input.txt", "../outputA.txt", solve).run();
      che::program("../input.txt", "../outputB.txt", [&] () {
        // brute force...
      }).run();
      assert(che::isContentConsistent("../outputA.txt", "../outputB.txt"));
    }
  }
};
[[maybe_unused]] DBG run_debug;
#endif
```
</details>