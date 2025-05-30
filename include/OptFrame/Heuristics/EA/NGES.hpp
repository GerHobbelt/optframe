// SPDX-License-Identifier: LGPL-3.0-or-later OR MIT
// Copyright (C) 2007-2024 - OptFrame - https://github.com/optframe/optframe

#ifndef OPTFRAME_HEURISTICS_EA_NGES_HPP_
#define OPTFRAME_HEURISTICS_EA_NGES_HPP_

// C
#include <math.h>

// C++
#include <memory>  // shared_ptr
#include <string>
#include <vector>

// optframe
#include <OptFrame/Core/Constructive.hpp>
#include <OptFrame/Core/Evaluation.hpp>
#include <OptFrame/Core/Evaluator.hpp>
#include <OptFrame/Core/NS.hpp>
#include <OptFrame/Core/RandGen.hpp>
#include <OptFrame/Search/LocalSearch.hpp>
#include <OptFrame/Search/SingleObjSearch.hpp>
#include <OptFrame/Timer.hpp>

// #include <gsl/gsl_rng.h>
// #include <gsl/gsl_randist.h>
// #include <OptFrame/Core/NSSeq.hpp>

// NGES - Neighborhood Guided Evolution Strategies

namespace optframe {

// template<XESolution XES, XEvaluation XEv = Evaluation<>>
template <XSolution S, XEvaluation XEv = Evaluation<>,
          XESolution XES = pair<S, XEv>>
struct NGESIndStructure {
  double pr;  // probability
  int nap;    // number of applications
  double sigmaN;
  double sigmaB;

  NGESIndStructure(double _pr, int _nap, double _sigmaN, double _sigmaB)
      : pr(_pr), nap(_nap), sigmaN(_sigmaN), sigmaB(_sigmaB) {}
};

// vNSeqMaxApplication -- vector with the max number of moves of each neigh
// optional local search, selectionMethod, mutationRate, others...
struct NGESParams {
  vector<int> vNSeqMaxApplication;
  int selectionMethod;
  double mutationRate;
  int mi;
  int lambda;
  int gMaxWithoutImprovement;
  string outputFile;
  int batch;

  NGESParams(std::vector<int> _vNSeqMaxApplication, int _selectionMethod,
             double _mutationRate, int _mi, int _lambda,
             int _gMaxWithoutImprovement, string _outputFile, int _batch)
      : vNSeqMaxApplication(_vNSeqMaxApplication),
        selectionMethod(_selectionMethod),
        mutationRate(_mutationRate),
        mi(_mi),
        lambda(_lambda),
        gMaxWithoutImprovement(_gMaxWithoutImprovement),
        outputFile(_outputFile),
        batch(_batch) {}
};

template <XSolution S, XEvaluation XEv = Evaluation<>,
          XESolution XES = pair<S, XEv>>
struct NGESInd {
  // S sInd; // probability
  // Evaluation<> e;
  pair<S, XEv> ind;

  S& getS() { return ind.first; }

  XEv& getEv() { return ind.second; }

  vector<NGESIndStructure<S, XEv>> vEsStructureInd;  // number of applications
  vector<int> vNSInd;

  // CANNOT MOVE EXTERNAL NON-TEMPORARY REFERENCES (???)
  /*
   NGESInd(S& _sInd, Evaluation<>& _e, vector<NGESIndStructure<S, XEv>>&
   _vEsStructureInd, int nNS) : sInd(std::move(_sInd)) , e(std::move(_e)) ,
   vEsStructureInd(std::move(_vEsStructureInd))
   {
      for (int i = 0; i < nNS; i++)
         vNSInd.push_back(i);

      random_shuffle(vNSInd.begin(), vNSInd.end());
      //change to rg shuffle, because seed may influence result
   }

   NGESInd(S& _sInd, Evaluation<>& _e, vector<NGESIndStructure<S, XEv>>&
   _vEsStructureInd, vector<int>& _vNSInd) : sInd(std::move(_sInd)) ,
   e(std::move(_e)) , vEsStructureInd(std::move(_vEsStructureInd)) ,
   vNSInd(std::move(_vNSInd))
   {
   }
*/

