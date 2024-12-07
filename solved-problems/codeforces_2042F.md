# 题目链接：https://codeforces.com/contest/2042/problem/F

本题TAG：线段树

    fsj-o：
        背景：这是一道非常棒的题目。我个人很喜欢线段树，因为我很拿手，这一题让我看到新品种线段树的合并信息种类，让我欣喜若狂啊。扩展这个维护方式是连对区间k个子段最大和的维护也不会远矣。


### 线段树究竟是怎样的数据结构呢，它是红黑树的平替？它是树堆的另一条路径？至少，红黑树和树堆能做到的，我全部都能用线段树改造出了也可以做到的方法。
让我想到了这张图，（我也许可以画画线段树版本的这张图）
<details>
<summary>Edge娘的挣扎</summary>

![Edge_GRIL_remainingValueDesired.png](..%2Fmedias%2FEdge_GRIL_remainingValueDesired.png)

</details>

# 本题AC代码：
<details>
<summary> 👇 </summary>

```cpp
#include <bits/stdc++.h>
using namespace std;
template<class T> ostream& operator<<(ostream& os, const vector<T>& a) { for (const T& x : a) { os << x << ' '; } return os; }
template<class T, T (*op)(T, T), T (*e)(), class P, T (*mapping)(T, P), P (*id)()>
struct SegmentTree {
  int n;
  vector<T> t;
  SegmentTree(vector<T> a): n(1<<( __lg((int)a.size())+1 )), t(2*n, e()) {
    for (int i = 0; i < (int)a.size(); i++) t[i+n] = a[i];
    for (int i = n-1; i >= 1; i--) t[i] = op(t[2*i], t[2*i+1]);
  }
  T modifyQuery(int l, int r, P x = id(), int v = 1, int tl = 0, int tr = -1) {
    if (tr == -1) tr = n-1;
    if (r < tl || tr < l) return e();
    if (l <= tl && tr <= r) {
      return t[v] = mapping(t[v], x);
    }
    int tm = (tl+tr)/2;
    T lResult = modifyQuery(l, r, x, 2*v, tl, tm);
    T rResult = modifyQuery(l, r, x, 2*v+1, tm+1, tr);
    return t[v] = op(t[2*v], t[2*v+1]), op(lResult, rResult);
  }
};

// 线段树设置
using T = struct info {
  bool nodeIsVaild = false;
  int aValue = -1;
  int bValue = -1;
  int64_t aSum = -1;
  int64_t preA_Br = -1;
  int64_t Bl_sufA = -1;
  int64_t answer1 = -1;
  int64_t preA_Br_add_Bl_sufA = -1;
  int64_t preA_Br_add_answer1 = -1;
  int64_t answer1_add_Bl_sufA = -1;
  int64_t answer2 = -1;
};
T e() {
  return T{false};
}
T op(T v, T u) {
  if (!v.nodeIsVaild && !u.nodeIsVaild) return e();
  if (!v.nodeIsVaild) return u;
  if (!u.nodeIsVaild) return v;
  T result = e();
  result.nodeIsVaild = true;
  result.aValue = -1;
  result.bValue = -1;
  result.aSum = v.aSum+u.aSum; // WA2：怎么可以是 result.aValue = v.aSum+u.aValue; 呢，还检查不到。
  result.preA_Br = max(v.preA_Br, v.aSum+u.preA_Br);
  result.Bl_sufA = max(v.Bl_sufA+u.aSum, u.Bl_sufA);
  result.answer1 = max({v.answer1, u.answer1, v.Bl_sufA+u.preA_Br});
  result.preA_Br_add_Bl_sufA = max({v.preA_Br_add_Bl_sufA+u.aSum, v.aSum+u.preA_Br_add_Bl_sufA, v.preA_Br+u.Bl_sufA});
  result.preA_Br_add_answer1 = max({v.preA_Br_add_answer1, v.preA_Br+u.answer1, v.preA_Br_add_Bl_sufA+u.preA_Br, v.aSum+u.preA_Br_add_answer1});
  result.answer1_add_Bl_sufA = max({u.answer1_add_Bl_sufA, v.answer1+u.Bl_sufA, v.Bl_sufA+u.preA_Br_add_Bl_sufA, v.answer1_add_Bl_sufA+u.aSum});
  result.answer2 = max({v.answer2, u.answer2, v.answer1+u.answer1, v.Bl_sufA+u.preA_Br_add_answer1, v.answer1_add_Bl_sufA+u.preA_Br});
  return result;
}
using P = struct upd {
  char type = '~';
  int assignValue = -1;
};
P id() {
  return P{'~'};
}
T mapping(T v, P x) {
  if (x.type == '~') return v;
  T result = e();
  result.nodeIsVaild = true;
  result.aValue = (x.type == 'a' ? x.assignValue : v.aValue);
  result.bValue = (x.type == 'b' ? x.assignValue : v.bValue);
  result.aSum = result.aValue;
  result.preA_Br = result.Bl_sufA = result.aValue+result.bValue;
  result.answer1 = result.aValue+2LL*result.bValue; // WA1 这条1e9会炸
  result.preA_Br_add_Bl_sufA = -1e18;
  result.preA_Br_add_answer1 = -1e18;
  result.answer1_add_Bl_sufA = -1e18;
  result.answer2 = -1e18;
  return result;
}

void solve() {
  int n;
  cin >> n;
  vector<int> a(n), b(n);
  for (int &x : a) cin >> x;
  for (int &x : b) cin >> x;
  vector<T> infos(n, e());
  for (int i = 0; i < n; i++) {
    infos[i] = mapping(infos[i], P{'a', a[i]});
    infos[i] = mapping(infos[i], P{'b', b[i]});
  }
  SegmentTree<T, op, e, P, mapping, id> Tree(infos);
  int q;
  cin >> q;
  for (int i = 0; i < q; i++) {
    int type;
    cin >> type;
    if (type == 1) {
      int p, x;
      cin >> p >> x;
      --p;
      Tree.modifyQuery(p, p, P{'a', x});
    } else if (type == 2) {
      int p, x;
      cin >> p >> x;
      --p;
      Tree.modifyQuery(p, p, P{'b', x});
    } else {
      int l, r;
      cin >> l >> r;
      --l; --r;
      cout << Tree.modifyQuery(l, r).answer2 << '\n';
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

#ifdef LOCAL_DBG
#include "dbg/che.h" // https://github.com/visitpage/ONLINE_JUDGE_LOCAL_DBG/blob/main/che.h
#include "dbg/gen.h" // https://github.com/visitpage/ONLINE_JUDGE_LOCAL_DBG/blob/main/gen.h
struct DBG {
  DBG() {
    while (true) {
      che::program("../input.txt", "../outputA.txt", solve).run();
      che::program("../input.txt", "../outputB.txt", [&] () {
        int n;
        cin >> n;
        vector<int> a(n);
        vector<int> b(n);
        for (int &x : a) cin >> x;
        for (int &x : b) cin >> x;
        int q;
        cin >> q;
        auto calculate = [&] (int l, int r) -> int {
          int result = b[l]+b[r];
          for (int i = l; i <= r; i++) result += a[i];
          return result;
        };
        for (int i = 0; i < q; i++) {
          int t;
          cin >> t;
          if (t == 1) {
            int p, x;
            cin >> p >> x;
            --p;
            a[p] = x;
          } else if (t == 2) {
            int p, x;
            cin >> p >> x;
            --p;
            b[p] = x;
          } else {
            int l, r;
            cin >> l >> r;
            --l; --r;
            int mx = -1e9;
//            array<int, 4> pos;
            for (int i = l; i <= r; i++) {
              for (int j = i; j <= r; j++) {
                for (int k = j+1; k <= r; k++) {
                  for (int o = k; o <= r; o++) {
                    int val = calculate(i, j)+calculate(k, o);
                    if (val > mx) {
                      mx = max(mx, val);
//                      pos = {i, j, k, o};
                    }
                  }
                }
              }
            }
//            auto [i, j, k, o] = pos;
//            cerr << "[l, r] = " << i << ", " << j << ", [l, r] = " << k << ", " << o << '\n';
            cout << mx << '\n';
          }
        }
      }).run();
      assert(che::isContentConsistent("../outputA.txt", "../outputB.txt"));

      gen::testcases("../input.txt", 1, [&] (ofstream& file) {
        using namespace gen;
        int n = 20;
        file << n << '\n';
        file << Ints(-100, 100)(n) << '\n';
        file << Ints(-100, 100)(n) << '\n';
        int q = 10;
        file << q << '\n';
        for (int i = 0; i < q; i++) {
          int t = Int(1, 3)();
          file << t << ' ';
          if (t == 1 || t == 2) {
            file << Int(1, n)() << ' ' << Int(-100, 100)() << '\n';
          } else {
            file << Ints(1, n)(2, 2) << '\n';
          }
        }
      });
    }
  }
};
[[maybe_unused]] DBG run_debug;
#endif
```
</details>