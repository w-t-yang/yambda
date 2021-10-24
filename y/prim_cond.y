let c_i_0 0
let c_i_1 1
let c_s_0 ""
let c_s_1 "a"
let c_l_0 ()
let c_l_1 (` 1 2 3)

cond c_i_0 (` 1 2 3) c_s_0 2 c_l_0 3 c_i_1 4
cond c_i_1 1 c_i_0 0
cond c_s_1 "str" c_i_0 0
cond c_l_1 (` 1 2 3) c_i_0 0

cond c_i_0 1 c_s_0 2 c_l_0 3

cond (cond 0) 1 c_i_1 2

cond 0 1
cond 1
