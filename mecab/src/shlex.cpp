#include "shlex.h"
#include <vector>
#include <string>
#include <stdexcept>

// Implementation is based on Python's shlex module.
// https://github.com/python/cpython/blob/10cbd1fe88d1095a03cce24fb126d479668a67c3/Lib/shlex.py#L253

shlex::optional<std::string> shlex::shlex::get_token()
{
  return read_token();
}

shlex::optional<std::string> shlex::shlex::read_token()
{
  bool quoted = false;
  char escapedstate = ' ';
  while (true)
  {
    const bool eof = pos_ >= s_.size();
    const char nextchar = eof ? '\0' : s_[pos_];
    pos_++;
    if (state_ == '\0')
    {
      token_ = "";
      break;
    }
    else if (state_ == ' ')
    {
      if (eof)
      {
        state_ = '\0';
        break;
      }
      else if (nextchar == ' ' || nextchar == '\t' || nextchar == '\r' || nextchar == '\n')
      {
        if (!token_.empty() || quoted)
        {
          break; // emit current token
        }
        else
        {
          continue;
        }
      }
      else if (nextchar == '\\')
      {
        escapedstate = 'a';
        state_ = nextchar;
      }
      else if (nextchar == '\'' || nextchar == '"')
      {
        state_ = nextchar;
      }
      else
      {
        token_ = nextchar;
        state_ = 'a';
      }
    }
    else if (state_ == '\'' || state_ == '"')
    {
      quoted = true;
      if (eof)
      {
        throw std::runtime_error("no closing quotation");
      }
      if (nextchar == state_)
      {
        state_ = 'a';
      }
      else if (nextchar == '\\' && state_ == '"')
      {
        escapedstate = state_;
        state_ = nextchar;
      }
      else
      {
        token_ += nextchar;
      }
    }
    else if (state_ == '\\')
    {
      if (eof)
      {
        throw std::runtime_error("no escaped character");
      }
      // In posix shells, only the quote itself or the escape
      // character may be escaped within quotes.
      if ((escapedstate == '\'' || escapedstate == '"') && nextchar != state_ && nextchar != escapedstate)
      {
        token_ += state_;
      }
      token_ += nextchar;
      state_ = escapedstate;
    }
    else if (state_ == 'a')
    {
      if (eof)
      {
        state_ = '\0';
        break;
      }
      else if (nextchar == ' ' || nextchar == '\t' || nextchar == '\r' || nextchar == '\n')
      {
        state_ = ' ';
        if (!token_.empty() || quoted)
        {
          break; // emit current token
        }
        else
        {
          continue;
        }
      }
      else if (nextchar == '\'' || nextchar == '"')
      {
        state_ = nextchar;
      }
      else if (nextchar == '\\')
      {
        escapedstate = 'a';
        state_ = nextchar;
      }
      else
      {
        token_ += nextchar;
      }
    }
  }

  std::string result = token_;
  token_.clear();
  if (!quoted && result.empty())
  {
    return optional<std::string>();
  }
  return optional<std::string>(result);
}

std::vector<std::string> shlex::split(const std::string &s)
{
  std::vector<std::string> result;
  shlex lex(s);
  while (true)
  {
    optional<std::string> token = lex.get_token();
    if (!token)
    {
      break;
    }
    result.push_back(*token);
  }
  return result;
}

static bool contains_unsafe_char(const std::string &s)
{
  for (std::string::const_iterator it = s.begin(); it != s.end(); ++it)
  {
    char c = *it;
    if ('0' <= c && c <= '9')
    {
      continue;
    }
    if ('A' <= c && c <= 'Z')
    {
      continue;
    }
    if ('a' <= c && c <= 'z')
    {
      continue;
    }
    if (c == '@' || c == '_' || c == '%' || c == '+' || c == '=' || c == ':' || c == ',' || c == '.' || c == '/' || c == '-' || c == '\'')
    {
      continue;
    }
    return true;
  }

  return false;
}

std::string shlex::quote(const std::string &s)
{
  if (s.empty())
  {
    return "''";
  }
  if (!contains_unsafe_char(s))
  {
    return s;
  }

  // use single quotes, and put single quotes into double quotes
  // the string $'b is then quoted as '$'"'"'b'
  std::string result = "'";
  for (std::string::const_iterator it = s.begin(); it != s.end(); ++it)
  {
    char c = *it;
    if (c == '\'')
    {
      result += "'\"'\"'";
    }
    else
    {
      result += c;
    }
  }
  result += "'";
  return result;
}
