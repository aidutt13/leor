#include "Parser/AST.h"

namespace leor
{
  BasicAST::BasicAST(
    Type type,
    const std::tuple<uint64_t, uint64_t>& pos
  )
    : type(type), pos(pos)
  { }

  IntAST::IntAST(
    int64_t value,
    const std::tuple<uint64_t, uint64_t>& pos
  )
    : BasicAST(Type::INT, pos), value(value)
  { }

  FloatAST::FloatAST(
    double value,
    const std::tuple<uint64_t, uint64_t>& pos
  )
    : BasicAST(Type::FLOAT, pos), value(value)
  { }

  StringAST::StringAST(
    std::string value,
    const std::tuple<uint64_t, uint64_t>& pos
  )
    : BasicAST(Type::STRING, pos), value(value)
  { }

  CharAST::CharAST(
    char value,
    const std::tuple<uint64_t, uint64_t>& pos
  )
    : BasicAST(Type::CHAR, pos), value(value)
  { }

  VarAST::VarAST(
    std::string name,
    const std::tuple<uint64_t, uint64_t>& pos
  )
    : BasicAST(Type::VAR, pos), name(name)
  { }
} // namespace leor
