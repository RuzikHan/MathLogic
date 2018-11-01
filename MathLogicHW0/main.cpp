#include <iostream>
#include <fstream>

using namespace std;

string expressionParser(string);

string negationParser(string s) {
    if (s[0] == '(')
        return expressionParser(s.substr(1, s.length() - 2));
    if (s[0] == '!') return "(!" + negationParser(s.substr(1)) + ")"; else return s;
}

string conjunctionParser(string s) {
    int brackets_count = 0;
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
    for (int i = s.length() - 1; i > 0; i--)
        if (s[i] == '(') brackets_count++;
        else if (s[i] == ')') brackets_count--;
        else if (brackets_count == 0 && s[i] == '|')
            return "(|," + disjunctionParser(s.substr(0, i)) + "," + conjunctionParser(s.substr(i + 1)) + ")";
    return conjunctionParser(s);
}

string expressionParser(string s) {
    int brackets_count = 0;
    for (int i = 0; i < s.length() - 1; i++) {
        if (s[i] == '(') brackets_count++;
        else if (s[i] == ')') brackets_count--;
        else if (brackets_count == 0 && s[i] == '-' && s[i + 1] == '>')
            return "(->," + disjunctionParser(s.substr(0, i)) + "," + expressionParser(s.substr(i + 2)) + ")";
    }
    return disjunctionParser(s);
}

int main() {
    std::ios::sync_with_stdio(false);
    //ifstream cin("input.txt");
    //ofstream cout("output.txt");
    string s;
    getline(cin, s);
    long long j = 0;
    while (j < s.length()) {
        switch(s[j]) {
            case ')':
            case '(':
            case '!':
            case '>':
            case '-':
            case '|':
            case '&':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                break;
            default: {
                char c = s[j];
                int checkcode = (int) c;
                if (!(checkcode >= 65 && checkcode <= 90)) s.erase(j, 1);
            }
        }
        j++;
    }
    string ans = expressionParser(s);
    cout << ans << endl;
    return 0;
}