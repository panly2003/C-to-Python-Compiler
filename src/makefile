lexer:
	bison -d yacc.ypp
	flex -o lex.cpp lex.l
	g++ lex.cpp yacc.tab.cpp cjson/cJSON.c -o lexer
parser:
	bison -d yacc.ypp
	flex -o lex.cpp lex.l
	g++ lex.cpp yacc.tab.cpp codegen.cpp AST.cpp parser_main.cpp cjson/cJSON.c -o parser 

code:
	bison -d yacc.ypp
	flex -o lex.cpp lex.l
	g++ main.cpp yacc.tab.cpp lex.cpp cjson/cJSON.c codegen.cpp AST.cpp -o main 

clean:
	rm yacc.tab.cpp yacc.tab.hpp lexer parser lex.cpp *.o bubbleSort palindrome_string main