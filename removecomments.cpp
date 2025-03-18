/*
    Implementation of the RemoveComments class
    by: Kathy

    Description: This file contains the implementation of the
    RemoveComments class function declared in the header file.
*/

#include <fstream>
#include <iostream>

#include "removecomments.hpp"

/*
    States to determine what type of comment is being read.
*/
enum class State {
    START,
    SINGLELINE,
    MULTILINE,
    STRING
};

/*
    This function removes comments from a .c file and outputs a
    .c file where the comments are replaced with whitespace.
*/
void RemoveComments::removeComments(const std::string &inputFilename, const std::string &outputFilename) {
    std::ifstream inFile(inputFilename);
    std::ofstream outFile(outputFilename);

    int lineNumber = 1; // there's no line 0... :)
    int beginComment = 0;

    // Check if input file exists
    if (!inFile) {
        std::cout << "Error: Unable to open input file to remove comments." << std::endl;
        return;
    }

    // Check if input file is empty
    if (inFile.peek() == EOF) {
        std::cout << "Input file is empty." << std::endl;
        return;
    }

    State currentState = State::START;
    char currentChar;

    while (inFile.get(currentChar)) {
        if (currentChar == '\n') {
            lineNumber++;
        }
        switch (currentState) {
            // State for anything not in a comment
            case State::START:
                if (currentChar == '"') {
                    currentState = State::STRING;
                }
                if (currentChar == '*' && inFile.peek() == '/') {
                    std::cout << "ERROR: Program contains C - style, unterminated comment on line "
                              << lineNumber << std::endl;
                }
                if (currentChar == '/') {
                    if (inFile.peek() == '/') {
                        currentState = State::SINGLELINE;
                        outFile << " ";
                    }
                    else if (inFile.peek() == '*') {
                        currentState = State::MULTILINE;
                        beginComment = lineNumber;
                        outFile << " ";
                    }
                    else {
                        outFile << currentChar;
                    }
                }
                else {
                    outFile << currentChar;
                }
                break;

            case State::SINGLELINE:
                outFile << " ";
                if (currentChar == '\n') {
                    outFile << "\n";
                    currentState = State::START;
                }
                break;

            case State::MULTILINE:
                outFile << " ";
                if (currentChar == '\n') {
                    outFile << "\n";
                }

                if (currentChar == '*' && inFile.peek() == '/') {
                    currentState = State::START;
                    inFile.ignore(); // skip the / that we found
                    outFile << " "; // output a space for / that we skipped
                }

                if (inFile.peek() == EOF) {
                    std::cout << "ERROR: Program contains C - style, unterminated comment on line " 
                              << beginComment << std::endl;
                }
                break;

            case State::STRING:
                outFile << currentChar;
                if (currentChar == '"') {
                    currentState = State::START;
                }
                break;
        }
    }

    inFile.close();
    outFile.close();
}
