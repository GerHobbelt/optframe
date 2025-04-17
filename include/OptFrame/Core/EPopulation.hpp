// SPDX-License-Identifier: LGPL-3.0-or-later OR MIT
// Copyright (C) 2007-2022 - OptFrame - https://github.com/optframe/optframe

#ifndef OPTFRAME_EPOPULATION_HPP_
#define OPTFRAME_EPOPULATION_HPP_

#if (__cplusplus < 202302L) || !defined(OPTFRAME_CXX_MODULES)

// C++
#include <string>
#include <utility>
#include <vector>
//
#include <OptFrame/Component.hpp>
#include <OptFrame/Core/Evaluation.hpp>
// #include <OptFrame/Core/Evaluator.hpp>
// #include <OptFrame/Helper/Solution.hpp>

#define MOD_EXPORT
#else

// CANNOT IMPORT HERE... Already part of optframe.core
/*
import std;
import optframe.component;
import optframe.concepts;
*/

// do NOT import/export modules on .hpp... only on .cppm

#define MOD_EXPORT export

#endif

namespace optframe {

//
// EPopulation class is 'final' (WHY??? NOT ANYMORE...)
//
// REQUIRES ostream on XES
MOD_EXPORT template <XESolution XES>  // XES only for evaluation purposes!
class EPopulation : public Component {
  using S = typename XES::first_type;
  using XEv = typename XES::second_type;

 protected:
  std::vector<XES*> p;

 public:
  using value_type = XES;
  EPopulation() {}

  EPopulation(const EPopulation& epop) {
    for (unsigned i = 0; i < epop.size(); i++)
      p.push_back(new XES(epop.at(i)));  // implicit copy constructor
  }

  // move constructor ('steal' population from dying epop)
  EPopulation(EPopulation&& epop) : p{epop.p} {}

  virtual ~EPopulation() { clear(); }

  unsigned size() const { return p.size(); }

  // supports X2ESolution concept standard
  XES& at(unsigned c) { return (*p.at(c)); }

  // supports X2ESolution concept standard
  const XES& at(unsigned c) const { return (*p.at(c)); }

  void push_back(XES* c) {
    if (c)  // not null
      p.push_back(c);
  }

  void push_back(const XES& c) {
    // copy constructor is required
    p.push_back(new XES(c));
  }

  // chromossome is near dying... take everything and drop the corpse!!
  void push_back(XES&& c) { p.push_back(new XES(std::move(c))); }

  // ============ push_back( S )

  void push_back(const S& cs) {
    // copy constructor is required
    p.push_back(new XES(cs, XEv{}));
  }

  XEv& getFitness(int pos) { return p.at(pos)->second; }

  void setFitness(unsigned pos, const XEv& v) { p.at(pos)->second = v; }

  // clear and kill
  void clear() {
    for (unsigned i = 0; i < p.size(); i++) delete p.at(i);
    p.clear();
  }

  virtual EPopulation<XES>& operator=(const EPopulation<XES>& epop) {
    if (&epop == this)  // auto ref check
      return *this;

    clear();  // this is NOT p.clear... pay attention!

    for (unsigned i = 0; i < epop.p.size(); i++)
      p.push_back(new XES(*epop.p[i]));  // implicit copy constructor over XES

    return (*this);
  }

  // assignment from dying epop
  EPopulation<XES>& operator=(EPopulation<XES>&& epop) {
    clear();  // this is NOT vector p.clear... pay attention!
    p = std::move(epop.p);
    epop.p.clear();  // this is vector clear
    return (*this);
  }

  virtual EPopulation<XES>& clone() const {
    return *new EPopulation<XES>(*this);
  }

  static std::string idComponent() {
    std::stringstream ss;
    ss << Component::idComponent() << ":EPopulation";
    return ss.str();
  }

  std::string id() const override { return idComponent(); }

  void print() const override { std::cout << toString() << std::endl; }

  std::string toString() const override {
    std::stringstream ss;
    ss << "EPopulation(" << p.size() << ")";
    ss << std::endl;

    for (unsigned i = 0; i < p.size(); i++) {
      // ss << *p.at(i) << std::endl;
      // TODO: do not force operator<< here
    }
    return ss.str();
  }

};  // class EPopulation

}  // namespace optframe

// compilation tests
// TODO: only add if not including "printable.h"
// static_assert(X2ESolution<EPopulation<std::pair<Solution<double>,
// Evaluation<double>>>, std::pair<Solution<double>, Evaluation<double>>>);

#endif /* OPTFRAME_POPULATION_HPP_ */
