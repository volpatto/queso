/*--------------------------------------------------------------------------
 *--------------------------------------------------------------------------
 *
 * Copyright (C) 2008 The PECOS Development Team
 *
 * Please see http://pecos.ices.utexas.edu for more information.
 *
 * This file is part of the QUESO Library (Quantification of Uncertainty
 * for Estimation, Simulation and Optimization).
 *
 * QUESO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QUESO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QUESO. If not, see <http://www.gnu.org/licenses/>.
 *
 *--------------------------------------------------------------------------
 *
 * $Id$
 *
 * Brief description of this file: 
 * 
 *--------------------------------------------------------------------------
 *-------------------------------------------------------------------------- */

#ifndef __UQ_PROPAG_PROBLEM_H__
#define __UQ_PROPAG_PROBLEM_H__

#include <uqVectorFunction.h>
#include <uqMonteCarloSG.h>
#include <uqVectorRV.h>
#include <uqSequenceOfVectors.h>

#undef UQ_PROPAG_PROBLEM_READS_SOLVER_OPTION

#define UQ_PROPAG_PROBLEM_FILENAME_FOR_NO_OUTPUT_FILE "."

// _ODV = option default value
#define UQ_PROPAG_PROBLEM_COMPUTE_SOLUTION_ODV 1
#define UQ_PROPAG_PROBLEM_OUTPUT_FILE_NAME_ODV UQ_PROPAG_PROBLEM_FILENAME_FOR_NO_OUTPUT_FILE
#ifdef UQ_PROPAG_PROBLEM_READS_SOLVER_OPTION
#define UQ_PROPAG_PROBLEM_SOLVER_ODV           "mc" // Monte Carlo
#endif

template <class P_V,class P_M,class Q_V,class Q_M>
class uqStatisticalForwardProblemClass
{
public:
  uqStatisticalForwardProblemClass(const char*                                       prefix,
                                   const uqBaseVectorRVClass      <P_V,P_M>&         paramRv,
                                   const uqBaseVectorFunctionClass<P_V,P_M,Q_V,Q_M>& qoiFunction,
                                   uqGenericVectorRVClass         <Q_V,Q_M>&         qoiRv);
 ~uqStatisticalForwardProblemClass();

        bool                             computeSolutionFlag() const;
        void                             solveWithMonteCarlo();
  const uqGenericVectorRVClass<Q_V,Q_M>& qoiRv              () const;
  const uqBaseVectorCdfClass  <Q_V,Q_M>& qoiRv_unifiedCdf   () const;

        void                             print              (std::ostream& os) const;

private:
        void                             commonConstructor  ();
        void                             defineMyOptions    (po::options_description& optionsDesc);
        void                             getMyOptionValues  (po::options_description& optionsDesc);

  const uqBaseEnvironmentClass&                     m_env;
        std::string                                 m_prefix;

        po::options_description*                    m_optionsDesc;
        std::string                                 m_option_help;
	std::string                                 m_option_computeSolution;
        std::string                                 m_option_outputFileName;
#ifdef UQ_PROPAG_PROBLEM_READS_SOLVER_OPTION
        std::string                                 m_option_solver;
#endif

        bool                                        m_computeSolution;
        std::string                                 m_outputFileName;
#ifdef UQ_PROPAG_PROBLEM_READS_SOLVER_OPTION
	std::string                                 m_solverString;
#endif

  const uqBaseVectorRVClass      <P_V,P_M>&         m_paramRv;
  const uqBaseVectorFunctionClass<P_V,P_M,Q_V,Q_M>& m_qoiFunction;
        uqGenericVectorRVClass   <Q_V,Q_M>&         m_qoiRv; // FIX ME: Maybe not always generic ?

        uqBaseVectorPdfClass     <Q_V,Q_M>*         m_solutionPdf;
        uqBaseVectorMdfClass     <Q_V,Q_M>*         m_solutionMdf;
        uqBaseVectorCdfClass     <Q_V,Q_M>*         m_solutionCdf;
        uqBaseVectorRealizerClass<Q_V,Q_M>*         m_solutionRealizer;

