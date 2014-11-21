#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>

enum Token {
    tok_eof = -1;
    tok_def = -2;
    tok_extern = -3;
    tok_identifier = -4;
    tok_number = -5;
}

static std::string IdentifierStr;
static double NumVal;

static int gettok() {
    static int LastChar = '';

    while (isspace(LastChar)) {
        LastChar = getchar();
    }

    if (isalpha(LastChar)) {
        IdentifierStr = LastChar;
        while (isalnum(LastChar = getchar())) {
            IdentifierStr += LastChar;
        }
        if (IdentifierStr == "def") {
            return tok_def;
        } else if (IdentifierStr == "extern") {
            return tok_extern;
        } else {
            return tok_identifier;
        }
    }

    if (isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');
        NumVal = strtod(NumStr.c_str(), 0);
        return tok_number;
    }

    if (LastChar == '#') {
        do {
            LastChar = getchar();
        } while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
        if (LastChar != EOF) {
            return gettok();
        }
    }

    if (LastChar == EOF) {
        return tok_eof;
    }

    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}

namespace {
    class ExprAST {
    public:
        virtual ~ExprAST() {};
    };

    class NumberExprAST : public ExprAST {
    public:
        NumberExprAST(double val) {};
    };

    class VariableExprAST : public ExprAST {
        std::string Name;
    public:
        VariableExprAST(const std::string &name) : Name(name) {}
    };

    class BinaryExprAST : public ExprAST {
    public:
        BinaryExprAST(char op, ExprAST *lhs, ExprAST *rhs) {}
    };

    class CallExprAST : public ExprAST {
        std::string Callee;
        std::vector<ExprAST*> Args;
    public:
        CallExprAST(const std::string &callee, std::vector<ExprAST*> &args)
        : Callee(callee), Args(args) {}
    };

    class PrototypeAST {
        std::string Name;
        std::vector<std::string> Args;
    public:
        PrototypeAST(const std::string &name, const std::vector<std::string> & args): Name(name), Args(args) {}
    };

    class FunctionAST {
    public:
        FunctionAST(PrototypeAST *proto, ExprAST *body) {}
    };
}

static int CurTok;
static int getNextToken() {
    return CurTok = gettok();
}

static std::map<char, int> BinopPrecedence;

static int GetTokPrecedence() {
    if (!isascii(CurTok)) {
        return -1;
    }
    int TokPrec = BinopPrecedence[CurTok];
    if (TokPrec <= 0) {
        return -1;
    }
    return TokPrec;
}





















