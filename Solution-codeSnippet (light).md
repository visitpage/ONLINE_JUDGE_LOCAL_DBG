# 我果然还是需要复制粘贴代码片段的，就不手写，绝不手写，你能怎么找我



# BinaryLifting （树的倍增LCA）
```cpp
void binaryLiftingCodeSnippet(int n, vector<vector<int>>& edges) {
	int m = edges.size();
	std::vector<std::vector<std::pair<int, int>>> G(n);
	for (int i = 0; i < m; i++) {
		auto& e = edges[i];
		int v = e[0], u = e[1], w = e[2];
		G[v].push_back({u, w-1});
		G[u].push_back({v, w-1});
	}
	const int K = __lg(n)+1;
	std::vector<std::vector<int>> par(K, std::vector<int>(n, -1));
	std::vector<std::vector<int>> dp(n, std::vector<int>(26));
	std::vector<int> dep(n);
	auto init = [&] (auto&& self, int v, int p) -> void {
		par[0][v] = p;
		for (auto [u, w] : G[v]) {
			if (u == p) continue;
			dp[u] = dp[v]; 
			dp[u][w]++;
			dep[u] = dep[v]+1;
			self(self, u, v);
		}
	};
	init(init, 0, -1);
	for (int i = 1; i < K; i++) {
		for (int v = 0; v < n; v++) {
			if (par[i-1][v] != -1) par[i][v] = par[i-1][par[i-1][v]];
		}
	}
	auto lca = [&] (int v, int u) -> int {
		if (dep[v] > dep[u]) std::swap(v, u);
		for (int i = 0; i < K; i++) {
			if ((dep[u]-dep[v])>>i&1) u = par[i][u];
		}
		if (u == v) return u;
		for (int i = K-1; i >= 0; i--) {
			if (par[i][v] != par[i][u]) {
				v = par[i][v];
				u = par[i][u]; 
			}
		}
		return par[0][v];
	};
}
```

# CombMod （预处理组合数）
```cpp
struct CombMod {
  std::vector<mint> fac, invfac;  
  CombMod(int n) : fac(n + 3), invfac(n + 3) {  
    fac[0] = 1;  
    for (int i = 0; i < n; i++) fac[i + 1] = fac[i] * (i + 1);  
    invfac[n] = fac[n].inv();  
    for (int i = n; i > 0; i--) invfac[i - 1] = invfac[i] * i;  
  }  
  mint a(int n, int r) {  
    assert(n < (int)fac.size());
    if (r < 0 || r > n) return 0;  
    return fac[n] * invfac[n - r];  
  }  
  mint c(int n, int r) {  
    assert(n >= 0 && r >= 0); // [I Love Balls](https://codeforces.com/contest/1983/problem/E)
    if (r < 0 || r > n) return 0;  
    return a(n, r) * invfac[r];  
  }  
};
```

# Disjoint Set Union (路径压缩并查集)
```cpp
struct DSU {
    std::vector<int> par, sz;
    DSU(int n): par(n), sz(n, 1) {
        std::iota(par.begin(), par.end(), 0);
    }
    int root(int v) {
        return (par[v] == v ? v : par[v] = root(par[v]));
    }
    bool join(int v, int u) {
        int a = root(v), b = root(u);
        if (a == b) return false;
        if (sz[a] > sz[b]) swap(a, b);
        par[a] = b;
        sz[b] += sz[a];
        return true;
    }
    int size(int v) {
        return sz[root(v)];
    }
    bool same(int v, int u) {
        return root(v) == root(u);
    }
};
```

# Diameter of Tree （求带权树的直径）
```cpp
void solve() {    
  int n;    
  std::cin >> n;    
  std::vector<std::vector<std::pair<int, int>>> G(n);    
  long long ans = 0LL;    
  for (int i = 0; i < n-1; i++) {    
    int v, u, w;    
    std::cin >> v >> u >> w;    
    --v; --u;    
    G[v].push_back({u, w}); a   
    G[u].push_back({v, w});    
    ans += 2LL*w;    
  }    
  auto bfs = [&](int gs) -> std::vector<long long> {    
    std::vector<long long> dist(n, 1e18);    
    std::vector<int> q {gs};    
    dist[gs] = 0;    
    for (int i = 0; i < q.size(); i++) {    
      int v = q[i];    
      for (auto [u, w] : G[v]) {    
        if (dist[u] == 1e18) {    
          dist[u] = dist[v]+w;    
          q.push_back(u);    
        }    
      }    
    }    
    return dist;    
  };    
  std::vector<long long> dep = bfs(0);    
  int a = max_element(all(dep))-dep.begin();    
  std::vector<long long> dista = bfs(a);    
  int b = max_element(all(dista))-dista.begin();    
  long long mx = dista[b];    
  std::cout << ans-mx << '\n';    
}
```

