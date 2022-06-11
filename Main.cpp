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
    std::cout << "\nEnter the filename here:" << std::endl;
    std::string file;
    std::getline(std::cin, file);
    // std::cout << "FILE: " << file;
    std::string linex;
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
        std::vector<std::string> lines;
        while (getline(myfile, linex))
        {
            //code += line;
            if (stack.finishEarly)
            {
               // printf("finish early\n");
                stack.finishEarly = false;
                glosta_gc::collectStatementGarbage(stack);
                return;
            }
            lines.push_back(linex);
            //gloneb_vm(line.c_str(), stack, lineinfo);
           
        }
        for (int i = 0; i < lines.size(); i++)
        {
            std::string line = lines[i];
            //std::cout << "line: " << line;
            if (stack.finishEarly)
            {
                printf("finish early\n");
                glosta_gc::collectStatementGarbage(stack);
                stack.finishEarly = false;
                break;
            }
            if (stack.gotoLine > -1)
            {
               // std::cout << "gotoline " << stack.gotoLine;
                i = (stack.gotoLine - 1);
                stack.gotoLine = -1;
            }
            else
            {
                gloneb_vm(line.c_str(), stack, i);
            }

        }
        std::cout << std::endl;

        myfile.close();
    }

}

int main()
{
    srand(time(0));

    /*Syntax is like this lol:
    
    print 8 + 9

    spaces are always required
    strings need to be defined

    print "hello world!"
    
    */
    Stack stack;
    const char* tcode = "if 1 == 1\nendif";
    //gloneb_vm(tcode, stack);
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
        for (int i = 0; i < lines.size(); i++)
        {
            std::string line = lines[i];
            //std::cout << "line: " << line;
            if (stack.finishEarly)
            {
                printf("finish early\n");
                glosta_gc::collectStatementGarbage(stack);
                stack.finishEarly = false;
                break;
            }
            if (stack.gotoLine > -1)
            {
                //std::cout << "gotoline " << stack.gotoLine;
                i = (stack.gotoLine - 1);
                stack.gotoLine = -1;
            }
            else
            {
                try
                {
                    gloneb_vm(line.c_str(), stack, i);
                }
                catch (std::exception er)
                {
                    std::cout << "Error [LINE " << i << "] | " << er.what() << std::endl;
                }
            }
            std::cout << std::endl;
        }
    }
}
