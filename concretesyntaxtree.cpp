/*
    Implementation of the ConcreteSyntaxTree class
    by: Owen, Roy, Kathy, Kyle

    Description: This file contains the implementations of
    the ConcreteSyntaxTree class functions declared in the
    header file.
*/

#include <fstream>
#include <iostream>

#include "concretesyntaxtree.hpp"

/*
    This is the default constructor for the ConcreteSyntaxTree class.
    The root node and current node are initialized to nullptr.
*/
ConcreteSyntaxTree::ConcreteSyntaxTree() {
    root = nullptr;
    currentNode = nullptr;
    invalidSyntax = false;
    errorLineNumber = 0;
}

/*
    This function creates the concrete syntax tree from the token list
    utilizing left child and right sibling relationships.
*/
void ConcreteSyntaxTree::createCST(Tokenization& tokenizer) {
    // if there were errors while tokenizing, return
    if (tokenizer.invalidToken) {
        return;
    }

    bool nextIsChild = false;

    // create cst
    for (int i = 0; i < tokenizer.tokenList.size(); i++) {
        // create a tree node for current token
        TreeNode* newNode = new TreeNode(tokenizer.tokenList.at(i).value, 
                                         tokenizer.tokenList.at(i).lineNumber);
        
        // set root of tree
        if (i == 0) {
            root = newNode;
            currentNode = root;
        }
        
        // figure out of token is going to be a child or sibling
        if (nextIsChild) {
            // current token is a child because of previous token
            currentNode->leftChild = newNode;
            currentNode = newNode;

            // reset nextIsChild
            nextIsChild = false;
        }
        else {
            if (newNode->token == "{" || newNode->token == "}") {
                // these tokens are always going to be left children
                currentNode->leftChild = newNode;
                currentNode = newNode;
            }
            else {
                // otherwise the token will always be right sibling
                currentNode->rightSibling = newNode;
                currentNode = newNode;
            }
        }
        
        // next token is always a child if we encounter {, }, or ;
        if (currentNode->token == "{" || currentNode->token == "}" || currentNode->token == ";") {
            nextIsChild = true;
        }
    }

    // check for errors in the CST
    errorCheckCST();
}

/*
    This function checks for syntax errors that might exist
    in the CST.
*/
void ConcreteSyntaxTree::errorCheckCST() {
    if (!root) {
        std::cout << "Error: there is no root." << std::endl;
        return;
    }
    
    currentNode = root;

    while (currentNode->leftChild || currentNode->rightSibling) {
        // check array declaration size is positive integer
        if (currentNode->token == "[") {
            // check next sibling is not a negative integer
            currentNode = currentNode->rightSibling;
            if (currentNode->token[0] == '-') {
                // set errors
                invalidSyntax = true;
                errorType = "array declaration size must be a positive integer.";
                errorLineNumber = currentNode->lineNumber;
                return;
            }
        }

        // check variable declarations are not reserved words
        if (currentNode->token == "char" || currentNode->token == "int" ||
            currentNode->token == "bool") {

            // checking next sibling is not a reserved word
            currentNode = currentNode->rightSibling;
            if (currentNode->token == "char" || currentNode->token == "int" ||
                currentNode->token == "bool" || currentNode->token == "void" ||
                currentNode->token == "if"   || currentNode->token == "else" || 
                currentNode->token == "function" || currentNode->token == "printf") {
                // set errors
                invalidSyntax = true;
                errorType = "reserved word \"" + currentNode->token + 
                            "\" cannot be used for the name of a variable.";
                errorLineNumber = currentNode->lineNumber;
                return;
            }
        }

        // check function names are not reserved words
        if (currentNode->token == "function") {
            // move two siblings over
            currentNode = currentNode->rightSibling;
            currentNode = currentNode->rightSibling;

            // check it is not using reserved word
            if (currentNode->token == "char" || currentNode->token == "int" ||
                currentNode->token == "bool" || currentNode->token == "void" ||
                currentNode->token == "if"   || currentNode->token == "else" || 
                currentNode->token == "function" || currentNode->token == "printf") {
                // set errors
                invalidSyntax = true;
                errorType = "reserved word \"" + currentNode->token +
                            "\" cannot be used for the name of a function.";
                errorLineNumber = currentNode->lineNumber;
            }
        }

        // move current node to next node
        if (currentNode->rightSibling) {
            currentNode = currentNode->rightSibling;
        }
        else if (currentNode->leftChild) {
            currentNode = currentNode->leftChild;
        }
    }
}

/*
    This function displays the CST, but if there is an error the
    syntax error will be printed in an output file.
*/
void ConcreteSyntaxTree::displayCST(std::string outputFilename) {
    // if there is no root, that means there was an error tokenizing
    if (!root) {
        return;
    }

    std::ofstream outFile(outputFilename);
    
    // error detected when creating cst
    if (invalidSyntax) {
        outFile << "Syntax error on line " << errorLineNumber << ": " << errorType << std::endl;
        return;
    }
    
    // start at root node
    currentNode = root;

    // keep running until both left child and right sibling are null
    while (currentNode->leftChild || currentNode->rightSibling) {
        outFile << currentNode->token;

        // set up to print right sibling or left child
        if (currentNode->rightSibling) {
            outFile << " -> ";
            currentNode = currentNode->rightSibling;
        }
        else if (currentNode->leftChild) {
            outFile << " -> NULL" << std::endl;
            outFile << "child of " << currentNode->token << ": ";
            currentNode = currentNode->leftChild;
        }
    }

    // print last node
    outFile << currentNode->token << " -> NULL" << std::endl;
}