# Div Floor Or Ceil （整数除法的上取整或下取整）
```cpp
// author: gpt4o
// editor: fsj-o
// description: 一般的除法取整方法 `a/b` 和 `(a+b-1)/b` 无法处理结果为有理数且为负数的情况，此时需要这两个方法。

template<class T>  
T divFloor(T a, T b) {  
  if (b == 0) { throw std::invalid_argument("Division by zero"); }  
  T result = a / b;  
  // 如果有余数且负数，则需要向下取整  
  if (a % b != 0 && (a ^ b) < 0) result--;  
  return result;  
}  

template<class T>  
T divCeil(T a, T b) {  
  if (b == 0) { throw std::invalid_argument("Division by zero"); }  
  T result = a / b;  
  // 如果有余数且正数，则需要向上取整  
  if (a % b != 0 && (a ^ b) > 0) result++;  
  return result;  
}
```

# ExpMod （快速幂）
```cpp
int expmod(long long a, long long b, int m) {  
  a %= m;  
  long long c = 1;  
  while (b) {  
    if (b&1) c = c*a%m;  
    a = a*a%m;  
    b >>= 1;  
  }  
  return c;  
};
```

# Fenwick Tree
```cpp
struct Fenwick {
  std::vector<int> t;
  Fenwick(int n): t(n+1) {}
  void add(int i, int x) {
    ++i;
    while (i < t.size()) {
      t[i] += x;
      i += i&-i;
    }
  }
  int query(int i) {
    ++i;
    int res = 0;
    while (i > 0) {
      res += t[i];
      i &= i-1;
    }
    return res;
  }
  int query(int i, int j) {
    // WA：++i; ++j; 这行请不要多加。
    if (i > j) return 0;
    return query(j)-query(i-1);
  }
};
```

# Floyd-Warshall Method （仅对稠密图使用）
```cpp
std::vector G(n, std::vector<int>(n, 1e9));  
for (auto& e : edges) {  
  int x = e[0], y = e[1], w = e[2];  
  G[x][y] = G[y][x] = w;  
}  
for (int i = 0; i < n; i++) G[i][i] = 0;  
for (int k = 0; k < n; k++) {  
  for (int i = 0; i < n; i++) {  
    if (G[i][k] == 1e9) continue; // 稀疏图优化
    for (int j = 0; j < n; j++) {  
      if (G[i][k] + G[k][j] < G[i][j]) {  
        G[i][j] = G[i][k] + G[k][j];  
      }  
    }  
  }  
}
```

# Intersection Point（两个二元一次方程的交点）
```cpp
// problem：https://atcoder.jp/contests/abc377/tasks/abc377_f
// description: 求a1*x+b2*y+c1和a2*x+b2*y+c2的整数交点，交点限制在[1, n][1, n]范围内。
auto inter = [&] (tuple<int, int, int> v, tuple<int, int, int> u)-> tuple<int, int, bool> {  
  auto [a1, b1, c1] = v;  
  auto [a2, b2, c2] = u;  
  if (1LL*a1*b2 == 1LL*a2*b1) return {-1, -1, false}; // 平行无交点  
  if ((1LL*b1*c2-1LL*b2*c1)%(1LL*a1*b2-1LL*b1*a2) != 0) return {-1, -1, false}; // 无整数交点  
  long long x = (1LL*b1*c2-1LL*b2*c1)/(1LL*a1*b2-1LL*b1*a2);  
  long long y = (b1 != 0 ? (1LL*-a1*x-c1)/b1 : (1LL*-a2*x-c2)/b2);  
  if (x < 1 || y < 1 || x > n || y > n) return {-1, -1, false}; // 超出边界，本题为[1, n][1, n]
  return {x, y, true};  
};
```

# Kth Biggest Elem
```cpp
template<class T> T kthLargestElem(std::vector<T>& a, int k) {  
  int n = a.size();  
  if (n < k) return -1;  
  nth_element(a.begin(), a.begin()+n-k, a.end());  
  return a[n-k];  
}
```

# Linear Equations（某二元一次方程的通解）
```cpp
// S: signed integer  
template<class S>  
S gcd(S a, S b, S& x, S& y) {  
  if (b == 0) {  
    x = 1; y = 0;  
    return a;  
  }  
  S x1, y1;  
  S g = gcd(b, a % b, x1, y1);  
  x = y1; y = x1 - (a / b) * y1;  
  return g;  
}  
template<class S>  
bool find_any_solution(S a, S b, S c, S& x, S& y, S& g) {  
  g = gcd(abs(a), abs(b), x, y);  
  if (c % g) return false;  
  x *= c / g;  
  y *= c / g;  
  if (a < 0) x = -x;  
  if (b < 0) y = -y;  
  return true;  
}
// S: signed integer  
template<class S>  
S divFloor(S a, S b) {  
  return a / b - ((a ^ b) < 0 && a % b != 0 ? 1 : 0);  
}  
template<class S>  
S divCeil(S a, S b) {  
  return a / b + ((a ^ b) > 0 && a % b != 0 ? 1 : 0);  
}
// 通解： x + (b / g) * k 和 y - (a / g) * k （k为任意整数），任意解由通解易得
// 当方程C固定时，将find_any_solution函数改装进main函数以提升效率
```

