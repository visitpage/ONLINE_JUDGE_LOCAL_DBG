# cp技巧汇总
1. 该文件仅供书写，不可读取，我希望在不允许复制粘贴的编码环境中，能够快速输入模板。
2. 该文件修剪模板，将我原有的（那一些允许复制粘贴使用）的模板必要的代码剪出。
3. 接下来，我将充分的练习这些代码默写，以实现快速且准确地记忆。
# MOD （模运算）
### expMOD （快速幂）
```cpp
int expMOD(int64_t a, int64_t b, int MOD) {  
  a %= MOD;  
  int64_t c = 1;  
  while (b) {  
    if (b&1) c = c*a%MOD;  
    a = a*a%MOD;  
    b >>= 1;  
  }  
  return (int)c;  
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
### Binomial（二项式系数）
```cpp
template<class T> struct Binomial {  
  vector<T> factorial;  
  vector<T> inverseFactorial;  
  Binomial(int n): factorial(n+1), inverseFactorial(n+1) {  
    factorial[0] = 1;  
    for (int i = 1; i <= n; i++) factorial[i] = factorial[i-1]*i;  
    inverseFactorial[n] = expMOD(factorial[n].x, MOD-2, MOD);  
    for (int i = n; i >= 1; i--) inverseFactorial[i-1] = inverseFactorial[i]*i;  
  }  
  mint binom(int n, int m) {  
    return factorial[n]*inverseFactorial[m]*inverseFactorial[n-m];  
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

### LazySegtree（模板化参考AC-library）
```cpp
template<class T, T(*op)(T, T), T(*e)(),
        class P, T(*mapping)(T, P), P(*composition)(P, P), P(*id)()> struct LazySegtree {
  int n;
  vector<T> t;
  vector<P> lazy;
  LazySegtree(vector<T> a): n(a.size()), t(4*n), lazy(4*n, P()) { }
  void apply(int v, P x) {
    t[v] = mapping(t[v], x);
    lazy[v] = composition(lazy[v], x);
  }
  void push(int v) {
    if (lazy[v] == id()) return;
    apply(2*v, lazy[v]);
    apply(2*v+1, lazy[v]);
    lazy[v] = id();
  }
  T modifyQuery(int v, int tl, int tr, int l, int r, P x) {
    if (r < tl || tr < l) {
      return e();
    } else if (l == tl && tr == r) {
      return apply(v, x), t[v];
    } else {
      push(v);
      int tm = (tl+tr)/2;
      T lResult = modifyQuery(2*v, tl, tm, max(l, tl), min(r, tm), x);
      T rResult = modifyQuery(2*v+1, tm+1, tr, max(l, tm+1), min(r, tr), x);
      return t[v] = op(t[2*v], t[2*v+1]), op(lResult, rResult);
    }
  }
  T modify(int l, int r, P x = id()) { modifyQuery(1, 0, n-1, l, r, x); }
  T query(int l, int r, P x = id()) { modifyQuery(1, 0, n-1, l, r, x); }
};
```