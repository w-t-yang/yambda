return 1
return 1

1 return 2 3 4
4

1 (return 2) 3 4
return 2

(1 (2 (3 (return 4 0) 5) 6) 7)
return 4 0


let a 1
1

def test
    cond a (return "true") "false"
ref test

test
"true"

let a 0
0

test
"false"
