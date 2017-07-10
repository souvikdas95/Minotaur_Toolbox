mode(1)
//
// Demo of qcqpglob.sci
//

//    Sample Only
//    Min. Z = 100 - x^2 + 2 * y^2 - 3 * x * y + 4 * x - 5 * y
//    Subject to:
//    -10 <= x <= 6
//    -10 <= y <= 6
halt()   // Press return to continue
 
n = 2;
m = 0;
h_obj = [-2, -1; -2, 4];
f_obj = [4, -5];
c_obj = 100;
h_con = list();
f_con = list();
lb_con = [];
ub_con = [];
maxcputime = 600;
lb_var = [-10, -10];
ub_var = [6, 6];
[kSoln, kSolnVector, status] = qcqpglob(n, m, h_obj, f_obj, c_obj, h_con, f_con, lb_con, ub_con, lb_var, ub_var, maxcputime);
halt()   // Press return to continue
 
//========= E N D === O F === D E M O =========//