        uqMonteCarloSGClass      <P_V,P_M,Q_V,Q_M>* m_mcSeqGenerator;
        uqBaseVectorSequenceClass<Q_V,Q_M>*         m_chain;
        uqArrayOfOneDGridsClass  <Q_V,Q_M>*         m_mdfGrids;
        uqArrayOfOneDTablesClass <Q_V,Q_M>*         m_mdfValues;
        uqArrayOfOneDGridsClass  <Q_V,Q_M>*         m_cdfGrids;
        uqArrayOfOneDTablesClass <Q_V,Q_M>*         m_cdfValues;

        uqBaseVectorCdfClass     <Q_V,Q_M>*         m_unifiedSolutionCdf;
        uqArrayOfOneDGridsClass  <Q_V,Q_M>*         m_unifiedCdfGrids;
        uqArrayOfOneDTablesClass <Q_V,Q_M>*         m_unifiedCdfValues;
};

template<class P_V,class P_M,class Q_V,class Q_M>
std::ostream& operator<<(std::ostream& os, const uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>& obj);

template <class P_V,class P_M,class Q_V,class Q_M>
uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::uqStatisticalForwardProblemClass(
  const char*                                       prefix,
  const uqBaseVectorRVClass      <P_V,P_M>&         paramRv,
  const uqBaseVectorFunctionClass<P_V,P_M,Q_V,Q_M>& qoiFunction,
        uqGenericVectorRVClass   <Q_V,Q_M>&         qoiRv)
  :
  m_env                   (paramRv.env()),
  m_prefix                ((std::string)(prefix) + "fp_"),
  m_optionsDesc           (new po::options_description("UQ Propagation Problem")),
  m_option_help           (m_prefix + "help"           ),
  m_option_computeSolution(m_prefix + "computeSolution"),
  m_option_outputFileName (m_prefix + "outputFileName" ),
#ifdef UQ_PROPAG_PROBLEM_READS_SOLVER_OPTION
  m_option_solver         (m_prefix + "solver"         ),
#endif
  m_computeSolution       (UQ_PROPAG_PROBLEM_COMPUTE_SOLUTION_ODV),
  m_outputFileName        (UQ_PROPAG_PROBLEM_OUTPUT_FILE_NAME_ODV),
#ifdef UQ_PROPAG_PROBLEM_READS_SOLVER_OPTION
  m_solverString          (UQ_PROPAG_PROBLEM_SOLVER_ODV),
#endif
  m_paramRv               (paramRv),
  m_qoiFunction           (qoiFunction),
  m_qoiRv                 (qoiRv),
  m_solutionPdf           (NULL),
  m_solutionMdf           (NULL),
  m_solutionCdf           (NULL),
  m_solutionRealizer      (NULL),
  m_mcSeqGenerator        (NULL),
  m_chain                 (NULL),
  m_mdfGrids              (NULL),
  m_mdfValues             (NULL),
  m_cdfGrids              (NULL),
  m_cdfValues             (NULL),
  m_unifiedSolutionCdf    (NULL),
  m_unifiedCdfGrids       (NULL),
  m_unifiedCdfValues      (NULL)
{
  if (m_env.subScreenFile()) {
    *m_env.subScreenFile() << "Entering uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::constructor()"
                           << ": prefix = "              << m_prefix
                           << std::endl;
  }

  defineMyOptions                (*m_optionsDesc);
  m_env.scanInputFileForMyOptions(*m_optionsDesc);
  getMyOptionValues              (*m_optionsDesc);

  if (m_env.subScreenFile()) {
    *m_env.subScreenFile() << "In uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::constructor()"
                           << ": after getting values of options, state of object is:"
                           << "\n" << *this
                           << std::endl;
  }

  if (m_env.subScreenFile()) {
    *m_env.subScreenFile() << "Leaving uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::constructor()"
                           << ": prefix = "              << m_prefix
                           << std::endl;
  }
}

