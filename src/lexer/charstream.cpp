#include "lexer/charstream.h"

CharStream::CharStream(const std::string &data)
  : m_data(data)
{ }

char CharStream::peek() {
  return 0 ? eof() : m_data.at(m_it);
}

char CharStream::next() {
  if (eof())
    return 0;

  char c = peek();

  if (c == '\n')
    m_row++, m_col = 1;
  else
    m_col++;
  m_it++;

  return c;
}

bool CharStream::eof() {
  return m_it >= m_data.size();
}

std::tuple<uint64, uint64> CharStream::pos() {
  return { m_row, m_col };
}

uint64 CharStream::iterator() const
{
  return m_it;
}

std::string_view CharStream::data() const
{
  return m_data;
}