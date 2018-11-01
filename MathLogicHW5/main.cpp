#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

string start_line;

string psp[14] = {"(((())))", "((()()))", "((())())", "((()))()", "(()(()))", "(()()())", "(()())()", "(())(())",
                  "(())()()", "()((()))", "()(()())", "()(())()", "()()(())", "()()()()"};

map<char, int> variables;
map<int, char> invert_variables;

struct node {
    string value;
    set<string> lines;
    int num;
    node *left = NULL;
    node *right = NULL;
    node *parent = NULL;
};

string expressionParser(string);

string negationParser(string s) {
    if (s.length() == 0) return "";
    if (s[0] == '(')
        return expressionParser(s.substr(1, s.length() - 2));
    if (s[0] == '!') return "(!" + negationParser(s.substr(1)) + ")"; else return s;
}

string conjunctionParser(string s) {
    int brackets_count = 0;
    if (s.length() == 0) return "";
    for (int i = s.length() - 1; i >= 0; i--)
        if (s[i] == '(') brackets_count++;
        else if (s[i] == ')') brackets_count--;
        else if (brackets_count == 0 && s[i] == '&') {
            return "(&," + conjunctionParser(s.substr(0, i)) + "," + negationParser(s.substr(i + 1)) + ")";
        }
    return negationParser(s);
}

string disjunctionParser(string s) {
    int brackets_count = 0;
    if (s.length() == 0) return "";
    for (int i = s.length() - 1; i >= 0; i--)
        if (s[i] == '(') brackets_count++;
        else if (s[i] == ')') brackets_count--;
        else if (brackets_count == 0 && s[i] == '|')
            return "(|," + disjunctionParser(s.substr(0, i)) + "," + conjunctionParser(s.substr(i + 1)) + ")";
    return conjunctionParser(s);
}

string expressionParser(string s) {
    int brackets_count = 0;
    if (s.length() == 0) return "";
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '(') brackets_count++;
        else if (s[i] == ')') brackets_count--;
        else if (brackets_count == 0 && s[i] == '-' && i < s.length() - 1 && s[i + 1] == '>')
            return "(->," + disjunctionParser(s.substr(0, i)) + "," + expressionParser(s.substr(i + 2)) + ")";
    }
    return disjunctionParser(s);
}


void add(string s, node *tree) {
    if (s[0] == '(') {
        s.erase(0, 1);
        s.erase(s.length() - 1, 1);
    }
    int start = 0;
    int brackets_count = 0;
    string left_s = "", right_s = "", val = "";
    bool part = false;
    if (s[0] == '-') {
        start = 3;
        val = "->";
    } else {
        start = 2;
        val = s[0];
    }
    for (int i = start; i < s.length(); i++) {
        if (s[i] == '(') brackets_count++;
        else if (s[i] == ')') brackets_count--;
        if (brackets_count == 0 && s[i] == ',') part = true;
        else if (!part) left_s += s[i]; else right_s += s[i];
    }
    tree->left = new node;
    tree->right = new node;
    if (val == "&" || val == "|" || val == "->") {
        tree->value = val;
        add(left_s, tree->left);
        add(right_s, tree->right);
    } else if (val == "!") {
        tree->value = val;
        add(s.substr(1, s.length() - 1), tree->left);
        tree->right = nullptr;
    } else {
        tree->value = val;
        tree->left = nullptr;
        tree->right = nullptr;
    }
}

void find_variable(string s) {
    int count = 1;
    for (int i = 0; i < s.length(); i++) {
        auto checkcode = int(s[i]);
        if (checkcode >= 65 && checkcode <= 90) {
            if (variables.count(s[i]) == 0) {
                variables[s[i]] = variables.size();
                invert_variables[count] = s[i];
                count++;
            }
        }
    }
}

void create_tree(string s, node *tree, int val) {
    if (val < s.length()) {
        if (s[val] == '(') {
            tree->left = new node;
            tree->left->num = val + 1;
            tree->left->parent = tree;
            create_tree(s, tree->left, val + 1);
        } else {
            tree = tree->parent;
            while (tree->right != NULL) tree = tree->parent;
            tree->right = new node;
            tree->right->num = val + 1;
            tree->right->parent = tree;
            create_tree(s, tree->right, val + 1);
        }
    }
}

int nums[9];

bool get2next() {
    bool check = true;
    for (int i = 0; i < 9; i++)
        if (nums[i] != 1) check = false;
    if (check) return false;
    int i = 8;
    while (i >= 0 && nums[i] == 1) {
        nums[i] = 0;
        i--;
    }
    nums[i]++;
    return true;
}

