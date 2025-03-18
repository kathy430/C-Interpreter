/*
    Assignment 4: Recursive Decent Parser
    by: Kathy

    SymbolTable header file
    Description: The SymbolTable class contains a struct 
    Symbol that is used to create a linked list of 
    functions, procedures, and variable declarations.
*/    

#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <string>

#include "concretesyntaxtree.hpp"

struct Symbol {
    bool isParameter;
    std::string functionName;
    std::string identifierName;
    std::string identifierType;
    std::string datatype;
    bool isArray;
    int arraySize;
    int scope;
    int lineNumber;
    Symbol* next;

    // default constructor
    Symbol() : isParameter(false),
               functionName(""),
               identifierName(""),
               identifierType(""),
               datatype(""),
               isArray(false),
               arraySize(0),
               scope(0),
               lineNumber(0),
               next(nullptr) {}
};

class SymbolTable {
    public:
        // default constructor
        SymbolTable();

        // member functions
        void createSymbolTable(const ConcreteSyntaxTree& cst);
        int readBlock(TreeNode* currentNode, int scope);
        void createVariables(TreeNode* currentNode, int scope);
        void insertSymbol(Symbol* symbol);
        void displaySymbolTable(std::string outputFilename);

    private:
        Symbol* head;
        Symbol* currentSymbol;
        TreeNode* currentCSTNode;
        int size;

        // error handling
        void errorCheckSymbolTable();
        bool invalidSyntax;
        int errorLineNumber;
        std::string errorType;
};

#endif
