#ifndef _CODEGEN_H
#define _CODEGEN_H

#include "AST.h"
#include <stack>
#include <sstream>
#include <vector>

using namespace std;

ostream &operator<<(ostream &os, IDType type);

class BlockNode;

// 符号表中的一个条目
struct tableItem
{
public:
    string newName, oldName;
    IDType type;
    shared_ptr<IdentifierNode> *expValue;
};

// CodeGenContext保存着整个编译过程中所需的全局变量
class CodeGenContext
{
public:
    int indent;                      // 缩进
    stringstream code;               // 输出代码流
    vector<vector<tableItem>> table; // 变量符号表
    vector<string> funcDeclaration;  // 函数符号表
    vector<string> errorMsg;         // 错误信息

public:
    CodeGenContext()
    {
        indent = -1;
    }

    tableItem *judgeDefinition(string name); // 在符号表中查找变量的定义
    tableItem *judgeRedefinition(string name); // 在符号表中查找变量的定义
    int judgeBlockRedefinition(string name);

    void generateCode(BlockNode *root); // 开始生成代码
};

#endif