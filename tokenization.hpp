/*
    Assignment 2: Tokenization
    by: Owen, Roy, Kathy, Kyle

    Tokenization header file
    Description: The Tokenization class contains functions to
    tokenize a .c file and to display the tokens or error that
    is found in the .c file.
*/

#ifndef TOKENIZATION_HPP
#define TOKENIZATION_HPP

#include <string>
#include <vector>

struct Token {
    std::string type;
    std::string value;
    int lineNumber;
};

class Tokenization {
    public:
        // default constructor
        Tokenization();

        // member function
        void tokenize(const std::string& inputFilename);
        void displayTokens(const std::string &outputFilename);

        // declare friend class
        friend class ConcreteSyntaxTree;

    private:
        std::vector<Token> tokenList;
        Token token;
        bool invalidToken;
        std::string invalidType;
        int errorLineNumber;
};

#endif