# LazySegtree（*模板化参考AC-Library*）
```cpp
template<class S, S (*op)(S, S), S (*e)(),
         class F, S (*mp)(S, F), F (*comp)(F, F), F (*id)()>
struct LazySegtree {
  // static_assert(op(e(), e()) == e(), "LazySegtree: currectly op(e(), e()) != e(). ");
  // static_assert(mp(e(), id()) == e(), "LazySegtree: currectly mp(e(), id()) != e(). ");
  // static_assert(comp(id(), id()) == id(), "LazySegtree: currectly comp(id(), id()) != id(). ");
  std::vector<S> t;
  std::vector<F> lazy;
  int n;
  LazySegtree(int _n, S x = e()) : n(_n), t(4 * _n), lazy(4 * _n, id()) {
    std::function<S(int, int, int)> init = [&] (int v, int tl, int tr) -> S {
      if (tl == tr) return t[v] = x;
      int tm = (tl + tr) / 2;
      return t[v] = op(init(2*v, tl, tm), init(2*v + 1, tm + 1, tr));
    };
    init(1, 0, _n-1); // take attention!
  }
  void push(int v) {
    if (lazy[v] == id()) return;
    t[2*v] = mp(t[2*v], lazy[v]);
    t[2*v+1] = mp(t[2*v+1], lazy[v]);
    lazy[2*v] = comp(lazy[2*v], lazy[v]);
    lazy[2*v+1] = comp(lazy[2*v+1], lazy[v]);
    lazy[v] = id();
  }
  void modify(int v, int tl, int tr, int l, int r, F x) {
    if (r < tl || tr < l) return;
    if (tl == l && tr == r) {
      t[v] = mp(t[v], x);
      lazy[v] = comp(lazy[v], x);
      return; // take attention!
    }
    push(v);
    int tm = (tl+tr)/2;
    modify(2*v, tl, tm, std::max(l, tl), std::min(r, tm), x);
    modify(2*v+1, tm+1, tr, std::max(l, tm+1), std::min(r, tr), x);
    t[v] = op(t[2*v], t[2*v+1]);
    return;
  }
  S query(int v, int tl, int tr, int l, int r) {
    if (r < tl || tr < l) return e();
    if (tl == l && tr == r) return t[v];
    push(v);
    int tm = (tl+tr)/2;
    S lRes = query(2*v, tl, tm, std::max(tl, l), std::min(tm, r));
    S rRes = query(2*v+1, tm+1, tr, std::max(tm+1, l), std::min(tr, r));
    return op(lRes, rRes);
  }
  void modify(int l, int r, F x) { modify(1, 0, n-1, l, r, x); }
  S query(int l, int r) { return query(1, 0, n-1, l, r); }
  
  // 用于线段树二分
  std::vector<std::tuple<int, int, int>> getVertexes(int l, int r) {
    assert(0 <= l); assert(r <= n-1);
    std::vector<std::tuple<int, int, int>> res;
    auto collect = [&] (auto&& self, int v, int tl, int tr) -> void {
      if (r < tl || tr < l) return;
      if (l <= tl && tr <= r) {
        res.push_back({v, tl, tr});
        return;
      }
      push(v); // 需要push，这些选中的节点，马上就要使用了。
      int tm = (tl+tr)/2;
      self(self, 2*v, tl, tm);
      self(self, 2*v+1, tm+1, tr);
      return;
    };
    collect(collect, 1, 0, n-1);
    return res;
  }
  // 用法参考“音乐会”这题：https://leetcode.cn/problems/booking-concert-tickets-in-groups/
};
// 
// =======================================分割线=======================================
// 
// Example:
// 线段树维护和
using info = std::pair<long long, int>;
info op(info a, info b) { return info{a.first+b.first, a.second+b.second}; }
info e() { return info{0LL, 0}; }

// lazyTag区间加
using F1 = long long;
info mp1(info a, F1 x) { return info {a.first+1LL*a.second*x, a.second}; }
F1 comp1(F1 a, F1 x) { return a+x; }
F1 id1() { return 0LL; }

// 或者lazyTag区间assign
using F2 = int;
info mp2(info a, F2 x) { return (x == -1 ? a : info { 1LL*a.second*x, a.second}); }
F2 comp2(F2 a, F2 x) { return (x == -1 ? a : x); }
F2 id2() { return -1; }

// 创建线段树例
//  LazySegtree<info, op, e, F1, mp1, comp1, id1> T(n, {0LL, 1});

// Debug (放在modify(l, r, x)函数中)
#ifdef DEBUG
	std::cout << "After modify(" << l << ", " << r << ", " << x << "), LazyTree currectly: \n";
	for (int i = 0; i < n; i++) {
	  std::cout << "l(" << i << "): ";
	  for (int j = l; j < n; j++) {
		std::cout << query(i, j).first << " \n"[j == n-1];
	  }
	}
	std::cout << "\n\n";
#endif
```

