//
//  Andres Gandara
//  parser.h
//
//  Created by Andy Gandara on 3/30/18.
//  Copyright © 2018 Andy Gandara. All rights reserved.
//

#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <vector>
#include <map>
#include "lexer.h"

using namespace std;

typedef struct Scope {
    Scope* parent;
    map <string,bool> types;
    map <string,bool> variables;
    bool nested;
} Scope;
Scope* currentScope = NULL;
vector<Scope*> allScopes;

class Parser {
private:
    LexicalAnalyzer lexer;
    void syntax_error();
    Token expect(TokenType expected_type);
    Token peek();
    Token doublePeek();
    bool variable_exists(Scope*, string);
    void parse_program();
    void parse_scope();
    void parse_scope_list();
    void parse_type_name();
    void parse_var_decl();
    void parse_id_list();
    void parse_stmt_list();
    void parse_stmt();
    void parse_assign_stmt();
    void parse_while_stmt();
    void parse_expr();
    void parse_condition();
    void parse_primary();
    void parse_relational_operator();
    void parse_arithmetic_operator();
    void parse_boolean_operator();
    void parse_bool_const();
    
public:
    void begin_parsing();
};

#endif

