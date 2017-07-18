// Number of Decision Variables should be > 0. Check input #1
 
n = 0;	// <--
m = 1;
h_obj = [-2, -1; -2, 4];
f_obj = [4, -5];
c_obj = 100;
h_con = list();
h_con(1) = [2, 0; 0, 2];
f_con = list();
f_con(1) = [-1, -1];
lb_con = [-100];
ub_con = [60];
maxcputime = 600;
lb_var = [-10, -10];
ub_var = [6, 6];
[kSoln, kSolnVector, status] = qcqpglob(n, m, h_obj, f_obj, c_obj, h_con, f_con, lb_con, ub_con, lb_var, ub_var, maxcputime);
