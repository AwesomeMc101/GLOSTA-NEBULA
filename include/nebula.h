#include <vector>
#include <Windows.h>
#include <string>

#include <iostream>
#include <map>


/*

* WRITTEN BY AWESOMEMC101 FOR LUNAR GUARD TECHNOLOGIES

* I forgot the date :(

* I DO PLAN ON MAYBE UPDATING THIS :)

*/

namespace Variables
{
	class vartype {
	public:
		const char* name;
		char* value;
	};

	class varStack
	{
	public:
		//std::vector<vartype> variables;
		std::map<std::string, std::string> variables{};

		void pop(const char* name, char* val)
		{
			variables[name] = val;

			//std::cout << "vstack name: " << va.name << " || val: " << va.value << std::endl;

			
		}
		bool checkIsVar(const char* name)
		{
			/*for (vartype va : variables)
			{
				if (strcmp(va.name, name) == 0)
				{
					return true;
				}
			}*/
			if (variables.find(name) != variables.end()) {
				return true;
			}
			return false;
		}
		char* retValue(std::string _var)
		{
			/*for (int i = 0; i < variables.size(); i++)
			{
				vartype var = variables[i];
				if (strcmp(var.name, _var.c_str()) == 0)
				{
					std::cout << "varname: " << _var.c_str() << " or normal " << _var << std::endl;
					std::cout << "ret value true (" << var.value << ")\n";
					return var.value;
				}
			}*/
			return (char*)variables[_var].c_str();
			return (char*)"NO_SUCH_VARIABLE";
		}
	};
}

class Stack
{
public:
	std::vector<std::string> stack;
	int top = -1;
	Variables::varStack variableStack;

	void pop(std::string arg)
	{
		stack.push_back(arg);
		top++;
	}

	std::string gettop()
	{
		return stack[top];
	}
};

typedef enum TYPES
{
	NIL,
	KEYCALL,
	STRING,
	ODOUBLE,
	INTEGER,
	VARDEF,
	VARNAME,
	VARREF, //reference to var
	RANDOMVAL,
	/* --- */
	ADD,
	SUB,
	MUL,
	DIV,
	DEQUAL,
	UEQUAL,
	OPENIF,
	CLOSEIF
} typeSet;

namespace keycall
{
	std::vector<const char*> keycalls = { "print" };
	void print(const char* key)
	{
		std::cout << "\nPRINTING: " << key << std::endl;
	}

	void keycall(std::string key, char* arg, Stack& stack)
	{
		if (key == "print")
		{
			print(arg);
		}
	}
}

namespace evalIf
{
	bool evalIf(std::string d1, std::string d2)
	{
		if (d1 == d2)
		{
			return true;
		}
		return false;
	}
}

namespace miniLexer
{
	BOOL isKeycall(std::string x)
	{
		for (const char* u : keycall::keycalls)
		{
			if (u == x)
			{
				return true;
			}
		}
		return false;
	}

	BOOL isVarName(std::string x, Stack stack)
	{
		return stack.variableStack.checkIsVar(x.c_str());
	}

	BOOL isSwitchIf(std::string x)
	{
		if (x == "if")
		{
			return 1;
		}
		else if (x == "endif")
		{
			return 2;
		}
		return 0;
	}
	BOOL isNumber(std::string data)
	{
		//std::cout << "Passed Data to isNum: " << data << std::endl;
		for (int i = 0; i < data.length(); i++) {
			if (data[i] != '0' && data[i] != '1' && data[i] != '2' &&
				data[i] != '3' && data[i] != '4' && data[i] != '5' &&
				data[i] != '6' && data[i] != '7' && data[i] != '8' &&
				data[i] != '9')
			{
				return 0;
			}
		}
		return 1;
	}
	BOOL isDouble(std::string data)
	{
		if (!isNumber(data)) { return 0; }

		unsigned long int length = data.length();
		for (int i = 0; i < length; i++)
		{
			if (data[i] == '.')
			{
				return 1;
			}
		}
		return 0;
	}
	BOOL isString(std::string data)
	{
		if (data[0] == '"' || data[0] == '\'')
		{
			return true;
		}
		return false;
	}
	BOOL isCloseString(std::string data)
	{
		if (data[data.length()-1] == '"' || data[data.length()-1] == '\'')
		{
			return true;
		}
		return false;
	}
	BOOL isVarDef(std::string data)
	{
		if (data[0] == 'v' && data[1] == 'a' && data[2] == 'r')
		{
			return true;
		}
		return false;
	}

	typeSet isOperator(char* data)
	{
		//no need to use std::string data since its 1 character
		if (data[0] == '+')
		{
			return typeSet::ADD;
		}
		if (data[0] == '-')
		{
			return typeSet::SUB;
		}
		if (data[0] == '*')
		{
			return typeSet::MUL;
		}
		if (data[0] == '/')
		{
			return typeSet::DIV;
		}
		if (data[0] == '=' && data[1] == '=')
		{
			return typeSet::DEQUAL;
		}
		if (data[0] == '!' && data[1] == '=')
		{
			return typeSet::UEQUAL;
		}
		return typeSet::NIL;
	}


