#ifndef __AST_H__
#define __AST_H__

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

#include <memory>
#include <string>
#include "cjson/cJSON.h"

using namespace std;

class CodeGenContext;
class BlockNode;
class StatementNode;
class ExpressionNode;
class VariableDeclarationNode;
class ExpressionStatementNode;

typedef vector<shared_ptr<StatementNode>> StatementList;
typedef vector<shared_ptr<ExpressionNode>> ExpressionList;
typedef vector<shared_ptr<VariableDeclarationNode>> VariableList;

enum IDType
{
    INTTYPE,
    CHARTYPE
};

class Node
{
public:
    Node() {}
    virtual ~Node() {}
    virtual void generateJson(cJSON *node) {}
    virtual void codeGen(CodeGenContext &context) {}
    virtual string getValue() { return nullptr; }
    virtual string getNodeType() { return nullptr; }
};

// 表达式
class ExpressionNode : public Node
{
public:
    ExpressionNode() {}
    void generateJson(cJSON *node)
    {
        // printf("ExpressionNode\n");
        cJSON_AddStringToObject(node, "ExpressionNode", "epsilon");
    }
    virtual void codeGen(CodeGenContext &context) {}
    virtual string getValue() { return nullptr; }
    virtual string getNodeType() { return nullptr; }

};

// 程序语句
class StatementNode : public Node
{
public:
    StatementNode() {}
    void generateJson(cJSON *node)
    {
        // printf("StatementNode\n");
        cJSON_AddStringToObject(node, "StatementNode", "epsilon");
    }
    virtual void codeGen(CodeGenContext &context) {}
    virtual string getValue() { return nullptr; }
    virtual string getNodeType() { return nullptr; }
};

// 整数
class IntegerNode : public ExpressionNode
{
public:
    int value;

    IntegerNode() {}

    IntegerNode(int value)
        : value(value)
    {
    }
    void generateJson(cJSON *node)
    {
        // printf("IntegerNode\n");
        char str[10];
        sprintf(str, "%d", value);
        cJSON_AddStringToObject(node, "IntegerNode", str);
    }
    virtual void codeGen(CodeGenContext &context);
    virtual string getValue() { return nullptr; }
    virtual string getNodeType() { return "IntegerNode"; }
};

// 变量名/函数名
class IdentifierNode : public ExpressionNode
{
public:
    string name; // 变量名

    IdentifierNode() {}

    IdentifierNode(const string &name)
        : name(name)
    {
    }
    void generateJson(cJSON *node)
    {
        // printf("IdentifierNode\n");
        cJSON_AddStringToObject(node, "IdentifierNode", name.c_str());
    }
    virtual void codeGen(CodeGenContext &context);
    virtual string getValue() { return nullptr; }
    virtual string getNodeType() { return "IdentifierNode"; }
};

// 函数调用
class CallFuncNode : public ExpressionNode
{
public:
    const shared_ptr<IdentifierNode> id;                                  // 函数名
    shared_ptr<ExpressionList> arguments = make_shared<ExpressionList>(); // 参数列表

    CallFuncNode()
    {
    }

    // 有参数的函数调用
    CallFuncNode(const shared_ptr<IdentifierNode> id, shared_ptr<ExpressionList> arguments)
        : id(id), arguments(arguments)
    {
    }
    // 无参数的函数调用
    CallFuncNode(const shared_ptr<IdentifierNode> id)
        : id(id)
    {
    }
    void generateJson(cJSON *node)
    {
        // printf("CallFuncNode\n");

        cJSON *arr = cJSON_CreateArray();
        cJSON *childnode = cJSON_CreateObject();
        id->generateJson(childnode);
        cJSON_AddItemToArray(arr, childnode);

        for (auto item = arguments->begin(); item != arguments->end(); item++)
        {
            childnode = cJSON_CreateObject();
            (*item)->generateJson(childnode);
            cJSON_AddItemToArray(arr, childnode);
        }
        cJSON_AddItemToObject(node, "CallFuncNode", arr);
    }
    virtual void codeGen(CodeGenContext &context);
    virtual string getValue() { return nullptr; }
    virtual string getNodeType() { return "CallFuncNode"; }
};

