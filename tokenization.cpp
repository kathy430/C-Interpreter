/*
    Implementation of the Tokenization class
    by: Owen, Roy, Kathy, Kyle

    Description: This file contains the implementation of
    the Tokenization class functions declared in the header
    file.
*/

#include <fstream>
#include <iostream>

#include "tokenization.hpp"

/*
    This enumerated class contains the Backus-Naur form
    tokens.
*/
enum class BNF {
    START,
    ESCAPED_CHARACTER,
    L_PAREN,
    R_PAREN,
    L_BRACKET,
    R_BRACKET,
    L_BRACE,
    R_BRACE,
    DOUBLE_QUOTE,
    SINGLE_QUOTE,
    SEMICOLON,
    COMMA,
    ASSIGNMENT_OPERATOR,
    PLUS,
    MINUS,
    ASTERISK,
    DIVIDE,
    MODULO,
    CARET,
    LT,
    GT,
    LT_EQUAL,
    GT_EQUAL,
    BOOLEAN_AND,
    BOOLEAN_OR,
    BOOLEAN_NOT,
    BOOLEAN_EQUAL,
    BOOLEAN_NOT_EQUAL,
    STRING,
    INTEGER,
    IDENTIFIER
};

/*
    The default constructor initializes the variables declared
    in the header file.
*/
Tokenization::Tokenization() {
    token.type = "";
    token.value = "";
    invalidToken = false;
    invalidType = "";
    errorLineNumber = 0;
}

