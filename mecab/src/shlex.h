#ifndef SHOGO82148_SHLEX_H_
#define SHOGO82148_SHLEX_H_

#include <vector>
#include <string>
#include <stdexcept>

namespace shlex
{
  // simple backport of std::optional
  template <typename T>
  class optional
  {
  private:
    bool has_value_;
    T value_;

  public:
    typedef T value_type;

    struct nullopt_t
    {
      explicit nullopt_t() {}
    };
    static const nullopt_t nullopt;

    // default constructor
    optional() : has_value_(false), value_() {}
    optional(nullopt_t) : has_value_(false), value_() {}

    // copy constructor
    optional(const optional &other) : has_value_(other.has_value_), value_(other.value_) {}

    // constructor with value
    optional(const T &value) : has_value_(true), value_(value) {}

    // destructor
    ~optional() {}

    // copy assignment operator
    optional &operator=(const optional &other)
    {
      if (this != &other)
      {
        has_value_ = other.has_value_;
        value_ = other.value_;
      }
      return *this;
    }

    // nullopt assignment operator
    optional &operator=(nullopt_t)
    {
      has_value_ = false;
      value_ = T();
      return *this;
    }

    // value assignment operator
    optional &operator=(const T &value)
    {
      has_value_ = true;
      value_ = value;
      return *this;
    }

    // returns true if the optional has a value
    bool has_value() const
    {
      return has_value_;
    }
    operator bool() const
    {
      return has_value_;
    }

    // returns the value if the optional has a value, otherwise throws an exception
    T &value()
    {
      if (!has_value_)
      {
        throw std::runtime_error("optional does not have a value");
      }
      return value_;
    }
    const T &value() const
    {
      if (!has_value_)
      {
        throw std::runtime_error("optional does not have a value");
      }
      return value_;
    }
    T &operator*()
    {
      return value();
    }
    const T &operator*() const
    {
      return value();
    }
  };

  template <typename T>
  const typename optional<T>::nullopt_t optional<T>::nullopt = optional<T>::nullopt_t();

  class shlex
  {
  private:
    std::string s_;
    std::string::size_type pos_;
    std::string token_;
    char state_;

  public:
    shlex(const std::string &s) : s_(s), pos_(0), token_(""), state_(' ') {}

    // get a token from the input string.
    optional<std::string> get_token();
    optional<std::string> read_token();
  };

  // split the string s using shell-like syntax.
  std::vector<std::string> split(const std::string &s);

  // returns a shell-escaped version of the string s.
  std::string quote(const std::string &s);
}

#endif // SHOGO82148_SHLEX_H_
