#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

FILE* fp = NULL;
int row = 1;

const int KEYWORD_MAX = 11;
char keyWord[][10]={
    "void","main","int","double","for","while","switch","case","if","else","return"
};

bool isKeyWord(string token) {
    for (int i = 0; i < KEYWORD_MAX; i++) {
        if (token == keyWord[i]) return true;
    }
    return false;
}

bool isLetter(char letter) {
    return (letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z');
}

bool isDigit(char ch) {
    return ch >= '0' && ch <= '9';
}

int getKeyWordCode(string token) {
    for (int i = 0; i < KEYWORD_MAX; i++) {
        if (token == keyWord[i]) return i + 1; // 对应枚举中关键字的顺序
    }
    return TK_UNDEF;
}

bool initLexer(const char* filename) {
    // 使用 fopen_s (Visual Studio) 或 fopen (Standard)
    // 这里为了兼容你的 Lab1 使用 fopen_s
    if (fopen_s(&fp, filename, "r") != 0) {
        return false;
    }
    row = 1;
    return true;
}

void closeLexer() {
    if (fp) fclose(fp);
}

Token getNextToken() {
    Token currentToken;
    currentToken.type = TK_UNDEF; // 默认为未定义
    currentToken.value = "";
    
    char ch;

    // 1. 跳过空白符 (空格、Tab、换行)
    // 这里的逻辑是：一直读，直到读到一个有意义的字符
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == ' ' || ch == '\t') {
            continue;
        }
        if (ch == '\n') {
            row++;
            continue;
        }
        break; // 遇到了非空白字符，跳出循环，开始识别
    }

    // 2. 处理 EOF (文件结束)
    if (ch == EOF) {
        currentToken.type = TK_EOF;
        currentToken.value = "EOF";
        return currentToken;
    }

    // 3. 识别 标识符 或 关键字 (复用 Lab1 逻辑)
    if (isLetter(ch)) {
        string tempStr = "";
        while (isLetter(ch) || isDigit(ch)) {
            tempStr.push_back(ch);
            ch = fgetc(fp);
        }
        // 多读了一个字符，回退
        fseek(fp, -1L, SEEK_CUR);

        currentToken.value = tempStr;
        if (isKeyWord(tempStr)) {
            currentToken.type = (TokenCode)getKeyWordCode(tempStr);
        } else {
            currentToken.type = TK_IDENT;
        }
        return currentToken; // 直接返回，不再打印
    }

    // 4. 识别 数字 (复用 Lab1 逻辑)
    if (isDigit(ch)) {
        string tempStr = "";
        int isDouble = 0;
        
        while (isDigit(ch)) {
            tempStr.push_back(ch);
            ch = fgetc(fp);
            if (ch == '.' && isDouble == 0) {
                // 检查小数点后是否还有数字
                char nextCh = fgetc(fp);
                if (isDigit(nextCh)) {
                    isDouble = 1;
                    tempStr.push_back(ch); // push '.'
                    ch = nextCh; // ch becomes the digit after dot
                } else {
                    // 如果小数点后不是数字，回退 (比如 1. 这种情况的处理，视具体需求而定)
                    fseek(fp, -1L, SEEK_CUR); 
                }
            }
        }
        fseek(fp, -1L, SEEK_CUR);

        currentToken.value = tempStr;
        currentToken.type = (isDouble == 1) ? TK_DOUBLE : TK_INT;
        return currentToken;
    }

    // 5. 识别 运算符和分隔符 (复用 Lab1 switch 逻辑)
    // 注意：这里需要把 ch 存入 value，并设置 type，然后 break 返回
    currentToken.value.push_back(ch); // 先把第一个字符存进去

    switch (ch) {
        case '+': currentToken.type = TK_PLUS; break;
        case '-': currentToken.type = TK_MINUS; break;
        case '*': currentToken.type = TK_STAR; break;
        case '/': currentToken.type = TK_DIVIDE; break;
        case '(': currentToken.type = TK_OPENPA; break;
        case ')': currentToken.type = TK_CLOSEPA; break;
        case '[': currentToken.type = TK_OPENBR; break;
        case ']': currentToken.type = TK_CLOSEBR; break;
        case '{': currentToken.type = TK_BEGIN; break;
        case '}': currentToken.type = TK_END; break;
        case ',': currentToken.type = TK_COMMA; break;
        case ';': currentToken.type = TK_SEMICOLOM; break;
        
        case '=': {
            char nextCh = fgetc(fp);
            if (nextCh == '=') {
                currentToken.value.push_back(nextCh);
                currentToken.type = TK_EQ;
            } else {
                fseek(fp, -1L, SEEK_CUR);
                currentToken.type = TK_ASSIGN;
            }
            break;
        }
        case '<': {
            char nextCh = fgetc(fp);
            if (nextCh == '=') {
                currentToken.value.push_back(nextCh);
                currentToken.type = TK_LEQ;
            } else {
                fseek(fp, -1L, SEEK_CUR);
                currentToken.type = TK_LT;
            }
            break;
        }
        case '>': {
            char nextCh = fgetc(fp);
            if (nextCh == '=') {
                currentToken.value.push_back(nextCh);
                currentToken.type = TK_GEQ;
            } else {
                fseek(fp, -1L, SEEK_CUR);
                currentToken.type = TK_GT;
            }
            break;
        }
        default:
            currentToken.type = TK_UNDEF;
            break;
    }

    return currentToken;
}