/*
    Main file
    by: Kathy

    Description: This file contains the main function for
    the program and is used to read in user's input files
    to remove comments and tokenize the code.
*/
#include <iostream>

#include "removecomments.hpp"
#include "tokenization.hpp"
#include "concretesyntaxtree.hpp"
#include "symboltable.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    // Open the file specified in the command-line argument
    std::string inputFile = argv[1];
    
    std::string outputFile = inputFile;
    // modify outputFile name to be similar to input file
    outputFile.pop_back();
    outputFile.pop_back();
    outputFile += "-comments_replaced_with_white_space.c";
    
    // remove comments from input file
    RemoveComments::removeComments(inputFile, outputFile);

    // tokenize the output file
    Tokenization tokenizer;
    tokenizer.tokenize(outputFile);

    // modify output file name
    inputFile.pop_back();
    inputFile.pop_back();
    outputFile = "output-" + inputFile + ".txt";
    //tokenizer.displayTokens(outputFile);

    // create cst from the tokenizer's list
    ConcreteSyntaxTree cst;
    cst.createCST(tokenizer);
    //cst.displayCST(outputFile);

    // create symbol table
    SymbolTable symbolTable;
    symbolTable.createSymbolTable(cst);
    symbolTable.displaySymbolTable(outputFile);

    return 0;
}
