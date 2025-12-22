#include <iostream>
#include <string>
#include <cstdlib>
#include "lexer.h"

using namespace std;

// --- 全局变量 ---
Token lookahead; // 当前预读到的 Token

// --- 辅助函数：打印缩进 ---
// depth 表示树的深度，深度越深，缩进越多
void printTree(string nodeName, int depth) {
    for (int i = 0; i < depth; i++) {
        cout << "  "; // 每一层缩进2个空格
    }
    cout << nodeName << endl;
}

// --- 辅助函数：打印Token并读取下一个 ---
void match(TokenCode expected, int depth) {
    if (lookahead.type == expected) {
        // 打印叶子节点（具体的Token值）
        for (int i = 0; i < depth; i++) cout << "  ";
        cout << lookahead.value << endl;
        
        lookahead = getNextToken(); // 读取下一个
    } else {
        cout << "[Syntax Error] Expected token type " << expected 
            << " but got " << lookahead.type 
            << " (" << lookahead.value << ") at line " << row << endl;
        exit(1); // 遇到错误直接退出
    }
}

// --- 递归函数声明 ---
void parseStatementList(int depth);
void parseStatement(int depth);
void parseExpression(int depth);

// 1. 解析表达式 (简化版：只处理简单的运算)
void parseExpression(int depth) {
    printTree("Expression", depth);
    
    // 假设表达式以 ID, NUM 或 ( 开头
    if (lookahead.type == TK_IDENT || lookahead.type == TK_INT || lookahead.type == TK_DOUBLE) {
        // 打印第一个操作数
        for (int i = 0; i < depth + 1; i++) cout << "  ";
        cout << lookahead.value << endl;
        lookahead = getNextToken();
    }
    
    // 如果后面跟着运算符 (+ - * / > <)
    while (lookahead.type == TK_PLUS || lookahead.type == TK_MINUS || 
        lookahead.type == TK_STAR || lookahead.type == TK_DIVIDE ||
        lookahead.type == TK_GT || lookahead.type == TK_LT || lookahead.type == TK_EQ) {
        
        // 打印运算符
        for (int i = 0; i < depth + 1; i++) cout << "  ";
        cout << "Op: " << lookahead.value << endl;
        lookahead = getNextToken();
        
        // 打印下一个操作数 (简化处理)
        if (lookahead.type == TK_IDENT || lookahead.type == TK_INT || lookahead.type == TK_DOUBLE) {
            for (int i = 0; i < depth + 1; i++) cout << "  ";
            cout << lookahead.value << endl;
            lookahead = getNextToken();
        }
    }
}

// 2. 解析单条语句
void parseStatement(int depth) {
    // 判别是哪种语句
    
    // Case 1: If 语句
    if (lookahead.type == KW_IF) {
        printTree("IfStmt", depth);
        match(KW_IF, depth + 1);
        match(TK_OPENPA, depth + 1);
        parseExpression(depth + 1);
        match(TK_CLOSEPA, depth + 1);
        match(TK_BEGIN, depth + 1);   // {
        parseStatementList(depth + 1); // 递归解析块内语句
        match(TK_END, depth + 1);     // }
    }
    // Case 2: While 语句
    else if (lookahead.type == KW_WHILE) {
        printTree("WhileStmt", depth);
        match(KW_WHILE, depth + 1);
        match(TK_OPENPA, depth + 1);
        parseExpression(depth + 1);
        match(TK_CLOSEPA, depth + 1);
        match(TK_BEGIN, depth + 1);
        parseStatementList(depth + 1);
        match(TK_END, depth + 1);
    }
    // Case 3: 声明语句 (int a;)
    else if (lookahead.type == KW_INT || lookahead.type == KW_DOUBLE) {
        printTree("Declaration", depth);
        // 打印类型
        for (int i = 0; i < depth + 1; i++) cout << "  ";
        cout << "Type: " << lookahead.value << endl;
        lookahead = getNextToken();
        
        match(TK_IDENT, depth + 1);
        match(TK_SEMICOLOM, depth + 1);
    }
    // Case 4: 赋值语句 (a = 10;)
    else if (lookahead.type == TK_IDENT) {
        printTree("Assignment", depth);
        match(TK_IDENT, depth + 1); // 变量名
        match(TK_ASSIGN, depth + 1); // =
        parseExpression(depth + 1);  // 值
        match(TK_SEMICOLOM, depth + 1); // ;
    }
    // Case 5: Return 语句
    else if (lookahead.type == KW_RETURN) {
        printTree("ReturnStmt", depth);
        match(KW_RETURN, depth + 1);
        parseExpression(depth + 1);
        match(TK_SEMICOLOM, depth + 1);
    }
    else {
        // 跳过无法识别的 Token，防止死循环
        lookahead = getNextToken();
    }
}

// 3. 解析语句列表 (StatementList -> Statement StatementList | epsilon)
void parseStatementList(int depth) {
    // 只要不是右大括号 } 且不是文件结束，就一直解析语句
    while (lookahead.type != TK_END && lookahead.type != TK_EOF) {
        parseStatement(depth);
    }
}

// 4. 解析主程序
void parseProgram() {
    printTree("Program", 0);
    
    // int main ( ) { ... }
    if (lookahead.type == KW_INT || lookahead.type == KW_VOID) {
        match(lookahead.type, 1); // int / void
    }
    match(KW_MAIN, 1);
    match(TK_OPENPA, 1);
    match(TK_CLOSEPA, 1);
    match(TK_BEGIN, 1); // {
    
    parseStatementList(2); // 进入代码块，深度+2
    
    match(TK_END, 1);   // }
}

int main() {
    if (!initLexer("input.txt")) {
        cout << "Error: Cannot open input.txt" << endl;
        return 1;
    }

    lookahead = getNextToken(); // 初始化：读取第一个 Token
    
    parseProgram(); // 开始分析
    
    cout << "\nSyntax Analysis Completed Successfully!" << endl;
    closeLexer();
    return 0;
}