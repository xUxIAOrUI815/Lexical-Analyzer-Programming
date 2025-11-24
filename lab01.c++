#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

/*
input: 字符流
output: 标记序列
*/

/* 单词编码 
枚举类型 定义词法分析器能识别的所有词元类型
将所有关键字、运算符、分隔符、常量、标识符、未定义的符号存储为枚举类型
后续的代码只需要处理这些数字编号，比处理字符串高效且清晰
*/
enum TokenCode{

    /* 未定义 */
    TK_UNDEF = 0,       // 未定义的符号 用于错误处理

    /* 关键字 */
	KW_VOID,	//void关键字
	KW_MAIN,	//main关键字
	KW_INT,		//int关键字
	KW_DOUBLE,	//double关键字
	KW_FOR,		//for关键字
	KW_WHILE,	//while关键字
	KW_SWITCH,	//switch关键字
	KW_CASE,	//case关键字
	KW_IF,		//if关键字
	KW_ELSE,	//else关键字
	KW_RETURN,	//return关键字

	/* 运算符 */
	TK_PLUS,	//+加号
	TK_MINUS,	//-减号
	TK_STAR,	//*乘号
	TK_DIVIDE,	///除号
	TK_ASSIGN,	//=赋值运算符
	TK_EQ,		//==等于号
	TK_LT,		//<小于号
	TK_LEQ,		//<=小于等于号
	TK_GT,		//>大于号
	TK_GEQ,		//>=大于等于号

	/* 分隔符 */
	TK_OPENPA,	//(左圆括号
	TK_CLOSEPA,	//)右圆括号
	TK_OPENBR,	//[左中括号
	TK_CLOSEBR,	//]右中括号
	TK_BEGIN,	//{左大括号
	TK_END,		//}右大括号
	TK_COMMA,	//,逗号
	TK_SEMOCOLOM,	//;分号
	
	/* 常量 */
	TK_INT,		//整型常量
	TK_DOUBLE,	//浮点型常量

	/* 标识符 */
	TK_IDENT
};

/* 全局参数 */
TokenCode code=TK_UNDEF;    // 记录单词的种别码
const int MAX=11;           // 关键字数量
string token = " ";
int row = 1;
char keyWord[][10]={ "void","main","int","double","for","while","switch","case","if","else","return" };	//存储关键词

/* 打印词法分析的结果
将识别到的词元，按代码，词素 打印出来
 */
void print(TokenCode code){
    // 贯穿特性
    switch(code){
        /* 未定义 */
        case TK_UNDEF:
            cout<<'('<<code<<','<<token<<')'<<"未识别的符号在第"<<row<<"行"<<endl;
            return;
            break;
        /* 关键字 */
        case KW_VOID:
        case KW_MAIN:
        case KW_INT:
        case KW_DOUBLE:
        case KW_FOR:
        case KW_WHILE:
        case KW_SWITCH:
        case KW_CASE:
        case KW_IF:
        case KW_ELSE:
        case KW_RETURN:
            break;
        /* 运算符 */
        case TK_PLUS:
        case TK_MINUS:
        case TK_STAR:
        case TK_DIVIDE:
        case TK_ASSIGN:
        case TK_EQ:
        case TK_LT:
        case TK_LEQ:
        case TK_GT:
        case TK_GEQ:
            break;
        /* 分隔符 */
        case TK_OPENPA:
        case TK_CLOSEPA:
        case TK_OPENBR:
        case TK_CLOSEBR:
        case TK_BEGIN:
        case TK_END:
        case TK_COMMA:
        case TK_SEMOCOLOM:
            break;
        /* 常量 */
        case TK_INT:
        case TK_DOUBLE:
            break;
        case TK_IDENT:
            break;
        default:
            break;
    }
    cout<<'('<<code<<','<<token<<')'<<endl;
}

