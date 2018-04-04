//
//  Andres Gandara
//  parser.cc
//
//  Created by Andy Gandara on 3/30/18.
//  Copyright © 2018 Andy Gandara. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include "parser.h"

using namespace std;

// Recursively checks that the variable exists in the scope
bool Parser::variable_exists(Scope* scope,string str) {
    if (scope->variables.find(str) == scope->variables.end()) // variable string not found
    {
        if (scope->parent != NULL) // is nested scope
            return variable_exists(scope->parent, str); // checks parent recursively for variable
        else
            return false; // not found in any parents, so returns false
    }
    else // variable found
        return true;
}

void Parser::syntax_error()
{
    cout << "Syntax Error\n";
    exit(1);
}

Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

// Peeks token by getting and ungetting token
Token Parser::peek()
{
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    return t;
}

// Peek second token
Token Parser::doublePeek()
{
    Token t1 = lexer.GetToken();
    Token t2 = lexer.GetToken();
    lexer.UngetToken(t2);
    lexer.UngetToken(t1);
    return t2;
}

// Parsing

void Parser::parse_program()
{
    // program -> scope
    
    // TODO
    Token t = peek();
    if (t.token_type == LBRACE)
        parse_scope();
    else syntax_error();
}

void Parser::parse_scope()
{
     // scope -> LBRACE scope_list RBRACE
    
    // TODO
    Scope* update_scope = new Scope;
    update_scope->parent = currentScope;
    currentScope = update_scope;
    allScopes.push_back(update_scope);
    expect(LBRACE);
    parse_scope_list();
    expect(RBRACE);
    currentScope = currentScope->parent;
}


void Parser::parse_scope_list()
{
    // scope_list -> scope | var_decl | stmt | scope scope_list | var_decl scope_list | stmt scope_list
    
    // TODO
    Token t = peek();
    Token temp = lexer.GetToken();
    Token t1 = peek();
    lexer.UngetToken(temp);

    // scope
    if (t.token_type == LBRACE)
        parse_scope();

    // stmt
    else if (t.token_type == WHILE || (t.token_type == ID && t1.token_type == EQUAL))
        parse_stmt();

    // var_decl
    else if (t.token_type == ID && t1.token_type != EQUAL)
        parse_var_decl();

    else
        syntax_error();
    
    // scope scope_list || var_decl scope_list || stmt scope_list
    Token t3 = peek();
    if (t3.token_type == LBRACE || t3.token_type == ID || t3.token_type == WHILE)
        parse_scope_list();
}

void Parser::parse_var_decl()
{
    // var_decl -> id_list COLON type_name SEMICOLON
    
    // TODO
    parse_id_list();
    expect(COLON);
    parse_type_name();
    expect(SEMICOLON);
}

void Parser::parse_id_list()
{
    // id_list -> ID | ID COMMA id_list
    
    // TODO
    Token t = peek();
    Token temp = lexer.GetToken();//doublePeek();
    Token t1 = peek();
    lexer.UngetToken(temp);
    if (currentScope->variables.find(t.lexeme) != currentScope->variables.end()) // ID found in scope -- double declaration
    {
        while (peek().token_type == ID || peek().token_type == COMMA){
            lexer.GetToken();
        }
        expect(COLON);
        lexer.GetToken(); // move past type
        expect(SEMICOLON);
        cout << "ERROR CODE 1.1 " << t.lexeme << endl;
        exit(0);
    }
    else // ID not found in current scope -- proceeds to parse ID list
    {
        //currentScope->variables[t.lexeme] = 1;
        currentScope->variables[t.lexeme] = 0;
        if (t.token_type == ID && t1.token_type != COMMA){
            expect(ID);
        }
        else if (t.token_type == ID && t1.token_type == COMMA){
            expect(ID);
            expect(COMMA);
            parse_id_list();
        }
        else syntax_error();
    }
}

void Parser::parse_type_name()
{
    // type_name -> REAL | INT | BOOLEAN | STRING
    
    // TODO
    Token t = peek();
    if (t.token_type == REAL || t.token_type == INT || t.token_type == BOOLEAN || t.token_type == STRING)
    {
        lexer.GetToken();
        return;
    }
    else syntax_error();
}

void Parser::parse_stmt_list()
{
    // stmt_list -> stmt | stmt stmt_list
    
    // TODO
    Token t = peek();
    if (t.token_type == WHILE || t.token_type == ID)
        parse_stmt();
    else syntax_error();
    
    Token t1 = peek();
    if (t1.token_type == WHILE || t1.token_type == ID)
        parse_stmt_list();
}

