# "手写模板吧，手写模板吧，崇拜手写吧”
我希望在不允许复制粘贴的编码环境中快速输入模板，因此新建了这一个版本的Solution-codeSnippet。

1. 该文件整理了模板范式，我将原有的（那一些允许复制粘贴使用）的模板进行了修剪，只保留了必要的代码。
2. 该文件只写不读.

# MOD （模运算）
### powMOD （快速幂模）
```cpp
int64_t powMOD(int64_t a, int64_t b) {
  a %= MOD;
  int64_t c = 1;
  while (b > 0) {
    if ((b&1) == 1) c = c*a%MOD;
    a = a*a%MOD;
    b >>= 1;
  }
  return c;
}
```
### ModInt （模运算整数类）
```cpp
template<unsigned M> struct ModInt {  
  int64_t x;  
  ModInt(int64_t x = 0) : x((x % M + M) % M) {}  
  ModInt operator+(const ModInt& other) const { return ModInt(this->x + other.x); }  
  ModInt operator-(const ModInt& other) const { return ModInt(this->x - other.x); }  
  ModInt operator*(const ModInt& other) const { return ModInt(this->x * other.x); }  
  ModInt operator-() const { return ModInt(-this->x); }  
};
using mint = ModInt<MOD>;  
```
### BinomialMOD（二项式系数模）
```cpp
struct BinomialMOD {
  int n;
  vector<int64_t> factorial, inverseFactorial;
  BinomialMOD(int n): n(n), factorial(n+1), inverseFactorial(n+1) {
    factorial[0] = 1;
    for (int i = 1; i <= n; i++) factorial[i] = factorial[i-1]*i%MOD;
    inverseFactorial[n] = powMOD(factorial[n], MOD-2);
    for (int i = n; i >= 1; i--) inverseFactorial[i-1] = inverseFactorial[i]*i%MOD;
  }
  int64_t binom(int a, int k) { // 24C04：复杂的边界工作交给解决方案去解决
    assert (0 <= a && a <= n);
    assert (0 <= k && k <= n);
    assert (0 <= a-k && a-k <= n);
    return factorial[a]*inverseFactorial[k]%MOD*inverseFactorial[a-k]%MOD;
  }
};
```

# DS （数据结构）
### FenwickTree（Fenwick树）
```cpp
template<class T> struct FenwickTree {  
  int n;  
  vector<T> t;  
  FenwickTree(int n): n(n), t(n+1) {}  
  void add(int pos, T x) {  
    ++pos;  
    while (pos < n) {  
      t[pos] = t[pos]+x;  
      pos += pos&-pos;  
    }  
  }  
  T query(int pos) {  
    ++pos;  
    T result = 0;  
    while (pos > 0) {  
      result = result+t[pos];  
      pos &= pos-1;  
    }  
    return result;  
  }  
  T query(int l, int r) {  
    return query(r)-query(l-1);  
  }  
};
```
### LazySegtree（懒传播操作线段树）（模板化参考AC-library）
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

### SegmentTree（线段树）（模板化参考AC-library）
```cpp
template<class T, T(*op)(const T&, const T&), T(*e)(), class P, T(*mapping)(const T&, P), P(*id)()>
struct SegmentTree {
  int n;
  vector<T> t;
  SegmentTree(const vector<T>& a): n(1<<(__lg(a.size())+1)), t(2*n, e()) {
    copy(a.begin(), a.end(), t.begin()+n);
    for (int i = n-1; i >= 1; i--) t[i] = op(t[2*i], t[2*i+1]);
  }
  T modifyQuery(int l, int r, P x = id(), int v = 1, int tl = 0, int tr = -1) { // 修改时只支持单点改
    if (tr == -1) tr = n-1;
    if (r < tl || tr < l) return e();
    else if (l <= tl && tr <= r) {
      return t[v] = mapping(t[v], x);
    } else {
      int tm = (tl+tr)/2;
      T lResult = modifyQuery(l, r, x, 2*v, tl, tm);
      T rResult = modifyQuery(l, r, x, 2*v+1, tm+1, tr);
      return t[v] = op(t[2*v], t[2*v+1]), op(lResult, rResult);
    }
  }
};
```