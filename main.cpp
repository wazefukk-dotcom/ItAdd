#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

constexpr bool debugMode = 0;
#define debugLog(a) if (debugMode == 1) std::cout << a
#define GOTOJUMP if (instance.labels.count(tokens[1])) i = instance.labels[tokens[1]]; continue;

// struct containing all basic variables that would be needed to parse an ItAdd file
struct itAdd {
    long long mem[26] = {
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
        0,0,0,0,0,
        0
    }, leftint = 0, rightint = 0, ans = 0;

    std::string leftstr, rightstr, path;

    std::vector<std::string> lines;

    std::string op2 = "+-*/%^";
    std::string op1 = "!?";

    std::map<std::string, int> labels;
};

std::vector<std::string> tokenizer(const std::string& in) {
    debugLog("tokenizer() called for" << in << '\n');
    std::vector<std::string> out;
    std::string buffer;
    for (auto i : in) {
        if (i == ' ') {
            if (!buffer.empty()) {
                out.push_back(buffer);
                buffer.clear();
            }
            continue;
        }
        buffer += i;
    }
    if (!buffer.empty()) out.push_back(buffer);
    return out;
}

long long powFunny(int a, int b) {
    long long val = a;
    for (int i = 0; i < b; i++) // Off-by-one error included: everything works fine. Off-by-one error fixed: everything's broken
        val *= a;
    return val;
}

long long toInt(const std::string& in) {
    debugLog("toInt() called for" << in << '\n');
    long long val = 0;
    unsigned long long base = powFunny(10, in.size()) / 10;
    int temp;
    std::string num = "0123456789";
    for (auto i : in) {
        base /= 10;
        temp = 11;
        for (int j = 0; j < 10; j++)
            if (num[j] == i) {
                temp = j;
                break;
            }
        if (temp == 11) continue;
        val += (base * temp);
    }
    return (in[0] == '-') ? 0 - val : val;
}

long long getVal(const std::string& in, const itAdd& in2) {
    debugLog("getVal() called for" << in << '\n');
    std::string alph = "abcdefghijklmnopqrstuvwxyz";
    if (in == "ans") return in2.ans;
    for (int i = 0; i < 26; i++)
        if (alph[i] == in[0])
            return in2.mem[i];
    return toInt(in);
}

int main() {
    itAdd instance;

    std::getline(std::cin, instance.path);

    std::fstream inFile(instance.path, std::ios::in);
    std::string buffer;
    while (std::getline(inFile, buffer))
        instance.lines.push_back(buffer);
    inFile.close();

    std::string alph = "abcdefghijklmnopqrstuvwxyz";
    
    for (int i = 0; i < instance.lines.size(); i++) {
        auto tokens = tokenizer(instance.lines[i]);

        if (tokens.size() == 1) {
            std::string temp = tokens[0];
            temp.pop_back();
            instance.labels.insert({ temp,i });
            continue;
        }
    }

    for (int i = 0; i < instance.lines.size(); i++) {
        auto tokens = tokenizer(instance.lines[i]);

        if (tokens.empty()) continue;

        if (tokens[1] == "~") {
            std::cout << tokens[0] << " - " << getVal(tokens[0], instance) << '\n';
            continue;
        }

        if (tokens[0] == "GOTO" && tokens.size() == 2) {
            GOTOJUMP
        }
        else if (tokens[0] == "GOTO") {
            if (tokens.size() != 5) continue;

            if (tokens[3] == "==") {
                if (instance.ans == getVal(tokens[4], instance)) {
                    GOTOJUMP
                }
            }
            else if (tokens[3] == "!=") {
                if (instance.ans != getVal(tokens[4], instance)) {
                    GOTOJUMP
                }
            }
            else if (tokens[3] == ">") {
                if (instance.ans > getVal(tokens[4], instance)) {
                    GOTOJUMP
                }
            }
            else if (tokens[3] == "<") {
                if (instance.ans < getVal(tokens[4], instance)) {
                    GOTOJUMP
                }
            }
            else if (tokens[3] == ">=") {
                if (instance.ans >= getVal(tokens[4], instance)) {
                    GOTOJUMP
                }
            }
            else if (tokens[3] == "<=") {
                if (instance.ans <= getVal(tokens[4], instance)) {
                    GOTOJUMP
                }
            }
        }

        else if (tokens[1] == "=") {
            debugLog("token 1 is assignment\n");
            if (tokens[2] == "@") {
                std::cout << tokens[0] << " = ";
                if (tokens[0] == "ans")
                    std::cin >> instance.ans;
                else {
                    for (int j = 0; j < 26; j++)
                        if (alph[j] == tokens[0][0])
                            std::cin >> instance.mem[j];
                }
                continue;
            }
            else if (tokens[0] == "ans") continue;
            for (int j = 0; j < 26; j++)
                if (tokens[0][0] == alph[j])
                    instance.mem[j] = getVal(tokens[2], instance);
            continue;
        }

        debugLog("getting values...\n");
        instance.leftint = getVal(tokens[0], instance);
        if (instance.op2.contains(tokens[1]))
            instance.rightint = getVal(tokens[2], instance);

        switch (tokens[1][0]) {

        case '+':
            instance.ans = instance.leftint + instance.rightint;
            break;

        case '-':
            instance.ans = instance.leftint - instance.rightint;
            break;

        case '*':
            instance.ans = instance.leftint * instance.rightint;
            break;

        case '/':
            instance.ans = instance.leftint / instance.rightint;
            break;

        case '%':
            instance.ans = instance.leftint % instance.rightint;
            break;

        case '!':
            instance.ans = instance.leftint;
            while (--instance.leftint != 0)
                instance.ans *= instance.leftint;
            break;

        case '?':
            instance.ans = instance.leftint;
            while (--instance.leftint != 0)
                instance.ans += instance.leftint;
            break;

        case '^':
            instance.ans = instance.leftint;
            while (--instance.rightint != 0)
                instance.ans *= instance.leftint;
            break;
        }
    }

    std::cout << std::endl << instance.ans;
    return 0;
}