bool get4next() {
    bool check = true;
    for (int i = 0; i < 9; i++)
        if (nums[i] != 2) check = false;
    if (check) return false;
    int i = 8;
    while (i >= 0 && nums[i] == 2) {
        nums[i] = 0;
        i--;
    }
    nums[i]++;
    return true;
}

bool get8next() {
    bool check = true;
    for (int i = 0; i < 9; i++)
        if (nums[i] != 3) check = false;
    if (check) return false;
    int i = 8;
    while (i >= 0 && nums[i] == 3) {
        nums[i] = 0;
        i--;
    }
    nums[i]++;
    return true;
}

void fill_tree(node *tree, string s) {
    tree->lines.insert(s);
    if (tree->left != NULL) fill_tree(tree->left, s);
    if (tree->right != NULL) fill_tree(tree->right, s);
}

void go2tree(node *tree) {
    int cur_num = nums[tree->num];
    if (cur_num == 1) {
        string add_line = "";
        char c = invert_variables[1];
        add_line += c;
        tree->lines.insert(add_line);
        if (tree->left != NULL) fill_tree(tree->left, add_line);
        if (tree->right != NULL) fill_tree(tree->right, add_line);
    }
    if (tree->left != NULL) go2tree(tree->left);
    if (tree->right != NULL) go2tree(tree->right);

}

void go4tree(node *tree) {
    int cur_num = nums[tree->num];
    if (cur_num == 1) {
        string add_line = "";
        char c = invert_variables[1];
        add_line += c;
        tree->lines.insert(add_line);
        if (tree->left != NULL) fill_tree(tree->left, add_line);
        if (tree->right != NULL) fill_tree(tree->right, add_line);
    }
    if (cur_num == 2) {
        string add_line = "";
        char c = invert_variables[2];
        add_line += c;
        tree->lines.insert(add_line);
        if (tree->left != NULL) fill_tree(tree->left, add_line);
        if (tree->right != NULL) fill_tree(tree->right, add_line);
    }
    if (tree->left != NULL) go4tree(tree->left);
    if (tree->right != NULL) go4tree(tree->right);
}

void go8tree(node *tree) {
    int cur_num = nums[tree->num];
    if (cur_num == 1) {
        string add_line = "";
        char c = invert_variables[1];
        add_line += c;
        tree->lines.insert(add_line);
        if (tree->left != NULL) fill_tree(tree->left, add_line);
        if (tree->right != NULL) fill_tree(tree->right, add_line);
    }
    if (cur_num == 2) {
        string add_line = "";
        char c = invert_variables[2];
        add_line += c;
        tree->lines.insert(add_line);
        if (tree->left != NULL) fill_tree(tree->left, add_line);
        if (tree->right != NULL) fill_tree(tree->right, add_line);
    }
    if (cur_num == 3) {
        string add_line = "";
        char c = invert_variables[3];
        add_line += c;
        tree->lines.insert(add_line);
        if (tree->left != NULL) fill_tree(tree->left, add_line);
        if (tree->right != NULL) fill_tree(tree->right, add_line);
    }
    if (tree->left != NULL) go8tree(tree->left);
    if (tree->right != NULL) go8tree(tree->right);
}

void clear_sets(node *tree) {
    tree->lines.clear();
    if (tree->left != NULL) clear_sets(tree->left);
    if (tree->right != NULL) clear_sets(tree->right);
}

void check_or(node *tree, string line, string left, string right) {
    if (tree->lines.count(expressionParser(left)) || tree->lines.count(expressionParser(right)))
        tree->lines.insert(expressionParser(line));
    if (tree->left != NULL) check_or(tree->left, line, left, right);
    if (tree->right != NULL) check_or(tree->right, line, left, right);
}

void check_and(node *tree, string line, string left, string right) {
    if (tree->lines.count(expressionParser(left)) && tree->lines.count(expressionParser(right)))
        tree->lines.insert(expressionParser(line));
    if (tree->left != NULL) check_and(tree->left, line, left, right);
    if (tree->right != NULL) check_and(tree->right, line, left, right);
}

bool impl_f = true;

void down_check_impl(node *tree, string line, string left, string right) {
    if (tree->lines.count(expressionParser(left)) && !tree->lines.count(expressionParser(right))) impl_f = false;
    if (tree->left != NULL) down_check_impl(tree->left, line, left, right);
    if (tree->right != NULL) down_check_impl(tree->right, line, left, right);
}