# ModInt （模运算整数类）
```cpp
// https://github.com/hos-lyric/libra/blob/master/algebra/modint.h  
template <unsigned M_> struct ModInt {  
  static constexpr unsigned M = M_;  
  unsigned x;  
  constexpr ModInt() : x(0U) {}  
  constexpr ModInt(unsigned x_) : x(x_ % M) {}  
  constexpr ModInt(unsigned long long x_) : x(x_ % M) {}  
  constexpr ModInt(int x_) : x(((x_ %= static_cast<int>(M)) < 0) ? (x_ + static_cast<int>(M)) : x_) {}  
  constexpr ModInt(long long x_) : x(((x_ %= static_cast<long long>(M)) < 0) ? (x_ + static_cast<long long>(M)) : x_) {}  
  ModInt &operator+=(const ModInt &a) { x = ((x += a.x) >= M) ? (x - M) : x; return *this; }  
  ModInt &operator-=(const ModInt &a) { x = ((x -= a.x) >= M) ? (x + M) : x; return *this; }  
  ModInt &operator*=(const ModInt &a) { x = (static_cast<unsigned long long>(x) * a.x) % M; return *this; }  
  ModInt &operator/=(const ModInt &a) { return (*this *= a.inv()); }  
  ModInt pow(long long e) const {  
    if (e < 0) return inv().pow(-e);  
    ModInt a = *this, b = 1U; for (; e; e >>= 1) { if (e & 1) b *= a; a *= a; } return b;  
  }  
  ModInt inv() const {  
    unsigned a = M, b = x; int y = 0, z = 1;  
    for (; b; ) { const unsigned q = a / b; const unsigned c = a - q * b; a = b; b = c; const int w = y - static_cast<int>(q) * z; y = z; z = w; }  
    assert(a == 1U); return ModInt(y);  
  }  
  ModInt operator+() const { return *this; }  
  ModInt operator-() const { ModInt a; a.x = x ? (M - x) : 0U; return a; }  
  ModInt operator+(const ModInt &a) const { return (ModInt(*this) += a); }  
  ModInt operator-(const ModInt &a) const { return (ModInt(*this) -= a); }  
  ModInt operator*(const ModInt &a) const { return (ModInt(*this) *= a); }  
  ModInt operator/(const ModInt &a) const { return (ModInt(*this) /= a); }  
  template <class T> friend ModInt operator+(T a, const ModInt &b) { return (ModInt(a) += b); }  
  template <class T> friend ModInt operator-(T a, const ModInt &b) { return (ModInt(a) -= b); }  
  template <class T> friend ModInt operator*(T a, const ModInt &b) { return (ModInt(a) *= b); }  
  template <class T> friend ModInt operator/(T a, const ModInt &b) { return (ModInt(a) /= b); }  
  explicit operator bool() const { return x; }  
  bool operator==(const ModInt &a) const { return (x == a.x); }  
  bool operator!=(const ModInt &a) const { return (x != a.x); }  
  friend std::ostream &operator<<(std::ostream &os, const ModInt &a) { return os << a.x; }  
};
// using mint = ModInt<998244353>; // check 998_244_353  
const int MOD = (int)1e9+7;  
using mint = ModInt<MOD>;
```

# SegmentTree for DP（*模板化参考AC-Library*）
```cpp
template<class T, T (*op)(T, T), T (*e)()> struct Segtree {  
  int n;  
  std::vector<T> t;  
  Segtree(int n_): n(n_), t(4*n, e()) { assert(op(e(), e()) == e()); }  
  void modify(int v, int tl, int tr, int p, T x) {  
    if (tl == tr) {  
      t[v] = op(t[v], x); // 非assign  
      return;  
    }  
    int tm = (tl+tr)/2;  
    if (p <= tm) {  
      modify(2*v, tl, tm, p, x);  
    } else {  
      modify(2*v+1, tm+1, tr, p, x);  
    }  
    t[v] = op(t[2*v], t[2*v+1]);  
  }  
  void modify(int p, T x) {  
    modify(1, 0, n-1, p, x);  
  }  
  T query(int v, int tl, int tr, int l, int r) {  
    if (l > tr || r < tl) return e();  
    if (l <= tl && tr <= r) return t[v];  
    int tm = (tl+tr)/2;  
    return op(query(2*v, tl, tm, l, r),  
              query(2*v+1, tm+1, tr, l, r));  
  }  
  T query(int l, int r) {  
    return query(1, 0, n-1, l, r);  
  }  
};
```


# Sieve of primes（预处理质数）
```cpp
struct Sieve {
  std::vector<int> lp; // 用线性筛处理lp
  std::vector<int> pr;
  Sieve(int n) {
    lp.resize(n+1);
    for (int i=2; i <= n; ++i) {
      if (lp[i] == 0) {
        lp[i] = i;
        pr.push_back(i);
      }
      for (int j = 0; i * pr[j] <= n; ++j) {
        lp[i * pr[j]] = pr[j];
        if (pr[j] == lp[i]) {
          break;
        }
      }
    }
  }
};
```
# Trie （前缀树）
```cpp
struct Trie {  
  struct node {  
    bool isEnd = false;  
    std::array<int, 26> nex {};  
  };  
  std::vector<node> t {node{}, node{}};  
  void insert(const std::string& s) {  
    int n = s.size();  
    int v = 1;  
    for (int i = 0; i < n; i++) {  
      if (!t[v].nex[s[i]-'a']) {  
        t[v].nex[s[i]-'a'] = t.size();  
        t.push_back(node{});  
      }  
      v = t[v].nex[s[i]-'a'];  
    }  
    t[v].isEnd = true;  
  }  
  bool query(const std::string& s) {  
    int n = s.size();  
    int v = 1;  
    for (int i = 0; i < n; i++) {  
      int c = s[i]-'a';  
      if (!t[v].nex[c]) {  
        return false;  
      }  
      v = t[v].nex[c];  
    }  
    return t[v].isEnd;  
  }  
};
```

