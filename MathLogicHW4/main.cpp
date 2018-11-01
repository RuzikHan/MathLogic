#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<vector<int>> e, g;

int n, impl_one, impl_zero;

vector<bool> mark;
vector<int> dp1, dp2, dpa, dpb;

void dfs(int u, int depth) {
    mark[u] = true;
    dp1[u] = depth;
    for (int j = 0; j < e[u].size(); j++) {
        int y = e[u][j];
        if (!mark[y]) {
            dfs(y, depth + 1);
        }
    }
}

void dfs2(int u, int depth) {
    mark[u] = true;
    dp2[u] = depth;
    for (int j = 0; j < e[u].size(); j++) {
        int y = e[u][j];
        if (!mark[y]) dfs2(y, depth + 1);
    }
}

void dfsa(int u, int depth) {
    mark[u] = true;
    dpa[u] = depth;
    for (int j = 0; j < e[u].size(); j++) {
        int y = e[u][j];
        if (!mark[y]) {
            dfsa(y, depth + 1);
        }
    }
}

void dfsb(int u, int depth) {
    mark[u] = true;
    dpb[u] = depth;
    for (int j = 0; j < e[u].size(); j++) {
        int y = e[u][j];
        if (!mark[y]) {
            dfsb(y, depth + 1);
        }
    }
}

int a_less_b(int x, int y) {
    if (x == y) return 1;
    for (int i = 0; i < n; i++) {
        dpa[i] = -1;
        dpb[i] = -1;
        mark[i] = false;
    }
    dfsa(x, 0);
    if (dpa[y] != -1) return 1;
    for (int i = 0; i < n; i++)
        mark[i] = false;
    dfsb(y, 0);
    if (dpb[x] != -1) return 0; else return 2;
}

int find_min(vector<int> num) {
    bool f = true;
    while (f) {
        f = false;
        for (int i = 0; i < num.size(); i++) {
            for (int j = 0; j < num.size(); j++) {
                if (i != j && num[i] != -1 && num[j] != -1) {
                    int tmp = a_less_b(num[i], num[j]);
                    if (tmp == 1) {
                        num[j] = -1;
                        f = true;
                    }
                    if (tmp == 0) {
                        num[i] = -1;
                        f = true;
                    }
                }
            }
        }
    }
    int count = 0, ans = -1;
    for (int i = 0; i < num.size(); i++) {
        if (num[i] != -1) {
            if (count != 0) return -1;
            count++;
            ans = num[i];
        }
    }
    return ans;
}

int find_max(vector<int> num) {
    bool f = true;
    while (f) {
        f = false;
        for (int i = 0; i < num.size(); i++) {
            for (int j = 0; j < num.size(); j++) {
                if (i != j && num[i] != -1 && num[j] != -1) {
                    int tmp = a_less_b(num[i], num[j]);
                    if (tmp == 1) {
                        num[i] = -1;
                        f = true;
                    }
                    if (tmp == 0) {
                        num[j] = -1;
                        f = true;
                    }
                }
            }
        }
    }
    int count = 0, ans = -1;
    for (int i = 0; i < num.size(); i++) {
        if (num[i] != -1) {
            if (count != 0) return -1;
            count++;
            ans = num[i];
        }
    }
    return ans;
}

int find_a_plus_b(int x, int y) {
    if (x == y) return x;
    for (int i = 0; i < n; i++) {
        dp1[i] = -1;
        dp2[i] = -1;
        mark[i] = false;
    }
    dfs(x, 0);
    for (int i = 0; i < n; i++)
        mark[i] = false;
    dfs2(y, 0);
    bool f = false;
    int vv = -2;
    int ans = -2;
    if (dp1[y] != -1) {
        f = true;
        vv = y;
    } else if (dp2[x] != -1) {
        f = true;
        vv = x;
    } else {
        vector<int> tmp;
        for (int k = 0; k < n; k++) {
            if (dp1[k] != -1 && dp2[k] != -1) {
                tmp.push_back(k);
            }
        }
        ans = find_min(tmp);
        if (ans != -1) {
            vv = ans;
            f = true;
        }
    }
    if (!f) return -1; else return vv;
}

void add_check() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                int pl = find_a_plus_b(i, j);
                if (pl == -1) {
                    cout << "Операция '+' не определена: " << i + 1 << "+" << j + 1;
                    exit(0);
                }
            }
        }
    }
}

void dfs3(int u, int depth) {
    mark[u] = true;
    dp1[u] = depth;
    for (int j = 0; j < g[u].size(); j++) {
        int y = g[u][j];
        if (!mark[y]) {
            dfs3(y, depth + 1);
        }
    }
}

