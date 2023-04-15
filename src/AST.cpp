#include "AST.h"
#include "codegen.h"
#include "yacc.tab.hpp"

void IntegerNode::codeGen(CodeGenContext &context)
{
    cerr << "Creating Python Code for: " << this->getNodeType() << endl;
    context.code << value;
}

void IdentifierNode::codeGen(CodeGenContext &context)
{
    cerr << "Creating Python Code for: " << this->getNodeType() << endl;
    // 检查之前是否定义过
    auto oldDeclaration = context.judgeDefinition(name);
    if (oldDeclaration == nullptr)
    {
        context.errorMsg.push_back("Identifier \"" + name + "\" is undefined");
    }
    else
    {
        context.code << oldDeclaration->newName;
    }
}

void CallFuncNode::codeGen(CodeGenContext &context)
{
    cerr << "Creating Python Code for: " << this->getNodeType() << endl;
    // printf函数
    if (id->name == "printf")
    {
        if (arguments->size() == 1)
        {
            context.code << "print" << '(';
            auto item = arguments->begin();
            (*item)->codeGen(context);
            context.code << ')';
        }
        else
        {
            context.code << "print" << '(';
            auto item = arguments->begin();
            (*item)->codeGen(context);
            context.code << " %";
            context.code << " (";
            for (item = arguments->begin() + 1; item < arguments->end(); item++)
            {
                (*item)->codeGen(context);
                context.code << ", ";
            }
            context.code << ')';
            context.code << ')';
        }
        return;
    }
    // scanf函数
    else if (id->name == "scanf")
    {
        auto second = arguments->begin() + 1;
        (*second)->codeGen(context);
        context.code << " = ";
        auto first = arguments->begin();
        string format = (*first)->getValue();
        ;
        if (format.compare("%d") == 0)
        {
            context.code << "eval(input())";
        }
        else
        {
            context.code << "input()";
        }
        return;
    }

    context.code << id->name << "(";
    ExpressionList::const_iterator it;
    for (it = arguments->begin(); it != arguments->end(); it++)
    {
        if (it != arguments->begin())
            context.code << ", ";
        (*it)->codeGen(context);
    }
    context.code << ")";
}

void AddrNode::codeGen(CodeGenContext &context)
{
    cerr << "Creating Python Code for: " << this->getNodeType() << endl;
    if (isArray == true)
    {
        arrayIndex->codeGen(context);
    }
    else
    {
        id->codeGen(context);
    }
}

void AssignmentNode::codeGen(CodeGenContext &context)
{
    cerr << "Creating Python Code for: " << this->getNodeType() << endl;
    lhs->codeGen(context);
    context.code << " = ";
    rhs->codeGen(context);
}

void ArrayAssignmentNode::codeGen(CodeGenContext &context)
{
    cerr << "Creating Python Code for: " << this->getNodeType() << endl;
    arrayIndex->codeGen(context);
    context.code << " = ";
    expression->codeGen(context);
}

void LiteralNode::codeGen(CodeGenContext &context)
{
    cerr << "Creating Python Code for: " << this->getNodeType() << endl;
    context.code << "\"" << value << "\"";
}

void BlockNode::codeGen(CodeGenContext &context)
{
    cerr << "Creating Python Code for: " << this->getNodeType() << endl;
    context.indent++;
    context.table.push_back(vector<tableItem>());

    StatementList::const_iterator it;
    for (it = statements->begin(); it != statements->end(); it++)
    {
        context.code << string(context.indent, '\t');
        (*it)->codeGen(context);
    }

    if (statements->size() == 0)
    {
        context.code << string(context.indent, '\t') << "pass" << endl;
    }

    context.table.pop_back();
    context.indent--;
}

void ExpressionStatementNode::codeGen(CodeGenContext &context)
{
    cerr << "Creating Python Code for: " << this->getNodeType() << endl;
    expression->codeGen(context);
    context.code << endl;
}

void BinaryOperatorNode::codeGen(CodeGenContext &context)
{
    cerr << "Creating Python Code for: " << this->getNodeType() << endl;
    context.code << "(";
    lhs->codeGen(context);
    int flag = 0;
    switch (op)
    {
    case 1:
        context.code << " + ";
        break;
    case 2:
        context.code << " - ";
        break;
    case 3:
        context.code << " * ";
        break;
    case 4:
        context.code << " / ";
        break;
    case 5:
        context.code << " < ";
        flag = 1;
        break;
    case 6:
        context.code << " <= ";
        flag = 1;
        break;
    case 7:
        context.code << " > ";
        flag = 1;
        break;
    case 8:
        context.code << " >= ";
        flag = 1;
        break;
    case 9:
        context.code << " != ";
        flag = 1;
        break;
    }

    rhs->codeGen(context);
    context.code << ")";
    if (flag == 1)
    {
        context.code << ":";
    }
}

