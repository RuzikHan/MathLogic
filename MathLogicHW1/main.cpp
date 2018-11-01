#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

string axioms[10] = {"(->,A,(->,B,A))", "(->,(->,A,B),(->,(->,A,(->,B,C)),(->,A,C)))", "(->,A,(->,B,(&,A,B)))",
                     "(->,(&,A,B),A)", "(->,(&,A,B),B)", "(->,A,(|,A,B))", "(->,B,(|,A,B))",
                     "(->,(->,A,C),(->,(->,B,C),(->,(|,A,B),C)))", "(->,(->,A,B),(->,(->,A,(!B)),(!A)))",
                     "(->,(!(!A)),A)"};

vector<string> lines, expressionlines;

map<string, int> assumptions;

struct node {
    string value;
    node *left;
    node *right;
};

vector<node *> axioms_trees(10), lines_trees;

string checkline(string s) {
    long long j = 0;
    while (j < s.length()) {
        if (s[j] == ' ' || s[j] == '\t') {
            s.erase(j, 1);
            j--;
        }
        j++;
    }
    return s;
}

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

void create_expression_tree() {
    for (int i = 0; i < lines.size(); i++) {
        node *line = new node;
        expressionlines.push_back(expressionParser(lines[i]));
        add(expressionlines[i], line);
        lines_trees.push_back(line);
    }
}

void create_axioms_tree() {
    for (int i = 0; i < 10; i++) {
        node *axioma = new node;
        add(axioms[i], axioma);
        axioms_trees[i] = axioma;
    }
}

node *a;
node *b;
node *c;

bool compare_trees(node *tree1, node *tree2) {
    if (tree1 == nullptr && tree2 == nullptr) return true;
    if ((tree1 == nullptr && tree2 != nullptr) || (tree1 != nullptr && tree2 == nullptr)) return false;
    if (tree1->value == tree2->value)
        return compare_trees(tree1->left, tree2->left) & compare_trees(tree1->right, tree2->right);
    else return false;
}

bool axiom_search(node *tree, node *axiom_tree) {
    if (axiom_tree == nullptr && tree == nullptr) return true;
    if ((axiom_tree == nullptr && tree != nullptr) || (axiom_tree != nullptr && tree == nullptr)) return false;
    if (axiom_tree->value == "A") {
        if (a == nullptr) {
            a = new node;
            a->value = tree->value;
            a->left = tree->left;
            a->right = tree->right;
            return true;
        } else {
            return compare_trees(a, tree);
        }
    } else if (axiom_tree->value == "B") {
        if (b == nullptr) {
            b = new node;
            b->value = tree->value;
            b->left = tree->left;
            b->right = tree->right;
            return true;
        } else {
            return compare_trees(b, tree);
        }
    } else if (axiom_tree->value == "C") {
        if (c == nullptr) {
            c = new node;
            c->value = tree->value;
            c->left = tree->left;
            c->right = tree->right;
            return true;
        } else {
            return compare_trees(c, tree);
        }
    } else if ((axiom_tree->value == "&" || axiom_tree->value == "|" || axiom_tree->value == "->") &&
               (axiom_tree->value == tree->value)) {
        return axiom_search(tree->left, axiom_tree->left) & axiom_search(tree->right, axiom_tree->right);
    } else if (axiom_tree->value == tree->value) {
        return axiom_search(tree->left, axiom_tree->left);
    } else return false;
}

int axioms_check(int cur) {
    node *tmp = lines_trees[cur];
    for (int i = 0; i < 10; i++) {
        node *axiom_tmp = axioms_trees[i];
        a = nullptr;
        b = nullptr;
        c = nullptr;
        if (axiom_search(tmp, axiom_tmp)) {
            free(a);
            free(b);
            free(c);
            return i + 1;
        }
        free(a);
        free(b);
        free(c);
    }
    return -1;
}

map<string, vector<int>> right_part;
map<string, int> all_part;
map<int, string> left_part;

int assump_count = 1;

void mp_add(string s, int ind) {
    if (s[0] == '(') {
        s.erase(0, 1);
        s.erase(s.length() - 1, 1);
    }
    if (s[0] == '-') {
        int start = 3;
        int brackets_count = 0;
        string left_s = "", right_s = "";
        bool part = false;
        for (int i = start; i < s.length(); i++) {
            if (s[i] == '(') brackets_count++;
            else if (s[i] == ')') brackets_count--;
            if (brackets_count == 0 && s[i] == ',') part = true;
            else if (!part) left_s += s[i]; else right_s += s[i];
        }
        right_part[right_s].push_back(ind);
        left_part[ind] = left_s;
    }
}

void get_annotation(int cur) {
    string ss = expressionlines[cur];
    if (assumptions.count(ss) > 0) {
        cout << " (Предп. " << assump_count << ")\n";
        assump_count++;
        all_part[ss] = cur;
        mp_add(ss, cur);
        return;
    }
    int axioms_ans = axioms_check(cur);
    if (axioms_ans != -1) {
        cout << " (Сх. акс. " << axioms_ans << ")\n";
        all_part[ss] = cur;
        mp_add(ss, cur);
        return;
    }
    if (right_part.count(ss) > 0) {
        for (int i = 0; i < right_part[ss].size(); i++) {
            int first = right_part[ss][i];
            if (left_part.count(first) > 0) {
                string left_s = left_part[first];
                if (all_part.count(left_s) > 0) {
                    int second = all_part[left_s];
                    cout << " (M.P. " << first + 1 << ", " << second + 1 << ")\n";
                    all_part[ss] = cur;
                    mp_add(ss, cur);
                    return;
                }
            }
        }
    }
    all_part[ss] = cur;
    mp_add(ss, cur);
    cout << " (Не доказано)\n";
}

int main() {
    std::ios::sync_with_stdio(false);
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    string start_s;
    create_axioms_tree();
    getline(cin, start_s);
    long long j = 0;
    start_s = checkline(start_s);
    string s;
    while (true) {
        if (start_s[j] == ',') {
            string assumption = start_s.substr(0, j);
            start_s.erase(0, j + 1);
            if (assumption.length() > 0) assumptions[expressionParser(assumption)] = assumptions.size();
            j = -1;
        } else if (start_s[j] == '|' && start_s[j + 1] == '-') {
            string assumption = start_s.substr(0, j);
            if (assumption.length() > 0) assumptions[expressionParser(assumption)] = assumptions.size();
            break;
        }
        j++;
    }
    while (getline(cin, s)) {
        s = checkline(s);
        if (s.length() > 0) lines.push_back(s);
    }
    create_expression_tree();
    for (int i = 0; i < lines.size(); i++) {
        cout << "(" << i + 1 << ") " << lines[i];
        get_annotation(i);
    }
    return 0;
}