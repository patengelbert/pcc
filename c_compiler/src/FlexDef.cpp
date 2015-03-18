

#include "../includes/FlexDef.h"
#include <algorithm>
#include <iostream>

Node::Node(tokType id, std::string val)
	:id(id){
	this -> node_type = "baseNode";
	this -> val = val;
};

Node::~Node(){};
	
parserNode::parserNode(tokType id, std::string val, abstractNode* LHS, abstractNode* OP, abstractNode* RHS)
	:LHS(LHS),OP(OP),RHS(RHS){
	Node::node_type = "parserNode";
	Node::id = id;
	Node::val = val;
};

parserNode::~parserNode()
{
	delete LHS;
	delete OP;
	delete RHS;
};

variableNode::variableNode(tokType id, std::string val, type_t type, std::string namespacev)
	:storage(NULLS_T),length(NULLL_T),sign(NULLI_T),type(type), namespacev(namespacev)
{
	Node::node_type = "variableNode";
	Node::id = id;
	Node::val = val;
};

castNode::castNode(tokType id, type_t castType)
	:castType(castType)
{
	Node::node_type = "castNode";
	Node::id = id;
	Node::val = NULL_S;
};

forNode::forNode(tokType id, std::string val, abstractNode* initial, abstractNode* condition, abstractNode* repeat)
	:initial(initial),condition(condition), repeat(repeat)
{
	Node::node_type = "forNode";
	Node::id = id;
	Node::val = val;
}

forNode::~forNode()
{
	delete initial;
	delete condition;
	delete repeat;
}

condNode::condNode(tokType id, std::string val, abstractNode* condition, abstractNode* cond_true, abstractNode* cond_false)
	:condition(condition), cond_true(cond_true),cond_false(cond_false)
{
	Node::node_type = "condNode";
	Node::id = id;
	Node::val = val;
}

condNode::~condNode()
{
	delete condition;
	delete cond_true;
	delete cond_false;
}

functionNode::functionNode(tokType id, std::string val, abstractNode* functionDef, abstractNode* code)
	:code(code)
{
	Node::node_type = "functionNode";
	Node::id = id;
	Node::val = val;
	def = functionDef;
}

functionNode::~functionNode()
{
	delete code;
	delete def;
}

functionDecNode::functionDecNode(tokType id, variableNode* variableDef, std::vector<struct_member> parameters)
	:parameters(parameters)
{
	Node::id = id;
	Node::node_type = "functionDecNode";
	Node::val = variableDef -> val;
	storage = variableDef -> storage;
	sign = variableDef -> sign;
	length = variableDef -> length;
	
	type = variableDef -> type;
}

functionCallNode::functionCallNode(tokType id, std::string val, abstractNode* parameters)
	:parameters(parameters)
{
	Node::id = id;
	Node::node_type = "functionCallNode";
	Node::val = val;
}

functionCallNode::~functionCallNode()
{
	delete parameters;
}

struct find_type : std::unary_function<type_s, bool> {
    	std::string namespacev;
	std::string name;
    	find_type(std::string namespacev, std::string name):namespacev(namespacev), name(name) { }
    	bool operator()(type_s const& s) const {
        	return s.namespacev == namespacev && s.name == name;
    	}
};

void variableNode::evaluateModifiers(const list_t modifiers){	//throws modifiers_not_used_exception
	

	/*for(int i = 0; i < modifiers.size(); i++)
	{
		if(modifiers[i] ==  "long" && length == NULLL_T)
			length = LONG_T;
		else if(modifiers[i] == "short" && length == NULLL_T)
			length = SHORT_T;
		else if(modifiers[i] == "signed" && sign == NULLI_T)
			sign = SIGNED_T;
		else if(modifiers[i] == "unsigned" && sign == NULLI_T)
			sign = UNSIGNED_T;
		else if(modifiers[i] == "auto" && storage == NULLS_T)
			storage = AUTO_T;
		else if(modifiers[i] == "register" && storage == NULLS_T)
			storage = REGISTER_T;
		else if(modifiers[i] == "static" && storage == NULLS_T)
			storage = STATIC_T;
		else if(modifiers[i] == "extern" && storage == NULLS_T)
			storage = EXTERN_T;
		else{}
			//throw new modifiers_not_used_exception();
	}*/
}

type_t getType(const char* name, std::string namespacev)
{
	
	std::list<type_s>::iterator findIter = std::find_if(types.begin(), types.end(), find_type(namespacev,name));
	return (findIter != types.end()? &(*findIter):NULL);
}

type_t getPointer(const char* name)
{
	
	std::list<type_s>::iterator findIter = std::find_if(pointers.begin(), pointers.end(), find_type(NULL_S,name));
	return (findIter != pointers.end()? &(*findIter):NULL);
}


type_t addType(std::string namespacev, std::string name, type_s* base, std::vector<struct_member> members)
{
	type_s newType;
	newType.namespacev = namespacev;
	newType.name = name;
	newType.base = base;
	newType.members = members;
	types.insert(types.end(), newType);
	return &(types.back());
}

type_t addPointer(std::string name, type_s* deref)
{
	type_s newType;
	newType.namespacev = NULL_S;
	newType.name = name;
	newType.base = deref;
	newType.members = *(new std::vector<struct_member>());
	pointers.insert(pointers.end(), newType);
	return &(pointers.back());
}

std::vector<struct_member> build_struct_members(const struct_list_t memberList)
{
	std::vector<struct_member> members;
	std::list<struct_member>::iterator i;
	for( i = memberList->begin(); i != memberList->end(); i++)
	{
		members.push_back(*i);
	}
	return members;
}