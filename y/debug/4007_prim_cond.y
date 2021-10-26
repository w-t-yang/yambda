let c_i_0 0
0

let c_i_1 1
1

let c_s_0 ""
""

let c_s_1 "a"
"a"

let c_l_0 ()
()

let c_l_1 (` 1 2 3)
(` 1 2 3)

cond c_i_0 (` 1 2 3) c_s_0 2 c_l_0 3 c_i_1 4
4

cond c_i_1 1 c_i_0 0
1

cond c_s_1 "str" c_i_0 0
"str"

cond c_l_1 (` 1 2 3) c_i_0 0
(` 1 2 3)

cond c_i_0 1 c_s_0 2 c_l_0 3
NONE

cond (cond 0) 1 c_i_1 2
2

cond 1
1

cond 0 1
NONE

cond 0 1 "abc"
"abc"

cond 0 1 "abc" "def"
"def"
