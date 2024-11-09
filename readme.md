## 本仓库中一些头文件的`名称`、`功能`和`用法`说明：

## **一、"gen.h"** 
#### 1. `名称`："gen.h" 即 "generator.h"
#### 2. `功能`：生成随机测试用例（Generating Random Test Cases）
#### 3. `用法`：将以下代码放入 `main.cpp`
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
#### 然后在 gen::testcases() 的第三个参数中实现单个测试用例的工作逻辑。
#### 3.1 Example: https://codeforces.com/contest/2024/problem/C
```cpp
#ifdef LOCAL_DBG  
#include "dbg/gen.h" // https://github.com/visitpage/ONLINE_JUDGE_LOCAL_DBG/blob/main/gen.h
struct GEN {  
  GEN() {  
    gen::testcases("../input.txt", 1, [&] (ofstream& file) {  
      // wait a moment...
    });  
  }  
} GEN;  
#endif
```

## 二、"che.h"
#### 1. `名称`："che.h" 即 "checker.h"
#### 2. `功能`：检查当前解法和暴力解法输出是否一致（Check Solution Correctness）
#### 3. `用法`：将以下代码放入 `main.cpp`
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
#### 然后在两个中的一个 che::program() 的第三个参数中实现暴力解法的工作逻辑。
#### 3.1 Example: https://codeforces.com/contest/2024/problem/C
```cpp
#ifdef LOCAL_DBG
#include "dbg/che.h" // https://github.com/visitpage/ONLINE_JUDGE_LOCAL_DBG/blob/main/che.h 
struct CHE {
  CHE() {
    che::program("../input.txt", "../outputA.txt", solve).run();
    che::program("../input.txt", "../outputB.txt", [&] () {
      // wait a moment...
    }).run();
    assert(che::isContentConsistent("../outputA.txt", "../outputB.txt"));
  }
} CHE;
#endif
```