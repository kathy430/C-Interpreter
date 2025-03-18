/*
    Implementation of the SymbolTable class
    by: Owen, Roy, Kathy, Kyle

    Decription: This file contains the implementations of the
    SymbolTable class functions declared in the header file.
*/
#include <fstream>
#include <iostream>

#include "symboltable.hpp"

/*
    This is the default constructor for the SymbolTable class.
    All pointers are initialized to nullptr and integers are set
    to 0, while invalidSyntax is set to false.
*/
SymbolTable::SymbolTable() {
    head = nullptr;
    currentSymbol = nullptr;
    currentCSTNode = nullptr;
    size = 0;
    invalidSyntax = false;
    errorLineNumber = 0;
}

/*
    This function creates a symbol table from a CST. 
    Two functions are called to either read an entire block of code
    or to read global variable declarations while traversing through
    the CST. Error handling is done after the CST is created.
*/
void SymbolTable::createSymbolTable(const ConcreteSyntaxTree& cst) {
    int scope = 0;
    int braceScopeCounter = 0;
    currentCSTNode = cst.root;

    // keep running until both left child and right sibling are null
    while (currentCSTNode->leftChild || currentCSTNode->rightSibling) {
        if (currentCSTNode->token == "function" || currentCSTNode->token == "procedure") {
            scope = readBlock(currentCSTNode, scope);
        } 
        else if (currentCSTNode->token == "char" || currentCSTNode->token == "int" ||
                 currentCSTNode->token == "bool") {
            // read global scope variables
            createVariables(currentCSTNode, 0);
        }
        else {
            if (currentCSTNode->rightSibling) {
                currentCSTNode = currentCSTNode->rightSibling;
            }
            else if (currentCSTNode->leftChild) {
                currentCSTNode = currentCSTNode->leftChild;
            }
        }
    }

    // error check symbol table after creation
    errorCheckSymbolTable();
}

/*
    This function identifies functions and procedures and will
    read in a block of code determined by the number of braces.
    The scope is incremented by 1 for each block of code. 
    Variables in the block are accounted for by calling
    createVariables.
*/
int SymbolTable::readBlock(TreeNode* currentNode, int scope) {
    // a new procedure means new scope
    scope++;

    // brace counter to figure out when we are at end of scope
    int braceCounter = 0;

    currentCSTNode = currentNode;
    std::string functionName;
    Symbol* newSymbol = new Symbol();
    insertSymbol(newSymbol);

    // set symbol variables
    currentSymbol->identifierType = currentCSTNode->token;
    currentSymbol->scope = scope;
    currentSymbol->lineNumber = currentCSTNode->lineNumber;
    currentCSTNode = currentCSTNode->rightSibling;

    if (currentSymbol->identifierType == "procedure") {
        currentSymbol->datatype = "NOT APPLICABLE";
        currentSymbol->identifierName = currentCSTNode->token;
        functionName = currentSymbol->identifierName;
    }
    else if (currentSymbol->identifierType == "function") {
        currentSymbol->datatype = currentCSTNode->token;

        currentCSTNode = currentCSTNode->rightSibling;
        currentSymbol->identifierName = currentCSTNode->token;
        functionName = currentSymbol->identifierName;
    }

    // ignore void if it is there
    currentCSTNode = currentCSTNode->rightSibling;
    if (currentCSTNode->token == "(") {
        currentCSTNode = currentCSTNode->rightSibling;

        if (currentCSTNode->token == "void") {
            currentCSTNode = currentCSTNode->rightSibling;
        }
        else {
            if (currentCSTNode->token != ")") {
                while (currentCSTNode->token != ")") {
                    if (currentCSTNode->token == "int" || currentCSTNode->token == "bool" ||
                        currentCSTNode->token == "char") {
                        // new symbol for each parameter
                        Symbol* newSymbol = new Symbol();
                        insertSymbol(newSymbol);
                        
                        currentSymbol->isParameter = true;
                        currentSymbol->functionName = functionName;
                        currentSymbol->datatype = currentCSTNode->token;
                        currentSymbol->scope = scope;
                        currentSymbol->lineNumber = currentCSTNode->lineNumber;
    
                        currentCSTNode = currentCSTNode->rightSibling;
                        currentSymbol->identifierName = currentCSTNode->token;
    
                        // check if array
                        currentCSTNode = currentCSTNode->rightSibling;
                        if (currentCSTNode->token == "[") {
                            currentSymbol->isArray = true;
    
                            currentCSTNode = currentCSTNode->rightSibling;
                            currentSymbol->arraySize = std::stoi(currentCSTNode->token);
    
                            currentCSTNode = currentCSTNode->rightSibling; // token = ]
                            currentCSTNode = currentCSTNode->rightSibling; // token = )
                        }
    
                        // check if more parameters
                        if (currentCSTNode->token == ",") {
                            // more parameters
                            currentCSTNode = currentCSTNode->rightSibling;
                        }
                    }
                }
            }
        }
    }
    
    // read rest of function
    while (currentCSTNode->rightSibling || currentCSTNode->leftChild) {
        // get next cst token
        if (currentCSTNode->rightSibling) {
            currentCSTNode = currentCSTNode->rightSibling;
        }
        else if (currentCSTNode->leftChild) {
            currentCSTNode = currentCSTNode->leftChild;
        }

        // keep track of function scope
        if (currentCSTNode->token == "{") {
            braceCounter++;
        }
        else if (currentCSTNode->token == "}") {
            braceCounter--;
            if (braceCounter == 0) {
                // reach end of functiion, exit
                return scope;
            }
        }
        else if (currentCSTNode->token == "int" || currentCSTNode->token == "bool" ||
                 currentCSTNode->token == "char") {
            createVariables(currentCSTNode, scope);    
        }
    }
    
    return scope;
}

