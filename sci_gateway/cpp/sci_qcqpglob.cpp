// Glob QCQP Minotaur Scilab Interface Gateway
//
// qcqpglob(n, m, h_obj, f_obj, c_obj, h_con, f_con, lb_con, ub_con, lb_var, ub_bar, maxcputime);
//
// n -> no. of variables
// m -> no. of constraints
// h_obj -> n x n matrix
// f_obj -> n x 1 matrix or n-column-vector
// c_obj -> 1 x 1 constant
// h_con -> m sized list of n x n matrices or m x n x n 3D-matrix
// f_con -> m sized list of n-column-vector or m x n matrix
// lb_con -> m x 1 matrix or m-column-vector
// ub_con -> m x 1 matrix or m-column-vector
// lb_var -> n x 1 matrix or n-column-vector
// ub_var -> n x 1 matrix or n-column-vector
// maxcputime -> timeout in seconds for solving
//
//
#include <cmath>

#include "MinotaurConfig.h"
#include "BranchAndBound.h"
#include "Engine.h"
#include "EngineFactory.h"
#include "Environment.h"
#include "LexicoBrancher.h"
#include "LinearHandler.h"
#include "Logger.h"
#include "LPEngine.h"
#include "MaxVioBrancher.h"
#include "NodeIncRelaxer.h"
#include "NLPEngine.h"
#include "NLPMultiStart.h"
#include "NlPresHandler.h"
#include "Objective.h"
#include "Option.h"
#include "PCBProcessor.h"
#include "Presolver.h"
#include "ProblemSize.h"
#include "Problem.h"
#include "Relaxation.h"
#include "ReliabilityBrancher.h"
#include "SimpleTransformer.h"
#include "Solution.h"
#include "Timer.h"
#include "Transformer.h"
#include "TreeManager.h"
#include "CGraph.h"
#include "CNode.h"
#include "Variable.h"
#include "Function.h"
#include "LinearFunction.h"
#include "QuadraticFunction.h"

extern "C"{
	#include <stdlib.h>
	#include <api_scilab.h>
	#include <localization.h>
	#include <Scierror.h>
	#include <sciprint.h>
	
	int sci_qcqpglob(const char* fname, unsigned long fname_len);	// Main
}

#include "sci_iofunc.hpp"
#include "misc.hpp"

using namespace Minotaur;

// Declarations
int Transform(EnvPtr env, ProblemPtr p_orig, ProblemPtr &p_new, HandlerVector &handlers);
PresolverPtr CreatePresolver(ProblemPtr p, EnvPtr env, HandlerVector &handlers, size_t numVars = 0);
BranchAndBound* CreateBranchAndBound(EnvPtr env, ProblemPtr p, EnginePtr e, HandlerVector &handlers);
double* GetSolution(PresolverPtr pres, SolutionPtr sol, int n);
void GLOB_addObjective(ProblemPtr& p, double* H, double* f, double c);
void GLOB_addConstraint(ProblemPtr& p, double* H, double* f, double lb, double ub);

// Definitions
int Transform(EnvPtr env, ProblemPtr p_orig, ProblemPtr &p_new, HandlerVector &handlers) 
{
	// Init Vars
	SimpTranPtr trans = SimpTranPtr(); // NULL
	int status = 0;

	// Process
	handlers.clear();
	trans = (SimpTranPtr) new SimpleTransformer(env, p_orig);
	trans->reformulate(p_new, handlers, status);

	return status;
}

PresolverPtr CreatePresolver(ProblemPtr p, EnvPtr env, HandlerVector &handlers, size_t numVars = 0)
{
	// Init Vars
	PresolverPtr pres = PresolverPtr(); // NULL
	LinearHandlerPtr lhandler = LinearHandlerPtr();	// NULL
	NlPresHandlerPtr nlhand = NlPresHandlerPtr(); // NULL

	// Process
	p->calculateSize();
	if (env->getOptions()->findBool("presolve")->getValue() == true)
	{
		if (env->getOptions()->findBool("lin_presolve")->getValue() == true)
		{
			lhandler = (LinearHandlerPtr) new LinearHandler(env, p);
			handlers.push_back(lhandler);

			if (p->isQP() ||
				p->isQuadratic() ||
				p->isLinear() ||
				env->getOptions()->findBool("use_native_cgraph")->getValue() == true)
			{
				lhandler->setPreOptPurgeVars(true);
				lhandler->setPreOptPurgeCons(true);
				lhandler->setPreOptCoeffImp(true);
			}
			else
			{
				lhandler->setPreOptPurgeVars(false);
				lhandler->setPreOptPurgeCons(false);
				lhandler->setPreOptCoeffImp(false);
			}

			if (numVars > 0)
			{
				lhandler->setPreOptDualFix(false);
			}
			else
			{
				lhandler->setPreOptDualFix(true);
			}
		}

		if (env->getOptions()->findBool("nl_presolve")->getValue() == true &&
			!p->isLinear() && 
			env->getOptions()->findBool("use_native_cgraph")->getValue() == true)
		{
			nlhand = (NlPresHandlerPtr) new NlPresHandler(env, p);
			handlers.push_back(nlhand);
		}
	}
	pres = (PresolverPtr) new Presolver(p, env, handlers);

	return pres;
}