/* 字符分类函数 */
/* 判断字符是否为关键字 */
bool isKeyWord(string token){ 
    for(int i=0;i<MAX;i++){
        if(token.compare(keyWord[i]) == 0){
            return true;
        }
    }
    return false;
}
/* 判断字符是否为字母 */
bool isLetter(char letter){ 
	if ((letter >= 'a'&&letter <= 'z') || (letter >= 'A' &&letter <= 'Z'))
		return true;
	return false;
}
/* 判断字符是否为数字 */
bool isDigit(char ch){ 
    if(ch>='0'&&ch<='9'){
        return true;
    }
    return false;
}
/* 获取关键字的码值 */
int getKeyWordCode(string token){ 
    // 码值等于关键字在keyWord中的索引加1
    for (int i = 0; i < MAX; i++)
	{
		if (token.compare(keyWord[i]) == 0)	
			return i+1;
	}
	return -1;
}
/* 词法分析 */
void lexicalAnalysis(FILE *fp){     // 文件指针参数  
    char ch;
    while((ch=fgetc(fp)!=EOF)){
        token=ch;
        if(ch==' '||ch=='\t'||ch=='\n'){    // 忽略空格 Tab 和回车
            if(ch=='\n'){
                row++;
            }
            continue;
        }
        else if(isLetter(ch)){ 
            token="";
            while(isLetter(ch)||isDigit(ch)){   // 不是字母或数字时退出
                token.push_back(ch);
                ch=fgetc(fp);
            }

            fseek(fp,-1L,SEEK_CUR);
            if(isKeyWord(token)){
                code=TokenCode(getKeyWordCode(token));
            }
            else{
                code=TK_IDENT;
            }
        }
        else if(isDigit(ch)){   // 无符号常数以数字开头
            int isdouble=0; // 是否为浮点数
            token="";
            while(isDigit(ch)){     // 当前获取的字符为数字
                token.push_back(ch);    // 读取数字
                ch=fgetc(fp);
                if(ch=='.'&& isdouble==0){

                    // 小数点下一位是数字
                    if(isDigit(fgetc(fp))){
                        isdouble=1;
                        fseek(fp,-1L,SEEK_CUR);
                        token.push_back(ch);
                        ch=fgetc(fp);
                    }
                }
            }
            if(isdouble==1){
                code=TK_DOUBLE;
            }   
            else{
                code=TK_INT;
            }
            fseek(fp,-1L,SEEK_CUR);
        }
        else switch(ch){
            case '+':
                code=TK_PLUS;
                break;
            case '-':
                code=TK_MINUS;
                break;
            case '*':
                code=TK_STAR;
                break;
            case '/':
                code=TK_DIVIDE;
                break;
            case '=':{
                ch=fgetc(fp);
                if(ch=='='){
                    token.push_back(ch);
                    code=TK_EQ;
                }
                else{
                    code=TK_ASSIGN;
                    fseek(fp,-1L,SEEK_CUR);
                }
            }
            break;
            case '<':{
                ch=fgetc(fp);
                if(ch=='='){
                    token.push_back(ch);
                    code=TK_LEQ;
                }
                else{
                    code=TK_LT;
                    fseek(fp,-1L,SEEK_CUR);
                }
            }
            break;
            case '>':{
                ch=fgetc(fp);
                if(ch=='='){
                    token.push_back(ch);
                    code=TK_GEQ;
                }
                else{
                    code=TK_GT;
                    fseek(fp,-1L,SEEK_CUR);
                }
            }
            break;
            /* 分隔符 */
            case '(':
                code=TK_OPENPA;
                break;
            case ')':
                code=TK_CLOSEPA;
                break;
            case '[':
                code=TK_OPENBR;
                break;
            case ']':
                code=TK_CLOSEBR;
                break;
            case '{':
                code=TK_BEGIN;
                break;
            case '}':
                code=TK_END;
                break;
            case ',':
                code=TK_COMMA;
                break;
            case ';':
                code=TK_SEMOCOLOM;
                break;
            default:
                code=TK_UNDEF;
        }
        print(code);
    }
}
/* 主循环 */
int main(){
    string filename;
    FILE* fp;
    cout<<"请输入文件名："<<endl;
    while(true){
        cin>>filename;
        if((fopen_s(&fp,filename.c_str(),"r"))==0)
            break;
        else
            cout<<"文件不存在！请重新输入文件名："<<endl;
    }

    cout<<"词法分析结果："<<endl;
    lexicalAnalysis(fp);
    fclose(fp);
    return 0;
}