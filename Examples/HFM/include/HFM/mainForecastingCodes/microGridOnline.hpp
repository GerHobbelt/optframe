// ===================================
// Main.cpp file generated by OptFrame
// Project EFP
// ===================================

#include <math.h>
#include <stdlib.h>

#include <OptFrame/Core/RandGen.hpp>
#include <OptFrame/Util/RandGenMersenneTwister.hpp>
#include <iostream>
#include <numeric>

#include "../ForecastClass.hpp"

using namespace std;
using namespace optframe;
// using namespace HFM;

int microGridLiuAppliedEnergyOnline(int argc, char** argv) {
  std::cout
      << "Welcome to Micro Grid Liu Applied Energy Dataset Calibration Mode "
         "ONLINE..."
      << std::endl;
  RandGenMersenneTwister rg;
  sref<RandGen> rg2{new RandGenMersenneTwister{}};
  // long  1412730737
  long seed = time(nullptr);  // CalibrationMode
  // seed = 9;
  std::cout << "Seed = " << seed << std::endl;
  srand(seed);
  rg.setSeed(seed);

  if (argc != 5) {
    std::cout << "Parametros incorretos!" << std::endl;
    std::cout << "Os parametros esperados sao: nomeOutput targetTS "
                 "construtiveNRulesACF timeES"
              << std::endl;
    exit(1);
  }

  const char* caminhoOutput = argv[1];
  int argvTargetTimeSeries = atoi(argv[2]);
  int argvNumberOfRules = atoi(argv[3]);
  int argvNTR = atoi(argv[4]);

  // double argvAlphaACF = atof(argv[4]);

  //
  //	const char* caminho = argv[1];
  //	const char* caminhoValidation = argv[2];
  //	const char* caminhoOutput = argv[3];
  //	const char* caminhoParameters = argv[4];
  //	int instN = atoi(argv[5]);
  //	int stepsAhead = atoi(argv[6]);
  //	int mu = atoi(argv[7]);

  string nomeOutput = caminhoOutput;

  //===================================
  std::cout << "Parametros:" << std::endl;
  std::cout << "nomeOutput=" << nomeOutput << std::endl;
  std::cout << "argvTargetTimeSeries=" << argvTargetTimeSeries << std::endl;
  std::cout << "argvNumberOfRules=" << argvNumberOfRules << std::endl;
  // std::cout << "argvAlphaACF=" << argvAlphaACF << std::endl;
  //	getchar();
  //	cout << "instN=" << instN << std::endl;
  //	cout << "stepsAhead=" << stepsAhead << std::endl;
  //	cout << "mu=" << mu << std::endl;
  //===================================

  // CONFIG FILES FOR CONSTRUTIVE 0 AND 1

  vector<string> explanatoryVariables;

  // DATA FROM LIU
  string fileMicroGridA = "./MyProjects/HFM/Instance/microGridLiuAE/A";
  string fileMicroGridB = "./MyProjects/HFM/Instance/microGridLiuAE/B";
  string fileMicroGridC = "./MyProjects/HFM/Instance/microGridLiuAE/C";
  string fileMicroGridD = "./MyProjects/HFM/Instance/microGridLiuAE/D";

  // TREATED DATA FROM LIU
  string fileMicroGridAT = "./MyProjects/HFM/Instance/microGridLiuAETreated/A";
  string fileMicroGridBT = "./MyProjects/HFM/Instance/microGridLiuAETreated/B";
  string fileMicroGridCT = "./MyProjects/HFM/Instance/microGridLiuAETreated/C";
  string fileMicroGridDT = "./MyProjects/HFM/Instance/microGridLiuAETreated/D";
  vector<string> vInstances;
  vInstances.push_back(fileMicroGridAT);
  vInstances.push_back(fileMicroGridBT);
  vInstances.push_back(fileMicroGridCT);
  vInstances.push_back(fileMicroGridDT);

  explanatoryVariables.push_back(vInstances[argvTargetTimeSeries]);

  treatForecasts rF(explanatoryVariables);

  // vector<vector<double> > batchOfBlindResults; //vector with the blind
  // results of each batch

  /*int beginValidationSet = 0;
   int nTrainningRoundsValidation = 50;
   int nValidationSamples = problemParam.getNotUsedForTest() +
   nTrainningRoundsValidation * stepsAhead; std::cout << "nValidationSamples = "
   << nValidationSamples << std::endl; int nTotalForecastingsValidationSet =
   nValidationSamples;

   vector<vector<double> > validationSet; //validation set for calibration
   validationSet.push_back(rF.getPartsForecastsEndToBegin(0, beginValidationSet,
   nTotalForecastingsValidationSet));
   validationSet.push_back(rF.getPartsForecastsEndToBegin(1, beginValidationSet,
   nTotalForecastingsValidationSet + stepsAhead));
   validationSet.push_back(rF.getPartsForecastsEndToBegin(2, beginValidationSet,
   nTotalForecastingsValidationSet + stepsAhead));
   */

  //	int maxMu = 100;
  //	int maxInitialDesv = 10;
  //	int maxMutationDesv = 30;
  //	int maxPrecision = 300;
  int nBatches = 1;

  vector<vector<double>>
      vfoIndicatorCalibration;  // vector with the FO of each batch

  vector<SolutionEFP>
      vSolutionsBatches;  // vector with the solution of each batch

  for (int n = 0; n < nBatches; n++) {
    //		int contructiveNumberOfRules = rg.rand(maxPrecision) + 10;
    //		int evalFOMinimizer = rg.rand(NMETRICS); //tree is the number of
    // possible objetive function index minimizers 		int evalAprox =
    // rg.rand(2);
    ////Enayatifar aproximation using previous values 		int construtive
    ///=
    // rg.rand(3); 		double initialDesv = rg.rand(maxInitialDesv) +
    // 1; 		double mutationDesv = rg.rand(maxMutationDesv) + 1;
    // int mu = rg.rand(maxMu) + 1; 		int lambda = mu * 6;

    // limit ACF for construtive ACF
    //		double alphaACF = rg.rand01();
    //		int alphaSign = rg.rand(2);
    //		if (alphaSign == 0)
    //			alphaACF = alphaACF * -1;

    // ============ FORCES ======================
    //		initialDesv = 10;
    //		mutationDesv = 20;
    int mu = 100;
    int lambda = mu * 6;
    int evalFOMinimizer = MAPE_INDEX;
    int contructiveNumberOfRules = 100;
    int evalAprox = 2;
    double alphaACF = 0.5;
    int construtive = 2;
    // ============ END FORCES ======================

    // ============= METHOD PARAMETERS=================
    HFMParams methodParam;
    // seting up Continous ES params
    methodParam.setESInitialDesv(10);
    methodParam.setESMutationDesv(20);
    methodParam.setESMaxG(100000);

    // seting up ES params
    methodParam.setESMU(mu);
    methodParam.setESLambda(lambda);

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
    //		int nVR = 168;
    int nSA = 24;
    problemParam.setStepsAhead(nSA);
    int stepsAhead = problemParam.getStepsAhead();
    //========SET PROBLEM MAXIMUM LAG ===============
    problemParam.setMaxLag(672);
    int maxLag = problemParam.getMaxLag(0);

    // If maxUpperLag is greater than 0 model uses predicted data
    problemParam.setMaxUpperLag(0);
    //		int maxUpperLag = problemParam.getMaxUpperLag();
    //=================================================

    int nTrainningRounds = argvNTR;
    int nTotalForecastingsTrainningSet = maxLag + nTrainningRounds * stepsAhead;
    int timeES = 10;  // online training time

    vector<double> foIndicators;
    for (int beginTrainingSet = 168; beginTrainingSet >= stepsAhead;
         beginTrainingSet -= stepsAhead) {
      std::cout << "BeginTrainninningSet: " << beginTrainingSet;
      std::cout << "\t #SamplesTrainningSet: " << nTotalForecastingsTrainningSet
                << std::endl;
      std::cout << "#sizeTrainingSet: " << rF.getForecastsSize(0) << std::endl;
      std::cout << "maxNotUsed: " << problemParam.getMaxLag(0) << std::endl;
      std::cout << "#StepsAhead: " << stepsAhead << std::endl << std::endl;

      vector<vector<double>> trainningSet;  // trainningSetVector
      trainningSet.push_back(rF.getPartsForecastsEndToBegin(
          0, beginTrainingSet, nTotalForecastingsTrainningSet));
      ForecastClass forecastObject{trainningSet, problemParam, rg2,
                                   methodParam};

      std::optional<pair<SolutionHFM, Evaluation<>>> sol = std::nullopt;
      sol = forecastObject.run(timeES, 0, 0);

      vector<double> foIndicatorCalibration;
      vector<vector<double>> validationSet;
      validationSet.push_back(rF.getPartsForecastsEndToBegin(
          0, beginTrainingSet - stepsAhead, maxLag + stepsAhead));
      vector<double> errors =
          *forecastObject.returnErrors(sol->first.getR(), validationSet);
      foIndicators.push_back(errors[MAPE_INDEX]);
    }

    foIndicators.push_back(argvNTR);
    foIndicators.push_back(argvTargetTimeSeries);
    foIndicators.push_back(seed);
    vfoIndicatorCalibration.push_back(foIndicators);
  }

  double averageError = 0;
  for (int t = 0; t < (int)(vfoIndicatorCalibration[0].size() - 1); t++) {
    averageError += vfoIndicatorCalibration[0][t];
  }

  averageError /= (vfoIndicatorCalibration[0].size() - 1);
  vfoIndicatorCalibration[0].push_back(averageError);
  // =================== PRINTING RESULTS ========================
  for (int n = 0; n < nBatches; n++) {
    for (int i = 0; i < (int)vfoIndicatorCalibration[n].size(); i++)
      std::cout << vfoIndicatorCalibration[n][i] << "\t";

    std::cout << std::endl;
  }
  // =======================================================

  // =================== PRINTING RESULTS ON FILE ========================
  string calibrationFile = "./microGridCalibrationOnline";

  FILE* fResults = fopen(calibrationFile.c_str(), "a");
  for (int n = 0; n < nBatches; n++) {
    for (int i = 0; i < (int)vfoIndicatorCalibration[n].size(); i++)
      fprintf(fResults, "%.7f\t", vfoIndicatorCalibration[n][i]);
    fprintf(fResults, "\n");
  }

  fclose(fResults);
  // =======================================================

  return 0;
}

//
// for (int w = 4; w >= 1; w--)
//	{
//		vector<double> foIndicatorsMAPE;
//		vector<double> foIndicatorsRMSE;
//
//		for (int day = 1; day <= 7; day++)
//		{
//			vector<vector<double> > validationSet; //validation set
// for calibration
// validationSet.push_back(rF.getPartsForecastsEndToBegin(0, w * 168 -
// stepsAhead * day, nValidationSamples)); 			vector<double>
// foIndicators; 			foIndicators =
// forecastObject.returnErrors(sol, validationSet);
// foIndicatorsMAPE.push_back(foIndicators[MAPE_INDEX]);
//			foIndicatorsRMSE.push_back(foIndicators[RMSE_INDEX]);
//		}
//		double sumMAPE = accumulate(foIndicatorsMAPE.begin(),
// foIndicatorsMAPE.end(), 0.0); 		double sumRMSE =
// accumulate(foIndicatorsRMSE.begin(), foIndicatorsRMSE.end(), 0.0);
//
//		foIndicatorCalibration.push_back(sumMAPE/foIndicatorsMAPE.size());
//		foIndicatorCalibration.push_back(sumRMSE/foIndicatorsRMSE.size());
//	}
