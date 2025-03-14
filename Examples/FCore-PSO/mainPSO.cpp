
// C++
#include <algorithm>
#include <functional>
#include <iostream>
//
#include <OptFCore/FCore.hpp>
#include <OptFrame/Core.hpp>
#include <OptFrame/Heuristics.hpp>  // many metaheuristics here...
#include <OptFrame/Heuristics/EA/PSO.hpp>
#include <OptFrame/Scanner++/Scanner.hpp>
#include <OptFrame/Util/Matrix.hpp>
#include <OptFrame/printable/printable.hpp>

using namespace std;       // NOLINT
using namespace optframe;  // NOLINT

// next definitions come here within namespace
// this also works when defining in global scope (same as 'class')
namespace example_pso {

using ESolutionPSO =
    std::pair<std::vector<double>,  // first part of search space element:
                                    // solution (representation)
              Evaluation<double>    // second part of search space element:
                                    // evaluation (objective value)
              >;

Evaluation<double> fevaluatePSO(const std::vector<double>& s) {
  double f = 0;
  // TODO: calcular erros em relação a solução corrente 's'
  // (x-10)^2
  f = (s[0] - 10) * (s[0] - 10);
  return Evaluation<double>{f};
}

// Evaluate
// FEvaluator<ESolutionPSO, MinOrMax::MINIMIZE> evPSO{fevaluatePSO};

//
}  // namespace example_pso

// import everything on main()
using namespace std;          // NOLINT
using namespace optframe;     // NOLINT
using namespace scannerpp;    // NOLINT
using namespace example_pso;  // NOLINT

int main() {
  FEvaluator<ESolutionPSO, MinOrMax::MINIMIZE> evPSO{fevaluatePSO};
  // good random generator
  RandGen myRandGen;
  // Necessary if you want to set specific seeds
  // Otherwise heuristic uses new RandGen
  int64_t seed = 1000012345L;
  seed = time(NULL);
  myRandGen.setSeed(seed);

  // Model for setting the vector<double> boundaries
  int nParam = 1;
  vector<double> cI(nParam, -100.0);
  vector<double> cS(nParam, 100.0);

  // Particle Swarm Optimization
  // PSO(Evaluator<S, XEv, XES>& evaluator, unsigned pop_size, unsigned
  // iter_max, vector<double> cI, vector<double> cS, sref<RandGen> _rg)
  PSO<> myPSO{evPSO,
              10,  // pop_size
              50,  // iter_max
              cI,  // number of parameters is extracted from cI.size()
              cS,    myRandGen};
  myPSO.setVerboseR();

  auto statusPSO = myPSO.search(5.0);  // 5.0 seconds max

  std::cout << "BEST = " << statusPSO.best->second.evaluation()
            << " sol: " << statusPSO.best->first << std::endl;

  return 0;
}