BranchAndBound* CreateBranchAndBound(EnvPtr env, ProblemPtr p, EnginePtr e, HandlerVector &handlers)
{
	// Init Vars
	BranchAndBound* bab = new BranchAndBound(env, p);
	PCBProcessorPtr nproc = PCBProcessorPtr(); // NULL
	NodeIncRelaxerPtr nr = NodeIncRelaxerPtr(); // NULL
	RelaxationPtr rel = RelaxationPtr(); // NULL
	BrancherPtr br = BrancherPtr(); // NULL

	if (env->getOptions()->findString("brancher")->getValue() == "rel")
	{
		UInt t;
		ReliabilityBrancherPtr rel_br = (ReliabilityBrancherPtr) new ReliabilityBrancher(env, handlers);
		rel_br->setEngine(e);
		t = (p->getSize()->ints + p->getSize()->bins)/10;
		t = std::max(t, (UInt) 2);
		t = std::min(t, (UInt) 4);
		rel_br->setThresh(t);
		t = (UInt) p->getSize()->ints + p->getSize()->bins/20+2;
		t = std::min(t, (UInt) 10);
		rel_br->setMaxDepth(t);
		br = rel_br;
	}
	else if (env->getOptions()->findString("brancher")->getValue() == "maxvio")
	{
		MaxVioBrancherPtr mbr = (MaxVioBrancherPtr) new MaxVioBrancher(env, handlers);
		br = mbr;
	}
	else if (env->getOptions()->findString("brancher")->getValue() == "lex")
	{
		LexicoBrancherPtr lbr = (LexicoBrancherPtr) new LexicoBrancher(env, handlers);
		br = lbr;
	}

	nproc = (PCBProcessorPtr) new PCBProcessor(env, e, handlers);
	nproc->setBrancher(br);
	bab->setNodeProcessor(nproc);

	nr = (NodeIncRelaxerPtr) new NodeIncRelaxer(env, handlers);
	nr->setProblem(p);
	nr->setRelaxation(rel);
	nr->setEngine(e);
	bab->setNodeRelaxer(nr);
	bab->shouldCreateRoot(true);

	if (env->getOptions()->findBool("msheur")->getValue() == true && 
		(p->getSize()->bins == 0 && p->getSize()->ints == 0))
	{
		EngineFactory* efac = new EngineFactory(env);
		NLPEnginePtr nlp_e = efac->getNLPEngine();
		p->setNativeDer();
		NLPMSPtr ms_heur = (NLPMSPtr) new NLPMultiStart(env, p, nlp_e);
		bab->addPreRootHeur(ms_heur);
		delete efac;
	}

	return bab;
}

double* GetSolution(PresolverPtr pres, SolutionPtr sol, int n)
{
	const double* exSoln = NULL;
	double* ret = (double*)malloc(sizeof(double) * n);;
	UInt i;

	if (sol != SolutionPtr())
	{
		sol = pres->getPostSol(sol);
		if (sol != SolutionPtr())
			exSoln = sol->getPrimal();
	}

	if(exSoln != NULL)
	{
		for(i = 0; i < n; ++i)
			ret[i] = exSoln[i];
	}
	else	// Fail Safe
	{
		for(i = 0; i < n; ++i)
			ret[i] = 0.0;
	}

	return ret;
}