template <class P_V,class P_M,class Q_V,class Q_M>
uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::~uqStatisticalForwardProblemClass()
{
  if (m_unifiedCdfValues  ) delete m_unifiedCdfValues;
  if (m_unifiedCdfGrids   ) delete m_unifiedCdfGrids;
  if (m_unifiedSolutionCdf) delete m_unifiedSolutionCdf;
  if (m_cdfValues         ) delete m_cdfValues;
  if (m_cdfGrids          ) delete m_cdfGrids;
  if (m_mdfValues         ) delete m_mdfValues;
  if (m_mdfGrids          ) delete m_mdfGrids;
  if (m_chain) {
    m_chain->clear();
    delete m_chain;
  }
  if (m_mcSeqGenerator  ) delete m_mcSeqGenerator;
  if (m_solutionRealizer) delete m_solutionRealizer;
  if (m_solutionCdf     ) delete m_solutionCdf;
  if (m_solutionMdf     ) delete m_solutionMdf;
  if (m_solutionPdf     ) delete m_solutionPdf;
  if (m_optionsDesc     ) delete m_optionsDesc;
}

template<class P_V,class P_M,class Q_V,class Q_M>
void
uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::defineMyOptions(
  po::options_description& optionsDesc)
{
  optionsDesc.add_options()
    (m_option_help.c_str(),                                                                                             "produce help message for propagation problem")
    (m_option_computeSolution.c_str(), po::value<bool       >()->default_value(UQ_PROPAG_PROBLEM_COMPUTE_SOLUTION_ODV), "compute solution process"                    )
    (m_option_outputFileName.c_str(),  po::value<std::string>()->default_value(UQ_PROPAG_PROBLEM_OUTPUT_FILE_NAME_ODV), "name of output file"                         )
#ifdef UQ_PROPAG_PROBLEM_READS_SOLVER_OPTION
    (m_option_solver.c_str(),          po::value<std::string>()->default_value(UQ_PROPAG_PROBLEM_SOLVER_ODV          ), "algorithm for propagation"                   )
#endif
  ;

  return;
}

template<class P_V,class P_M,class Q_V,class Q_M>
void
uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::getMyOptionValues(
  po::options_description& optionsDesc)
{
  if (m_env.allOptionsMap().count(m_option_help.c_str())) {
    if (m_env.subScreenFile()) {
      *m_env.subScreenFile() << optionsDesc
                             << std::endl;
    }
  }

  if (m_env.allOptionsMap().count(m_option_computeSolution.c_str())) {
    m_computeSolution = m_env.allOptionsMap()[m_option_computeSolution.c_str()].as<bool>();
  }

  if (m_env.allOptionsMap().count(m_option_outputFileName.c_str())) {
    m_outputFileName = m_env.allOptionsMap()[m_option_outputFileName.c_str()].as<std::string>();
  }

#ifdef UQ_PROPAG_PROBLEM_READS_SOLVER_OPTION
  if (m_env.allOptionsMap().count(m_option_solver.c_str())) {
    m_solverString = m_env.allOptionsMap()[m_option_solver.c_str()].as<std::string>();
  }
#endif

  return;
}

template <class P_V,class P_M, class Q_V, class Q_M>
bool
  uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::computeSolutionFlag() const
{
  return m_computeSolution;
}