// 二元运算
class BinaryOperatorNode : public ExpressionNode
{
public:
    int op;                         // 操作符 +: 1, -: 2, *: 3, /: 4, <: 5, <=: 6, >: 7, >=: 8, !=: 9
    shared_ptr<ExpressionNode> lhs; // 左操作数
    shared_ptr<ExpressionNode> rhs; // 右操作数

    BinaryOperatorNode() {}

    BinaryOperatorNode(shared_ptr<ExpressionNode> lhs, int op, shared_ptr<ExpressionNode> rhs)
        : lhs(lhs), rhs(rhs), op(op)
    {
    }
    void generateJson(cJSON *node)
    {
        // printf("BinaryOperatorNode\n");
        cJSON *arr = cJSON_CreateArray();

        cJSON *childnode = cJSON_CreateObject();
        lhs->generateJson(childnode);
        cJSON_AddItemToArray(arr, childnode);

        childnode = cJSON_CreateObject();
        rhs->generateJson(childnode);
        cJSON_AddItemToArray(arr, childnode);

        cJSON_AddItemToObject(node, "BinaryOperatorNode", arr);
    }
    virtual void codeGen(CodeGenContext &context);
    virtual string getValue() { return nullptr; }
    virtual string getNodeType() { return "BinaryOperatorNode"; }
};

// 赋值，如 a = b + 1
class AssignmentNode : public ExpressionNode
{
public:
    shared_ptr<IdentifierNode> lhs;
    shared_ptr<ExpressionNode> rhs;

    AssignmentNode() {}

    AssignmentNode(shared_ptr<IdentifierNode> lhs, shared_ptr<ExpressionNode> rhs)
        : lhs(lhs), rhs(rhs)
    {
    }
    void generateJson(cJSON *node)
    {
        // printf("AssignmentNode\n");
        cJSON *arr = cJSON_CreateArray();

        cJSON *childnode = cJSON_CreateObject();
        lhs->generateJson(childnode);
        cJSON_AddItemToArray(arr, childnode);

        childnode = cJSON_CreateObject();
        rhs->generateJson(childnode);
        cJSON_AddItemToArray(arr, childnode);

        cJSON_AddItemToObject(node, "AssignmentNode", arr);
    }
    virtual void codeGen(CodeGenContext &context);
    virtual string getValue() { return nullptr; }
    virtual string getNodeType() { return "AssignmentNode"; }
};

// 代码块
class BlockNode : public ExpressionNode
{
public:
    shared_ptr<StatementList> statements = make_shared<StatementList>(); // 代码块由一条条Statement组成

    BlockNode()
    {
    }
    void generateJson(cJSON *node)
    {
        // printf("BlockNode\n");
        cJSON *arr = cJSON_CreateArray();

        for (auto item = statements->begin(); item != statements->end(); item++)
        {
            cJSON *childnode = cJSON_CreateObject();
            (*item)->generateJson(childnode);
            cJSON_AddItemToArray(arr, childnode);
        }
        cJSON_AddItemToObject(node, "BlockNode", arr);
    }
    virtual void codeGen(CodeGenContext &context);
    virtual string getValue() { return nullptr; }
    virtual string getNodeType() { return "BlockNode"; }
};

// Statement和Expression是有重叠部分的
// 由一条Expression构成的Statement
class ExpressionStatementNode : public StatementNode
{
public:
    shared_ptr<ExpressionNode> expression;

    ExpressionStatementNode() {}

    ExpressionStatementNode(shared_ptr<ExpressionNode> expression)
        : expression(expression)
    {
    }
    void generateJson(cJSON *node)
    {
        // printf("ExpressionStatementNode\n");
        cJSON *arr = cJSON_CreateArray();
        cJSON *childnode = cJSON_CreateObject();
        expression->generateJson(childnode);
        cJSON_AddItemToArray(arr, childnode);
        cJSON_AddItemToObject(node, "ExpressionStatementNode", arr);
    }
    virtual void codeGen(CodeGenContext &context);
    virtual string getValue() { return nullptr; }
    virtual string getNodeType() { return "ExpressionStatementNode"; }
};

