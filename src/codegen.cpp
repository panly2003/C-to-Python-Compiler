#include "AST.h"
#include "codegen.h"
#include "yacc.tab.hpp"

using namespace std;

void CodeGenContext::generateCode(BlockNode* root)
{
    cerr << "\n\n-----------------------Generate Process-----------------------\n\n" << endl;

    root->codeGen(*this);

    code << "if __name__ == '__main__':\n\tmain()" << endl;
}

// 检验同作用域中是否有某变量的声明
int CodeGenContext::judgeBlockRedefinition(string name)
{
    auto &block = table[table.size() - 1];
    for (int j = block.size() - 1; j >= 0; j--)
    {
        auto *decl = &block[j];
        if (decl->oldName == name)
        {
            return 1;
        }
    }
    return 0;
}

// 检验全局是否有某变量的声明
tableItem *CodeGenContext::judgeDefinition(string name)
{
    for (int i = table.size() - 1; i >= 0; i--)
    {
        auto &block = table[i];
        for (int j = block.size() - 1; j >= 0; j--)
        {
            auto *decl = &block[j];
            if (decl->oldName == name)
            {
                return decl;
            }
        }
    }
    return nullptr;
}

// 检验全局是否有某变量的重定义
tableItem *CodeGenContext::judgeRedefinition(string name)
{
    for (int i = table.size() - 2; i >= 0; i--)
    {
        auto &block = table[i];
        for (int j = block.size() - 1; j >= 0; j--)
        {
            auto *decl = &block[j];
            if (decl->newName == name)
            {
                return decl;
            }
        }
    }
    return nullptr;
}

