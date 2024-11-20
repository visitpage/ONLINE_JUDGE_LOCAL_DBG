1. 该文件仅供书写，不可读取，我希望在不允许复制粘贴的编码环境中，能够快速输入模板。
2. 该文件整理了模板范式，我将原有的（那一些允许复制粘贴使用）的模板进行了修剪，只保留了必要的代码。
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