template <class P_V,class P_M,class Q_V,class Q_M>
void
uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::solveWithMonteCarlo()
{
  m_env.fullComm().Barrier();
  //m_env.syncPrintDebugMsg("Entering uqStatisticalForwardProblemClass<P_V,P_M>::solveWithMonteCarlo()",3000000,m_env.fullComm());

  if (m_computeSolution == false) {
    if ((m_env.subScreenFile())) {
      *m_env.subScreenFile() << "In uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::solveWithMonteCarlo()"
                             << ": avoiding solution, as requested by user"
                             << std::endl;
    }
    return;
  }
  if ((m_env.subScreenFile())) {
    *m_env.subScreenFile() << "In uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::solveWithMonteCarlo()"
                           << ": computing solution, as requested by user"
                           << std::endl;
  }

  if (m_solutionPdf     ) delete m_solutionPdf;
  if (m_solutionMdf     ) delete m_solutionMdf;
  if (m_solutionCdf     ) delete m_solutionCdf;
  if (m_solutionRealizer) delete m_solutionRealizer;
  if (m_mcSeqGenerator  ) delete m_mcSeqGenerator;
  if (m_chain) {
    m_chain->clear();
    delete m_chain;
  }
  if (m_mdfGrids          ) delete m_mdfGrids;
  if (m_mdfValues         ) delete m_mdfValues;
  if (m_cdfGrids          ) delete m_cdfGrids;
  if (m_cdfValues         ) delete m_cdfValues;
  if (m_unifiedSolutionCdf) delete m_unifiedSolutionCdf;
  if (m_unifiedCdfGrids   ) delete m_unifiedCdfGrids;
  if (m_unifiedCdfValues  ) delete m_unifiedCdfValues;

  Q_V numEvaluationPointsVec(m_qoiRv.imageSet().vectorSpace().zeroVector());
  numEvaluationPointsVec.cwSet(250.);

  // Compute output realizer: Monte Carlo approach
  m_chain = new uqSequenceOfVectorsClass<Q_V,Q_M>(m_qoiRv.imageSet().vectorSpace(),0,m_prefix+"chain");
  m_mcSeqGenerator = new uqMonteCarloSGClass<P_V,P_M,Q_V,Q_M>(m_prefix.c_str(),
                                                              m_paramRv,
                                                              m_qoiFunction,
                                                              m_qoiRv);
  m_mcSeqGenerator->generateSequence(*m_chain);
  m_solutionRealizer = new uqSequentialVectorRealizerClass<Q_V,Q_M>(m_prefix.c_str(),
                                                                    *m_chain);
  m_qoiRv.setRealizer(*m_solutionRealizer);

  // Compute output mdf: uniform sampling approach
  m_mdfGrids  = new uqArrayOfOneDGridsClass <Q_V,Q_M>((m_prefix+"Mdf_").c_str(),m_qoiRv.imageSet().vectorSpace());
  m_mdfValues = new uqArrayOfOneDTablesClass<Q_V,Q_M>((m_prefix+"Mdf_").c_str(),m_qoiRv.imageSet().vectorSpace());
  m_chain->uniformlySampledMdf(numEvaluationPointsVec, // input
                               *m_mdfGrids,            // output
                               *m_mdfValues);          // output

  m_solutionMdf = new uqSampledVectorMdfClass<Q_V,Q_M>(m_prefix.c_str(),
                                                       *m_mdfGrids,
                                                       *m_mdfValues);
  m_qoiRv.setMdf(*m_solutionMdf);

  // Compute output cdf: uniform sampling approach
  m_cdfGrids  = new uqArrayOfOneDGridsClass <Q_V,Q_M>((m_prefix+"Cdf_").c_str(),m_qoiRv.imageSet().vectorSpace());
  m_cdfValues = new uqArrayOfOneDTablesClass<Q_V,Q_M>((m_prefix+"Cdf_").c_str(),m_qoiRv.imageSet().vectorSpace());
  m_chain->uniformlySampledCdf(numEvaluationPointsVec, // input
                               *m_cdfGrids,            // output
                               *m_cdfValues);          // output

  m_solutionCdf = new uqSampledVectorCdfClass<Q_V,Q_M>(m_prefix.c_str(),
                                                       *m_cdfGrids,
                                                       *m_cdfValues);
  m_qoiRv.setCdf(*m_solutionCdf);

  // Compute unified cdf if necessary
  if (m_env.numSubEnvironments() > 1) {
    m_unifiedCdfGrids  = new uqArrayOfOneDGridsClass <Q_V,Q_M>((m_prefix+"unifiedCdf_").c_str(),m_qoiRv.imageSet().vectorSpace());
    m_unifiedCdfValues = new uqArrayOfOneDTablesClass<Q_V,Q_M>((m_prefix+"unifiedCdf_").c_str(),m_qoiRv.imageSet().vectorSpace());
    m_chain->unifiedUniformlySampledCdf(numEvaluationPointsVec, // input
                                        *m_unifiedCdfGrids,     // output
                                        *m_unifiedCdfValues);   // output

    m_unifiedSolutionCdf = new uqSampledVectorCdfClass<Q_V,Q_M>((m_prefix+"unified").c_str(),
                                                                *m_unifiedCdfGrids,
                                                                *m_unifiedCdfValues);
  }

  if (m_outputFileName != UQ_PROPAG_PROBLEM_FILENAME_FOR_NO_OUTPUT_FILE) {
    if (m_env.subRank() == 0) {
      // Write output file
      if (m_env.subScreenFile()) {
        *m_env.subScreenFile() << "Opening output file '" << m_outputFileName
                               << "' for propagation problem with problem with prefix = " << m_prefix
                               << std::endl;
      }

      // Open file
      std::ofstream* ofsvar = new std::ofstream((m_outputFileName+"_subenv"+m_env.subIdString()+".m").c_str(), std::ofstream::out | std::ofstream::in | std::ofstream::ate);
      if ((ofsvar            == NULL ) ||
          (ofsvar->is_open() == false)) {
        delete ofsvar;
        ofsvar = new std::ofstream(m_outputFileName.c_str(), std::ofstream::out | std::ofstream::trunc);
      }
      UQ_FATAL_TEST_MACRO((ofsvar && ofsvar->is_open()) == false,
                          m_env.rank(),
                          "uqStatisticalForwardProblem<P_V,P_M,Q_V,Q_M>::solveWithBayesMarkovChain()",
                          "failed to open file");

      m_qoiRv.mdf().print(*ofsvar);
      *ofsvar << m_qoiRv.cdf();

      // Write unified cdf if necessary
      if (m_env.numSubEnvironments() > 1) {
        if (m_qoiRv.imageSet().vectorSpace().zeroVector().numberOfProcessorsRequiredForStorage() == 1) {
          if (m_env.intra0Rank() == 0) {
            *ofsvar << *m_unifiedSolutionCdf;
          }
        }
        else {
          UQ_FATAL_TEST_MACRO(true,
                              m_env.rank(),
                              "uqStatisticalForwardProblem<P_V,P_M,Q_V,Q_M>::solveWithBayesMarkovChain()",
                              "unifed cdf writing, parallel vectors not supported yet");
        }
      }

      // Close file
      ofsvar->close();
      delete ofsvar;
      if (m_env.subScreenFile()) {
        *m_env.subScreenFile() << "Closed output file '" << m_outputFileName
                               << "' for propagation problem with problem with prefix = " << m_prefix
                               << std::endl;
      }
    }
  }
  if (m_env.subScreenFile()) {
    *m_env.subScreenFile() << std::endl;
  }

  //m_env.syncPrintDebugMsg("Leaving uqStatisticalForwardProblemClass<P_V,P_M>::solveWithMonteCarlo()",3000000,m_env.fullComm());
  m_env.fullComm().Barrier();

  return;
}

