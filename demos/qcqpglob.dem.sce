mode(1)
//
// Demo of qcqpglob.sci
//

// Sample Only
n = 5;
m = 3;
h_obj = [
0, 0, 0, 1, -1;
0, 0, 0, 0, 0;
0, 0, 0, 0, 0;
0, 0, 0, 0, 0;
-1, 0, 0, 0, 0
];
f_obj = [    0, 1, 0, 0, 0    ];
c_obj = 2;
h_con = list();
h_con(1) = [
2, 0, 0, 0, 0;
0, 2, 0, 0, 0;
0, 0, 2, 0 ,0;
0, 0, 0, 0, 0;
0, 0, 0, 0, 0
];
h_con(2) = [
-2, 0, 0, 0, 0;
0, 0, 0, 0, 0;
0, 0, 0, 0 ,0;
0, 0, 0, 0, 0;
0, 0, 0, 0, 0
];
h_con(3) = [
0, 0, 0, 0, 0;
0, 0, -1, 0, 0;
0, 0, 0, 0 ,0;
0, 0, 0, 0, 0;
0, 0, 0, 0, 0
];
f_con = list();
f_con(1) = [    0, 0, 0, 0, 0    ];
f_con(2) = [    0, 0, 0, 1, 0    ];
f_con(3) = [    0, 0, 0, 0, 1    ];
lb_con = [    1, 0, 0    ];
ub_con = [    1, 0, 0    ];
maxcputime = 600;
lb_var = [    -%inf, -%inf, -%inf, -%inf, -%inf    ];
ub_var = [    +%inf, +%inf, +%inf, +%inf, +%inf    ];
[kSoln, kSolnVector, status] = qcqpglob(n, m, h_obj, f_obj, c_obj, h_con, f_con, lb_con, ub_con, lb_var, ub_var, maxcputime);
halt()   // Press return to continue
 
//========= E N D === O F === D E M O =========//
