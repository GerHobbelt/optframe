// ===================================
// Main.cpp file generated by OptFrame
// Project EFP
// ===================================

#include <math.h>
#include <stdlib.h>

#include <OptFrame/Core/RandGen.hpp>
#include <OptFrame/Util/RandGenMersenneTwister.hpp>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>

#include "../ForecastClass.hpp"

using namespace std;
using namespace optframe;
using namespace HFM;
extern int nThreads;

int musicGen(int argc, char** argv) {
  nThreads = 2;
  std::cout
      << "Welcome to automatic music learning and forecasting and generation"
      << std::endl;
  RandGenMersenneTwister rg;
  sref<RandGen> rg2{new RandGenMersenneTwister{}};
  // long  1412730737
  long seed = time(nullptr);  // CalibrationMode
  seed = 1;
  std::cout << "Seed = " << seed << std::endl;
  srand(seed);
  rg.setSeed(seed);
  std::cout << "nThreads = " << nThreads << std::endl;

  //	if (argc != 2)
  //	{
  //		cout << "Parametros incorretos!" << std::endl;
  //		cout << "Os parametros esperados sao: stockMarketTimeSeries" <<
  // endl; 		exit(1);
  //	}

  //	const char* caminhoOutput = argv[1];

  //	string inputTimeSeries = caminhoOutput;
  //	inputTimeSeries = "MyProjects/HFM/Instance/mp3/bluesMonoChannel_10_20";
  //	inputTimeSeries = "MyProjects/HFM/Instance/mp3/jazzMonoChannel_10_20";

  //===================================
  std::cout << "Parametros:" << std::endl;
  //	cout << "nomeOutput=" << inputTimeSeries << std::endl;

  vector<string> explanatoryVariables;
  explanatoryVariables.push_back(
      "MyProjects/HFM/Instance/mp3/bluesMonoChannelAvgNative");
  explanatoryVariables.push_back(
      "MyProjects/HFM/Instance/mp3/jazzMonoChannelAvgNative");
  explanatoryVariables.push_back(
      "MyProjects/HFM/Instance/mp3/jazzMonoChannelAvgNative");
  //	explanatoryVariables.push_back("MyProjects/HFM/Instance/mp3/bluesMonoChannel_10_20");
  //	explanatoryVariables.push_back("MyProjects/HFM/Instance/mp3/jazzMonoChannel_10_20");
  //	explanatoryVariables.push_back("MyProjects/HFM/Instance/mp3/ExperimentoMonoChannelAvg");

  treatForecasts rF(explanatoryVariables);

  vector<string> timeSeriesToBeForecasted;
  timeSeriesToBeForecasted.push_back(
      "MyProjects/HFM/Instance/mp3/bluesMonoChannelAvgNative");
  timeSeriesToBeForecasted.push_back(
      "MyProjects/HFM/Instance/mp3/jazzMonoChannelAvgNative");
  timeSeriesToBeForecasted.push_back(
      "MyProjects/HFM/Instance/mp3/jazzMonoChannelAvgNative");
  //	timeSeriesToBeForecasted.push_back("MyProjects/HFM/Instance/mp3/jazzMonoChannel_10_20");
  //	timeSeriesToBeForecasted.push_back("MyProjects/HFM/Instance/mp3/ExperimentoMonoChannelAvg");

  treatForecasts tFToBeForecasted(timeSeriesToBeForecasted);

  // Parametros do metodo
  int mu = 10;
  int lambda = mu * 6;
  int evalFOMinimizer = MAPE_INDEX;
  int contructiveNumberOfRules = 100;
  int evalAprox = 0;
  double alphaACF = -1;
  int construtive = 2;
  // ============ END FORCES ======================

  // ============= METHOD PARAMETERS=================
  HFMParams methodParam;
  // seting up ES params
  methodParam.setESMU(mu);
  methodParam.setESLambda(lambda);
  methodParam.setESMaxG(10000);

  // seting up ACF construtive params
  methodParam.setConstrutiveMethod(construtive);
  methodParam.setConstrutivePrecision(contructiveNumberOfRules);
  vector<double> vAlphaACFlimits;
  vAlphaACFlimits.push_back(alphaACF);
  methodParam.setConstrutiveLimitAlphaACF(vAlphaACFlimits);

  // seting up Eval params
  methodParam.setEvalAprox(evalAprox);
  methodParam.setEvalFOMinimizer(evalFOMinimizer);
  // ==========================================

  // ================== READ FILE ============== CONSTRUTIVE 0 AND 1
  ProblemParameters problemParam;
  // ProblemParameters problemParam(vParametersFiles[randomParametersFiles]);
  int timeES = 5 * 60;
  double fs = 44100;
  double nSecondsForFeeding = 0.2;
  int maxLag = nSecondsForFeeding * fs;
  double nSecondsOfForecasts = 0.1;
  int fh = nSecondsOfForecasts * fs;
  double nSecondsOfTraining = 3;
  // forcing values
  fh = 1000;
  maxLag = 150;

  problemParam.setStepsAhead(fh);
  problemParam.setMaxLag(maxLag);
  problemParam.setToRoundedForecasts(true);
  problemParam.setToNonNegativeForecasts(false);

  // If maxUpperLag is greater than 0 model uses predicted data
  problemParam.setMaxUpperLag(0);
  //=================================================

  std::cout << std::setprecision(9);
  std::cout << std::fixed;
  int nTotalForecastingsTrainningSet = nSecondsOfTraining * fs + maxLag;
  int beginTrainingSet = fs;
  double NTRaprox = (nTotalForecastingsTrainningSet - maxLag) / double(fh);
  std::cout << "#timeSeriesSize: " << rF.getForecastsSize(0) << std::endl;
  std::cout << "#nTotalForecastingsTrainningSet: "
            << nTotalForecastingsTrainningSet << std::endl;
  std::cout << "#~NTR: " << NTRaprox << std::endl;
  std::cout << "#maxNotUsed: " << maxLag << std::endl;
  std::cout << "#StepsAhead: " << fh << std::endl << std::endl;
  std::cout << "#trainingTimeWithES: " << timeES << std::endl << std::endl;

  vector<vector<double>> trainningSet;  // trainningSetVector
  trainningSet.push_back(rF.getPartsForecastsBeginToEnd(
      2, beginTrainingSet, nTotalForecastingsTrainningSet));

  int multiplier = 1;

  for (int n = 0; n < (int)trainningSet[0].size(); n++)
    trainningSet[0][n] *= multiplier;

  ForecastClass* forecastObject;

  forecastObject =
      new ForecastClass{trainningSet, problemParam, rg2, methodParam};
  std::optional<pair<SolutionHFM, Evaluation<>>> sol =
      forecastObject->run(timeES, 0, 0);

  pair<vector<double>*, vector<double>*>* forecastsAndTargets = nullptr;
  for (int m = 0; m < (int)timeSeriesToBeForecasted.size(); m++) {
    vector<vector<double>> validationSet;  // validationSetVector
    validationSet.push_back(tFToBeForecasted.getPartsForecastsBeginToEnd(
        m, 0, tFToBeForecasted.getForecastsSize(m)));

    for (int n = 0; n < (int)validationSet[0].size(); n++)
      validationSet[0][n] *= multiplier;

    forecastsAndTargets = forecastObject->returnForecastsAndTargets(
        sol->first.getR(), validationSet);

    for (int n = 0; n < (int)forecastsAndTargets->first->size(); n++) {
      forecastsAndTargets->first->at(n) /= multiplier;
      forecastsAndTargets->second->at(n) /= multiplier;
    }

    std::stringstream ss;
    ss << "forecast_Params_FH" << fh << "_max" << maxLag << "_Tr" << timeES
       << "_Times" << nSecondsOfTraining << "_" << nSecondsForFeeding << "_"
       << nSecondsOfForecasts << "_TS" << "_TS" << m << ".txt";
    forecastObject->exportForecasts(*forecastsAndTargets->second, ss.str());
  }

  forecastObject->exportForecasts(*forecastsAndTargets->first,
                                  "./targetSmall.txt");

  std::cout << "Music has been created." << std::endl;

  return 0;
}

