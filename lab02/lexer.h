// 对外给 parser.cpp 展示接口
// 定义所有的Tokne类型，Token结构体以及函数声明。

#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <iostream>

using namespace std;

enum TokenCode {
    /* 未定义 */
    TK_UNDEF = 0,

    /* 关键字 */
    KW_VOID, KW_MAIN, KW_INT, KW_DOUBLE, KW_FOR, KW_WHILE, KW_SWITCH, KW_CASE, KW_IF, KW_ELSE, KW_RETURN,

    /* 运算符 */
    TK_PLUS, TK_MINUS, TK_STAR, TK_DIVIDE, TK_ASSIGN, TK_EQ, TK_LT, TK_LEQ, TK_GT, TK_GEQ,

    /* 分隔符 */
    TK_OPENPA, TK_CLOSEPA, TK_OPENBR, TK_CLOSEBR, TK_BEGIN, TK_END, TK_COMMA, TK_SEMICOLOM,

    /* 常量 & 标识符 */
    TK_INT, TK_DOUBLE, TK_IDENT,

    /* 新增：文件结束符，告诉 Parser 分析结束 */
    TK_EOF 
};

struct Token {
    TokenCode type;
    string value;
};

extern int row;     // 全局变量 行号 用于 Parser 报错

bool initLexer(const char* filename);

void closeLexer();

Token getNextToken();

#endif