  // -----------------------
  // copy-based constructors

  NGESInd(const pair<S, Evaluation<>>& _ind,
          const vector<NGESIndStructure<S, XEv>>& _vEsStructureInd, int nNS)
      : ind(_ind), vEsStructureInd(_vEsStructureInd) {
    for (int i = 0; i < nNS; i++) vNSInd.push_back(i);

    random_shuffle(vNSInd.begin(), vNSInd.end());
    // change to rg shuffle, because seed may influence result
  }

  NGESInd(const pair<S, Evaluation<>>& _ind,
          const vector<NGESIndStructure<S, XEv>>& _vEsStructureInd,
          const vector<int>& _vNSInd)
      : ind(_ind), vEsStructureInd(_vEsStructureInd), vNSInd(_vNSInd) {}

  /*
   // -----------------------
   // move-based constructors

   NGESInd(const S&& _sInd, const Evaluation<>&& _e, const
   vector<NGESIndStructure<S, XEv>>&& _vEsStructureInd, int nNS) : sInd(_sInd)
     , e(_e)
     , vEsStructureInd(_vEsStructureInd)
   {
      for (int i = 0; i < nNS; i++)
         vNSInd.push_back(i);

      random_shuffle(vNSInd.begin(), vNSInd.end());
      //change to rg shuffle, because seed may influence result
   }

   NGESInd(const S&& _sInd, const Evaluation<>&& _e, const
   vector<NGESIndStructure<S, XEv>>&& _vEsStructureInd, const vector<int>&&
   _vNSInd) : sInd(_sInd) , e(_e) , vEsStructureInd(_vEsStructureInd) ,
   vNSInd(_vNSInd)
   {
   }
*/

  // ----------------
  // move constructor

  NGESInd(NGESInd<S, XEv>&& nges)
      : ind(std::move(nges.ind)),
        vEsStructureInd(std::move(nges.vEsStructureInd)),
        vNSInd(std::move(nges.vNSInd))

  {}

  //	NGESInd(const NGESInd<S, XEv>& nges) :
  //			sInd(nges.sInd), e(nges.e),
  // vEsStructureInd(nges.vEsStructureInd), vNSInd(nges.vNSInd)
  //
  //	{
  //
  //	}
  //
  //	NGESInd& operator=(const NGESInd<S, XEv>& nges)
  //	{
  //		sInd = nges.sInd;
  //		e = nges.e;
  //		vEsStructureInd = nges.vEsStructureInd;
  //		vNSInd = nges.vNSInd;
  //		return (*this);
  //
  //	}

  ~NGESInd() {}

  // do not 'override'
  void print() const {
    std::cout << "NGESInd: {";
    ind.first.print();
    ind.second.print();
    std::cout << "}" << std::endl;
  }
};

// CADA INDIVIDUO EH UM PAR DE SOLUCAO E UMA TUPLE COM O PARAMETROS DA
// ESTRATEGIA
//
// template<XSolution S, XEvaluation XEv = Evaluation<>, XESolution XES =
// pair<S, XEv>>
template <XESolution XES>
class NGES : public SingleObjSearch<XES> {
 private:
  using S = typename XES::first_type;
  using XEv = typename XES::second_type;
  using XSH = XES;
  // typedef vector<NGESInd<S, XEv>*> NGESPopulation;
  using NGESPopulation = vector<std::shared_ptr<NGESInd<S, XEv>>>;

  /*
   Evaluator<S, XEv, XES>& eval;
   //Constructive<S>& constructive;
   InitialSearch<XES>& constructive;
   vector<NS<XES, XSH>*> vNS;
   LocalSearch<XES>& ls;
   RandGen& rg;
   NGESParams& ngesParams;
*/
  sref<GeneralEvaluator<XES>> eval;
  // Constructive<S>& constructive;
  sref<InitialSearch<XES>> constructive;
  vsref<NS<XES, XSH>> vNS;
  sref<LocalSearch<XES>> ls;
  sref<RandGen> rg;
  NGESParams ngesParams;
  //