void VariableDeclarationNode::codeGen(CodeGenContext &context)
{
    cerr << "Creating Python Code for: " << this->getNodeType() << endl;

    // 变量声明时的重命名检测与处理:
    // 如果同一个作用域出现重复，则报错
    // 如果和全局出现重复，则新起一个名字存入符号表
    // 如果没有重复，直接存入符号表
    if (context.judgeBlockRedefinition(id->name))
    {
        context.errorMsg.push_back("Redefinition of identifier \"" + id->name + "\"");
        return;
    }
    auto &block = context.table[context.table.size() - 1];
    auto oldDeclaration = context.judgeRedefinition(id->name);
    string newName = id->name;
    if (oldDeclaration != nullptr)
    {
        newName = id->name + '_' + to_string(context.table.size() - 1);
    }
    block.push_back(tableItem{newName, id->name, type, &id});
    if (assignmentExpr != nullptr)
    {
        auto var = new IdentifierNode(id->name);
        AssignmentNode tmp(shared_ptr<IdentifierNode>(var), assignmentExpr);
        tmp.codeGen(context);
    }
    else
    {
        context.code << newName << " = None";
    }
    context.code << endl;
}


void FunctionDeclarationNode::codeGen(CodeGenContext &context)
{
    cerr << "Creating Python Code for: " << this->getNodeType() << endl;

    // 检查函数之前是否被定义过
    for (int i = 0; i < context.funcDeclaration.size(); i++)
    {
        if (context.funcDeclaration[i] == id->name)
        {
            context.errorMsg.push_back("Redefinition of function \"" + id->name + "\"");
            return;
        }
    }

    context.funcDeclaration.push_back(id->name);
    context.code << "def " << id->name << "(";

    context.table.push_back(vector<tableItem>());
    VariableList::const_iterator it;
    // 检查函数参数的重名，如果重名则赋予new Name
    for (it = arguments->begin(); it != arguments->end(); it++)
    {
        auto block = context.table[context.table.size() - 1];
        auto oldDeclaration = context.judgeRedefinition((*it)->id->name);
        string newName = (*it)->id->name;
        if (oldDeclaration != nullptr)
        {
            newName = (*it)->id->name + '_' + to_string(context.table.size() - 1);
        }
        block.push_back(tableItem{newName, (*it)->id->name, (*it)->type, &(*it)->id});

        if (it != arguments->begin())
            context.code << ", ";
        context.code << newName;
    }
    context.code << "):" << endl;

    auto &firstBlock = context.table[0];
    if (firstBlock.size())
    {
        context.code << "\tglobal ";
        for (int i = 0; i < firstBlock.size(); i++)
        {
            if (i)
                context.code << ", ";
            context.code << firstBlock[i].oldName;
        }
        context.code << endl;
    }

    block->codeGen(context);
    context.table.pop_back();
    context.code << endl;
}

void IfStatementNode::codeGen(CodeGenContext &context)
{
    cerr << "Creating Python Code for: " << this->getNodeType() << endl;

    context.code << "if ";
    condition->codeGen(context);
    context.code << endl;

    trueBlock->codeGen(context);

    if (falseBlock != nullptr)
    {
        context.code << string(context.indent, '\t') << "else:" << endl;
        falseBlock->codeGen(context);
    }
}

void ForStatementNode::codeGen(CodeGenContext &context)
{
    cerr << "Creating Python Code for: " << this->getNodeType() << endl;

    context.table.push_back(vector<tableItem>());
    if (initial != nullptr)
    {
        initial->codeGen(context);
    }

    context.code << string(context.indent, '\t');
    context.code << "while ";
    condition->codeGen(context);

    block->codeGen(context);

    context.indent++;
    context.code << string(context.indent, '\t');
    increment->codeGen(context);
    context.code << endl;
    context.indent--;
    context.table.pop_back();
}

void ReturnStatementNode::codeGen(CodeGenContext &context)
{
    cerr << "Creating Python Code for: " << this->getNodeType() << endl;

    context.code << "return ";
    if (expression)
    {
        expression->codeGen(context);
    }
    context.code << endl;
}

void ArrayDeclarationNode::codeGen(CodeGenContext &context)
{
    cerr << "Creating Python Code for: " << this->getNodeType() << endl;

    // 检查定义与重定义
    auto &block = context.table[context.table.size() - 1];
    if (context.judgeBlockRedefinition(id->name))
    {
        context.errorMsg.push_back(": Redefinition of identifier \"" + id->name + "\"");
        return;
    }
    auto oldDeclaration = context.judgeRedefinition(id->name);
    string newName = id->name;
    if (oldDeclaration != nullptr)
    {
        newName = id->name + '_' + to_string(context.indent);
    }
    block.push_back(tableItem{newName, id->name, type, &id});
    context.code << newName << " = [None] * ";
    lengthExpression->codeGen(context);
    context.code << endl;
}

void ArrayIndexNode::codeGen(CodeGenContext &context)
{
    cerr << "Creating Python Code for: " << this->getNodeType() << endl;

    arrayName->codeGen(context);
    context.code << "[";
    expression->codeGen(context);
    context.code << "]";
}