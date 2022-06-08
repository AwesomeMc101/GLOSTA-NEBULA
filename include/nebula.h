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

namespace UserStack
{
	class userStack
	{
	public:
		std::string name;
		std::vector<std::string> items;
		int top = -1;
	};
	class userstackStack
	{
	public:
		std::vector<userStack> uStack;

		void newStack(std::string name)
		{
			userStack uS;
			uS.name = name;
			uStack.push_back(uS);
			return;
		}
		void pushStack(std::string name, std::string value)
		{
			for (int i = 0; i < uStack.size(); i++)
			{
				if (uStack[i].name == name)
				{
					uStack[i].items.push_back(value);
					uStack[i].top++;
					return;
				}
			}
		}
		std::string indexStack(std::string name, std::string index)
		{
			for (int i = 0; i < uStack.size(); i++)
			{
				if (uStack[i].name == name)
				{
					if (index == "top" || index == "size")
					{
						return uStack[i].items[uStack[i].top];
					}
					else
					{
						try {
							std::string to_return = uStack[i].items[std::stoi(index)];
							return to_return;
						}
						catch (std::exception er)
						{
							return "OUT_OF_RANGE";
						}
					}
				}
			}
			return "NO_SUCH_STACK";
		}
		bool isStack(std::string name)
		{
			//std::cout << "Passed stack name: " << name << std::endl;
			for (int i = 0; i < uStack.size(); i++)
			{
				//std::cout << "Scanned stack name: " << uStack[i].name << std::endl;
				if (uStack[i].name == name)
				{
					return true;
				}
			}
			return false;
		}
	};
}

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
		void setValue(std::string _var, std::string _val)
		{
			//std::cout << "Setting " << _var << " to " << _val << std::endl;
			variables[_var] = _val;
			
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
			//std::cout << "Map Size: " << variables.size() << std::endl;
			return (char*)variables[_var].c_str();
			return (char*)"NO_SUCH_VARIABLE";
		}
	};
}

namespace userKeycall
{
	class uKeycallStack
	{
	public:
		std::map<std::string, std::vector<std::string>> keycalls{};

		void pop(std::string name, std::vector<std::string> code)
		{
			keycalls[name] = code;	
		}
		bool isuKeycall(std::string name)
		{
			if (keycalls.find(name) != keycalls.end()) {
				return true;
			}
			return false;
		}
		std::vector<std::string> returnCode(std::string name)
		{
			return keycalls[name];
		}
	};
}

class Stack
{
public:
	std::vector<std::string> stack;
	int top = -1;
	Variables::varStack variableStack;
	userKeycall::uKeycallStack uKeycallStack;
	UserStack::userstackStack userStackStack;

	void pop(std::string arg)
	{
		stack.push_back(arg);
		top++;
	}

	std::string gettop()
	{
		return stack[top];
	}

	bool ifWrong = false;

	bool functionWriter = false;
	std::string functionWriterName = "";
	std::vector<std::string> functionsCode;

	bool loopWriter = false;
	long long int toLoopNum = 0;
	std::vector<std::string> loopCode;
	bool runLoop = false;

	bool finishEarly = false;
};

typedef enum TYPES
{
	NIL,
	KEYCALL,
	UKEYCALL,
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
	POW,
	DEQUAL,
	UEQUAL,
	GREATERTHAN,
	LESSTHAN,
	OPENIF,
	CLOSEIF,
	ELSEIF, //just else, not else if
	OPENFUNCTION,
	CLOSEFUNCTION,
	COMMENT,
	/* ---- */
	NEWSTACK,
	NEWSTACKNAME,
	PUSHSTACK,
	PUSHSTACKNAME,
	PUSHSTACKVALUE,
	INDEXSTACK,
	INDEXSTACKNAME,
	INDEXSTACKAT,
	/* ---- */
	STARTFOR,
	ENDFOR,
	FINISHEARLY,
} typeSet;


void gloneb_vm(const char* code, Stack& stack);

namespace keycall
{
	std::vector<const char*> keycalls = { "print", "getinput", "rand"};
	void print(const char* key)
	{
		std::cout << "\nPRINTING: " << key << std::endl;
	}