/*
    This function reads variables starting from the currentNode in a CST
    and adds them to the linked list. The scope of these variables are set
    to the scope that was passed in as an argument.
*/
void SymbolTable::createVariables(TreeNode* currentNode, int scope) {
    currentCSTNode = currentNode;
    // save datatype incase there are multiple var declarations
    std::string datatype = currentCSTNode->token;
    
    while (currentCSTNode->token != ";") {
        // new symbol for each variable
        Symbol* newSymbol = new Symbol();
        insertSymbol(newSymbol);
        
        currentSymbol->datatype = datatype;
        currentSymbol->identifierType = "datatype";
        currentSymbol->scope = scope;
        currentSymbol->lineNumber = currentCSTNode->lineNumber;
        
        currentCSTNode = currentCSTNode->rightSibling;
        currentSymbol->identifierName = currentCSTNode->token;

        currentCSTNode = currentCSTNode->rightSibling;
        // variable is an array
        if (currentCSTNode->token == "[") {
            currentSymbol->isArray = true;

            currentCSTNode = currentCSTNode->rightSibling;
            currentSymbol->arraySize = std::stoi(currentCSTNode->token);

            // token should now be ] after this statement
            currentCSTNode = currentCSTNode->rightSibling;
            currentCSTNode = currentCSTNode->rightSibling;
        }
    }
}

/*
    This function takes in a pointer to a symbol and adds 
    the symbol to the private linked list of the SymbolTable class.
*/
void SymbolTable::insertSymbol(Symbol* symbol) {
    // return if symbol doesnt exist
    if (!symbol) {
        return;
    }
    // set head if there is no head
    if (!head) {
        head = symbol;
        currentSymbol = head;
    }
    else {
        currentSymbol->next = symbol;
        currentSymbol = currentSymbol->next;
    }
    size++;
}

