# CFL-Parser
Parser for a small context-free language. Detects syntax errors and multiple declaration errors (Error Code 1.1).

## Rules
program             -> scope\
scope               -> LBRACE scope_list RBRACE\
scope_list          -> scope\
scope_list          -> var_decl\
scope_list          -> stmt\
scope_list          ->scope scope_list\
scope_list          -> var_decl scope_list\
scope_list          -> stmt scope_list\
var_decl            -> id_list COLON type_name SEMICOLON\
id_list             -> ID\
id_list             -> ID id_list\
type_name           -> REAL | INT | BOOLEAN | STRING\
stmt_list           -> stmt\
stmt_list           -> stmt stmt_list\
stmt                -> assign_stmt\
stmt                -> while_stmt\
assign_stmt         -> ID EQUAL expr SEMICOLON\
while_stmt          -> WHILE condition LBRACE stmt_list RBRACE\
while_stmt          -> WHILE condition stmt\
expr                -> arithmetic_operator expr expr\
expr                -> boolean_operator expr expr\
expr                -> relational_operator expr expr\
expr                -> NOT expr\
expr                -> primary\
arithmetic_operator -> PLUS | MINUS | MULT | DIV\
boolean_operator    -> AND | OR | XOR\
relational_operator -> GREATER | GTEQ | LESS | LTEQ | NOTEQUAL\
primary             -> ID | NUM | REALNUM | STRING_CONSTANT | bool_const\
bool_const          -> TRUE | FALSE\
condition           -> LPAREN expr RPAREN

## Tokens
LBRACE:           {\
RBRACE:           }\
COLON:            :\
SEMICOLON:        ;\
REAL:             REAL\
INT:              INT\
BOOLEAN:          BOOLEAN\
STRING:           STRING\
WHILE:            WHILE\
COMMA:            ,\
LPAREN:           (\
RPAREN:           )\
EQUAL:            =\
PLUS:             +\
MULT:             * \
DIV:              /\
AND:              ^\
OR:               |\
XOR:              &\
NOT:              ~\
GREATER:          >\
GTEQ:             >=\
LESS:             <\
LTEQ:             <=\
NOTEQUAL:         <>\
TRUE:             TRUE\
FALSE:            FALSE\
STRING_CONSTANT:  (letter | digit)* \
ID:               letter (letter | digit)* \
NUM:              0 | (pdigit digit*)\
REALNUM:          NUM . digit digit*

where

letter: a | b | c | ... | z | A | B | C | ... | Z\
digit: 0 | 1 | 2 | ... | 9\
pdigit: 1 | 2 | 3 | ... | 9
