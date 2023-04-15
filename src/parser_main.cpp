#include "cjson/cJSON.h"
#include "AST.h"

#include <iostream>

extern FILE *yyin, *yyout;

extern int yyparse();

extern BlockNode* root;

int main(int argc, char ** argv)
{
	extern FILE *yyin;
	if (argc >= 2)
	{
		if ((yyin = fopen(argv[1], "r")) == NULL)
		{
			printf("Can't open file %s\n", argv[1]);
			return 1;
		}
	}
	yyparse();
	fclose(yyin);
	cJSON *json_root = NULL;
	json_root = cJSON_CreateObject();
	root->generateJson(json_root);
	char *out = cJSON_Print(json_root);
	printf("%s\n", out);

	return 0;
}