/*
    This function tokenizes the input file and stores the tokens
    in a vector.
*/
void Tokenization::tokenize(const std::string &inputFilename) {
    std::ifstream inFile(inputFilename);

    // check if input file exists
    if (!inFile) {
        std::cout << "Error: Unable to open input file to tokenize." << std::endl;
        return;
    }

    // check if input file is empty
    if (inFile.peek() == EOF) {
        std::cout << "Input file is empty." << std::endl;
        return;
    }

    // set start state
    BNF currentState = BNF::START;

    // variables needed for switch case
    char currentChar;
    bool doubleQuoteStart = true;
    bool singleQuoteStart = true;
    char endingQuote;
    int lineNumber = 1;
    
    // get tokens 
    while (inFile.get(currentChar) && !invalidToken) {
        if (currentChar == '\n') {
            lineNumber++;
            continue;
        }
        else if (currentChar == ' ') {
            continue; // skip spaces
        }
    
        switch (currentState) {
            case BNF::START:
                // determine state based on current character
                if (currentChar == '\\') {
                    currentState = BNF::ESCAPED_CHARACTER;
                }
                else if (currentChar == '(') {
                    currentState = BNF::L_PAREN;
                }
                else if (currentChar == ')') {
                    currentState = BNF::R_PAREN;
                }
                else if (currentChar == '[') {
                    currentState = BNF::L_BRACKET;
                }
                else if (currentChar == ']') {
                    currentState = BNF::R_BRACKET;
                }
                else if (currentChar == '{') {
                    currentState = BNF::L_BRACE;
                }
                else if (currentChar == '}') {
                    currentState = BNF::R_BRACE;
                }
                else if (currentChar == '\"') {
                    currentState = BNF::DOUBLE_QUOTE;
                }
                else if (currentChar == '\'') {
                    currentState = BNF::SINGLE_QUOTE;
                }
                else if (currentChar == ';') {
                    currentState = BNF::SEMICOLON;
                }
                else if (currentChar == ',') {
                    currentState = BNF::COMMA;
                }
                else if (currentChar == '=') {
                    if (inFile.peek() == '=') {
                        currentState = BNF::BOOLEAN_EQUAL;
                    }
                    else {
                        currentState = BNF::ASSIGNMENT_OPERATOR;
                    }
                }
                else if (currentChar == '+') {
                    if (isdigit(inFile.peek())) {
                        currentState = BNF::INTEGER;
                    }
                    else {
                        currentState = BNF::PLUS;
                    }
                }
                else if (currentChar == '-') {
                    if (isdigit(inFile.peek())) {
                        currentState = BNF::INTEGER;
                    }
                    else {
                        currentState = BNF::MINUS;
                    }
                }
                else if (currentChar == '*') {
                    currentState = BNF::ASTERISK;
                }
                else if (currentChar == '/') {
                    currentState = BNF::DIVIDE;
                }
                else if (currentChar == '%') {
                    currentState = BNF::MODULO;
                }
                else if (currentChar == '^') {
                    currentState = BNF::CARET;
                }
                else if (currentChar == '<') {
                    if (inFile.peek() == '=') {
                        currentState = BNF::LT_EQUAL;
                    }
                    else {
                        currentState = BNF::LT;
                    }
                }
                else if (currentChar == '>') {
                    if (inFile.peek() == '=') {
                        currentState = BNF::GT_EQUAL;
                    }
                    else {
                        currentState = BNF::GT;
                    }
                }
                else if (currentChar == '&') {
                    if (inFile.peek() == '&') {
                        currentState = BNF::BOOLEAN_AND;
                    }
                }
                else if (currentChar == '|') {
                    if (inFile.peek() == '|') {
                        currentState = BNF::BOOLEAN_OR;
                    }
                }
                else if (currentChar == '!') {
                    if (inFile.peek() == '=') {
                        currentState = BNF::BOOLEAN_NOT_EQUAL;
                    }
                    else {
                        currentState = BNF::BOOLEAN_NOT;
                    }
                }
                else if (isdigit(currentChar)) {
                    currentState = BNF::INTEGER;
                }
                else if (isalpha(currentChar)) {
                    currentState = BNF::IDENTIFIER;
                }
                else {
                    currentState = BNF::START;
                }
                
                // return character (if not space) for updated switch case
                if (!isspace(currentChar)) {
                    token.value = ""; // clear token value
                    inFile.putback(currentChar);
                }
                
                
                break;
            case BNF::ESCAPED_CHARACTER:
                token.type = "ESCAPED_CHARACTER";
                while (!isspace(currentChar)) {
                    token.value += currentChar;
                    inFile.get(currentChar);
                }
                currentState = BNF::START;
                break;
            case BNF::L_PAREN:
                token.type = "LEFT_PARENTHESIS";
                token.value = currentChar;
                currentState = BNF::START;
                break;
            case BNF::R_PAREN:
                token.type = "RIGHT_PARENTHESIS";
                token.value = currentChar;
                currentState = BNF::START;
                break;
            case BNF::L_BRACKET:
                token.type = "LEFT_BRACKET";
                token.value = currentChar;
                currentState = BNF::START;
                break;
            case BNF::R_BRACKET:
                token.type = "RIGHT_BRACKET";
                token.value = currentChar;
                currentState = BNF::START;
                break;
            case BNF::L_BRACE:
                token.type = "LEFT_BRACE";
                token.value = currentChar;
                currentState = BNF::START;
                break;
            case BNF::R_BRACE:
                token.type = "RIGHT_BRACE";
                token.value = currentChar;
                currentState = BNF::START;
                break;
            case BNF::DOUBLE_QUOTE:
                token.type = "DOUBLE_QUOTE";
                token.value = currentChar;

                // determine if starting quote
                if (doubleQuoteStart) {
                    doubleQuoteStart = false;
                    // read in string
                    currentState = BNF::STRING;
                }
                else {
                    doubleQuoteStart = true;
                    currentState = BNF::START;
                }
                break;
            case BNF::SINGLE_QUOTE:
                token.type = "SINGLE_QUOTE";
                token.value = currentChar;

                // determine if starting quote
                if (singleQuoteStart) {
                    singleQuoteStart = false;
                    // read in string
                    currentState = BNF::STRING;
                }
                else {
                    singleQuoteStart = true;
                    currentState = BNF::START;
                }
                break;
            case BNF::SEMICOLON:
                token.type = "SEMICOLON";
                token.value = currentChar;
                currentState = BNF::START;
                break;
            case BNF::COMMA:
                token.type = "COMMA";
                token.value = currentChar;
                currentState = BNF::START;
                break;
            case BNF::ASSIGNMENT_OPERATOR:
                token.type = "ASSIGNMENT";
                token.value = currentChar;
                currentState = BNF::START;
                break;
            case BNF::PLUS:
                token.type = "PLUS";
                token.value = currentChar;
                currentState = BNF::START;
                break;
            case BNF::MINUS:
                token.type = "MINUS";
                token.value = currentChar;
                currentState = BNF::START;
                break;
            case BNF::ASTERISK:
                token.type = "ASTERISK";
                token.value = currentChar;
                currentState = BNF::START;
                break;
            case BNF::DIVIDE:
                token.type = "DIVIDE";
                token.value = currentChar;
                currentState = BNF::START;
                break;
            case BNF::MODULO:
                token.type = "MODULO";
                token.value = currentChar;
                currentState = BNF::START;
                break;
            case BNF::CARET:
                token.type = "CARET";
                token.value = currentChar;
                currentState = BNF::START;
                break;
            case BNF::LT:
                token.type = "LESS_THAN";
                token.value = currentChar;
                currentState = BNF::START;
                break;
            case BNF::GT:
                token.type = "GREATER_THAN";
                token.value = currentChar;
                currentState = BNF::START;
                break;
            case BNF::LT_EQUAL:
                token.type = "LESS_THAN_OR_EQUAL";
                token.value += currentChar;
                // get =
                inFile.get(currentChar);
                token.value += currentChar;
                currentState = BNF::START;
                break;
            case BNF::GT_EQUAL:
                token.type = "GREATER_THAN_OR_EQUAL";
                token.value += currentChar;
                // get =
                inFile.get(currentChar);
                token.value += currentChar;
                currentState = BNF::START;
                break;
            case BNF::BOOLEAN_AND:
                token.type = "BOOLEAN_AND";
                token.value += currentChar;
                // get second &
                inFile.get(currentChar);
                token.value += currentChar;
                currentState = BNF::START;
                break;
            case BNF::BOOLEAN_OR:
                token.type = "BOOLEAN_OR";
                token.value += currentChar;
                // get second |
                inFile.get(currentChar);
                token.value += currentChar;
                currentState = BNF::START;
                break;
            case BNF::BOOLEAN_NOT:
                token.type = "BOOLEAN_NOT";
                token.value = currentChar;
                currentState = BNF::START;
                break;
            case BNF::BOOLEAN_EQUAL:
                token.type = "BOOLEAN_EQUAL";
                token.value += currentChar;
                // get second equals sign
                inFile.get(currentChar);
                token.value += currentChar;
                currentState = BNF::START;
                break;
            case BNF::BOOLEAN_NOT_EQUAL:
                token.type = "NOT_EQUAL";
                token.value += currentChar;
                // get equal sign
                inFile.get(currentChar);
                token.value += currentChar;
                currentState = BNF::START;
                break;
            case BNF::STRING:
                token.type = "STRING";
                
                // set ending quote based on starting quote
                if (!doubleQuoteStart) {
                    endingQuote = '"';
                }
                else if (!singleQuoteStart) {
                    endingQuote = '\'';
                }
                
                // clear token value that holds beginning quote
                token.value = "";
                
                while (currentChar != endingQuote) {
                    token.value += currentChar;

                    // if there is an escaped quote get it
                    if (currentChar == '\\') {
                        inFile.get(currentChar);
                        token.value += currentChar;
                    }

                    // if we reach eof then there is an error
                    if (inFile.peek() == EOF) {
                        invalidToken = true;
                        invalidType = "string";
                        errorLineNumber = lineNumber;
                        break;
                    }
                    
                    inFile.get(currentChar);
                }
                // put back the ending quote
                inFile.putback(currentChar);
                
                // change state back to quote type
                if (!doubleQuoteStart) {
                    currentState = BNF::DOUBLE_QUOTE;
                }
                else if (!singleQuoteStart) {
                    currentState = BNF::SINGLE_QUOTE;
                }
                break;
            case BNF::INTEGER:
                token.type = "INTEGER";
                if (currentChar == '-') {
                    token.value += currentChar;
                    inFile.get(currentChar);
                }
                else if (currentChar == '+') {
                    inFile.get(currentChar);
                }
                
                while (isdigit(currentChar)) {
                    token.value += currentChar;
                    inFile.get(currentChar);
                }
                // if while loop ends on a char
                if (isalpha(currentChar)) {
                    invalidToken = true;
                    invalidType = "integer";
                    errorLineNumber = lineNumber;
                }
                // put back char that isn't digit
                inFile.putback(currentChar);
                currentState = BNF::START;
                break;
            case BNF::IDENTIFIER:
                token.type = "IDENTIFIER";
                while (isalnum(currentChar) || currentChar == '_') {
                    token.value += currentChar;
                    inFile.get(currentChar);
                }
                // put back char that isn't alphanumeric
                inFile.putback(currentChar);
                currentState = BNF::START;
                break;
        }
        // add token to list only if token has a type
        if (token.value != "") {
            token.lineNumber = lineNumber;
            tokenList.push_back(token);
        }
    }
}

/*
    This function displays the tokens stored in the tokenList vector.
    If there is an error with one of the tokens, the syntax error will
    be outputted instead.
*/
void Tokenization::displayTokens(const std::string &outputFilename) {
    // open outFile
    std::ofstream outFile(outputFilename);
    
    // if there are no invalid tokens, display tokens
    if (!invalidToken && tokenList.size() != 0) {
        outFile << "Token list:" << std::endl << std::endl;
        
        for (int i = 0; i < tokenList.size(); i++) {
            outFile << "Token type: " << tokenList.at(i).type << std::endl;
            outFile << "Token:      " << tokenList.at(i).value << std::endl;
            outFile << std::endl;
        }
    }
    else {
        outFile << "Syntax error on line " << errorLineNumber << ": ";

        if (invalidType == "string") {
            outFile << "unterminated string quote." << std::endl;
        }
        else {
            outFile << "invalid " << invalidType << "." << std::endl;
        }
    }
}