void check_impl(node *tree, string line, string left, string right) {
    if (tree->lines.count(expressionParser(left)) && !tree->lines.count(expressionParser(right))) impl_f = false;
    if (tree->left != NULL) down_check_impl(tree->left, line, left, right);
    if (tree->right != NULL) down_check_impl(tree->right, line, left, right);
    if (impl_f) tree->lines.insert(expressionParser(line));
}

void impl(node *tree, string line, string left, string right) {
    impl_f = true;
    check_impl(tree, line, left, right);
    if (tree->left != NULL) impl(tree->left, line, left, right);
    if (tree->right != NULL) impl(tree->right, line, left, right);
}

bool neg_f = true;

void down_check_neg(node *tree, string line, string left) {
    if (tree->lines.count(expressionParser(left))) neg_f = false;
    if (tree->left != NULL) down_check_neg(tree->left, line, left);
    if (tree->right != NULL) down_check_neg(tree->right, line, left);
}

void check_neg(node *tree, string line, string left) {
    if (tree->lines.count(expressionParser(left))) neg_f = false;
    if (tree->left != NULL) down_check_neg(tree->left, line, left);
    if (tree->right != NULL) down_check_neg(tree->right, line, left);
    if (neg_f) tree->lines.insert(expressionParser(line));
}

void neg(node *tree, string line, string left) {
    neg_f = true;
    check_neg(tree, line, left);
    if (tree->left != NULL) neg(tree->left, line, left);
    if (tree->right != NULL) neg(tree->right, line, left);
}

void start_solve(node *tree, node *kr_tree) {
    if (tree->value == "|" || tree->value == "&" || tree->value == "->") {
        if (tree->left != NULL) start_solve(tree->left, kr_tree);
        if (tree->right != NULL) start_solve(tree->right, kr_tree);
        string new_val = "(" + tree->left->value + ")" + tree->value + "(" + tree->right->value + ")";
        if (tree->value == "|") check_or(kr_tree, new_val, tree->left->value, tree->right->value);
        if (tree->value == "&") check_and(kr_tree, new_val, tree->left->value, tree->right->value);
        if (tree->value == "->") impl(kr_tree, new_val, tree->left->value, tree->right->value);
        tree->value = new_val;
    } else if (tree->value == "!") {
        if (tree->left != NULL) start_solve(tree->left, kr_tree);
        string new_val = "!(" + tree->left->value + ")";
        neg(kr_tree, new_val, tree->left->value);
        tree->value = new_val;
    }
}

bool find_f = true;

void ready_check(node *tree, string line) {
    if (!tree->lines.count(expressionParser(line))) find_f = false;
    if (tree->left != NULL) ready_check(tree->left, line);
    if (tree->right != NULL) ready_check(tree->right, line);
}

vector<set<int>> first_top;

set<int> test_set;
set<set<int>> sec_top;

void run_set(node *tree) {
    test_set.insert(tree->num);
    if (tree->left != NULL) run_set(tree->left);
    if (tree->right != NULL) run_set(tree->right);
}

void create_set(node *tree) {
    test_set.clear();
    run_set(tree);
    first_top.push_back(test_set);
    if (tree->left != NULL) create_set(tree->left);
    if (tree->right != NULL) create_set(tree->right);
}


int ans_nums[10];

bool getnextnum() {
    bool check = true;
    for (int i = 0; i < 10; i++)
        if (ans_nums[i] != 1) check = false;
    if (check) return false;
    int i = 9;
    while (i >= 0 && ans_nums[i] == 1) {
        ans_nums[i] = 0;
        i--;
    }
    ans_nums[i]++;
    return true;
}

set<int> variable_set1, variable_set2, variable_set3;