  // Local variables
  int nNS;

 public:
  // Evaluator, constructive, vNS -- vector with neighboorhods strucutures able
  // to move solution,
  //  selectionMethod: 0-low selection pressure (mi,lambda);1 selection pressure
  //  (mi+lambda)
  // TODO - Check why vector<NSSeq*> can not be passed as parameter - Tried but
  // failled
  //
  // NGES(Evaluator<S, XEv, XES>& _eval, InitialSearch<XES>& _constructive,
  // vector<NS<XES, XSH>*> _vNS, LocalSearch<XES>& _ls, RandGen& _rg,
  // NGESParams& _ngesParams)
  //
  NGES(sref<GeneralEvaluator<XES>> _eval,
       sref<InitialSearch<XES>> _constructive, vsref<NS<XES, XSH>> _vNS,
       sref<LocalSearch<XES>> _ls, sref<RandGen> _rg, NGESParams _ngesParams)
      : eval(_eval),
        constructive(_constructive),
        vNS(_vNS),
        ls(_ls),
        rg(_rg),
        ngesParams(_ngesParams) {
    nNS = vNS.size();
  }

  virtual ~NGES() {}

  void mutateESParams(std::vector<NGESIndStructure<S, XEv>>& p,
                      vector<int>& vNSInd, const int nNS) {
    double z = rg->rand01();
    if (z <= ngesParams.mutationRate) {
      int posX = rg->rand(nNS);
      int posY = rg->rand(nNS);
      if (nNS > 1) {
        while (posY == posX) {
          posY = rg->rand(nNS);
        }
      }

      // Swaps only NS order, since the index are preserved for function
      // applyMutationOperators
      iter_swap(vNSInd.begin() + posX, vNSInd.begin() + posY);
    }

    for (int param = 0; param < nNS; param++)  // 8 vizinhancas
    {
      p[param].sigmaN += rg->randG(0, 0.1) / 100.0;
      if (p[param].sigmaN < 0) p[param].sigmaN = 0;
      if (p[param].sigmaN > 3) p[param].sigmaN = 3;
      p[param].pr += rg->randG(0, p[param].sigmaN);
      if (p[param].pr < 0) p[param].pr = 0;
      if (p[param].pr > 1) p[param].pr = 1;

      p[param].sigmaB += rg->randG(0, 0.1) / 100.0;
      if (p[param].sigmaB < 0) p[param].sigmaB = 0;
      if (p[param].sigmaB > 1) p[param].sigmaB = 1;
      p[param].nap += rg->randBN(p[param].sigmaB, 10);
      if (p[param].nap < 1) p[param].nap = 1;
      if (p[param].nap > ngesParams.vNSeqMaxApplication[param])
        p[param].nap = ngesParams.vNSeqMaxApplication[param];
    }
  }

  void applyMutationOperators(S& s, const vector<NGESIndStructure<S, XEv>>& p,
                              const vector<int> vNSInd, const int nNS) {
    XES se = make_pair(s, Evaluation<>());
    for (int i = 0; i < nNS; i++) {
      int param = vNSInd[i];  // Extract index
      double rx = rg->rand01();
      if (rx < p[param].pr)
        for (int a = 0; a < p[param].nap; a++) {
          uptr<Move<XES, XSH>> mov_tmp = vNS[param]->randomMove(se);
          //					int tries = 0;
          //					int maxTries = 1;
          //
          //					while
          //((!mov_tmp->canBeApplied(*s))
          //&& (tries < maxTries))
          //					{
          //						delete mov_tmp;
          //						mov_tmp =
          //&vNS[param]->move(*s);
          // tries++;
          //					}

          if (mov_tmp->canBeApplied(se)) {
            // Move<S, XEv>* mov_rev = mov_tmp->apply(s);
            mov_tmp->apply(se);
            // delete mov_rev;
          } else {
            //						cout << "cannot be
            // applied NS:"
            //<< param; 						cout <<
            // "\tnumber of tries:" << tries << std::endl;
            // getchar();
          }

          // delete mov_tmp;
        }
    }
    //
    s = se.first;
  }