# Z-function （KMP同位代替）
```cpp
// from https://cp-algorithms.com/string/z-function.html  
std::vector<int> z_function(std::string s) {
    int n = s.size();
    std::vector<int> z(n);
    int l = 0, r = 0;
    for(int i = 1; i < n; i++) {
        if(i < r) {
            z[i] = min(r - i, z[i - l]);
        }
        while(i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            z[i]++;
        }
        if(i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }
    return z;
}
```


# ~~Ex. Hard-to-Use~~

# BaseRingTreeSolution （处理基环树的常用方式）
```cpp 
// author: A_G  
const int MOD = 1e9 + 7;  
  
// 以下是使用例：  
// https://codeforces.com/contest/1863/problem/G  
int main() {  
  std::ios::sync_with_stdio(0); std::cin.tie(0);  
  
  int n;  
  std::cin >> n;  
  std::vector<int> a(n);  
  std::vector<std::vector<int>> H(n); // inverse graph  
  for (int i = 0; i < n; i++) {  
    std::cin >> a[i];  
    --a[i];  
    H[a[i]].push_back(i);  
  }  
  
  std::vector<int> in(n);  
  for (int i = 0; i < n; i++) {  
    in[a[i]]++;  
  }  
  
  std::vector<int> q;  
  for (int i = 0; i < n; i++) {  
    if (in[i] == 0) q.push_back(i);  
  }  
  for (int i = 0; i < q.size(); i++) {  
    int x = q[i];  
    if (--in[a[x]] == 0) {  
      q.push_back(a[x]);  
    }  
  }  
  
  std::vector<bool> incycle(n, true);  
  for (int i : q) {  
    incycle[i] = false;  
  }  
  
  // recal in array  
  fill(in.begin(), in.end(), 0);  
  for (int i = 0; i < n; i++) {  
    in[a[i]]++;  
  }  
  
  std::vector<bool> vis(n);  
  long long ans = 1LL;  
  for (int i = 0; i < n; i++) {  
    if (incycle[i] && !vis[i]) {  
      int x = i;  
      std::vector<int> cycle;  
      while (1) {  
        vis[x] = true;  
        cycle.push_back(x);  
        x = a[x];  
        if (x == i) break;  
      }  
      std::vector<int> other;  
      std::function<void(int)> dfs = [&](int v) {  
        for (int u : H[v]) {  
          if (vis[u]) continue;  
          vis[u] = true;  
          other.push_back(u);  
          dfs(u);  
        }  
      };  
      for (int v : cycle) dfs(v);  
  
      // process cycle && process other  
      long long cur = 1;  
      for (int v : cycle) {  
        cur = cur * (in[v] + 1) % MOD;  
      }  
      for (int v : cycle) {  
        cur = ((cur - in[v]) % MOD + MOD) % MOD;  
      }  
      ans = ans * cur % MOD;  
  
      for (int v : other) {  
        ans = ans * (in[v] + 1) % MOD;  
      }  
    }  
  }  
  
  std::cout << ans << '\n';  
}
```

# Critical Connections(Tarjan Method) （求无向图关键连接O(n+m)）
```cpp
// Based on https://cp-algorithms.com/graph/bridge-searching.html
class Solution {  
public:  
  std::vector<std::vector<int>> criticalConnections(int n, std::vector<std::vector<int>>& connections) {  
    std::vector<std::vector<int>> G(n);  
    for (auto& e : connections) {  
      int x = e[0], y = e[1];  
      G[x].push_back(y);  
      G[y].push_back(x);  
    }  
    std::vector<int> vis(n), tin(n), dp(n);  
    int timer = -1;  
    std::vector<std::vector<int>> ans;  
    std::function<void(int, int)> dfs = [&] (int v, int p) {  
      vis[v] = 1;  
      tin[v] = dp[v] = ++timer;  
      for (int u : G[v]) {  
        if (u == p) continue;  
        if (vis[u]) {  
          dp[v] = std::min(dp[v], dp[u]); // 会更小。  
        } else {  
          dfs(u, v);  
          if (dp[u] > tin[v]) ans.push_back({u, v});  
          dp[v] = std::min(dp[v], dp[u]);  
        }  
      }  
    }; dfs(0, -1);  
    return ans;  
  }  
};
```

