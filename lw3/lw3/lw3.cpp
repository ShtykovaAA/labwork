#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <exception>
#include <map>
#include <stack>
#include <algorithm>
using namespace std;

string x = "0000000011111111";
string y = "0000111100001111";
string z = "0011001100110011";
string w = "0101010101010101";
vector<bool> res = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

map<char, int> priority = { {'(', 0}, {'^', 1}, {'/', 1}, {'^', 2}, {'>', 3}, {'+', 4}, {'|', 5}, {'&', 6}, {'!', 7} };

string Evaluate(string& s1, string& s2, char oper) {
    string result = s2;

    if (oper == '=') {  //экивалентность 
        for (int i = 0; i < s1.length(); i++) {
            if (s1[i] != result[i]) {
                result[i] = '0';
            }
            else {
                result[i] = '1';
            }
        }
    }
    else if (oper == '>') {  //импликация 
        for (int i = 0; i < s1.length(); i++) {
            if ((s1[i] == '0') &&  (result[i] == '1')) {
                result[i] = '0';
            }
            else {
                result[i] = '1';
            }
        }
    }
    else if (oper == '^') {   //стрелка пирса
        for (int i = 0; i < s1.length(); i++) {
            if ((s1[i] == '0') && (result[i] == '0')) {
                result[i] = '1';
            }
            else {
                result[i] = '0';
            }
        }
    }
    else if (oper == '/') {   //штрих шеффера
        for (int i = 0; i < s1.length(); i++) {
            if ((s1[i] == '1') && (result[i] == '1')) {
                result[i] = '0';
            }
            else {
                result[i] = '1';
            }
        }
    }
    else if (oper == '+') {  //сложение по модулю 2
        for (int i = 0; i < s1.length(); i++) {
            if (s1[i] != result[i]) {
                result[i] = '1';
            }
            else {
                result[i] = '0';
            }
        }
    }
    else if (oper == '|') {  //лог. или
        for (int i = 0; i < s1.length(); i++) {
            if ((s1[i] == '0') && (result[i] == '0')) {
                result[i] = '0';
            }
            else {
                result[i] = '1';
            }
        }
    }
    else {   //лог. и
        for (int i = 0; i < s1.length(); i++) {
            if ((s1[i] == '1') && (result[i] == '1')) {
                result[i] = '1';
            }
            else {
                result[i] = '0';
            }
        }
    }
    return result;
}

string Negation(string& s) {
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '0') {
            s[i] = '1';
        }
        else {
            s[i] = '0';
        }
    }
    return s;
}

bool IsArgum(char c) {
    if (c == 'x' || c == 'y' || c == 'z' || c == 'w') {
        return true;
    }
    return false;
}

bool IsHighPriority(const char& c1, const char& c2) {
    if (priority[c2] > priority[c1]) {
        return true;
    }
    return false;
}

string Calculate(string& s) {
    vector<string> arguments;           //аргументы
    vector<char> operations;       //операции

    for (int i = 0; i < s.length(); i++) {
        if (IsArgum(s[i])) {     //
            if (s[i] == 'x') {
                arguments.push_back(x);
            }
            else if (s[i] == 'y') {
                arguments.push_back(y);
            }
            else if (s[i] == 'z') {
                arguments.push_back(z);
            }
            else {
                arguments.push_back(w);
            }
            if (!operations.empty()) {
                if (operations[operations.size() - 1] == '!') {
                    arguments[arguments.size() - 1] = Negation(arguments[arguments.size() - 1]);
                    operations.pop_back();
                }
            }
        }
        else {
            if (operations.empty()) {
                operations.push_back(s[i]);
            }
            else { 
                if (s[i] == ')') {   
                    char pred = operations[operations.size()-1];       //вытащили последнюю операцию
                    bool flag = true;
                    while (flag) {
                        string arg1 = arguments[arguments.size() - 1]; arguments.pop_back();
                        string arg2 = arguments[arguments.size() - 1]; arguments.pop_back();
                        string add = Evaluate(arg1, arg2, pred);
                        arguments.push_back(add);
                        operations.pop_back();
                        pred = operations[operations.size() - 1];
                        if (pred == '(') {
                            operations.pop_back();
                            flag = false;
                        }
                    }
                    if (!operations.empty()) {
                        if (operations[operations.size() - 1] == '!') {
                            string arg = arguments[arguments.size() - 1]; arguments.pop_back();
                            string add = Negation(arg);
                            arguments.push_back(add);
                            operations.pop_back();
                        }
                    }
                }
                else if (s[i] == '(') {
                    operations.push_back(s[i]);
                }
                else {
                    char pred = operations[operations.size() - 1];
                    if (IsHighPriority(pred, s[i])) {         // больше ли текущий приоритет в сравнении с предыдщуем
                        operations.push_back(s[i]);
                    }
                    else {
                        bool flag = true;
                        char pred = operations[operations.size() - 1];
                        while (flag && !operations.empty()) {
                            if (!IsHighPriority(pred, s[i])) {
                                string arg1 = arguments[arguments.size() - 1]; arguments.pop_back();
                                string arg2 = arguments[arguments.size() - 1]; arguments.pop_back();
                                string add = Evaluate(arg1, arg2, pred);
                                arguments.push_back(add);
                                operations.pop_back();
                                if (!operations.empty()) {
                                    pred = operations[operations.size() - 1];
                                }
                            }
                            else {
                                flag = false;
                            }
                        }
                        operations.push_back(s[i]);
                    }
                }
            }
        }
    }
    if (arguments.size() != 1) {
        while (operations.size() != 0) {
            char oper = operations[operations.size() - 1];
            string arg1 = arguments[arguments.size() - 1]; arguments.pop_back();
            string arg2 = arguments[arguments.size() - 1]; arguments.pop_back();
            string add = Evaluate(arg1, arg2, oper);
            arguments.push_back(add);
            operations.pop_back();
        }
        return arguments[0];
    }
    return arguments[0];
}
bool IsCorrect(string& s) {
    int c1 = count(s.begin(), s.end(), '(');   //кол-во скобок разное
    int c2 = count(s.begin(), s.end(), ')');
    if (c1 != c2) {
        cout << "1";
        return false;
    }
    for (int i = 0; i < s.size(); i++) {
        if (s[i] != 'x' && s[i] != 'y' && s[i] != 'z' && s[i] != 'w' &&
            s[i] != '=' && s[i] != '^' && s[i] != '>' && s[i] != '!' &&
            s[i] != '|' && s[i] != '&' && s[i] != '/' && s[i] != '+' && s[i] != '(' && s[i] != ')') {
            cout << "2";
            cout << s[i]<< endl;
            return false;
        }
    }
    return true;
}
void Output(ofstream& out, const string& s) {
    string x = "0000000011111111";
    string y = "0000111100001111";
    string z = "0011001100110011";
    string w = "0101010101010101";
    out << "x y z w res" << endl;
    for (int i = 0; i < x.length(); i++) {
        out << x[i] << ' ' << y[i] << ' ' << z[i] << ' ' << w[i] << ' ' << s[i] << endl;
    }
}

int main()
{
    ifstream input("input.txt");
    string line;
    if (!input.is_open()) {
        cout << "file is not open" << endl;
    }
    getline(input, line);
    const string path = "output.txt";
    ofstream output(path);
    if (IsCorrect(line)) {
        output << line << endl << endl;
        string result = Calculate(line);
        Output(output, result);
    }
    else {
        output << line <<endl <<"Incorrect value";
    }

    return 0;
}

