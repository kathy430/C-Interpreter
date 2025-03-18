/*
    Assignment 1: Ignore Comments
    by: Kathy
    
    RemoveComments header file
    Description: The RemoveComments class contains a function that will 
    remove comments from a .c file and output a .c file with the comments 
    replaced with whitespace.
*/

#ifndef REMOVE_COMMENTS_HPP
#define REMOVE_COMMENTS_HPP

#include <string>

class RemoveComments {
    public:
        // member function
        static void removeComments(const std::string &inputFilename, 
                                   const std::string &outputFilename);
};

#endif