  void printPop(NGESPopulation& pop) {
    std::cout << "PRINT!" << std::endl;
    for (unsigned i = 0; i < pop.size(); i++) {
      std::cout << "i=" << i << ": ";
      pop[i]->print();
    }
  }

  // void competition(NGESPopulation& pop, NGESPopulation& offsprings, S& sStar,
  // Evaluation<>& eStar, int& iterWithoutImprovement, const int& gCurrent, int
  // selectionType)
  void competition(NGESPopulation& pop, NGESPopulation& offsprings,
                   pair<S, XEv>& star, int& iterWithoutImprovement,
                   const int& gCurrent, int selectionType) {
    // std::cout << "competition!" << std::endl;
    // std::cout << "OFFSPRINGS:" << std::endl;
    // printPop(offsprings);

    if (selectionType == 1) {
      offsprings.reserve(offsprings.size() + ngesParams.mi);
      for (int i = 0; i < ngesParams.mi; i++) offsprings.push_back(pop[i]);
    }

    // auto compInd = [&](NGESInd<S, XEv>* indA, NGESInd<S, XEv>* indB) -> bool
    // {
    auto compInd = [&](const shared_ptr<NGESInd<S, XEv>>& indA,
                       const shared_ptr<NGESInd<S, XEv>>& indB) -> bool {
      /*
         std::cout << "PRINT!!" << std::endl;
         std::cout << "getting elements:" << std::endl;
         assert(indA);
         std::cout << "A exists" << std::endl;
         indA->getEv().print();
         assert(indB);
         std::cout << "B exists" << std::endl;
         NGESInd<S, XEv>& eb = *indB;
         std::cout << "got B" << std::endl;
         //indB->getEv().print();
         eb.getEv().print();
         */
      return eval->betterStrict(indA->getEv(), indB->getEv());
      // TODO: fix bug here
    };

    // TODO: fix bug here
    sort(offsprings.begin(), offsprings.end(),
         compInd);  // ordena com QuickSort

    pop.clear();
    pop.resize(ngesParams.mi);
    double fo_pop = 0;

    for (int i = 0; i < ngesParams.mi; i++) {
      pop[i] = offsprings[i];
      fo_pop += pop[i]->getEv().evaluation();
    }

    // all shared_ptr now
    ////for (int i = 0; i < ngesParams.lambda; i++)
    ////   delete offsprings[i + ngesParams.mi];
    for (int i = 0; i < ngesParams.lambda; i++)
      offsprings[i + ngesParams.mi] = nullptr;

    Evaluation<> evBest{pop[0]->getEv()};

    // if (eval.betterThan(evBest, eStar)) {
    if (eval->betterStrict(evBest, star.second)) {
      ////			delete &eStar;
      ////			delete &sStar;
      // eStar = pop[0]->getEv();
      // sStar = pop[0]->getS();
      star = make_pair(pop[0]->getS(), pop[0]->getEv());
      ////			cout << "inside comp:" << &eStar << "/" <<
      /// eStar.evaluation() << std::endl;
      if (Component::information) {
        std::cout << "Gen:" << gCurrent
                  << " | noImp: " << iterWithoutImprovement;
        // std::cout << " | Best: " << eStar.evaluation() << "\t [";
        std::cout << " | Best: " << star.second.evaluation() << "\t [";
        for (int param = 0; param < nNS; param++) {
          std::cout << "(" << pop[0]->vEsStructureInd[param].pr;
          std::cout << ",";
          std::cout << pop[0]->vEsStructureInd[param].nap << ") ";
        }
        std::cout << "]\t";
        std::cout << pop[0]->vNSInd << std::endl;
        //				cout<<sStar.getR()<<endl;
        //				getchar();
      }

      if (Component::debug) {
        std::stringstream ss;
        ss << ngesParams.outputFile << "_"
           << "best";
        string outputBest = ss.str();
        FILE* arquivo = fopen(outputBest.c_str(), "a");
        if (!arquivo) {
          std::cout << "ERRO: falha ao criar arquivo \"outputBest.txt\""
                    << std::endl;
        } else {
          fprintf(arquivo, "%d\t%d\t%d\t", ngesParams.batch, gCurrent,
                  iterWithoutImprovement);
          for (int param = 0; param < nNS; param++) {
            double pr = pop[0]->vEsStructureInd[param].pr;
            int nap = pop[0]->vEsStructureInd[param].nap;
            double prNap = pr * nap;
            fprintf(arquivo, "%f\t%d\t%f\t", pr, nap, prNap);
          }
          fprintf(arquivo, "\n");
          fclose(arquivo);
        }
      }
      iterWithoutImprovement = 0;

    } else
      iterWithoutImprovement++;
  }

