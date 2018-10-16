/**
 * @file    StringUtil.cpp
 * @author  Jinwook Jung (jinwookjung@kaist.ac.kr)
 * @date    2018-01-24 08:24:22
 *
 * Created on Wed Jan 24 08:24:22 2018.
 */

#include "StringUtil.h"

/**
 * Tokenize the string and add the tokens into a deque.
 */
deque<string> StringUtil::tokenize (string str)
{
    istringstream iss(str);
    string token;
    deque<string> tokens;

    while (iss >> token) {
        tokens.emplace_back(token); 
    }

    return tokens;
}