void create_answer(node *tree) {
    create_set(tree);
    set<int> empty_set;
    first_top.push_back(empty_set);
    for (int i = 0; i < 10; i++)
        ans_nums[i] = 0;
    bool f = true;
    while (f) {
        set<int> val_set;
        for (int i = 0; i < 10; i++)
            if (ans_nums[i] == 1) {
                for (auto k: first_top[i])
                    val_set.insert(k);
            }
        sec_top.insert(val_set);
        f = getnextnum();
    }
    vector<vector<int>> bounds;
    bounds.resize(sec_top.size());
    int i = 0;
    for (auto set1 : sec_top) {
        int j = 0;
        for (auto set2 : sec_top) {
            if (i == j) bounds[i].push_back(j + 1);
            else if (i < j) {
                bool f1 = true;
                for (auto k : set1)
                    if (!set2.count(k)) f1 = false;
                if (f1) bounds[i].push_back(j + 1);
                else {
                    f1 = true;
                    for (auto k: set2)
                        if (!set1.count(k)) f1 = false;
                    if (f1) bounds[j].push_back(i + 1);
                }
            }
            j++;
        }
        i++;
    }
    cout << sec_top.size() << endl;
    for (int i = 0; i < bounds.size(); i++) {
        for (int j = 0; j < bounds[i].size(); j++) {
            cout << bounds[i][j] << " ";
        }
        cout << endl;
    }
    switch (variables.size()) {
        case 1: {
            int count = 1;
            for (auto k: sec_top) {
                if (k == variable_set1) {
                    cout << invert_variables[1] << "=" << count;
                    exit(0);
                }
                count++;
            }
            break;
        }
        case 2: {
            int count = 1;
            for (auto k: sec_top) {
                if (k == variable_set1) {
                    cout << invert_variables[1] << "=" << count << ",";
                    break;
                }
                count++;
            }
            count = 1;
            for (auto k: sec_top) {
                if (k == variable_set2) {
                    cout << invert_variables[2] << "=" << count;
                    exit(0);
                }
                count++;
            }
            break;
        }
        case 3: {
            int count = 1;
            for (auto k: sec_top) {
                if (k == variable_set1) {
                    cout << invert_variables[1] << "=" << count << ",";
                    break;
                }
                count++;
            }
            count = 1;
            for (auto k: sec_top) {
                if (k == variable_set2) {
                    cout << invert_variables[2] << "=" << count << ",";
                    break;
                }
                count++;
            }
            count = 1;
            for (auto k: sec_top) {
                if (k == variable_set3) {
                    cout << invert_variables[3] << "=" << count;
                    exit(0);
                }
                count++;
            }
            break;
        }
    }
}

void finalgo1(node *tree) {
    string line = "";
    char c = invert_variables[1];
    line += c;
    if (tree->lines.count(line)) variable_set1.insert(tree->num);
    if (tree->left != NULL) finalgo1(tree->left);
    if (tree->right != NULL) finalgo1(tree->right);
}

void finalgo2(node *tree) {
    string line = "";
    char c = invert_variables[2];
    line += c;
    if (tree->lines.count(line)) variable_set2.insert(tree->num);
    if (tree->left != NULL) finalgo2(tree->left);
    if (tree->right != NULL) finalgo2(tree->right);
}

void finalgo3(node *tree) {
    string line = "";
    char c = invert_variables[3];
    line += c;
    if (tree->lines.count(line)) variable_set3.insert(tree->num);
    if (tree->left != NULL) finalgo3(tree->left);
    if (tree->right != NULL) finalgo3(tree->right);
}

void solution() {
    for (int i = 0; i < 14; i++) {
        string s = psp[i];
        node *cur_tree = new node;
        cur_tree->num = 0;
        create_tree(s, cur_tree, 0);
        for (int j = 0; j < 9; j++)
            nums[j] = 0;
        switch (variables.size()) {
            case 1: {
                bool f = true;
                while (f) {
                    go2tree(cur_tree);
                    node *input_tree = new node;
                    add(expressionParser(start_line), input_tree);
                    start_solve(input_tree, cur_tree);
                    find_f = true;
                    ready_check(cur_tree, start_line);
                    if (!find_f) {
                        finalgo1(cur_tree);
                        create_answer(cur_tree);
                    }
                    clear_sets(cur_tree);
                    f = get2next();
                }
                break;
            }
            case 2: {
                bool f = true;
                while (f) {
                    go4tree(cur_tree);
                    node *input_tree = new node;
                    add(expressionParser(start_line), input_tree);
                    start_solve(input_tree, cur_tree);
                    find_f = true;
                    ready_check(cur_tree, start_line);
                    if (!find_f) {
                        finalgo1(cur_tree);
                        finalgo2(cur_tree);
                        create_answer(cur_tree);
                    }
                    clear_sets(cur_tree);
                    f = get4next();
                }
                break;
            }
            case 3: {
                bool f = true;
                while (f) {
                    go8tree(cur_tree);
                    node *input_tree = new node;
                    add(expressionParser(start_line), input_tree);
                    start_solve(input_tree, cur_tree);
                    find_f = true;
                    ready_check(cur_tree, start_line);
                    if (!find_f) {
                        finalgo1(cur_tree);
                        finalgo2(cur_tree);
                        finalgo3(cur_tree);
                        create_answer(cur_tree);
                    }
                    clear_sets(cur_tree);
                    f = get8next();
                }
                break;
            }
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    getline(cin, start_line);
    find_variable(start_line);
    solution();
    cout << "Формула общезначима";
    return 0;
}