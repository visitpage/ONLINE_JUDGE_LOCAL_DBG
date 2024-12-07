# 题目链接：https://codeforces.com/contest/2042/problem/F

本题TAG：线段树，动态链接性问题，模板题

    fsj-o: 
        背景：我遇见了ACM赛制十足难以检查的BUG，同时也是一点就通的BUG。比较好的是这次我总坚持认为自己的代码有错，而不觉得题目样例有疏漏。
    
    BUG信息：
        BUG名：“那个数组的长度是零！”型运行时错误
        BUG触发条件：Ⅰ 题目给出的数据范围包含长度0
                     Ⅱ 在创建STL容器时没有额外申请几个单位的长度。
                     Ⅲ 写了一些即使容器为空也会进行默认访问的代码导致RE（如这题的做动态链接性计算时的dfs）
        BUG危险程度：本BUG随着你对读题读数据范围时的思考减弱而变得难以发现。
                     通常是提交时收到结果RE，但是代码是否有粗心无果时，一旦思考数据范围时发现数据范围还有0，脑筋转过来发现了本BUG的出发条件满足了，随后加特判解决掉BUG。
        解决方案: (对应三个触发条件）
                  1. 改进对数据范围的思考，添加一轮思考数据范围包含0吗为0时是否足以触发BUG。
                  1. 相信老一辈的智慧，在数组/STL容器构造长度处多开内存（如果是习惯用全局数组的话，大力推荐本方案）
                  3. 修改代码习惯，思考哪些写法是存在默认访问的写法，将他们改造成非默认访问的写法。（这个实在做不到啊。）



# 本题AC代码：
<details>
<summary> 👇 </summary>

```cpp
#include <bits/stdc++.h>
using namespace std;
template<class T> ostream& operator<<(ostream& os, const vector<T>& a) { for (const T& x : a) { os << x << ' '; } return os; }

struct DSU_Rollback {
  struct dsu_save {
    int v, szV, u, szU;
  };
  vector<int> p, sz;
  int comps;
  vector<dsu_save> his;
  DSU_Rollback(int n): p(n), sz(n, 1), comps(n) {
    iota(p.begin(), p.end(), 0);
  }
  int root(int v) {
    return p[v] == v ? v : root(p[v]);
  }
  bool join(int v, int u) {
    int a = root(v), b = root(u);
    if (a == b) return false;
    if (sz[a] > sz[b]) swap(a, b);
    his.push_back({a, sz[a], b, sz[b]});
    p[a] = b;
    sz[b] += sz[a];
    comps--;
    return true;
  }
  void rollback() {
    if (his.empty()) return;
    auto [v, szV, u, szU] = his.back();
    his.pop_back();
    p[v] = v;
    sz[v] = szV;
    p[u] = u;
    sz[u] = szU;
    comps++;
  }
};


struct QueryTree {
  int q;
  vector<vector<pair<int, int>>> t;
  DSU_Rollback dsu;
  vector<int> answers;
  QueryTree(int q, int n): q(q), t(4*q), dsu(n), answers(q) {};
  void add(int l, int r, pair<int, int> toJoin, int v = 1, int tl = 0, int tr = -1) {
    if (tr < 0) tr = q-1;
    if (r < tl || tr < l) return;
    else if (l <= tl && tr <= r) {
      t[v].push_back(toJoin);
      return;
    } else {
      int tm = (tl+tr)/2;
      add(l, r, toJoin, 2*v, tl, tm);
      add(l, r, toJoin, 2*v+1, tm+1, tr);
      return;
    }
  }
  void dfs(int v = 1, int tl = 0, int tr = -1) {
    if (tr < 0) tr = q-1;
    int cnt = 0;
    for (auto [a, b] : t[v]) {
      if (dsu.join(a, b)) cnt++;
    }
    if (tl == tr) {
      answers[tl] = dsu.comps;
    } else {
      int tm = (tl+tr)/2;
      dfs(2*v, tl, tm);
      dfs(2*v+1, tm+1, tr);
    }
    for (int i = 0; i < cnt; i++) {
      dsu.rollback();
    }
  }
};

void solve() {
#ifdef LOCAL_DBG
  freopen("../input.txt", "r", stdin);
  freopen("../output.txt", "w", stdout);
#else
  freopen("connect.in", "r", stdin);
  freopen("connect.out", "w", stdout);
#endif

  int n, q;
  cin >> n >> q;
  if (q == 0) {
    return;
  }
  
  struct Query {
    char type;
    int v, u;
  };
  vector<Query> a(q);

  map<pair<int, int>, int> mp;
  QueryTree Tree(q, n);

  for (int i = 0; i < q; i++) {
    auto &[type, v, u] = a[i];
    cin >> type;
    if (type != '?') {
      cin >> v >> u;
      --v; --u;
      if (v > u) swap(v, u);
      if (type == '-') {
        auto it = mp.extract({v, u});
        int l = it.mapped(), r = i-1;
        Tree.add(l, r, {v, u});
      } else {
        mp[{v, u}] = i;
      }
    }
  }

  for (auto [vu, i] : mp) {
    Tree.add(i, q-1, vu);
  }

  Tree.dfs();

  for (int i = 0; i < q; i++) {
    if (a[i].type == '?') {
      cout << Tree.answers[i] << '\n';
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

//#ifdef LOCAL_DBG1
//#include "dbg/che.h" // https://github.com/visitpage/ONLINE_JUDGE_LOCAL_DBG/blob/main/che.h
//#include "dbg/gen.h" // https://github.com/visitpage/ONLINE_JUDGE_LOCAL_DBG/blob/main/gen.h
//struct DBG {
//  DBG() {
//    while (true) {
//      gen::testcases("../input.txt", 1, [&] (ofstream& file) {
//        // generating testcase...
//      });
//      che::program("../input.txt", "../outputA.txt", solve).run();
//      che::program("../input.txt", "../outputB.txt", [&] () {
//        // brute force...
//      }).run();
//      assert(che::isContentConsistent("../outputA.txt", "../outputB.txt"));
//    }
//  }
//};
//[[maybe_unused]] DBG run_debug;
//#endif
```
</details>