	void getinput(Stack& stack, const char* var)
	{
		std::string towrite = "";
		std::getline(std::cin, towrite);
		
		if (stack.variableStack.checkIsVar(stack.stack[stack.top - 1].c_str()))
		{
			//std::cout << "Variable found" << std::endl;
			stack.variableStack.setValue(stack.stack[stack.top - 1], towrite);
		}
		else
		{
			//std::cout << "No such variable: " << stack.stack[stack.top - 1] << std::endl;
		}

	}

	void random(char* _arg, Stack& stack)
	{
		std::string arg = _arg;

		int rat = rand() % (std::stoi(arg));
		stack.pop(std::to_string(rat));
	}

	void keycall(std::string key, char* arg, Stack& stack)
	{
		if (key == "print")
		{
			print(arg);
		}
		if (key == "getinput")
		{
			getinput(stack, arg);
		}
		if (key == "rand")
		{
			random(arg, stack);
		}
	}
}

namespace uKeycall
{
	void execute(Stack& stack, std::string name)
	{
		std::vector<std::string> code = stack.uKeycallStack.returnCode(name);

		Stack tstack;

		for (std::string co : code)
		{
			gloneb_vm(co.c_str(), stack);
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

namespace glosta_gc
{
	void collectStatementGarbage(Stack& stack)
	{
		stack.finishEarly = false;
		stack.functionWriter = false;
		stack.functionsCode.clear();
		stack.functionWriterName.clear();
		stack.loopCode.clear();
		stack.loopWriter = false;
		stack.runLoop = false;
		stack.toLoopNum = 0;
		stack.ifWrong = false;
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

	BOOL isSwitchFunction(std::string x)
	{
		if (x == "function")
		{
			return 1;
		}
		else if (x == "closefunction")
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

	BOOL isUKeycall(Stack stack, std::string data)
	{
		if (stack.uKeycallStack.isuKeycall(data))
		{
			return true;
		}
		return false;
	}

	BOOL isComment(std::string data)
	{
		if (data[0] == '$' && data[1] == '$')
		{
			return true;
		}
		return false;
	}

	BOOL isNewStack(std::string data)
	{
		if (data == "newstack")
		{
			return true;
		}
		return false;
	}

	BOOL isPushStack(std::string data)
	{
		if (data == "pushstack")
		{
			return true;
		}
		return false;
	}

	BOOL isIndexStack(std::string data)
	{
		if (data == "indexstack")
		{
			return true;
		}
		return false;
	}

	BOOL isFinishEarly(std::string data)
	{
		if (data == "@FINISH")
		{
			return true;
		}
		return false;
	}

	BOOL isForLoop(std::string data)
	{
		if (data[0] == 'f' && data[1] == 'o' && data[2] == 'r')
		{
			return true;
		}
		return false;
	}
	BOOL isEndLoop(std::string data)
	{
		if (data[0] == 'e' && data[1] == 'n' && data[2] == 'd' &&
			data[3] == 'l' && data[4] == 'o' && data[5] == 'o' && data[6] == 'p')
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
		if (data[0] == '^')
		{
			return typeSet::POW;
		}
		if (data[0] == '=' && data[1] == '=')
		{
			return typeSet::DEQUAL;
		}
		if (data[0] == '!' && data[1] == '=')
		{
			return typeSet::UEQUAL;
		}
		if (data[0] == '>')
		{
			return typeSet::GREATERTHAN;
		}
		if (data[0] == '<')
		{
			return typeSet::LESSTHAN;
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
		if (isUKeycall(stack, x))
		{
			return typeSet::UKEYCALL;
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
		if (isSwitchFunction(x))
		{
			switch (isSwitchFunction(x))
			{
			case 1:
				return typeSet::OPENFUNCTION;
				break;
			case 2:
				return typeSet::CLOSEFUNCTION;
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
		if (last_type == typeSet::OPENFUNCTION)
		{
			return typeSet::STRING;
		}
		if (isVarName(x, stack))
		{
			/* only use the non-return code for on-interpret var->val */
			//x = stack.variableStack.retValue(x);
			return typeSet::VARREF;
		}
		if (isComment(x))
		{
			return typeSet::COMMENT;
		}

		if (isNewStack(x))
		{
			return typeSet::NEWSTACK;
		}
		if (last_type == typeSet::NEWSTACK)
		{
			return typeSet::NEWSTACKNAME;
		}
		
		if (isPushStack(x))
		{
			return typeSet::PUSHSTACK;
		}
		if (last_type == typeSet::PUSHSTACK)
		{
			if (stack.userStackStack.isStack(x))
			{
				return typeSet::PUSHSTACKNAME;
			}
			else
			{
				//std::cout << "ret string on push" << std::endl;
				return typeSet::STRING;
			}
			
		}
		if (last_type == typeSet::PUSHSTACKNAME)
		{
			return typeSet::PUSHSTACKVALUE;
		}

		if (isIndexStack(x))
		{
			return typeSet::INDEXSTACK;
		}
		if (last_type == typeSet::INDEXSTACK)
		{
			return typeSet::INDEXSTACKNAME;
		}
		if (last_type == typeSet::INDEXSTACKNAME)
		{
			return typeSet::INDEXSTACKAT;
		}

		if (isForLoop(x))
		{
			return typeSet::STARTFOR;
		}
		if (isEndLoop(x))
		{
			return typeSet::ENDFOR;
		}

		if (isFinishEarly(x))
		{
			return typeSet::FINISHEARLY;
		}
		return typeSet::NIL;
	}
}
void gloneb_vm(const char* code, Stack& stack)
{
	if (stack.finishEarly)
	{
		return;
	}
	if (stack.functionWriter)
	{
		if (strcmp(code, "closefunction") != 0)
		{
			stack.functionsCode.push_back(code);
		}
		else
		{
			stack.uKeycallStack.pop(stack.functionWriterName, stack.functionsCode);
			stack.functionWriterName = "";
			stack.functionsCode.clear();
			stack.functionWriter = false;
		}
		return;
	}
	if (stack.loopWriter)
	{
		if (miniLexer::isEndLoop(code))
		{
			stack.loopWriter = false;
			for (long long int lop = 0; lop < stack.toLoopNum; lop++)
			{
				for (std::string co : stack.loopCode)
				{
					std::string tcode = "var idx " + std::to_string(lop);
					gloneb_vm(tcode.c_str(), stack);
					gloneb_vm(co.c_str(), stack);
				}
			}
			stack.loopCode.clear();
			stack.loopWriter = false;
			stack.runLoop = false;
			stack.toLoopNum = 0;
		}
		else
		{
			stack.loopCode.push_back(code);
		}
		return;
	}
	const char* holster = code;
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
			continue;
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
			stack.pop(keys[i]);
			keys[i] = stack.variableStack.retValue(keys[i]);
			//std::cout << "new keys[i]: " << keys[i] << std::endl;
			
			stack.pop(keys[i]);
		}
		if (type == typeSet::COMMENT)
		{
			break;
		}
		types.push_back(type);
	}

	bool skipnext = false;
	if (stack.ifWrong)
	{
		for (int type = types.size() - 1; type > -1; type--)
		{
			if (types[type] == typeSet::CLOSEIF)
			{
				stack.ifWrong = false;
			}
		}
		return;
	}

	//std::cout << "running code: " << code << std::endl;
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
		case typeSet::UKEYCALL:
			//std::cout << "ukeycall\n";
			uKeycall::execute(stack, stack.gettop());
			break;
		case typeSet::ADD:
			//if (types[type - 1] == typeSet::STRING || types[type + 1] == typeSet::STRING)
			//{
			//	stack.pop(keys[type - 1] + ' ' + (std::string)stack.gettop());
			//}
			//else
			//{
			//	//std::cout << "Adding: " << (keys[type - 1]);
			//	//std::cout << " + " << (stack.gettop()) << std::endl;
			//	stack.pop(std::to_string(std::stod(keys[type - 1]) + std::stod(stack.gettop())));
			//}
			if (types[type - 1] == typeSet::ODOUBLE || types[type + 1] == typeSet::ODOUBLE ||
				types[type - 1] == typeSet::INTEGER || types[type + 1] == typeSet::INTEGER)
			{
				stack.pop(std::to_string(std::stod(keys[type - 1]) + std::stod(stack.gettop())));
			}
			else
			{
				stack.pop(keys[type - 1] + (std::string)stack.gettop());
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
		case typeSet::POW: //how men
			stack.pop(std::to_string(pow(std::stoi(keys[type - 1]) , std::stoi(stack.gettop()))));
			skipnext = true;
			break;
		case typeSet::VARNAME:
			stack.variableStack.pop(keys[type].c_str(), (char*)stack.gettop().c_str());
			break;
		case typeSet::DEQUAL:
			if (stack.gettop() == keys[type - 1])
			{
				skipnext = true;
				stack.pop("true");
			}
			else
			{
				skipnext = true;
				stack.pop("false");
			}
			break;
		case typeSet::UEQUAL:
			if (stack.gettop() == keys[type - 1])
			{
				skipnext = true;
				stack.pop("false");
			}
			else
			{
				skipnext = true;
				stack.pop("true");
			}
			break;
		case typeSet::GREATERTHAN:
			//std::cout << "Checking if " + stack.gettop() + " > " << keys[type - 1];
			if (keys[type - 1] > stack.gettop())
			{
				skipnext = true;
				stack.pop("true");
			}
			else
			{
				skipnext = true;
				stack.pop("false");
			}
			break;
		case typeSet::LESSTHAN:
			//std::cout << "Checking if " + stack.gettop() + " > " << keys[type - 1];
			if (keys[type - 1] < stack.gettop())
			{
				skipnext = true;
				stack.pop("true");
			}
			else
			{
				skipnext = true;
				stack.pop("false");
			}
			break;
		case typeSet::OPENIF:
			//std::cout << "\nOpenif with stacktop: " << stack.gettop() << std::endl;
			if (stack.gettop() == "true")
			{
				//run
			}
			else
			{
				//dont run
				//std::cout << "set wrong" << std::endl;
				stack.ifWrong = true;
			}
			break;
		case typeSet::OPENFUNCTION:
			stack.functionWriter = true;
			stack.functionWriterName = stack.gettop();
			break;
		case typeSet::NEWSTACKNAME:
			stack.pop(keys[type]);
			break;
		case typeSet::PUSHSTACKNAME:
			stack.pop(keys[type]);
			break;
		case typeSet::PUSHSTACKVALUE:
			stack.pop(keys[type]);
			break;
		case typeSet::INDEXSTACKNAME:
			stack.pop(keys[type]);
			break;
		case typeSet::INDEXSTACKAT:
			stack.pop(keys[type]);
			break;
		case typeSet::NEWSTACK:
			//std::cout << "newstack: " << stack.gettop() << std::endl;
			stack.userStackStack.newStack(stack.gettop());
			break;
		case typeSet::PUSHSTACK:
			if (stack.userStackStack.isStack(stack.gettop()))
			{
				//std::cout << "pushstack: " << stack.gettop();
				// std::cout << " | " << stack.stack[stack.top - 1] << std::endl;
				stack.userStackStack.pushStack(stack.gettop(), stack.stack[stack.top - 1]);
			}
			else
			{
				std::cout << "No such stack called " << stack.gettop() << std::endl;
			}
			break;
		case typeSet::INDEXSTACK:
			if (stack.userStackStack.isStack(stack.gettop()))
			{
				stack.pop(stack.userStackStack.indexStack(stack.gettop(), stack.stack[stack.top - 1]));
			}
			else
			{
				std::cout << "No such stack called " << stack.gettop() << std::endl;
			}
			break;
		case typeSet::STARTFOR:
			stack.loopWriter = true;
			stack.toLoopNum = std::stol(stack.gettop());
			break;
		case typeSet::ENDFOR:
			stack.loopWriter = false;
			stack.runLoop = true;
			break;
		case typeSet::FINISHEARLY:
			//std::cout << "finish early" << std::endl;
			stack.finishEarly = true;
			return;
			break;
		//case typeSet::VARREF:
		//	stack.pop(stack.variableStack.retValue(keys[type]));
		}
		
	}
		
}