/*
    This function traverses through the linked list of symbols
    starting from the head and checking each one afterwards to 
    see if there are duplicate declarations of variables. If a 
    duplicate is found, error variables are set to true and
    prepared for display.
*/
void SymbolTable::errorCheckSymbolTable() {
    // pointer to check other symbols
    Symbol* symbolChecker = nullptr;
    
    // start at head
    currentSymbol = head;
    
    // check linked list for same scope declaration
    for (int i = 0; i < size; i++) {
        // always check starting at the next symbol
        symbolChecker = currentSymbol->next;
        
        // check the rest of the list same or global scope declarations
        while (!invalidSyntax && symbolChecker) {
            // if same name
            if (currentSymbol->identifierName == symbolChecker->identifierName) {
                // and same scope
                if (currentSymbol->scope == symbolChecker->scope) {
                    invalidSyntax = true;
                    errorLineNumber = symbolChecker->lineNumber;
                    errorType = ": variable \"" + symbolChecker->identifierName + 
                                "\" is already defined locally";
                }
                // or currentSymbol is a global variable
                else if (currentSymbol->scope == 0) {
                    invalidSyntax = true;
                    errorLineNumber = symbolChecker->lineNumber;
                    errorType = ": variable \"" + symbolChecker->identifierName + 
                                "\" is already defined globally";
                }
            }
            // check next symbol
            symbolChecker = symbolChecker->next;
        }
        // move to next symbol
        currentSymbol = currentSymbol->next;
    }
    return;
}

/*
    This function will display the symbol table to the output file
    if there were no errors detected, otherwise the error will be
    displayed instead.
*/
void SymbolTable::displaySymbolTable(std::string outputFilename) {
    // if linked list doesn't exist, return
    if (!head) {
        return;
    }

    std::ofstream outFile(outputFilename);

    // if there is an error, print that out
    if (invalidSyntax) {
        outFile << "Error on line " << errorLineNumber << errorType << std::endl;
        return;
    }

    // print out the delcarations in the symbol table
    currentSymbol = head;
    while (currentSymbol) {
        if (!currentSymbol->isParameter) {
            outFile << "IDENTIFIER_NAME: " << currentSymbol->identifierName << std::endl;
            outFile << "IDENTIFIER_TYPE: " << currentSymbol->identifierType << std::endl;
            outFile << "DATATYPE: " << currentSymbol->datatype << std::endl;
            
            outFile << "DATATYPE_IS_ARRAY: ";
            if (currentSymbol->isArray) {
                outFile << "yes" << std::endl;
            }
            else {
                outFile << "no" << std::endl;
            }

            outFile << "DATATYPE_ARRAY_SIZE: " << currentSymbol->arraySize << std::endl;
            outFile << "SCOPE: " << currentSymbol->scope << std::endl << std::endl;
        }
        currentSymbol = currentSymbol->next;
    }

    // print out parameters in symbol table
    currentSymbol = head;
    std::string currentParamFunc;
    while (currentSymbol) {
        if (currentSymbol->isParameter) {
            if (currentParamFunc != currentSymbol->functionName) {
                currentParamFunc = currentSymbol->functionName;
                outFile << "PARAMETER LIST FOR: " << currentSymbol->functionName << std::endl;
            }

            // output all parameters for current function
            if (currentSymbol->functionName == currentParamFunc) {
                outFile << "IDENTIFIER_NAME: " << currentSymbol->identifierName << std::endl;
                outFile << "DATATYPE: " << currentSymbol->datatype << std::endl;
    
                outFile << "DATATYPE_IS_ARRAY: ";
                if (currentSymbol->isArray) {
                    outFile << "yes" << std::endl;
                }
                else {
                    outFile << "no" << std::endl;
                }
    
                outFile << "DATATYPE_ARRAY_SIZE: " << currentSymbol->arraySize << std::endl;
                outFile<< "SCOPE: " << currentSymbol->scope << std::endl << std::endl;
            }
        }
        currentSymbol = currentSymbol->next;
    }
    return;
}