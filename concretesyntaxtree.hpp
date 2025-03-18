/*
    Assignment 3: Recursive Decent Parser
    by: Kathy

    ConcreteSyntaxTree header file
    Description: The ConcreteSyntaxTree class contains functions
    to create a concrete syntax tree utilizing a Left-Child,
    Right-Sibling binary tree. This file also contains a TreeNode
    structure that is used to create the tree.
*/

#ifndef CONCRETE_SYNTAX_TREE_HPP
#define CONCRETE_SYNTAX_TREE_HPP

#include <string>

#include "tokenization.hpp"

struct TreeNode {
    std::string token;
    int lineNumber;
    TreeNode* leftChild;
    TreeNode* rightSibling;

    // tree node constructor
    TreeNode(std::string data, int lineNum) : 
        token(data), lineNumber(lineNum), leftChild(nullptr), rightSibling(nullptr) {}
};

class ConcreteSyntaxTree {
    public:
        // default constructor
        ConcreteSyntaxTree();

        // member functions
        void createCST(Tokenization& tokenizer);
        void displayCST(std::string outputFilename);

        // friend class
        friend class SymbolTable;

    private:
        // private function
        void errorCheckCST();
        
        TreeNode* root;
        TreeNode* currentNode;
        bool invalidSyntax;
        int errorLineNumber;
        std::string errorType;
};

#endif
