#include "cjson/cJSON.h"
#include "AST.h"
#include "codegen.h"

#include <iostream>

extern FILE *yyin, *yyout;

extern int yyparse();

extern BlockNode *root;

using namespace std;

int main(int argc, char **argv)
{
	extern FILE *yyin;
	if (argc >= 2)
	{
		if ((yyin = fopen(argv[1], "r")) == NULL)
		{
			printf("Can't open file %s\n", argv[1]);
			return 1;
		}
        freopen("test.py", "w", stdout);
	}
	yyparse();
	fclose(yyin);

    CodeGenContext context;
    context.generateCode(root);

	// 输出编译结果

    if (context.errorMsg.size() != 0)
    {
        cerr << "Error occurred" << endl;
        for (auto msg : context.errorMsg)
        {
            cerr << msg << endl;
        }
        fclose(stdin);
        fclose(stdout);
        return -1;
    }
    
    cerr << "\n\n-----------------------Success!-----------------------\n\n" << endl;
    cout << context.code.str() << endl;

    fclose(stdin);
    fclose(stdout);

	return 0;
}