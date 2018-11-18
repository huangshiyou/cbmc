/*******************************************************************\

Module:

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef CPROVER_SOLVERS_SMT2_SMT2_TOKENIZER_H
#define CPROVER_SOLVERS_SMT2_SMT2_TOKENIZER_H

#include <util/exception_utils.h>
#include <util/parser.h>

#include <string>

class smt2_tokenizert:public parsert
{
public:
  explicit smt2_tokenizert(std::istream &_in)
    : ok(true), peeked(false), token(NONE), parenthesis_level(0)
  {
    in=&_in;
    line_no=1;
  }

  operator bool()
  {
    return ok;
  }

protected:
  std::string buffer;
  bool ok, peeked;
  using tokent = enum {
    NONE,
    END_OF_FILE,
    STRING_LITERAL,
    NUMERAL,
    SYMBOL,
    OPEN,
    CLOSE
  };
  tokent token;

  tokent next_token();

  tokent peek()
  {
    if(peeked)
      return token;
    else
    {
      get_token_from_stream();
      peeked=true;
      return token;
    }
  }

  mstreamt &error()
  {
    ok=false;
    messaget::error().source_location.set_line(line_no);
    return messaget::error();
  }

  /// skip any tokens until all parentheses are closed
  /// or the end of file is reached
  void skip_to_end_of_list();

  struct smt2_errort : public cprover_exception_baset
  {
    /// this is a dummy -- errors are reported via the error() stream
    std::string what() const override
    {
      return "";
    }
  };

private:
  tokent get_decimal_numeral();
  tokent get_hex_numeral();
  tokent get_bin_numeral();
  tokent get_simple_symbol();
  tokent get_quoted_symbol();
  tokent get_string_literal();
  static bool is_simple_symbol_character(char);

  /// read a token from the input stream and store it in 'token'
  void get_token_from_stream();

protected:
  // this is increased for '(' and decreased for ')'
  std::size_t parenthesis_level;
};

#endif // CPROVER_SOLVERS_SMT2_SMT2_PARSER_H