void GLOB_addObjective(ProblemPtr& p, double* H, double* f, double c)
{
	VariableConstIterator vbeg = p->varsBegin();
	VariableConstIterator vend = p->varsEnd();
	CGraphPtr cg = CGraphPtr();	// NULL
	FunctionPtr fun = FunctionPtr(); // NULL

	QuadraticFunctionPtr qf = (QuadraticFunctionPtr) new QuadraticFunction(H, vbeg, vend);
	LinearFunctionPtr lf = (LinearFunctionPtr) new LinearFunction(f, vbeg, vend, 1e-9);

	if(qf->getNumTerms() + lf->getNumTerms() == 0)
	{
		// Not an Objective!
		return;
	}
	
	if(qf->getNumTerms() == 0)
	{
		fun = (FunctionPtr) new Function(lf);
	}
	else
	{
		cg = (CGraphPtr) new CGraph(qf, lf);
		cg->finalize();
		fun = (FunctionPtr) new Function(cg);
	}

	p->newObjective(fun, c, Minimize);
}

void GLOB_addConstraint(ProblemPtr& p, double* H, double* f, double lb, double ub)
{
	VariableConstIterator vbeg = p->varsBegin();
	VariableConstIterator vend = p->varsEnd();
	CGraphPtr cg = CGraphPtr(); // NULL
	FunctionPtr fun = FunctionPtr(); // NULL

	QuadraticFunctionPtr qf = (QuadraticFunctionPtr) new QuadraticFunction(H, vbeg, vend);
	LinearFunctionPtr lf = (LinearFunctionPtr) new LinearFunction(f, vbeg, vend, 1e-9);

	if(qf->getNumTerms() + lf->getNumTerms() == 0)
	{
		// Not a Constraint!
		return;
	}
	
	if(qf->getNumTerms() == 0)
	{
		fun = (FunctionPtr) new Function(lf);
	}
	else
	{
		cg = (CGraphPtr) new CGraph(qf, lf);
		cg->finalize();
		fun = (FunctionPtr) new Function(cg);
	}

	p->newConstraint(fun, lb, ub);
}