// 变量声明/定义 如int a
class VariableDeclarationNode : public StatementNode
{
public:
    IDType type;                   // 类型
    shared_ptr<IdentifierNode> id; // 变量名 如a
    shared_ptr<ExpressionNode> assignmentExpr = nullptr;

    VariableDeclarationNode() {}

    VariableDeclarationNode(IDType type, shared_ptr<IdentifierNode> id, shared_ptr<ExpressionNode> assignmentExpr = NULL)
        : type(type), id(id), assignmentExpr(assignmentExpr)
    {
    }
    void generateJson(cJSON *node)
    {
        // printf("VariableDeclarationNode\n");
        cJSON *arr = cJSON_CreateArray();
        cJSON *childnode = cJSON_CreateObject();
        childnode = cJSON_CreateObject();
        id->generateJson(childnode);
        cJSON_AddItemToArray(arr, childnode);
        if (assignmentExpr != nullptr)
        {
            childnode = cJSON_CreateObject();
            assignmentExpr->generateJson(childnode);
            cJSON_AddItemToArray(arr, childnode);
        }
        cJSON_AddItemToObject(node, "VariableDeclarationNode", arr);
    }
    virtual void codeGen(CodeGenContext &context);
    virtual string getValue() { return nullptr; }
    virtual string getNodeType() { return "VariableDeclarationNode"; }
};

// 函数声明/定义, 如 int func(int a, int b);
class FunctionDeclarationNode : public StatementNode
{
public:
    IDType type;                                                      // 返回值类型
    shared_ptr<IdentifierNode> id;                                    // 函数名
    shared_ptr<VariableList> arguments = make_shared<VariableList>(); // 参数列表
    shared_ptr<BlockNode> block;                                      // 函数体
    bool isExternal = false;

    FunctionDeclarationNode() {}

    FunctionDeclarationNode(IDType type, shared_ptr<IdentifierNode> id, shared_ptr<VariableList> arguments, shared_ptr<BlockNode> block, bool isExt = false)
        : type(type), id(id), arguments(arguments), block(block), isExternal(isExt)
    {
    }
    void generateJson(cJSON *node)
    {
        // printf("FunctionDeclarationNode\n");
        cJSON *arr = cJSON_CreateArray();
        cJSON *childnode = cJSON_CreateObject();
        childnode = cJSON_CreateObject();
        id->generateJson(childnode);
        cJSON_AddItemToArray(arr, childnode);
        if (arguments != nullptr)
        {
            for (auto item = arguments->begin(); item != arguments->end(); item++)
            {
                childnode = cJSON_CreateObject();
                (*item)->generateJson(childnode);
                cJSON_AddItemToArray(arr, childnode);
            }
        }
        if (block != nullptr)
        {
            childnode = cJSON_CreateObject();
            block->generateJson(childnode);
            cJSON_AddItemToArray(arr, childnode);
        }
        cJSON_AddItemToObject(node, "FunctionDeclarationNode", arr);
    }
    virtual void codeGen(CodeGenContext &context);
    virtual string getValue() { return nullptr; }
    virtual string getNodeType() { return "FunctionDeclarationNode"; }
};

// 返回语句
class ReturnStatementNode : public StatementNode
{
public:
    shared_ptr<ExpressionNode> expression;

    ReturnStatementNode() {}

    ReturnStatementNode(shared_ptr<ExpressionNode> expression)
        : expression(expression)
    {
    }
    void generateJson(cJSON *node)
    {
        // printf("ReturnStatementNode\n");
        cJSON *arr = cJSON_CreateArray();
        cJSON *childnode = cJSON_CreateObject();
        expression->generateJson(childnode);
        cJSON_AddItemToArray(arr, childnode);
        cJSON_AddItemToObject(node, "ReturnStatementNode", arr);
    }
    virtual void codeGen(CodeGenContext &context);
    virtual string getValue() { return nullptr; }
    virtual string getNodeType() { return "ReturnStatementNode"; }
};

