/*
How I picture the syntax

print "kys"
if 6==6
print "true"
endif
var fal 69


Current Syntax Supports:

var kal = 5
print kal * 7

if kal == 5
print "kal is 5"
endif
if kal != 6
print "kal is not 6"
endif

function "printhal"
print "hallo"
closefunction

printhal

*/
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

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

void GLOSTA_LOADFILE(Stack& stack)
{
    //runfile(hello.txt)
    std::cout << "Enter the filename here:" << std::endl;
    std::string file;
    std::getline(std::cin, file);
    // std::cout << "FILE: " << file;
    std::string line;
    std::ifstream myfile(file);
    if (!myfile.is_open())
    {
        std::cout << "File could not be opened!" << std::endl;
        return;
    }
    if (myfile.is_open())
    {
        std::cout << "\n\n";
        std::string code = "";
        while (getline(myfile, line))
        {
            //code += line;
            gloneb_vm(line.c_str(), stack);
        }

        myfile.close();
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

        if (code == "loadfile")
        {
            GLOSTA_LOADFILE(stack);
            continue;
        }

        std::vector<std::string> lines = lineBreak::lineScan(code);
        for (std::string line : lines)
        {
            //std::cout << "line: " << line;
            gloneb_vm(line.c_str(), stack);
            std::cout << std::endl;
        }
    }
}
