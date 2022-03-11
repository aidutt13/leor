#include "parser/ast.h"

#ifdef LEOR_DEBUG
  #include <iostream>
#endif

using namespace std::literals;

AST::~AST()
{
  delete node;
}

const std::map<AST::Type, std::string_view> AST::TypeToString
{{
  { Type::NONE,     "NONE"sv     },
  { Type::EOB,      "EOB"sv      },
  { Type::BOOL,     "BOOL"sv     },
  { Type::CHAR,     "CHAR"sv     },
  { Type::INT,      "INT"sv      },
  { Type::FLOAT,    "FLOAT"sv    },
  { Type::STRING,   "STRING"sv   },
  { Type::VAR,      "VAR"sv      },
  { Type::BINARY,   "BINARY"sv   },
  { Type::ASSIGN,   "ASSIGN"sv   },
  { Type::VARDEF,   "VARDEF"sv   },
  { Type::PROG,     "PROG"sv     },
  { Type::FUNCTION, "FUNCTION"sv },
}};

std::ostream& operator<<(std::ostream& os, AST::Type t)
{

  try
  {
    os << AST::TypeToString.at(t);
  }
  catch (std::out_of_range ex)
  {
    #ifdef LEOR_DEBUG
      std::cerr << ex.what() << "AST::Type::" << (uint64)t << std::endl;
      exit(1);
    #else
      os << (uint64)t;
    #endif
  }
  return os;
}

NoneNode::NoneNode()
  : Node()
{ }

BinaryNode::BinaryNode(std::string _op, AST _left, AST _right)
  : Node(), op(_op), left(_left), right(_right)
{ }

VarDefNode::VarDefNode(std::string _name, std::string _typeName, bool _isConst, AST _value)
  : Node(), name(_name), typeName(_typeName), isConst(_isConst), value(_value)
{ }

FunctionNode::FunctionNode(
  std::string _name, std::string _returnType, std::vector<AST> _params, AST _body
)
  : Node(), name(_name), returnType(_returnType), params(_params), body(_body)
{ }

CallNode::CallNode(AST _func, std::vector<AST> _args)
  : Node(), func(_func), args(_args)
{ }

ProgNode::ProgNode(std::vector<AST> _prog)
  : Node(), prog(_prog)
{ }

template<typename NODE>
NODE* CastNode(Node* node)
{
  return static_cast<NODE*>( node );
}