// If语句块
class IfStatementNode : public StatementNode
{
public:
    shared_ptr<ExpressionNode> condition; // 条件
    shared_ptr<BlockNode> trueBlock;      // 满足条件时的语句块
    shared_ptr<BlockNode> falseBlock;     // 不满足条件时的语句块，可以没有

    IfStatementNode() {}

    IfStatementNode(shared_ptr<ExpressionNode> cond, shared_ptr<BlockNode> blk, shared_ptr<BlockNode> blk2 = nullptr)
        : condition(cond), trueBlock(blk), falseBlock(blk2)
    {
    }
    void generateJson(cJSON *node)
    {
        // printf("IfStatementNode\n");
        cJSON *arr = cJSON_CreateArray();

        cJSON *childnode = cJSON_CreateObject();
        condition->generateJson(childnode);
        cJSON_AddItemToArray(arr, childnode);

        childnode = cJSON_CreateObject();
        trueBlock->generateJson(childnode);
        cJSON_AddItemToArray(arr, childnode);
        if (falseBlock != nullptr)
        {
            childnode = cJSON_CreateObject();
            falseBlock->generateJson(childnode);
            cJSON_AddItemToArray(arr, childnode);
        }
        cJSON_AddItemToObject(node, "IfStatementNode", arr);
    }
    virtual void codeGen(CodeGenContext &context);
    virtual string getValue() { return nullptr; }
    virtual string getNodeType() { return "IfStatementNode"; }
};

// For语句块
class ForStatementNode : public StatementNode
{
public:
    shared_ptr<StatementNode> initial, condition; // 开始表达式，终止条件
    shared_ptr<ExpressionNode> increment;         // 变化
    shared_ptr<BlockNode> block;                  // for内部内容

    ForStatementNode() {}

    ForStatementNode(shared_ptr<BlockNode> b, shared_ptr<StatementNode> init = nullptr, shared_ptr<StatementNode> cond = nullptr, shared_ptr<ExpressionNode> incre = nullptr)
        : block(b), initial(init), condition(cond), increment(incre)
    {
    }
    void generateJson(cJSON *node)
    {
        // printf("ForStatementNode\n");
        cJSON *arr = cJSON_CreateArray();
        cJSON *childnode = cJSON_CreateObject();
        initial->generateJson(childnode);
        cJSON_AddItemToArray(arr, childnode);

        childnode = cJSON_CreateObject();
        condition->generateJson(childnode);
        cJSON_AddItemToArray(arr, childnode);

        childnode = cJSON_CreateObject();
        increment->generateJson(childnode);
        cJSON_AddItemToArray(arr, childnode);

        if (block != nullptr)
        {
            childnode = cJSON_CreateObject();
            block->generateJson(childnode);
            cJSON_AddItemToArray(arr, childnode);
        }
        cJSON_AddItemToObject(node, "ForStatementNode", arr);
    }
    virtual void codeGen(CodeGenContext &context);
    virtual string getValue() { return nullptr; }
    virtual string getNodeType() { return "ForStatementNode"; }
};

// 数组取index 如a[i-1]
class ArrayIndexNode : public ExpressionNode
{
public:
    shared_ptr<IdentifierNode> arrayName;  // 数组名
    shared_ptr<ExpressionNode> expression; // 取址表达式

    ArrayIndexNode() {}

    ArrayIndexNode(shared_ptr<IdentifierNode> name, shared_ptr<ExpressionNode> exp)
        : arrayName(name), expression(exp)
    {
    }

    void generateJson(cJSON *node)
    {
        // printf("ArrayIndexNode\n");
        cJSON *arr = cJSON_CreateArray();

        cJSON *childnode = cJSON_CreateObject();
        arrayName->generateJson(childnode);
        cJSON_AddItemToArray(arr, childnode);
        childnode = cJSON_CreateObject();
        expression->generateJson(childnode);
        cJSON_AddItemToArray(arr, childnode);
        cJSON_AddItemToObject(node, "ArrayIndexNode", arr);
    }
    virtual void codeGen(CodeGenContext &context);
    virtual string getValue() { return nullptr; }
    virtual string getNodeType() { return "ArrayIndexNode"; }
};

