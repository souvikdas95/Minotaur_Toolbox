function [kSoln, kSolnVector, status] = qcqpglob(varargin)
	//	Solves problem with Convex/Non-Convex Quadratically Constrained Quadratic Programming Problems with Global Optimization (GLOB)
	//	
	//	Calling Sequence
	//	[kSoln, kSolnVector] = qcqpglob(n, m, h_obj, f_obj, c_obj, h_con, f_con, lb_con, ub_con, lb_var, ub_var);
	//	[kSoln, kSolnVector, status] = qcqpglob(n, m, h_obj, f_obj, c_obj, h_con, f_con, lb_con, ub_con, lb_var, ub_var);
	//	[kSoln, kSolnVector] = qcqpglob(n, m, h_obj, f_obj, c_obj, h_con, f_con, lb_con, ub_con, lb_var, ub_var, maxcputime);
	//	[kSoln, kSolnVector, status] = qcqpglob(n, m, h_obj, f_obj, c_obj, h_con, f_con, lb_con, ub_con, lb_var, ub_var, maxcputime);
	//	
	//	Parameters
	//	n : a double, represents number of Decision Variables
	//	m : a double, represents number of Constraints
	//	h_obj : a matrix of double, n x n, represents Coefficients of Quadratic in Objective Function
	//	f_obj : a vector of double, sized n, represents Coefficients of Linear in Objective Function
	//	c_obj : a double, represents Constant term in Objective Function
	//	h_con : an m sized list, contains matrices of double, n x n, represents Coefficients of Quadratic in Constraints
	//	f_con : an m sized list, contains vectors of double, sized n, represents Coefficients of Linear in Constraints
	//	lb_con : a vector of double, sized m, represents Lower Bound of Constraints
	//	ub_con : a vector of double, sized m, represents Upper Bound of Constraints
	//	lb_var : a vector of double, sized n, represents Lower Bound of Decision Variables
	//	ub_var : a vector of double, sized n, represents Upper Bound of Decision Variables
	//	maxcputime : a double, represents max. cpu time allotted to BnB Solver
	//	kSoln : a double, represents value of Objective Function at Optimal Point
	//	kSolnVector : a vector of double, sized n, represents values of Decision Variables at Optimal Point
	//	status : contains the exit flag of Solver. See below
	//	
	//	Description
	//	Solves QCQP / Quadratically Constrained Quadratic Programming problems, especially those where local optimum is not the global optimum, i.e. when the problem is Non-Convex. Can solve Convex Problem as well.
	//
	//	<latex>
	//		\text{}\\
	//		\begin{align}
	//		& \text{minimize} && \tfrac12 x^\mathrm{T} H_0 x + f_0^\mathrm{T} x + c \\
	//		& \text{subject to} && {LB}_i \leq \tfrac12 x^\mathrm{T} H_i x + f_i^\mathrm{T} x \leq {UB}_i \quad \text{for } i = 1,\dots,m \\
	//		&&& Ax = b
	//		\end{align}\\
	//		\text{}\\
	//		\text{}\\
	//		\text{}\\
	//	</latex>
	//
	//	The status allows to know the status of the optimization which is given back by MINOTAUR.
	//	
	//	<itemizedlist>
	//		<listitem>status=0 : Not started solving</listitem>
	//		<listitem>status=1 : Started solving</listitem>
	//		<listitem>status=2 : Restarted solving</listitem>
	//		<listitem>status=3 : Optimal solution found</listitem>
	//		<listitem>status=4 : Detected infeasibility</listitem>
	//		<listitem>status=5 : Detected unboundedness of relaxation</listitem>
	//		<listitem>status=6 : Reached limit on gap</listitem>
	//		<listitem>status=7 : Reached limit on number of solutions</listitem>
	//		<listitem>status=8 : Reached iteration limit</listitem>
	//		<listitem>status=9 : Interrupted</listitem>
	//		<listitem>status=10 : Reached time limit</listitem>
	//		<listitem>status=11 : Reached the limit on number of solutions</listitem>
	//		<listitem>status=12 : Finished for some other reason</listitem>
	//	</itemizedlist>
	//
	//	The routine uses Minotaur Library for solving the quadratic problem, Minotaur is a library written in C++.
	//
	//	Examples
	//	//	Sample Only
	//	//	Min. Z = 100 - x^2 + 2 * y^2 - 3 * x * y + 4 * x - 5 * y
	//	//	Subject to:
	//	//	60 >= x^2 + y^2 - x - y >= -100
	//	//	-10 <= x <= 6
	//	//	-10 <= y <= 6
	//	
	//	n = 2;
	//	m = 1;
	//	h_obj = [-2, -1; -2, 4];
	//	f_obj = [4, -5];
	//	c_obj = 100;
	//	h_con = list();
	//	h_con(1) = [2, 0; 0, 2];
	//	f_con = list();
	//	f_con(1) = [-1, -1];
	//	lb_con = [-100];
	//	ub_con = [60];
	//	maxcputime = 600;
	//	lb_var = [-10, -10];
	//	ub_var = [6, 6];
	//	[kSoln, kSolnVector, status] = qcqpglob(n, m, h_obj, f_obj, c_obj, h_con, f_con, lb_con, ub_con, lb_var, ub_var, maxcputime);
	//	kSoln
	//	kSolnVector
	//	status
	//
	//	Authors
	//	Souvik Das
	
	// Validate Input & Output Arguments
	[lhs, rhs] = argn();
	Checklhs("qcqpglob", lhs, [2 3]);
	Checkrhs("qcqpglob", rhs, [11 12]);

	// Get Input
	n = varargin(1);
	m = varargin(2);
	h_obj = varargin(3);
	f_obj = varargin(4);
	c_obj = varargin(5);
	h_con = varargin(6);
	f_con = varargin(7);
	lb_con = varargin(8);
	ub_con = varargin(9);
	lb_var = varargin(10);
	ub_var = varargin(11);
	if(rhs > 11) then
		maxcputime = varargin(12);
	else
		maxcputime = 600;	// Default
	end

	// Validate Input
	Checktype("qcqpglob", n, "n", 1, "constant");
	Checkdims("qcqpglob", n, "n", 1, [1 1]);
	if(n <= 0) then
	    errmsg = msprintf(gettext("%s: Number of Decision Variables should be > 0. Check input #1\n"), "qcqpglob");
	    error(errmsg);
	end

	Checktype("qcqpglob", m, "m", 2, "constant");
	Checkdims("qcqpglob", m, "m", 2, [1 1]);
	if(m < 0) then
	    errmsg = msprintf(gettext("%s: Number of Constraints should be >= 0. Check input #2\n"), "qcqpglob");
	    error(errmsg);
	end

	Checktype("qcqpglob", h_obj, "h_obj", 3, "constant");
	Checkdims("qcqpglob", h_obj, "h_obj", 3, [n n]);

	Checktype("qcqpglob", f_obj, "f_obj", 4, "constant");
	f_obj = Filterdims("qcqpglob", f_obj, "f_obj", 4, [1 n]);

	Checktype("qcqpglob", c_obj, "c_obj", 5, "constant");
	Checkdims("qcqpglob", c_obj, "c_obj", 5, [1 1]);

	Checktype("qcqpglob", h_con, "h_con", 6, "list");
	Checkdims("qcqpglob", h_con, "h_con", 6, [m]);
	for i = 1:m
		Checktype("qcqpglob", h_con(i), "h_con(" + string(i) + ")", 6, "constant");
		Checkdims("qcqpglob", h_con(i), "h_con(" + string(i) + ")", 6, [n n]);
	end

	Checktype("qcqpglob", f_con, "f_con", 7, "list");
	Checkdims("qcqpglob", f_con, "f_con", 7, [m]);
	for i = 1:m
		Checktype("qcqpglob", f_con(i), "f_con(" + string(i) + ")", 7, "constant");
		f_con(i) = Filterdims("qcqpglob", f_con(i), "f_con(" + string(i) + ")", 7, [1 n]);
	end

	Checktype("qcqpglob", lb_con, "lb_con", 8, "constant");
	if(m <> 0) then
		lb_con = Filterdims("qcqpglob", lb_con, "lb_con", 8, [1 m]);
	else
		lb_con = [];
	end

	Checktype("qcqpglob", ub_con, "ub_con", 9, "constant");
	if(m <> 0) then
		ub_con = Filterdims("qcqpglob", ub_con, "ub_con", 9, [1 m]);
	else
		ub_con = [];
	end

	Checktype("qcqpglob", lb_var, "lb_var", 10, "constant");
	lb_var = Filterdims("qcqpglob", lb_var, "lb_var", 10, [1 n]);

	Checktype("qcqpglob", ub_var, "ub_var", 11, "constant");
	ub_var = Filterdims("qcqpglob", ub_var, "ub_var", 11, [1 n]);

	if(maxcputime <= 0) then
		errmsg = msprintf(gettext("%s: ''maxcputime'' should be > 0. Check input #12"), "qcqpglob");
   		error(errmsg);
   	end

	// Default Output
	kSoln = 0;
	kSolnVector = [];
	status = 0;

	// Solve
	[kSoln, kSolnVector, status] = solve_qcqpglob(n, m, h_obj, f_obj, c_obj, h_con, f_con, lb_con, ub_con, lb_var, ub_var, maxcputime);

	// Print Status
	select status
	case 0 then
		printf("\nNot started solving\n");
	case 1 then
		printf("\nStarted solving\n");
	case 2 then
		printf("\nRestarted solving\n");
	case 3 then
		printf("\nOptimal solution found\n");
	case 4 then
		printf("\nDetected infeasibility\n");
	case 5 then
		printf("\nDetected unboundedness of relaxation\n");
	case 6 then
		printf("\nReached limit on gap\n");
	case 7 then
		printf("\nReached limit on number of solutions\n");
	case 8 then
		printf("\nReached iteration limit\n");
	case 9 then
		printf("\nInterrupted\n");
	case 10 then
		printf("\nReached time limit\n");
	case 11 then
		printf("\nReached the limit on number of solutions\n");
	case 12 then
		printf("\nFinished for some other reason\n");
	else
		printf("\nUnknown solve status\n");
	end

endfunction
