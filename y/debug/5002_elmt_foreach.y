foreach
ERROR

foreach (1 2 3 ) 4
4

foreach (` 1 2 3)
ERROR

foreach (` 1 2 3) 4
4

let x 0
0

let y 1
1

let x =: (+ y x)
1

x
1

let a 0
0

foreach (` 1 2 3)
    let a =: (+ a $_)
6

a
6

foreach (1 2 3)
    let a (+ a $_)
9

a
9

foreach NONE (+ 1 2 3)
6