template <class P_V,class P_M,class Q_V,class Q_M>
const uqGenericVectorRVClass<Q_V,Q_M>& 
uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::qoiRv() const
{
  return m_qoiRv;
}

template <class P_V,class P_M,class Q_V,class Q_M>
const uqBaseVectorCdfClass<Q_V,Q_M>&
uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::qoiRv_unifiedCdf() const
{
  if (m_env.numSubEnvironments() == 1) {
    return m_qoiRv.cdf();
  }

  if (m_env.intra0Rank() < 0) {
    return m_qoiRv.cdf();
  }

  UQ_FATAL_TEST_MACRO(m_unifiedSolutionCdf == NULL,
                      m_env.rank(),
                      "uqStatisticalForwardProblem<P_V,P_M,Q_V,Q_M>::qoiRv_unifiedCdf()",
                      "variable is NULL");
  return *m_unifiedSolutionCdf;
}


template <class P_V,class P_M,class Q_V,class Q_M>
void
uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::print(std::ostream& os) const
{
  os << "\n" << m_option_computeSolution << " = " << m_computeSolution
     << "\n" << m_option_outputFileName  << " = " << m_outputFileName
#ifdef UQ_PROPAG_PROBLEM_READS_SOLVER_OPTION
     << "\n" << m_option_solver          << " = " << m_solverString
#endif
     << std::endl;
}

template<class P_V,class P_M,class Q_V,class Q_M>
std::ostream& operator<<(std::ostream& os, const uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>& obj)
{
  obj.print(os);

  return os;
}
#endif // __UQ_PROPAG_PROBLEM_H__
