#include "Parser/AST.h"

namespace leor
{
  using namespace std::string_literals;

  AST& AST::set(Type type)
  {
    this->type = type;
    return *this;
  }

  AST& AST::set(const std::tuple<uint64_t, uint64_t>& pos)
  {
    this->pos = pos;
    return *this;
  }

  AST& AST::set(const std::string& key, const AST& value)
  {
    values.insert_or_assign(key, Base(std::move(value)));
    return *this;
  }

  AST AST::None()
  {
    return AST().set(AST::Type::NONE);
  }

  AST AST::Bool(const bool& value, const std::tuple<uint64_t, uint64_t> pos)
  {
    return AST().set(AST::Type::BOOL).set(pos).set("value", value);
  }

  AST AST::Int(const int64_t& value, const std::tuple<uint64_t, uint64_t> pos)
  {
    return AST().set(AST::Type::INT).set(pos).set("value", value);
  }

  AST AST::Float(const double& value, const std::tuple<uint64_t, uint64_t> pos)
  {
    return AST().set(AST::Type::FLOAT).set(pos).set("value", value);
  }

  AST AST::String(const std::string& value, const std::tuple<uint64_t, uint64_t> pos)
  {
    return AST().set(AST::Type::STRING).set(pos).set("value", value);
  }

  AST AST::Char(const char& value, const std::tuple<uint64_t, uint64_t> pos)
  {
    return AST().set(AST::Type::CHAR).set(pos).set("value", value);
  }

  AST AST::Var(const std::string& value, const std::tuple<uint64_t, uint64_t> pos)
  {
    return AST().set(AST::Type::VAR).set(pos).set("value", value);
  }

  AST AST::Function(
    const std::string& name,
    const std::vector<AST>& args,
    const AST& body,
    const std::string& type,
    const std::tuple<uint64_t, uint64_t> pos
  ) {
    
    return AST()
      .set(AST::Type::FUNCTION)
      .set(pos)
      .set("name", name)
      .set("args", args)
      .set("type", type)
      .set("body", body);
  }

  AST AST::Variable(
    const std::string& name,
    const std::string& type,
    const AST& value,
    const bool& is_constant,
    const std::tuple<uint64_t, uint64_t> pos
  ) {

    return AST()
      .set(AST::Type::VARIABLE)
      .set(pos)
      .set("name", name)
      .set("type", type)
      .set("value", value)
      .set("is_constant", is_constant);
  }

  AST AST::Call(
    AST function,
    const std::vector<AST>& args,
    const std::tuple<uint64_t, uint64_t> pos
  ) {

    return AST()
      .set(AST::Type::CALL)
      .set(pos)
      .set("function", function)
      .set("args", args);
  }

  AST AST::Binary(
    const std::string& op,
    const AST& left,
    const AST& right,
    const std::tuple<uint64_t, uint64_t> pos
  ) {

    return AST()
      .set(AST::Type::BINARY)
      .set(pos)
      .set("op", op)
      .set("left", left)
      .set("right", right);
  }

  AST AST::Assign(
    const std::string& op,
    const AST& left,
    const AST& right,
    const std::tuple<uint64_t, uint64_t> pos
  ) {

    return AST::Binary(op, left, right, pos)
      .set(AST::Type::ASSIGN);
  }

  AST AST::Prog(
    const std::vector<AST>& prog,
    const std::tuple<uint64_t, uint64_t> pos
  ) {

    return AST()
      .set(AST::Type::PROG)
      .set(pos)
      .set("prog", prog);
  }

  const std::unordered_map<std::string, uint64_t> OP_PRECEDENCE
  {{
    {"=",  1},
    {"||", 2},
    {"&&", 3},
    {"<",  7}, {">",  7}, {"<=", 7}, {">=", 7}, {"==", 7}, {"!=", 7},
    {"+", 10}, {"-", 10},
    {"*", 20}, {"/", 20}, {"%", 20},
  }};
} // namespace leor