int musicGenMidiCSV(int argc, char** argv) {
  nThreads = 2;
  std::cout << "Welcome to automatic music MIDI learning and forecasting and "
               "generation"
            << std::endl;
  RandGenMersenneTwister rg;
  sref<RandGen> rg2{new RandGenMersenneTwister{}};
  // long  1412730737
  long seed = time(nullptr);  // CalibrationMode
  seed = 1;
  std::cout << "Seed = " << seed << std::endl;
  srand(seed);
  rg.setSeed(seed);
  std::cout << "nThreads = " << nThreads << std::endl;

  //	if (argc != 2)
  //	{
  //		cout << "Parametros incorretos!" << std::endl;
  //		cout << "Os parametros esperados sao: stockMarketTimeSeries" <<
  // endl; 		exit(1);
  //	}

  //	const char* caminhoOutput = argv[1];

  //	string inputTimeSeries = caminhoOutput;
  //	inputTimeSeries = "MyProjects/HFM/Instance/mp3/bluesMonoChannel_10_20";
  //	inputTimeSeries = "MyProjects/HFM/Instance/mp3/jazzMonoChannel_10_20";

  //===================================
  std::cout << "Parametros:" << std::endl;
  //	cout << "nomeOutput=" << inputTimeSeries << std::endl;

  vector<string> explanatoryVariables;
  explanatoryVariables.push_back(
      "MyProjects/HFM/Instance/mp3/midi/"
      "Unchain_My_Heart_Ray_Charles_SaxOnly-H.tsr");
  explanatoryVariables.push_back(
      "MyProjects/HFM/Instance/mp3/midi/"
      "Anthropology_Josef_Huber_PianoOnly.tsr");

  treatForecasts rF(explanatoryVariables);

  vector<string> timeSeriesToBeForecasted = explanatoryVariables;

  treatForecasts tFToBeForecasted(timeSeriesToBeForecasted);

  // Parametros do metodo
  int mu = 10;
  int lambda = mu * 6;
  int evalFOMinimizer = MAE_INDEX;
  int contructiveNumberOfRules = 500;
  int evalAprox = 0;
  double alphaACF = -1;
  int construtive = 2;
  // ============ END FORCES ======================

  // ============= METHOD PARAMETERS=================
  HFMParams methodParam;
  // seting up ES params
  methodParam.setESMU(mu);
  methodParam.setESLambda(lambda);
  methodParam.setESMaxG(10000);

  // seting up ACF construtive params
  methodParam.setConstrutiveMethod(construtive);
  methodParam.setConstrutivePrecision(contructiveNumberOfRules);
  vector<double> vAlphaACFlimits;
  vAlphaACFlimits.push_back(alphaACF);
  methodParam.setConstrutiveLimitAlphaACF(vAlphaACFlimits);

  // seting up Eval params
  methodParam.setEvalAprox(evalAprox);
  methodParam.setEvalFOMinimizer(evalFOMinimizer);
  // ==========================================

  // ================== READ FILE ============== CONSTRUTIVE 0 AND 1
  ProblemParameters problemParam;
  // ProblemParameters problemParam(vParametersFiles[randomParametersFiles]);
  int timeES = 1800;
  //	double fs = 1000;
  //	double nSecondsForFeeding = 0.2;
  //	int maxLag = nSecondsForFeeding * fs;
  //	double nSecondsOfForecasts = 0.1;
  //	int fh = nSecondsOfForecasts * fs;
  //	double nSecondsOfTraining = 3;
  // forcing values
  int fh = 1;
  int maxLag = 10000;
  int targetTSFile = 1;

  problemParam.setStepsAhead(fh);
  problemParam.setMaxLag(maxLag);
  problemParam.setToRoundedForecasts(true);
  problemParam.setToBinaryForecasts(true);

  // If maxUpperLag is greater than 0 model uses predicted data
  problemParam.setMaxUpperLag(0);
  //=================================================

  std::cout << std::setprecision(3);
  std::cout << std::fixed;
  int nTotalForecastingsTrainningSet = 20000;
  int beginTrainingSet = 0;
  double NTRaprox = (nTotalForecastingsTrainningSet - maxLag) / double(fh);
  std::cout << "#timeSeriesSize: " << rF.getForecastsSize(0) << std::endl;
  std::cout << "#nTotalForecastingsTrainningSet: "
            << nTotalForecastingsTrainningSet << std::endl;
  std::cout << "#~NTR: " << NTRaprox << std::endl;
  std::cout << "#maxNotUsed: " << maxLag << std::endl;
  std::cout << "#StepsAhead: " << fh << std::endl << std::endl;
  std::cout << "#trainingTimeWithES: " << timeES << std::endl << std::endl;

  vector<vector<double>> trainningSet;  // trainningSetVector

  trainningSet.push_back(rF.getPartsForecastsBeginToEnd(
      targetTSFile, beginTrainingSet, nTotalForecastingsTrainningSet));

  ForecastClass* forecastObject;

  forecastObject =
      new ForecastClass{trainningSet, problemParam, rg2, methodParam};
  std::optional<pair<SolutionHFM, Evaluation<>>> sol =
      forecastObject->run(timeES, 0, 0);

  pair<vector<double>*, vector<double>*>* forecastsAndTargets = nullptr;
  for (int m = 0; m < (int)timeSeriesToBeForecasted.size(); m++) {
    vector<vector<double>> validationSet;  // validationSetVector
    validationSet.push_back(tFToBeForecasted.getPartsForecastsBeginToEnd(
        m, 0, tFToBeForecasted.getForecastsSize(m)));

    forecastsAndTargets = forecastObject->returnForecastsAndTargets(
        sol->first.getR(), validationSet);

    std::stringstream ss;
    ss << "forecast_Params_FH_TrTs1_" << fh << "_max" << maxLag << "_Tr"
       << timeES << "nS" << nTotalForecastingsTrainningSet << "_TS" << m
       << ".txt";
    forecastObject->exportForecasts(*forecastsAndTargets->second, ss.str());
  }

  forecastObject->exportForecasts(*forecastsAndTargets->first,
                                  "./midiExportTest.txt");

  std::cout << "Music has been created." << std::endl;

  return 0;
}
