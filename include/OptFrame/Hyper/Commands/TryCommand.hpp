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

#ifndef TRY_MODULE_HPP_
#define TRY_MODULE_HPP_

#include <string>
#include <vector>

#include "../Command.hpp"

namespace optframe {

template <class R, class ADS = OPTFRAME_DEFAULT_ADS,
          class DS = OPTFRAME_DEFAULT_DS>
class TryCommand : public Command<R, ADS, DS> {
 private:
  Command<R, ADS, DS>* getCommand(std::vector<Command<R, ADS, DS>*>& modules,
                                  string module) {
    for (unsigned int i = 0; i < modules.size(); i++)
      if (module == modules[i]->id()) return modules[i];
    return nullptr;
  }

  bool exec_command(
      std::vector<Command<R, ADS, DS>*>& all_modules,
      vector<PreprocessFunction<R, ADS, DS>*>& allFunctions,
      HeuristicFactory<R, ADS, DS>& factory,
      std::map<std::string, std::string>& dictionary,
      std::map<std::string, std::vector<std::string>>& ldictionary,
      string command) {
    Scanner scanner(command);
    string module = scanner.next();
    Command<R, ADS, DS>* m = getCommand(all_modules, module);

    if (m == nullptr) return false;

    string* rest = m->preprocess(allFunctions, factory, dictionary, ldictionary,
                                 scanner.rest());

    if (!rest) return nullptr;

    bool b = m->run(all_modules, allFunctions, factory, dictionary, ldictionary,
                    *rest);

    delete rest;

    return b;
  }

 public:
  virtual ~TryCommand() {}

  string id() { return "try"; }

  string usage() {
    return "try block_of_try_commands [except block_of_exception_commands]";
  }

  bool run(std::vector<Command<R, ADS, DS>*>& allCommands,
           vector<PreprocessFunction<R, ADS, DS>*>& allFunctions,
           HeuristicFactory<R, ADS, DS>& factory,
           std::map<std::string, std::string>& dictionary,
           std::map<std::string, std::vector<std::string>>& ldictionary,
           string input) {
    Scanner scanner(input);

    if (!scanner.hasNext()) {
      std::cout << "Usage: " << usage() << std::endl;
      return false;
    }

    vector<string> ltry;
    vector<string>* p_ltry = OptFrameList::readBlock(scanner);
    if (p_ltry) {
      ltry = vector<string>(*p_ltry);
      delete p_ltry;
    } else
      return false;

    vector<string> lcatch;
    if (scanner.hasNext()) {
      string text_except = scanner.next();

      if (text_except != "except") {
        std::cout << "try command: error expecting word 'except' and got '"
                  << text_except << "'" << std::endl;
        return false;
      }

      vector<string>* p_lcatch = OptFrameList::readBlock(scanner);
      if (p_lcatch) {
        lcatch = vector<string>(*p_lcatch);
        delete p_lcatch;
      } else
        return false;
    }

    if (!Command<R, ADS, DS>::testUnused(id(), scanner)) return false;

    bool ok = true;

    for (unsigned int c = 0; c < ltry.size(); c++) {
      string command = ltry.at(c);

      if (command.at(0) == '%')  // first line comment
        command = "";

      if (command != "")
        if (!exec_command(allCommands, allFunctions, factory, dictionary,
                          ldictionary, command)) {
          // EXPECTED ERROR, NO MESSAGE!
          /*
                                   if (ltry.at(c) == "")
                                           cout << "try command: (TRY) empty
             command! (perhaps an extra comma in list?)" << std::endl; else cout
             << "try command: (TRY) problem in command '" << ltry.at(c) << "'"
             << std::endl;
                                   */

          ok = false;
          break;
        }
    }

    if (!ok) {
      // DEBUG MESSAGE ONLY
      /*
                     if(lcatch.size()>0)
                             cout << "try_catch command: calling CATCH block" <<
         std::endl;
                     */

      for (unsigned int c = 0; c < lcatch.size(); c++) {
        string command = lcatch.at(c);

        if (command.at(0) == '%')  // first line comment
          command = "";

        if (command != "")
          if (!exec_command(allCommands, allFunctions, factory, dictionary,
                            ldictionary, command)) {
            if (lcatch.at(c) == "")
              std::cout << "try command: (AFTER ERROR) empty command! (perhaps "
                           "an extra comma in list?)"
                        << std::endl;
            else
              std::cout << "try command: (AFTER ERROR) error in command '"
                        << lcatch.at(c) << "'!" << std::endl;

            return false;
          }
      }
    }

    return true;
  }

  // should preprocess only until list of commands
  virtual string* preprocess(
      std::vector<PreprocessFunction<R, ADS, DS>*>& allFunctions,
      HeuristicFactory<R, ADS, DS>& hf,
      const std::map<std::string, std::string>& dictionary,
      const std::map<std::string, std::vector<std::string>>& ldictionary,
      string input) {
    string ibegin = "";
    string iend = "";
    unsigned j = 0;
    for (unsigned i = 0; i < input.length(); i++) {
      if (input.at(i) == '{')
        break;
      else
        ibegin += input.at(i);
      j++;
    }

    for (unsigned k = j; k < input.length(); k++) iend += input.at(k);

    string* ninput = Command<R, ADS, DS>::defaultPreprocess(
        allFunctions, hf, dictionary, ldictionary, ibegin);

    if (!ninput) return nullptr;

    ninput->append(" ");  // after boolean value
    ninput->append(iend);

    return ninput;
  }
};

}  // namespace optframe

#endif /* TRY_MODULE_HPP_ */
