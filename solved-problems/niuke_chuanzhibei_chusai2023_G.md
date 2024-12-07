# 题目链接：https://ac.nowcoder.com/acm/contest/71300/G

本题TAG（不重要）：bfs/dfs，树形DP。

    fsj-o：
        背景，我在本题的调试中遇到了两年不见的老BUG，这让我想起了，这两年遇到了不胜数的让我连续数个小时DEBUG的情况，有些BUG堪称传说级，我打算将它们收集记录。
    
    本BUG信息：
        BUG名: 试图对STL容器刻舟求剑
        BUG触发条件：Ⅰ 在函数中创建引用变量引用STL容器的内存，
                    Ⅱ 接着使用了STL容器提供的有可能会使它发生扩容的方法（如push_back()，push()）后，
                    Ⅲ 随后继续使用了刚才创建的引用变量。
        BUG危险程度：本BUG随着你对STL容器的信任程度的升级变得难以发现。
                     通常在解决方案的其他所有BUG都自己认为清除后，却依然无法理解代码行为时，通过插入大量的打印来意识到并解决此BUG，
                     有时不幸的是，会有题目给出的测试样例并不足以达成本BUG的触发条件，这时一定会对选手的DEBUG水准进一步造成了一定的挑战。
        解决方案: 1. 更改代码习惯，非必要不取引用，取引用是否**必要**取决于它是否对解决方案的时间复杂度造成切实的影响。
                  2. （不推荐）流程化本BUG的检查，每当DEBUG失望时进行一次流程化的检查，对于本BUG主要检查解决方案是否存在触发本BUG的条件。
                  3. 目前已知本BUG只在两个常用代码（堆优化dijkstra；vector形bfs）处出现，在这两种代码处多考虑引用变量是否可取也许也足够了。

# 本BUG已发现的出现位置
    位置1：vector形式bfs、出队时
        错误示例（代码块）：
        ```cpp
            // 下述代码展示对一颗无向树进行vector形式的bfs
            // 假设已定义邻接表 vector<vector<int>> G(n); 
            // 代码将从树的结点0处开始进行bfs。
        
            vector<pair<int, int>> q {{0, -1}}; // bfs 队列
            for (int i = 0; i < (int)q.size(); i++) {
                auto &[v, p] = a[i]; // 创建引用变量
                for (int u : G[v]) {
                    if (u == p) continue;
                    colors[u] = (pairId[v] == pairId[u] ? colors[v] : (colors[v] == 'R' ? 'B' : 'R')); // 某题的维护
                    a.push_back({u, v}); // STL容器进行可能会使其扩容的操作
                }
                // 代码展示树的bfs，所以此处for (int u : G[v]) 这个循环很可能操作不止一轮，如果循环时由于push_back使bfs队列发生了扩容，循环外创建的对当前队首的引用变量 &[v, p] 就会是危险指针，下一轮循环继续操作危险指针就会产生未定义行为。
            }
        ```

        正确做法：取队首时不取引用

    位置2：堆优化dijkstra、取堆顶时
        描述：这是刻舟求剑BUG的另一个经典的出现位置，尝试着写一个堆优化dijkstra并且用引用来对优先队列进行出队，然后写剩余的dijkstra的代码时会发现，完美达成本BUG的触发条件，硬着头皮取引用将导致严重的后果。

        正确做法： 取堆顶时不取引用

    刻舟求剑这个BUG似乎和**队列**绑定出现了，我使用std::set却从未遇到过这个BUG，原因可能是由于不像那些在函数中创建的引用变量，set的迭代器会随着set的内存调整一同更新。这令我我有些好奇，比起简单的不取引用，更加正确的做法是否和迭代器有关。

# 我试图将本BUG娘化以加强对它的印象

当对STL操作不当会引起这个BUG，关于STL的娘化形象还没有公众的定义，于是我自己想象了一遍，结合了C++和STL的一些特性进行了涂鸦。

<details>
<summary>我的个人势 STL娘(^ <) 的形象图</summary>

![STL_MONSTER_Bug-Snapshot.png](..%2Fmedias%2FSTL_MONSTER_Bug-Snapshot.png)

</details>

文字版说明：
1. STL娘的脖子处，有可以插拔头部的地方。这象征着STL最瞩目的**模板**特性、源于STL在编译时会进行**插入式**地生成代码。 
2. 她的本体是一只小怪兽藏在她厚实的内衬里面。
3. 她的内衬是非常厚重的布料，非常不透气，这象征着STL的高度封装。
4. 她有一件戴帽外套，分上下两部分，上半身是崭新的，这象征了C++保持向后兼容所形成的外表完备可靠，下半身是阑珊的，这代表了C++保持向后兼容所造成的外表也有破破烂烂。
5. 她的外套的下半身内侧，贴着许多尖牙和一张完全藏住了的大嘴，这是小怪兽伸出来呼吸和捕食用的。
6. 她很在意自己在别人眼中的印象，如果某人试图暗指着它的外套说“这不是破破烂烂吗”，小怪兽就会很生气。
7. 她在图中选择插上少女可爱的头和调制出(^ <)的表情是让人放松禁戒用的，在它的长期可靠的伪装下，有时就突然用嘴咬了一口。
8. 她的角色本身还是比较严肃的类型，这张可爱调皮形象的**快照** -> 本页记载的在“刻舟求剑”这个BUG。


# 本题AC代码：
<details>
<summary> 👇 </summary>

```cpp
#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(0); cin.tie(0);

    int n;
    cin >> n;
    vector<vector<int>> G(n);
    for (int i = 0; i < n-1; i++) {
        int v, u;
        cin >> v >> u;
        --v; --u;
        G[v].push_back(u);
        G[u].push_back(v);
    }
    auto generatePairId = [id = 0] () mutable -> int { return id++; };
    vector<int> pairId(n, -1);
    auto dfs = [&] (auto&& self, int v, int p) -> void {
        for (int u : G[v]) {
            if (u == p) continue;
            self(self, u, v);
            if (pairId[u] == -1) {
                if (pairId[v] == -1) {
                    pairId[v] = pairId[u] = generatePairId();	
                } else {
                    cout << -1 << '\n';
                    exit(0);
                }
            }
        }
    };
    
    dfs(dfs, 0, -1);
    if (pairId[0] == -1) {
        cout << -1 << '\n';
        return 0;
    }
    
    string colors(n, '?');
    vector<pair<int, int>> q;
    q.push_back({0, -1}); // {结点编号，父亲编号} 
    colors[0] = 'R';
    for (int i = 0; i < (int)q.size(); i++) {
        auto [v, p] = q[i];  // 重要的是总是不对STL娘使用结构化绑定时取引用，它就几乎奈何不了我，它太优秀了。
        for (int u : G[v]) {
            if (u == p) continue;
            colors[u] = (pairId[v] == pairId[u] ? colors[v] : (colors[v] == 'R' ? 'B' : 'R'));
            q.push_back({u, v});
        }
    }
    
    cout << colors << '\n';
}
```
第一个dfs可以改成bfs吗，刚才觉得可以，现在觉得，应该，不能吧。

</details>