int sci_qcqpglob(const char* fname, unsigned long fname_len)
{
	//////////////////
	// Vars Declare //
	//////////////////

	// General Vars
	UInt i, j;

	// Solver Vars
	EnvPtr env = EnvPtr(); // NULL
	HandlerVector handlers;
	int err = 0;
	ProblemPtr p_orig = ProblemPtr(); // NULL
	ProblemPtr p_new = ProblemPtr(); // NULL
	EngineFactory* efac = NULL;
	LPEnginePtr lp_e = LPEnginePtr(); // NULL
	BranchAndBound* bab = NULL;
	SimpTranPtr trans = SimpTranPtr(); // NULL
	PresolverPtr pres_orig = PresolverPtr(); // NULL
	PresolverPtr pres_new = PresolverPtr(); // NULL
	VarVector orig_v;

	// Input Vars
	int n = 0;
	int m = 0;
	double* h_obj = NULL;
	double* f_obj = NULL;
	double c_obj = 0;
	double** h_con = NULL;
	double** f_con = NULL;
	double* lb_con = NULL;
	double* ub_con = NULL;
	double* lb_var = NULL;
	double* ub_var = NULL;
	double maxcputime = 0;

	// Output Vars
	double* kSolnVector = NULL;
	double kSoln = 0;
	int status = 0;

	// Extra Vars
	double* h_obj_rowm = NULL;	// Row Major
	double** h_con_rowm = NULL;	// Row Major

	//////////////////////
	// Scilab API Begin //
	//////////////////////

	// Check Arguments
	CheckInputArgument(pvApiCtx, 12, 12);
	CheckOutputArgument(pvApiCtx, 3, 3);
	
	// Input
	if(getInt32FromScilab(1, &n))
	{
		return 1;
	}
	if(getInt32FromScilab(2, &m))
	{
		return 1;
	}
	if(getFixedSizeDoubleMatrixFromScilab(3, n, n, &h_obj))
	{
		return 1;
	}
	if(getFixedSizeDoubleMatrixFromScilab(4, 1, n, &f_obj))
	{
		return 1;
	}
	if(getDoubleFromScilab(5, &c_obj))
	{
		return 1;
	}
	if(getFixedSizeListOfFixedSizeDoubleMatrixFromScilab(6, n, n, &h_con, m))
	{
		return 1;
	}
	if(getFixedSizeListOfFixedSizeDoubleMatrixFromScilab(7, 1, n, &f_con, m))
	{
		return 1;
	}
	if(getFixedSizeDoubleMatrixFromScilab(8, m ? 1 : 0, m, &lb_con))
	{
		return 1;
	}
	if(getFixedSizeDoubleMatrixFromScilab(9, m ? 1 : 0, m, &ub_con))
	{
		return 1;
	}
	if(getFixedSizeDoubleMatrixFromScilab(10, 1, n, &lb_var))
	{
		return 1;
	}
	if(getFixedSizeDoubleMatrixFromScilab(11, 1, n, &ub_var))
	{
		return 1;
	}
	if(getDoubleFromScilab(12, &maxcputime))
	{
		return 1;
	}

	// Convert Column Major to Row Major
	h_obj_rowm = ArrConvertMajor(h_obj, n, n);
	h_con_rowm = (double**)malloc(sizeof(double**) * m);
	for(i = 0; i < m; ++i)
		h_con_rowm[i] = ArrConvertMajor(h_con[i], n, n);

	////////////////////////////////
	// Setup Minotaur Environment //
	////////////////////////////////

	// Initialize Environment
	env = (EnvPtr) new Environment();

	// Set Fixed Options
	env->getOptions()->findBool("use_native_cgraph")->setValue(true);
	env->getOptions()->findBool("presolve")->setValue(true);
	env->getOptions()->findBool("nl_presolve")->setValue(true);
	env->getOptions()->findBool("lin_presolve")->setValue(true);
	env->getOptions()->findString("brancher")->setValue("maxvio");
	env->getOptions()->findBool("msheur")->setValue(true);
	env->getOptions()->findInt("pres_freq")->setValue(1);
	env->getOptions()->findString("lp_engine")->setValue("OsiClp");
	env->getOptions()->findString("nlp_engine")->setValue("IPOPT");

	// Set User-defined Options
	env->getOptions()->findDouble("bnb_time_limit")->setValue(maxcputime);

	//////////////////
	// Read Problem //
	//////////////////

	// Declare Problem Pointer
	p_orig = (ProblemPtr) new Problem();

	// Add Variables
	for(i = 0; i < n; ++i)
	{
		// Library Limits. Need Warning?
		orig_v.push_back(p_orig->newVariable(std::max(lb_var[i], -1e6), std::min(ub_var[i], 1e6), Integer));
	}

	// Add Objective
	GLOB_addObjective(p_orig, h_obj_rowm, f_obj, c_obj);

	// Add Constraints
	for(i = 0; i < m; ++i)
	{
		GLOB_addConstraint(p_orig, h_con_rowm[i], f_con[i], lb_con[i], ub_con[i]);
	}

	// Set Native Derivatives
	p_orig->setNativeDer();

	///////////
	// Solve //
	///////////

	// Start timer
	env->startTimer(err); assert(err==0);

	// Presolve Original Problem
	handlers.clear();
	pres_orig = CreatePresolver(p_orig, env, handlers, orig_v.size());
	if (env->getOptions()->findBool("presolve")->getValue() == true)
	{
		pres_orig->solve();
	}
	handlers.clear();

	// Transform (KKT & Reformulate)
	err = Transform(env, p_orig, p_new, handlers); assert(0==err);

	// Presolve Transformed Problem
	pres_new = (PresolverPtr) new Presolver(p_new, env, handlers);
	pres_new->solve();

	// Get LP Engine for Solving
	efac = new EngineFactory(env);
	lp_e = efac->getLPEngine();

	// Create Branch and Bound Object
	bab = CreateBranchAndBound(env, p_new, lp_e, handlers);

	// Start Solving
	bab->solve();

	// Get Solution
	status = bab->getStatus();
	kSoln = bab->getUb();
	kSolnVector = GetSolution(pres_orig, bab->getSolution(), n);
	kSoln += c_obj;	// Problem with Library?
	
	// Clean
	delete bab;
	delete efac;

	////////////////////
	// Scilab API End //
	////////////////////

	// Output
	if (returnDoubleToScilab(1, kSoln))
	{
		return 1;
	}
	if (returnDoubleMatrixToScilab(2, 1, n, kSolnVector))
	{
		return 1;
	}
	if (returnInt32ToScilab(3, status))
	{
		return 1;
	}

	// Cleanup
	free(h_obj_rowm);
	for(i = 0; i < m; ++i)
		free(h_con_rowm[i]);
	free(h_con_rowm);

	return 0;
}