void Parser::parse_stmt()
{
    // stmt -> assign_stmt | while_stmt
    
    // TODO
    Token t = peek();
    Token temp = lexer.GetToken();
    Token t1 = peek();
    lexer.UngetToken(temp);
    if (t.token_type == ID && t1.token_type == EQUAL)
        parse_assign_stmt();
    else if (t.token_type == WHILE && t1.token_type == LPAREN)
        parse_while_stmt();
    else syntax_error();
}

void Parser::parse_assign_stmt()
{
    // assign_stmt -> ID EQUAL expr SEMICOLON
    
    // TODO
    Token t = peek();
    expect(ID);
    expect(EQUAL);
    t = peek();
    if (t.token_type == ID && !variable_exists(currentScope, t.lexeme)) // Token is ID and ID not found in scope
    {
        lexer.GetToken();
        expect(SEMICOLON);
        cout << "ERROR CODE 1.2 " << t.lexeme << endl;
        exit(0);
    }
    parse_expr();
    expect(SEMICOLON);
}

void Parser::parse_while_stmt()
{
    // while_stmt -> WHILE condition LBRACE stmt_list RBRACE | WHILE condition stmt
    
    // TODO
    expect(WHILE);
    parse_condition();
    Token t = peek();
    if (t.token_type == LBRACE)
    {
        expect(LBRACE);
        parse_stmt_list();
        expect(RBRACE);
    }
    
    else if (t.token_type == ID || t.token_type == WHILE)
        parse_stmt();
    
    else syntax_error();
}

void Parser::parse_expr()
{
    // expr -> arithmetic_operator expr expr | boolean_operator expr expr | relational_operator expr expr | NOT expr | primary
    
    // TODO
    Token t = peek();
    if (t.token_type == PLUS || t.token_type == MINUS || t.token_type == MULT || t.token_type == DIV)
    {
        parse_arithmetic_operator();
        parse_expr();
        parse_expr();
    }
    else if (t.token_type == AND || t.token_type == OR || t.token_type == XOR)
    {
        parse_boolean_operator();
        parse_expr();
        parse_expr();
    }
    else if (t.token_type == GREATER || t.token_type == GTEQ || t.token_type == LESS || t.token_type == LTEQ || t.token_type == NOTEQUAL)
    {
        parse_relational_operator();
        parse_expr();
        parse_expr();
    }
    else if (t.token_type == NOT)
    {
        expect(NOT);
        parse_expr();
    }
    else if (t.token_type == ID || t.token_type == NUM || t.token_type == REALNUM || t.token_type == STRING_CONSTANT || t.token_type == TRUE || t.token_type == FALSE)
        parse_primary();
    else syntax_error();
}

void Parser::parse_arithmetic_operator()
{
    // arop -> PLUS | MINUS  | MULT | DIV
    
    // TODO
    Token t = lexer.GetToken();
    if (t.token_type == PLUS || t.token_type == MINUS || t.token_type == MULT || t.token_type == DIV)
        return;
    else syntax_error();
}

void Parser::parse_boolean_operator()
{
    // boolop -> AND | OR | XOR
    
    // TODO
    Token t = lexer.GetToken();
    if (t.token_type == AND || t.token_type == OR || t.token_type == XOR)
        return;
    else syntax_error();
}

void Parser::parse_relational_operator()
{
    // relop -> GREATER | GTEQ | LESS | LTEQ | NOTEQUAL
    
    // TODO
    Token t = lexer.GetToken();
    if (t.token_type == GREATER || t.token_type == GTEQ || t.token_type == LESS || t.token_type == NOTEQUAL || t.token_type == LTEQ)
        return;
    else syntax_error();
}

void Parser::parse_primary()
{
    // primary -> ID | NUM | REALNUM | STRING_CONSTANT | bool_constant
    
    // TODO
    Token t = lexer.GetToken();
    
    if (t.token_type == ID || t.token_type == NUM || t.token_type == REALNUM || t.token_type == STRING_CONSTANT)
        return;
    
    else if (t.token_type == TRUE || t.token_type == FALSE)
    {
        lexer.UngetToken(t);
        parse_bool_const();
    }
    
    else syntax_error();
}

void Parser::parse_bool_const()
{
    // bool_const -> TRUE | FALSE
    
    // TODO
    Token t = lexer.GetToken();
    if (t.token_type == TRUE || t.token_type ==  FALSE)
        return;
    else syntax_error();
}

void Parser::parse_condition()
{
    // condition -> LPAREN expr RPAREN
    
    // TODO
    expect(LPAREN);
    parse_expr();
    expect(RPAREN);
}

void Parser::begin_parsing()
{
    // TODO
    Token t = peek();
    if (t.token_type == LBRACE)
        parse_program();
    else syntax_error();}

int main()
{
    Parser parser;
    parser.begin_parsing();
    return 0;
}
