#pragma once

#include <ios>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include "mytypes.h"

class Node
{
public:
  Node() {}
  virtual ~Node() {}
};

struct AST
{
  enum Type
  {
    NONE, EOB,
    BOOL, CHAR,
    INT, FLOAT,
    STRING, VAR,
    BINARY, ASSIGN,
    VARDEF, CALL,
    PROG, FUNCTION
  };

  using PosType = typename std::tuple<uint64, uint64>;

  static const std::map<Type, std::string_view> TypeToString;

  Type type;
  Node* node;
  PosType pos;

  ~AST();

  friend std::ostream& operator<<(std::ostream&, const Type);
};

struct NoneNode : Node
{
  NoneNode();
};

#define VALUENODE(name, ValueType)\
struct name : Node\
{\
  ValueType value;\
  name(ValueType _value) : Node(), value(_value)\
  { }\
}

// template<typename T>
// struct ValueNode : Node
// {
//   T value;

//   ValueNode(T _value)
//     : Node()
//   {
//     value = _value;
//   }
// };

VALUENODE(BoolNode, bool);
VALUENODE(CharNode, int8);
VALUENODE(IntNode, int64);
VALUENODE(FloatNode, float);
VALUENODE(StringNode, std::string);

#undef VALUENODE

struct BinaryNode : Node
{
  std::string op;
  AST left;
  AST right;

  BinaryNode(std::string, AST, AST);
};

struct VarDefNode : Node
{
  std::string name;
  std::string typeName;
  bool isConst;
  AST value;

  VarDefNode(std::string, std::string, bool, AST);
};

struct FunctionNode : Node
{
  std::string name;
  std::string returnType;
  std::vector<AST> params;
  AST body;

  FunctionNode(std::string, std::string, std::vector<AST>, AST);
};

struct CallNode : Node
{
  AST func;
  std::vector<AST> args;

  CallNode(AST, std::vector<AST>);
};

struct ProgNode : Node
{
  std::vector<AST> prog;

  ProgNode(std::vector<AST>);
};

template<typename NODE>
static NODE CastNode(Node*);