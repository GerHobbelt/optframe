// OptFrame 4.2 - Optimization Framework
// Copyright (C) 2009-2021 - MIT LICENSE
// https://github.com/optframe/optframe
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef OPTFRAME_OPERATOR_ASSIGN_MODULE_HPP_
#define OPTFRAME_OPERATOR_ASSIGN_MODULE_HPP_

#include "../Command.hpp"

namespace optframe {

template <class R, class ADS = OPTFRAME_DEFAULT_ADS,
          class DS = OPTFRAME_DEFAULT_DS>
class OperatorAssignCommand : public Command<R, ADS, DS> {
 public:
  virtual ~OperatorAssignCommand() {}

  string id() { return "operator.assign"; }

  string usage() {
    return "operator.assign variable = value \nvariable=value (module called "
           "implicitly)";
  }

  virtual bool canHandle(string module_name, string command_body) {
    if (id() == module_name) return true;

    for (unsigned i = 0; i < Command<R, ADS, DS>::handles.size(); i++)
      if (Command<R, ADS, DS>::handles[i] == module_name) return true;

    string tbody = Scanner::trim(command_body);
    if ((tbody.length() > 0) && tbody[0] == '=') return true;

    return false;
  }

  virtual bool run(std::vector<Command<R, ADS, DS>*>& allCommands,
                   vector<PreprocessFunction<R, ADS, DS>*>& allFunctions,
                   HeuristicFactory<R, ADS, DS>& factory,
                   std::map<std::string, std::string>& dictionary,
                   std::map<std::string, std::vector<std::string>>& ldictionary,
                   string input, string module_name) {
    /// cout << "ASSIGN:'" << input << "' and module_name='" << module_name <<
    /// "'" << std::endl;
    module_name.append(" ");
    module_name.append(input);
    return run(allCommands, allFunctions, factory, dictionary, ldictionary,
               module_name);
  }

  bool run(std::vector<Command<R, ADS, DS>*>&,
           vector<PreprocessFunction<R, ADS, DS>*>&,
           HeuristicFactory<R, ADS, DS>&,
           std::map<std::string, std::string>& dictionary,
           std::map<std::string, std::vector<std::string>>& ldictionary,
           string input) {
    /// cout << "ASSIGN:'" << input << "'" << std::endl;

    Scanner scanner(input);

    if (!scanner.hasNext()) {
      std::cout << "module " << id() << " missing variable name!" << std::endl;
      return false;
    }

    string first = scanner.next();
    string var_name = first;
    if ((first == "operator.assign") || (first == "assign")) {
      // read again the variable
      if (!scanner.hasNext()) {
        std::cout << "module " << id() << " missing variable name!"
                  << std::endl;
        return false;
      }

      var_name = scanner.next();
    }

    if (!scanner.hasNext()) {
      std::cout << "module " << id() << " missing assignment '='!" << std::endl;
      return false;
    }

    string assign = scanner.next();

    if (assign != "=") {
      std::cout << "module " << id() << " error: expected '='!" << std::endl;
      return false;
    }

    if (!scanner.hasNext()) {
      std::cout << "module " << id() << " missing value!" << std::endl;
      return false;
    }

    string value = scanner.rest();

    if (!Command<R, ADS, DS>::define(var_name, value, dictionary,
                                     ldictionary)) {
      std::cout << "module " << id() << " error: failed to define '" << var_name
                << "' to '" << value << "'" << std::endl;
      return false;
    }

    return true;
  }

  string addSpacesForDoubleDots(string input) {
    string output = "";
    unsigned i = 0;
    while (i < input.length()) {
      if ((input[i] == '.') && ((i + 1) < input.length()) &&
          (input[i + 1] == '.')) {
        output.append(" .. ");
        i += 2;
      } else {
        output += input[i];
        i++;
      }
    }

    return output;
  }

  virtual string* preprocess(
      std::vector<PreprocessFunction<R, ADS, DS>*>& allFunctions,
      HeuristicFactory<R, ADS, DS>& hf,
      const std::map<std::string, std::string>& dictionary,
      const std::map<std::string, std::vector<std::string>>& ldictionary,
      string input) {
    // ============================================
    // add spaces before and after FIRST assignment
    // ============================================

    string new_input = "";

    unsigned irest = 0;
    for (unsigned i = 0; i < input.length(); i++)
      if (input[i] == '=') {
        new_input.append(" = ");
        irest = i + 1;
        break;
      } else
        new_input += input[i];

    string rest = "";

    for (unsigned i = irest; i < input.length(); i++) rest += input[i];

    // ==========================
    // call default preprocessing
    // ==========================

    // add spaces before and after double dots '..' and do normal preprocessing
    string* proc = Command<R, ADS, DS>::defaultPreprocess(
        allFunctions, hf, dictionary, ldictionary,
        addSpacesForDoubleDots(rest));

    if (!proc) return nullptr;

    new_input.append(*proc);
    delete proc;
    return new string(new_input);
  }
};

}  // namespace optframe

#endif /* OPTFRAME_LIST_DEFINE_MODULE_HPP_ */