  // virtual pair<S, Evaluation<>>* search(StopCriteria<XEv>& stopCriteria,
  // const S* _s = nullptr, const Evaluation<>* _e = nullptr) = 0; pair<S,
  // Evaluation<>>* search(StopCriteria<XEv>& stopCriteria, const S* _s =
  // nullptr, const Evaluation<>* _e = nullptr) override std::optional<pair<S,
  // XEv>> search(StopCriteria<XEv>& stopCriteria) override
  //
  // SearchStatus search(const StopCriteria<XEv>& stopCriteria) override
  SearchOutput<XES, XSH> searchBy(const StopCriteria<XEv>& stopCriteria,
                                  std::optional<XES> best) override {
    assert(!best);  // DO NOT RECEIVE IT HERE!
    // op<XES>& star = this->best;
    op<XES> star = nullopt;
    Timer tnow;
    NGESPopulation pop;
    ////// NGESPopulation pop(ngesParams.mi, nullptr);

    // S* sStar = nullptr;
    // Evaluation<>* eStar = nullptr;
    ////std::optional<pair<S, XEv>> star = std::nullopt;

    int iterWithoutImprovement = 0, gCurrent = 0;

    std::cout << "NGES search(" << stopCriteria.target_f << ","
              << stopCriteria.timelimit << ")" << std::endl;

    double inititPopFitness = 0;
    // =============================
    // Generating initial population
    // =============================
    pop.reserve(ngesParams.mi);

    for (int i = 0; i < ngesParams.mi; i++) {
      // PartialGreedyInitialSolutionOPM is(opm, 0.4, 0.4, 0.4); // waste, ore,
      // shovel

      // std::optional<S> s =
      // constructive.generateSolution(stopCriteria.timelimit); //MAKE MOVE TODO
      // Evaluation<> e = eval.evaluate(*s);
      // pair<S, XEv> se = *SingleObjSearch<XES>::genPair(constructive, eval,
      // stopCriteria.timelimit);
      pair<S, XEv> se = *constructive->initialSearch(stopCriteria).first;
      eval->reevaluate(se);

      vector<NGESIndStructure<S, XEv>> m;

      // probability, application, sigmaNomal, sigmaBinomial
      for (int aux = 0; aux < nNS; aux++)
        m.push_back(NGESIndStructure<S, XEv>(
            rg->rand01(), rg->randB(0.5, 10) + 1, rg->rand01(), rg->rand01()));

      // pass 'se' by copy (TODO: think if std::move is enough)
      // NGESInd<S, XEv>* ind = new NGESInd<S, XEv>(se, m, nNS); //TODO MAKE
      // MOVE
      std::shared_ptr<NGESInd<S, XEv>> ind(
          new NGESInd<S, XEv>(se, m, nNS));  // TODO MAKE MOVE
      pop.push_back(ind);

      if (i == 0) {
        // eStar = new Evaluation(pop[i]->getEv());
        // sStar = new S(pop[i]->getS());
        star = make_pair(pop[i]->getS(), pop[i]->getEv());
      } else {
        // if (eval.betterThan(pop[i]->getEv(), *eStar)) {
        if (eval->betterStrict(pop[i]->getEv(), star->second)) {
          //(*eStar) = pop[i]->getEv();
          //(*sStar) = pop[i]->getS();
          star = make_pair(pop[i]->getS(), pop[i]->getEv());
        }
      }

      inititPopFitness += pop[i]->getEv().evaluation();

      ////delete s;
    }

    if (Component::information) {
      // std::cout << "InitialPop avg: " << inititPopFitness / ngesParams.mi <<
      // "\t eStar:" << (double)eStar->evaluation() << std::endl;
      std::cout << "InitialPop avg: " << inititPopFitness / ngesParams.mi
                << "\t eStar:" << (double)star->second.evaluation()
                << std::endl;
    }
    // ===============================
    //

    // INICIA PARAMETROS DE MUTACAO
    vector<vector<pair<double, double>>> meanParamsGenerations;

    iterWithoutImprovement = 0;
    // while ((iterWithoutImprovement < ngesParams.gMaxWithoutImprovement) &&
    // ((tnow.now()) < stopCriteria.timelimit) &&
    // eval.betterThan(stopCriteria.target_f, (double)eStar->evaluation())) {
    while ((iterWithoutImprovement < ngesParams.gMaxWithoutImprovement) &&
           ((tnow.now()) < stopCriteria.timelimit) &&
           eval->betterStrict(stopCriteria.target_f, star->second)) {
      NGESPopulation popOffsprings(ngesParams.lambda, nullptr);
      double fo_filhos = 0;

      // GERA OS OFFSPRINGS
      for (int l = 0; l < ngesParams.lambda; l++) {
        int x = rg->rand(ngesParams.mi);

        // Cria Filho e Tuple de Parametros (pi,nap,vizinhança)
        S filho = pop[x]->getS();

        vector<NGESIndStructure<S, XEv>> vt = pop[x]->vEsStructureInd;
        vector<int> vNSOffspring = pop[x]->vNSInd;

        // Mutacao dos parametros l
        mutateESParams(vt, vNSOffspring, nNS);

        // applying Neighborhood Operators for generating Mutated Offspring
        applyMutationOperators(filho, vt, vNSOffspring, nNS);

        // TODO: Optional -- Local Search in each Offspring.
        // Do anything you want with filho and return filho_ls
        // S filho_ls = filho;

        //================
        // pair<S, XEv> se = make_pair(filho, eval->evaluate(filho));
        pair<S, XEv> se = {filho, XEv{}};  // TODO: fix this logic! Solution
                                           // must come with Evaluation already
        se.second.invalidate();            // force outdated
        eval->reevaluate(se);

        //================

        fo_filhos += se.second.evaluation();

        // NGESInd<S, XEv>* ind = new NGESInd<S, XEv>(se, vt, vNSOffspring);
        // //TODO MAKE MOVE
        std::shared_ptr<NGESInd<S, XEv>> ind(
            new NGESInd<S, XEv>(se, vt, vNSOffspring));  // TODO MAKE MOVE

        popOffsprings[l] = ind;
      }

      // std::cout << "Offspring mean FO, iter " << gAtual << ":\t" << fo_filhos
      // / mi
      // << std::endl;

      // TODO: optionally apply LS in the best individuals
      // aplicaBuscaLocalBests(pop_filhos, 2);
      // std::cout<<"Applying local Search ..."<<endl;

      // =====================Selection ==================
      // MI,LAMBDA or MI + LAMBDA
      switch (ngesParams.selectionMethod) {
        case 0:
          // competition(pop, popOffsprings, *sStar, *eStar,
          // iterWithoutImprovement, gCurrent, 0);
          competition(pop, popOffsprings, *star, iterWithoutImprovement,
                      gCurrent, 0);
          break;
        case 1:
          // competition(pop, popOffsprings, *sStar, *eStar,
          // iterWithoutImprovement, gCurrent, 1);
          competition(pop, popOffsprings, *star, iterWithoutImprovement,
                      gCurrent, 1);
          break;
          // TODO create other selection strategies

        default:
          std::cout << "Error! Selection not performed!" << std::endl;
          getchar();
      }
      // =====================End Selection ==================

      // ====================================================
      // Statistics regarding the evolution of the params
      if (Component::debug) {
        std::vector<std::pair<double, double>> meanParams;
        for (int param = 0; param < nNS; param++) {
          double meanPR = 0;
          double meanNAP = 0;
          for (int i = 0; i < ngesParams.mi; i++) {
            meanPR += pop[i]->vEsStructureInd[param].pr;
            meanNAP += pop[i]->vEsStructureInd[param].nap;
            //					cout << "(" <<
            // pop[i].second->at(param).pr; std::cout
            // <<
            // ","; std::cout
            // << pop[i].second->at(param).nap << ") ";
          }
          meanPR /= ngesParams.mi;
          meanNAP /= ngesParams.mi;
          meanParams.push_back(std::make_pair(meanPR, meanNAP));
          //				cout << "(" << meanPR;
          //				cout << ",";
          //				cout << meanNAP << ") ";
        }
        // std::cout << std::endl;
        meanParamsGenerations.push_back(meanParams);

        FILE* arquivo = fopen(ngesParams.outputFile.c_str(), "a");
        if (!arquivo) {
          std::cout << "ERRO: falha ao criar arquivo \"outputFileES.txt\""
                    << std::endl;
        } else {
          fprintf(arquivo, "%d\t%d\t", ngesParams.batch, gCurrent);
          for (int param = 0; param < nNS; param++) {
            double pr = meanParams[param].first;
            double nap = meanParams[param].second;
            double prNap = meanParams[param].second * meanParams[param].first;
            fprintf(arquivo, "%f\t%f\t%f\t", pr, nap, prNap);
          }
          fprintf(arquivo, "\n");
          fclose(arquivo);
        }
      }

      // ====================================================

      gCurrent++;
    }

    // all are shared_ptr now!
    ////for (int i = 0; i < ngesParams.mi; i++)
    ////   delete pop[i];
    for (int i = 0; i < ngesParams.mi; i++) pop[i] = nullptr;

    std::cout << "Fishing NGES search:\nIterWithoutImp: "
              << iterWithoutImprovement << "/"
              << ngesParams.gMaxWithoutImprovement << std::endl;
    // std::cout << "BestSol::" << eStar->evaluation() << "/" <<
    // stopCriteria.target_f << std::endl;
    std::cout << "BestSol::" << star->second.evaluation() << "/"
              << stopCriteria.target_f << std::endl;
    std::cout << "tnow.now():" << tnow.now() << "/" << stopCriteria.timelimit
              << std::endl;
    std::cout << "g:" << gCurrent << std::endl;
    std::cout << "NGES Finished" << std::endl;
    std::cout
        << "------------------------------------------------------------------"
           "--------"
        << std::endl;
    // getchar();

    ////pair<S, Evaluation<>>* pairToReturn = new pair<S,
    /// Evaluation<>>(std::make_pair(*sStar, *eStar)); /delete sStar; /delete
    /// eStar;
    ////return pairToReturn;
    // return make_optional(std::make_pair(*sStar, *eStar)); // TODO: fix leak
    /// return star;
    //
    // this->best = star;
    // return SearchStatus::NO_REPORT;
    //
    return SearchOutput<XES>{SearchStatus::NO_REPORT, star};
  }

  static std::string idComponent() {
    std::stringstream ss;
    ss << SingleObjSearch<XES>::idComponent() << "NGES";
    return ss.str();
  }

  std::string id() const override { return idComponent(); }
};
}  // namespace optframe

#endif  // OPTFRAME_HEURISTICS_EA_NGES_HPP_
