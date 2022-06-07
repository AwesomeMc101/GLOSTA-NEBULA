/*
How I picture the syntax

print "kys"
if 6==6
print "true"
endif
var fal 69

*/
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "virtualmachine.h"

namespace lineBreak
{
    std::vector<std::string> lineScan(std::string fullcode)
    {
        std::vector<std::string> key;
        std::istringstream f(fullcode);
        std::string line;
        while (std::getline(f, line)) {
            key.push_back(line.c_str());
            //std::cout << "Line Pushback: " << line << std::endl;
        }
        return key;
    }
}



int main()
{
    /*Syntax is like this lol:
    
    print 8 + 9

    spaces are always required
    strings need to be defined

    print "hello world!"
    
    */
    Stack stack;
    while (1)
    {
        std::string code;
        std::getline(std::cin, code);

        std::vector<std::string> lines = lineBreak::lineScan(code);
        for (std::string line : lines)
        {
            //std::cout << "line: " << line;
            gloneb_vm(line.c_str(), stack);
            std::cout << std::endl;
        }
    }
}
