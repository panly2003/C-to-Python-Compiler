%{
#include "stdio.h"
#include "AST.h"
#include "yacc.tab.hpp"

%}

%option noyywrap

/* regular definitions */
delim       [\t\n\r \.\.\.]
ws          {delim}+
letter      [A-Za-z]
digit       [0-9]
id          ({letter}|_)(({letter}|_)|{digit})*
string      (\"(.)*\")|(\'.\')

%%

{ws}        {  }
"if"          { return(IF); }
"extern"      { return(EXTERN);}
"for"         { return(FOR);}
"return"      { return(RETURN);}
";"           { return(SEMI);}
","           { return(COMMA);}
"("           { return(LSBORDER);}
")"           { return(RSBORDER);}
"["           { return(LMBORDER);}
"]"           { return(RMBORDER);}
"{"           { return(LLBORDER);}
"}"           { return(RLBORDER);}
"int"         { return(INT);}
"char"        { return(CHAR);}
{id}         { yylval.stringValue = strdup(yytext); return(ID); }
[0-9]+      { yylval.intValue = atoi(yytext); return(INTEGER); }
{string}    { yylval.stringValue = strdup(yytext); return(STRING); }
"<"         { return(LT); }
"<="        { return(LE); }
">"         { return(GT); }
">="        { return(GE); }
"!="        { return(NEQUAL); }
"="         { return(GVAL); }
"+"         { return(PLUS); }
"-"         { return(MINUS); }
"*"         { return(MUL); }
"/"         { return(DIV); }
"&"         { return(ADDR);}
.           { yylval.stringValue = strdup(yytext); return(INVALID);}

%%


// void writeout(int c){
//     switch(c){
//         case IF: fprintf(yyout, "(IF, \"%s\")\n", yytext);break;
//         case EXTERN: fprintf(yyout, "(EXTERN, \"%s\")\n", yytext);break;
//         case FOR: fprintf(yyout, "(FOR, \"%s\")\n", yytext);break;
//         case RETURN: fprintf(yyout, "(RETURN, \"%s\")\n", yytext);break;
//         case SEMI: fprintf(yyout, "(SEMI, \"%s\")\n", yytext);break;
//         case COMMA: fprintf(yyout, "(COMMA, \"%s\")\n", yytext);break;
//         case LSBORDER: fprintf(yyout, "(LSBORDER, \"%s\")\n", yytext);break;
//         case RSBORDER: fprintf(yyout, "(RSBORDER, \"%s\")\n", yytext);break;
//         case LMBORDER: fprintf(yyout, "(LMBORDER, \"%s\")\n", yytext);break;
//         case RMBORDER: fprintf(yyout, "(RMBORDER, \"%s\")\n", yytext);break;
//         case LLBORDER: fprintf(yyout, "(LLBORDER, \"%s\")\n", yytext);break;
//         case RLBORDER: fprintf(yyout, "(RLBORDER, \"%s\")\n", yytext);break;
//         case INT: fprintf(yyout, "(INT, \"%s\")\n", yytext);break;
//         case CHAR: fprintf(yyout, "(CHAR, \"%s\")\n", yytext);break;
//         case CHARPOINTER: fprintf(yyout, "(CHARPOINTER, \"%s\")\n", yytext);break;
//         case ID: fprintf(yyout, "(ID, \"%s\")\n", yytext);break;
//         case INTEGER: fprintf(yyout, "(INTEGER, \"%s\")\n", yytext);break;
//         case STRING: fprintf(yyout, "(STRING, \"%s\")\n", yytext);break;
//         case LT: fprintf(yyout, "(LT, \"%s\")\n", yytext);break;
//         case LE: fprintf(yyout, "(LE, \"%s\")\n", yytext);break;
//         case GT: fprintf(yyout, "(GT, \"%s\")\n", yytext);break;
//         case GE: fprintf(yyout, "(GE, \"%s\")\n", yytext);break;
//         case NEQUAL: fprintf(yyout, "(NEQUAL, \"%s\")\n", yytext);break;
//         case GVAL: fprintf(yyout, "(GVAL, \"%s\")\n", yytext);break;
//         case PLUS: fprintf(yyout, "(PLUS, \"%s\")\n", yytext);break;
//         case MINUS: fprintf(yyout, "(MINUS, \"%s\")\n", yytext);break;
//         case MUL: fprintf(yyout, "(MUL, \"%s\")\n", yytext);break;
//         case DIV: fprintf(yyout, "(DIV, \"%s\")\n", yytext);break;
//         case ADDR: fprintf(yyout, "(ADDR, \"%s\")\n", yytext);break;
//         case INVALID: fprintf(yyout, "(ERROR!!!, \"%s\")\n", yytext);break;
//         default: break;
//     }
//     return;
// }

// int main (int argc, char ** argv){
//     int c;
//     if (argc>=2){
//         if ((yyin = fopen(argv[1], "r")) == NULL){
//             printf("Can't open file %s\n", argv[1]);
//             return 1;
//         }
//         if (argc>=3){
//             yyout=fopen(argv[2], "w");
//         }
//     }

//     while ((c = yylex()) != 0) {
//         writeout(c);
//     }

//     if(argc>=2){
//         fclose(yyin);
//         if (argc>=3) fclose(yyout);
//     }
//     return 0;
// }