// 给数组的某元素赋值, 如a[i] = 3;
class ArrayAssignmentNode : public ExpressionNode
{
public:
    shared_ptr<ArrayIndexNode> arrayIndex;
    shared_ptr<ExpressionNode> expression;

    ArrayAssignmentNode() {}

    ArrayAssignmentNode(shared_ptr<ArrayIndexNode> index, shared_ptr<ExpressionNode> exp)
        : arrayIndex(index), expression(exp)
    {
    }
    void generateJson(cJSON *node)
    {
        // printf("ArrayAssignmentNode\n");
        cJSON *arr = cJSON_CreateArray();
        cJSON *childnode = cJSON_CreateObject();
        arrayIndex->generateJson(childnode);
        cJSON_AddItemToArray(arr, childnode);
        childnode = cJSON_CreateObject();
        expression->generateJson(childnode);
        cJSON_AddItemToArray(arr, childnode);
        cJSON_AddItemToObject(node, "ArrayAssignmentNode", arr);
    }
    virtual void codeGen(CodeGenContext &context);
    virtual string getValue() { return nullptr; }
    virtual string getNodeType() { return "ArrayAssignmentNode"; }
};

class LiteralNode : public ExpressionNode
{
public:
    string value;

    LiteralNode() {}

    LiteralNode(const string &str)
    {
        value = str.substr(1, str.length() - 2);
    }
    void generateJson(cJSON *node)
    {
        cJSON_AddStringToObject(node, "LiteralNode", value.c_str());
    }
    virtual void codeGen(CodeGenContext &context);
    virtual string getValue()
    {
        return value;
    }
    virtual string getNodeType() { return "LiteralNode"; }
};

class ArrayDeclarationNode : public StatementNode
{
public:
    IDType type;
    shared_ptr<IdentifierNode> id;
    shared_ptr<ExpressionNode> lengthExpression;
    ArrayDeclarationNode(IDType type, shared_ptr<IdentifierNode> id, shared_ptr<ExpressionNode> lengthExpression) : type(type), id(id), lengthExpression(lengthExpression) {}
    // virtual void codeGen(CodeGenContext &context);
    void generateJson(cJSON *node)
    {
        // printf("VariableDeclarationNode\n");
        cJSON *arr = cJSON_CreateArray();
        cJSON *childnode = cJSON_CreateObject();
        childnode = cJSON_CreateObject();
        id->generateJson(childnode);
        cJSON_AddItemToArray(arr, childnode);

        childnode = cJSON_CreateObject();
        lengthExpression->generateJson(childnode);
        cJSON_AddItemToArray(arr, childnode);

        cJSON_AddItemToObject(node, "ArrayDeclarationNode", arr);
    }
    virtual void codeGen(CodeGenContext &context);
    virtual string getValue() { return nullptr; }
    virtual string getNodeType() { return "ArrayDeclarationNode"; }
};

class AddrNode : public ExpressionNode
{
public:
    shared_ptr<ArrayIndexNode> arrayIndex;
    shared_ptr<IdentifierNode> id;
    bool isArray = false;

    AddrNode() {}

    AddrNode(shared_ptr<ArrayIndexNode> index)
        : arrayIndex(index), isArray(true)
    {
    }

    AddrNode(shared_ptr<IdentifierNode> id_)
        : id(id_)
    {
    }

    void generateJson(cJSON *node)
    {
        // printf("AddrNode\n");
        if (isArray)
        {
            cJSON *arr = cJSON_CreateArray();
            cJSON *childnode = cJSON_CreateObject();
            arrayIndex->generateJson(childnode);
            cJSON_AddItemToArray(arr, childnode);
            cJSON_AddItemToObject(node, "AddrNode", arr);
        }
        else
        {
            cJSON *arr = cJSON_CreateArray();
            cJSON *childnode = cJSON_CreateObject();
            id->generateJson(childnode);
            cJSON_AddItemToArray(arr, childnode);
            cJSON_AddItemToObject(node, "AddrNode", arr);
        }
    }
    virtual void codeGen(CodeGenContext &context);
    virtual string getValue() { return nullptr; }
    virtual string getNodeType() { return "AddrNode"; }
};

#endif