# DeterminantMod (Gaussian Method) （高斯消元解行列式O(n³)）
```cpp
// Problem: G - Spanning Tree
// based on heno239's Solution
// https://atcoder.jp/contests/jsc2021/submissions/21819246
// @require Expmod code
int expmod(long long a, long long b, int MOD) {  
  if (b == 0) return 1LL;  
  int c = expmod(a * a % MOD, b / 2, MOD);  
  if (b % 2) return a * c % MOD;  
  return c;  
}  
int detmod(std::vector<std::vector<long long>> mat, int MOD) {  
  int n = mat.size();  
  long long ans = 1;  
  for (int i = 0; i < n; i++) {  
    int pivot = i;  
    for (int j = i + 1; j < n; j++) {  
      if (abs(mat[pivot][i]) < abs(mat[j][i])) pivot = j;  
    }  
    if (mat[pivot][i] == 0) return 0;  
  
    if (pivot != i) ans = (-ans + MOD) % MOD;  
    swap(mat[pivot], mat[i]);  
  
    ans = ans * mat[i][i] % MOD;  
  
    int precalcINV = expmod(mat[i][i], MOD - 2, MOD);  
    for (int j = i + 1; j < n; j++) {  
      for (int k = n - 1; k >= i; k--) {  
        mat[j][k] = (mat[j][k] - mat[i][k] * (mat[j][i] * precalcINV % MOD) % MOD) % MOD;  
        if (mat[j][k] < 0) mat[j][k] += MOD;  
      }  
    }  
  }  
  return ans;  
}
```
## subTopic: Kirchhoff's matrix tree theorem（矩阵树定理）
![Kirchhoff's_matrix-tree-theorem.png](medias%2FKirchhoff%27s_matrix-tree-theorem.png)

# Digit DP（处理数位型DP的常用方式）
```cpp
// Example
// calculate the number of x :
// num1 <= x <= num2
// min_sum <= digit_sum(x) <= max_sum
class Solution {  
public:  
  int digitDPCodeSnippet(std::string num1, std::string num2, int min_sum, int max_sum) {  
    int sz1 = num1.size(), sz2 = num2.size();  
    num1 = std::string(sz2-sz1, '0')+num1;  
    std::vector<std::vector<int>> dp(sz2, std::vector<int>(sz2*9+1, -1));  
    const int MOD = 1e9+7;  
    auto dfs = [&] (auto&& self, int i, int sum, bool lo_limit, bool hi_limit) -> int {  
      if (i == sz2 && min_sum <= sum && sum <= max_sum) {  
        return 1;  
      }  
      if (!lo_limit && !hi_limit && dp[i][sum] != -1) return dp[i][sum];  
      int res = 0;  
      for (int d = (lo_limit ? num1[i]-'0' : 0); d <= (hi_limit ? num2[i]-'0' : 9); d++) {  
        res = res+self(self, i+1, sum+d, lo_limit && d == num1[i]-'0', hi_limit && d == num2[i]-'0');  
      }  
      if (!lo_limit && !hi_limit) dp[i][sum] = res;  
      return res;  
    };  
    return dfs(dfs, 0, 0, true, true);  
  }  
};
// Example. 2
// calculate the number of x :
// 1 <= x <= n
// digits in x are distinct (Leading zeros are not counted)
class Solution {  
public:  
  int countSpecialNumbers(int n) {  
    std::string s2 = std::to_string(n);  
    int sz = s2.size();  
    std::string s1 = std::string(sz, '0');  
    s1.back() = '1';  
    std::vector<std::vector<int>> memo(sz, std::vector<int>(1024, -1));  
    // std::cout << "s1 = " << s1 << ", s2 = " << s2 << std::endl;  
    auto dfs = [&] (auto&& self, int i, int mask, bool is_zero, bool lo_limit, bool hi_limit) -> int {  
      if (i == sz) return 1;  
      if (!lo_limit && !hi_limit && !is_zero && memo[i][mask] != -1) return memo[i][mask];  
      int lo = (lo_limit ? s1[i]-'0' : 0), hi = (hi_limit ? s2[i]-'0' : 9);  
      int res = 0;  
      for (int d = lo; d <= hi; d++) {  
        if (~mask>>d&1) {  
          res += self(self, i+1, (is_zero && d == 0 ? mask : mask|(1<<d)), is_zero && d == 0, lo_limit && d == s1[i]-'0', hi_limit && d == s2[i]-'0');  
        }  
      }  
      if (!lo_limit && !hi_limit && !is_zero) memo[i][mask] = res;  
      return res;  
    };  
    int ans = dfs(dfs, 0, 0, true, true, true);  
    return ans;  
  }  
};
```

# int128_t
```cpp
// 字面量  
__int128_t m18 = 1e18;  
__int128_t ans = 1292839283*m18+2372397;  
// 输出  
__int128_t x = ans;  
std::string buf;  
while (x) {  
buf += '0'+x%10;  
x /= 10;  
}  
std::reverse(buf.begin(), buf.end());  
std::cout << buf << '\n';
```

# Interval Xor Sum （区间内整数的异或和）
```cpp
// author: gpt4o
// editor: fsj-o
// description: calculate xor sum from `l` to `r`

template<class T> T xorFromZeroToX(T x) {

  if (x < 0) {  
    return 0; // 如果 x 为负数，返回零  
  }  
  
  // 根据 x 的模 4 值来计算从 0 到 x 的异或值  
  switch (x % 4) {  
    case 0: return x;       // 当 x % 4 == 0 时，结果为 x
    case 1: return 1;       // 当 x % 4 == 1 时，结果为 1
    case 2: return x + 1;   // 当 x % 4 == 2 时，结果为 x + 1
    case 3: return 0;       // 当 x % 4 == 3 时，结果为 0
  }  
  
  assert(false); // 不会到达这里  
}

// 计算 [l, l+1, ... r] 的异或值  
template<class T> T intervalXorSum(T l, T r) {  

  if (l > r) {  
    return 0; // 区间内没有数参与异或值的计算  
  }  
  
  return xorFromZeroToX(r) ^ xorFromZeroToX(l-1);  
}
```
# Item-Compression before 0/1-Knapsack-Problem（在01背包物品数量多并且SUM较小时，压缩背包物品数）
```cpp
void compress(std::vector<int>& a) {  
  std::priority_queue<int, std::vector<int>, std::greater<int>> pq;  
  for (int x : a) pq.emplace(x);  
  a.clear();  
  while (!pq.empty()) {  
    int x = pq.top();  
    pq.pop();  
    if (a.size() >= 2 && a[(int)a.size() - 2] == x) {  
      pq.emplace(2 * x);  
      a.pop_back();  
    } else {  
      a.push_back(x);  
    }  
  }  
}
// mainly used for Time Complexity: O(S*N) -> O(S*√S)
```

# Longest Palind-Substring（最长回文子串）
```cpp
// Problem: 5. 最长回文子串
// Based on 数据结构与算法's Solution
// https://leetcode.cn/problems/longest-palindromic-substring/solutions/614545/bao-li-zhong-xin-kuo-san-dong-tai-gui-hu-qdvv

std::vector<int> manacher(const std::string& s) {  
  int n = s.size();  
  std::string s1(2*n+1, '#');  
  for (int i = 0; i < n; i++) {  
    s1[2*i+1] = s[i];  
  }  
//   cout << "s1 = " << s1 << '\n';  
  std::vector<int> z(2*n+1, 1);  
  for (int i = 1, cen = 0; i < 2*n+1; i++) {  
    if (i <= cen+z[cen]-1) {  
      int j = cen-(i-cen); // 对称轴  
      assert(j >= 0);  
      z[i] = std::min(z[j], j-(cen-z[cen]+1)+1);  
    }  
    while (i+z[i] < 2*n+1 && i-z[i] >= 0 && s1[i+z[i]] == s1[i-z[i]]) {  
      z[i]++;  
    }  
    if (i+z[i]-1 > cen+z[cen]-1) {  
      cen = i;  
    }  
  }  
  return z;  
}  
std::string longestPalindrome(std::string s) {  
  std::vector<int> z = manacher(s);  
//   cout << "z = " << z << '\n';  
  int best = 0, n = s.size();  
  for (int i = 0; i < 2*n+1; i++) {  
    if (z[i] > z[best]) {  
      best = i;  
    }  
  }  
  int l = (best-z[best]+1), r = (best+z[best]-1); // 取出子串，先算扩展#串的l, r下标位置。  
  l /= 2; r /= 2; // 再位置各除2，就转换成原串的"[)"(左闭右开)位置。  
  return s.substr(l, r-l);  
}
```

# MinSumOfSubarrays (multiple queries)（查询不变数组LR范围内全部子数组的最小值之和，多次查询）
```cpp
// problem: G3. Yunli's Subarray Queries (extreme version)
// based on kotatsugame's Solution:
// https://codeforces.com/contest/2009/submission/279583019
// @require LazySegtree template

// 线段树维护和  
using info = std::pair<long long, int>;  
info op(info a, info b) { return info{a.first+b.first, a.second+b.second}; }  
info e() { return info{0LL, 0}; }  

// lazyTag区间加  
using F1 = long long;  
info mp1(info a, F1 x) { return info {a.first+1LL*a.second*x, a.second}; }  
F1 comp1(F1 a, F1 x) { return a+x; }  
F1 id1() { return 0LL; }  

// lazyTag2区间assign  
using F2 = int;  
info mp2(info a, F2 x) { return (x == -1 ? a : info { 1LL*a.second*x, a.second}); }  
F2 comp2(F2 a, F2 x) { return (x == -1 ? a : x); }  
F2 id2() { return -1; }  
  
std::vector<long long> minSumOfSubarrays(std::vector<int> a, std::vector<std::pair<int, int>> queries) {  
  int n = a.size(), q = queries.size();  
  std::vector<std::vector<std::pair<int, int>>> queries_by_l(n);  
  for (int i = 0; i < q; i++) {  
    auto [l, r] = queries[i];  
    queries_by_l[l].push_back({r, i});  
  }  
  a.push_back(*std::min_element(a.begin(), a.end())-1);  
  std::vector<int> stk {n};  
  LazySegtree<info, op, e, F1, mp1, comp1, id1> T1(n, info{0LL, 1});  
  LazySegtree<info, op, e, F2, mp2, comp2, id2> T0(n, info(0LL, 1));  
  std::vector<long long> ans(q);  
  for (int l = n-1; l >= 0; l--) {  
    while (a[stk.back()] >= a[l]) {  
      int lo = stk.back();  
      stk.pop_back();  
      int hi = stk.back()-1;  
      T1.modify(lo, hi, -1LL*l*a[lo]);  
    }  
    int hi = stk.back()-1;  
    stk.push_back(l);  
    T1.modify(l, hi, 1LL*l*a[l]);  
    T0.modify(l, hi, a[l]);  
    for (auto [r, i] : queries_by_l[l]) {  
      long long res1 = T1.query(l, r).first;  
      long long res0 = T0.query(l, r).first;  
      ans[i] = res1-1LL*(l-1)*res0;  
    }  
  }  
  return ans;  
}
```

# Persistent Segtree（可持久化线段树）
```cpp
template <class S, class info, info (*op)(info, info), info (*e)()>  
struct PersistentSegtree {  
  struct node {  
    info val = e();  
    int lc = 0;  
    int rc = 0;  
  };  
  std::vector<node> t {node{}};  
  int root = 0;  
  const S L, R;  
  PersistentSegtree(S _L, S _R): L(_L), R(_R) {}  
  int modify(int p, S tl, S tr, S pos, info x) {  
    int v = t.size();  
    t.push_back(node{});  
    if (tl == tr) {  
      t[v].val = op(t[p].val, x); // 非assign  
      return v;  
    }  
    S tm = tl+(tr-tl)/2;  
    if (pos <= tm) {  
      t[v].lc = modify(t[p].lc, tl, tm, pos, x);  
      t[v].rc = t[p].rc;  
    } else {  
      t[v].lc = t[p].lc;  
      t[v].rc = modify(t[p].rc, tm+1, tr, pos, x);  
    }  
    t[v].val = op(t[t[v].lc].val, t[t[v].rc].val);  
    return v;  
  }  
  info query(int p, S tl, S tr, S l, S r) {  
    if (!p) return e();  
    if (r < tl || tr < l) return e();  
    if (tl == l && tr == r) return t[p].val;  
    S tm = tl+(tr-tl)/2;  
    return op(query(t[p].lc, tl, tm, std::max(tl, l), std::min(tm, r)),  
              query(t[p].rc, tm+1, tr, std::max(tm+1, l), std::min(tr, r)));  
  }  
  void modify(S pos, info X) {  
    root = modify(root, L, R, pos, X);  
  }  
  info query(int l, int r) {  
    return query(root, L, R, l, r);  
  }  
};
// Example  
struct info {  
  int key = -1;  
  int val = -1;  
};  
info op(info a, info b) {  
  if (a.val > b.val) return a;  
  if (a.val < b.val) return b;  
  return a; // 返回任意一个  
}  
info e() {  
  return info{};  
}
// 可能存在常数效率问题，请在需要时进行结点数估算。
```
# Quadratic Equation（一元二次方程的整数解）
```cpp
tuple<long long, long long, bool> quadratic_equation(long long a, long long b, long long c) {  
  long long disc = b * b - 4 * a * c; // discriminant  
  if (disc < 0) return {-1, -1, false};  
  long long z = sqrt(disc);  
  while (z * z < disc) z++;  
  while (z * z > disc) z--;  
  if (z * z != disc) return {-1, -1, false};  
  long long r1 = (-b + z) / (2 * a);  
  long long r2 = (-b - z) / (2 * a);  
  return {r1, r2, true};  
}
// quadratic_equation(a, b, c) 解整数解，返回元组 -> {r1, r2, state}，state为true才使用r1，r2的值，否则方程无整数解。
```

# SuffixAutomaton（后缀自动机）
```cpp
// https://cp-algorithms.com/string/suffix-automaton.html
struct sam {  
  struct state {  
    int len;  
    int link;  
    std::map<int, int> next;  
  };  
  std::vector<state> st{state{0, -1, {}}};  
  int last = 0;  
  int sam_extend(int p, int c) {  
    int cur = st.size();  
    st.push_back(state{st[p].len + 1, -1, {}});  
    while (p != -1 && !st[p].next.count(c)) {  
      st[p].next[c] = cur;  
      p = st[p].link;  
    }  
    if (p == -1) {  
      st[cur].link = 0;  
    } else {  
      int q = st[p].next[c];  
      if (st[p].len + 1 == st[q].len) {  
        st[cur].link = q;  
      } else {  
        int clone = st.size();  
        st.push_back(state{st[p].len + 1, st[q].link, st[q].next});  
        while (p != -1 && st[p].next[c] == q) {  
          st[p].next[c] = clone;  
          p = st[p].link;  
        }  
        st[q].link = st[cur].link = clone;  
      }  
    }  
    return last = cur;  
  }  
  sam() = default;  
  sam(const std::string& s) {  
    for (int c : s) {  
      sam_extend(last, c);  
    }  
  }  
  // 返回后缀链接树的拓扑序  
  std::vector<int> topo() {  
    std::vector<int> cnt(st[last].len + 1), ord(st.size());  
    for (int i = 0; i < st.size(); i++) cnt[st[i].len]++;  
    for (int i = st[last].len; i >= 1; i--) cnt[i - 1] += cnt[i];  
    for (int i = 0; i < st.size(); i++) ord[--cnt[st[i].len]] = i;  
    return ord;  
  }  
};
```
## subTopic: std::map<int, int> to std::array<int, 26>
```cpp
template<typename Key, typename Value> struct map {  
  std::array<Value, 26> arr {};  
  Value &operator[](Key c) { return arr[c - 'a']; }  
  bool count(Key c) { return arr[c - 'a'] != Value(); }  
  struct iterator {  
    typename std::array<Value, 26>::iterator it;  
    map *m;  
    bool operator!=(const iterator& rhs) const { return it != rhs.it; }  
    iterator& operator++() { do {++it;} while (it != m->arr.end() && *it == Value()); return *this; }  
    std::pair<Key, Value> operator*() const { return {Key('a' + it - m->arr.begin()), *it}; }  
  };  
  iterator begin() { iterator it = {arr.begin(), this}; while ((*it).second == Value() && it != this->end()) { ++it; } return it; }  
  iterator end() { return {arr.end(), this}; };  
};
```