	typeSet returnType(std::string& x, std::vector<typeSet> types, Stack stack)
	{
		typeSet last_type = typeSet::NIL;
		if (types.size() > 0)
		{
			last_type = types[types.size() - 1];
			//std::cout << "\nLast type: " << last_type << std::endl;
		}
		//try
		//{
		//	//last_type = types[types.size() - 1];
		//}
		//catch (std::exception err)
		//{
		//	//last_type = typeSet::NIL;
		//}
		if (isKeycall(x))
		{
			return typeSet::KEYCALL;
		}
		if (isSwitchIf(x))
		{
			switch (isSwitchIf(x))
			{
			case 1:
				return typeSet::OPENIF;
				break;
			case 2:
				return typeSet::CLOSEIF;
				break;
			}
		}
		if (isDouble(x))
		{
			return typeSet::ODOUBLE;
		}
		if (isNumber(x))
		{
			return typeSet::INTEGER;
		}
		if (isString(x))
		{
			x.erase(std::remove(x.begin(), x.end(), '"'), x.end());
			x.erase(std::remove(x.begin(), x.end(), '\''), x.end());
			return typeSet::STRING;
		}
		if (isVarDef(x))
		{
			return typeSet::VARDEF;
		}
		if (isOperator((char*)x.c_str()) != typeSet::NIL)
		{
			return isOperator((char*)x.c_str());
		}
		if (last_type == typeSet::VARDEF) //needs an OR to check if its an already used varname
		{
			return typeSet::VARNAME;
		}
		if (isVarName(x, stack))
		{
			/* only use the non-return code for on-interpret var->val */
			//x = stack.variableStack.retValue(x);
			return typeSet::VARREF;
		}
		return typeSet::NIL;
	}
}

void gloneb_vm(const char* code, Stack& stack)
{
	//std::cout << "code: " << code << std::endl;
	std::vector<std::string> keys;
	std::vector<typeSet> types;

	char* token;
	char* rest = (char*)code;
	BOOL setToString = false;
	int stringLoc;
	while ((token = strtok_s(rest, " ", &rest)))
	{
		//std::cout << "Token: " << token << std::endl;
		if (setToString)
		{
			keys[stringLoc] += " ";
			keys[stringLoc] += token;
			if (miniLexer::isCloseString(token))
			{
				setToString = false;
			}
		}
		if (miniLexer::isString(token))
		{
			if (miniLexer::isCloseString(token))
			{
				keys.push_back(token);
				continue;
			}
			setToString = true;
			stringLoc = keys.size();
			keys.push_back(token);
		}
		else
		{
			keys.push_back(token);
		}
	}
	for (int i = 0; i < keys.size(); i++)
	{
		std::string key = keys[i];
		typeSet type = miniLexer::returnType(keys[i], types, stack);
		if (type == typeSet::VARREF)
		{
			keys[i] = stack.variableStack.retValue(keys[i]);
			//std::cout << "new keys[i]: " << keys[i] << std::endl;
			stack.pop(keys[i]);
		}
		types.push_back(type);
	}

	bool skipnext = false;
	for (int type = types.size() - 1; type > -1; type--)
	{
		if (skipnext)
		{
			skipnext = false;
			continue;
		}
		//std::cout << "OP_" << types[type];
		switch (types[type])
		{
		case typeSet::STRING:
			//std::cout << "pop stack string: " << keys[type] << std::endl;
			stack.pop((char*)(keys[type].c_str()));
			break;
		case typeSet::ODOUBLE:
			//std::cout << "pop stack double: " << keys[type] << std::endl;
			stack.pop((char*)(keys[type].c_str()));
			break;
		case typeSet::INTEGER:
			//std::cout << "pop stack int: " << keys[type] << std::endl;
			stack.pop((char*)(keys[type].c_str()));
			break;
		case typeSet::KEYCALL:
			//std::cout << "keycall: ";
			//std::cout << keys[type] << " | " << stack.gettop();
			keycall::keycall(keys[type], (char*)stack.gettop().c_str(), stack);
			break;
		case typeSet::ADD:
			if (types[type - 1] == typeSet::STRING)
			{
				stack.pop((char*)(const_cast<char*>(keys[type - 1].c_str()) + (std::string)stack.gettop()).c_str());
			}
			else
			{
				//std::cout << "Adding: " << (keys[type - 1]);
				std::cout << " + " << (stack.gettop()) << std::endl;
				stack.pop(std::to_string(std::stod(keys[type - 1]) + std::stod(stack.gettop())));
			}
			skipnext = true;
			break;
		case typeSet::SUB:
			//std::cout << "Adding: " << (keys[type - 1]);
			//std::cout << " + " << (stack.gettop()) << std::endl;
			stack.pop(std::to_string(std::stod(keys[type - 1]) - std::stod(stack.gettop())));
			skipnext = true;
			break;
		case typeSet::MUL:
			//std::cout << "Adding: " << (keys[type - 1]);
			//std::cout << " + " << (stack.gettop()) << std::endl;
			stack.pop(std::to_string(std::stod(keys[type - 1]) * std::stod(stack.gettop())));
			skipnext = true;
			break;
		case typeSet::DIV:
			//std::cout << "Adding: " << (keys[type - 1]);
			//std::cout << " + " << (stack.gettop()) << std::endl;
			stack.pop(std::to_string(std::stod(keys[type - 1]) / std::stod(stack.gettop())));
			skipnext = true;
			break;
		case typeSet::VARNAME:
			stack.variableStack.pop(keys[type].c_str(), (char*)stack.gettop().c_str());
			break;
		//case typeSet::VARREF:
		//	stack.pop(stack.variableStack.retValue(keys[type]));
		}
		
	}
		
}