void dfs4(int u, int depth) {
    mark[u] = true;
    dp2[u] = depth;
    for (int j = 0; j < g[u].size(); j++) {
        int y = g[u][j];
        if (!mark[y]) {
            dfs4(y, depth + 1);
        }
    }
}

int find_a_mul_b(int x, int y) {
    if (x == y) return x;
    for (int i = 0; i < n; i++) {
        dp1[i] = -1;
        dp2[i] = -1;
        mark[i] = false;
    }
    dfs3(x, 0);
    for (int i = 0; i < n; i++)
        mark[i] = false;
    dfs4(y, 0);
    bool f = false;
    int vv = -2;
    int ans = -2;
    if (dp1[y] != -1) {
        f = true;
        vv = y;
    } else if (dp2[x] != -1) {
        f = true;
        vv = x;
    } else {
        vector<int> tmp;
        for (int k = 0; k < n; k++) {
            if (dp1[k] != -1 && dp2[k] != -1) {
                tmp.push_back(k);
            }
        }
        ans = find_max(tmp);
        if (ans != -1) {
            vv = ans;
            f = true;
        }
    }
    if (!f) return -1; else return vv;
}

void mul_check() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                int ml = find_a_mul_b(i, j);
                if (ml == -1) {
                    cout << "Операция '*' не определена: " << i + 1 << "*" << j + 1;
                    exit(0);
                }
            }
        }
    }
}

void distr_check() {
    for (int a = 0; a < n; a++) {
        for (int b = 0; b < n; b++) {
            for (int c = 0; c < n; c++) {
                if (a != b && a != c && b != c) {
                    int left = find_a_mul_b(a, find_a_plus_b(b, c));
                    int right = find_a_plus_b(find_a_mul_b(a, b), find_a_mul_b(a, c));
                    if (left != right) {
                        cout << "Нарушается дистрибутивность: " << a + 1 << "*(" << b + 1 << "+" << c + 1 << ")";
                        exit(0);
                    }
                }
            }
        }
    }
}

int find_impl_one() {
    for (int i = 0; i < n; i++) {
        bool f = true;
        for (int j = 0; j < n; j++) {
            if (i != j) {
                int tmp = a_less_b(j, i);
                if (tmp != 1) {
                    f = false;
                    break;
                }
            }
        }
        if (f) return i;
    }
}

int find_impl_zero() {
    for (int i = 0; i < n; i++) {
        bool f = true;
        for (int j = 0; j < n; j++) {
            if (i != j) {
                int tmp = a_less_b(i, j);
                if (tmp != 1) {
                    f = false;
                    break;
                }
            }
        }
        if (f) return i;
    }
}

int a_impl_0(int x) {
    if (x == impl_zero) return impl_one;
    if (x == impl_one) return impl_zero;
    int ans;
    vector<int> num;
    for (int i = 0; i < n; i++) {
        int tmp = find_a_mul_b(x, i);
        if (tmp == impl_zero) {
            num.push_back(i);
        }
    }
    ans = find_max(num);
    return ans;
}

void bul_check() {
    for (int i = 0; i < n; i++) {
        int tmp = a_impl_0(i);
        if (tmp == -1) {
            cout << "Не булева алгебра: " << i + 1 << "+~" << i + 1;
            exit(0);
        }
        if (find_a_plus_b(i, tmp) != impl_one) {
            cout << "Не булева алгебра: " << i + 1 << "+~" << i + 1;
            exit(0);
        }
    }
    cout << "Булева алгебра";
}

int main() {
    std::ios::sync_with_stdio(false);
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    cin >> n;
    e.resize(n);
    g.resize(n);
    dp1.resize(n, -1);
    dp2.resize(n, -1);
    dpa.resize(n, -1);
    dpb.resize(n, -1);
    mark.resize(n, false);
    for (int i = 0; i < n; i++) {
        string s = "";
        while (s.length() == 0) {
            getline(cin, s);
        }
        int x = 0;
        for (int j = 0; j < s.length(); j++) {
            char c = s[j];
            if (c != ' ') {
                x = x * 10 + (c - '0');
            } else {
                if (x != 0) {
                    e[i].push_back(x - 1);
                    if (x - 1 != i) g[x - 1].push_back(i);
                }
                x = 0;
            }
        }
        if (x != 0) {
            e[i].push_back(x - 1);
            if (x - 1 != i) g[x - 1].push_back(i);
        }
    }
    for (int i = 0; i < n; i++)
        if (g[i].empty()) g[i].push_back(i);
    add_check();
    mul_check();
    distr_check();
    impl_one = find_impl_one();
    impl_zero = find_impl_zero();
    bul_check();
    return 0;
}