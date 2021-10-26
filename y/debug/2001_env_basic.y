ref let
ref let

let a 1
1


ref a
1

a
1

let b 2
2

ref b
2

let x 1 2 3
error

let "a" 1
error

let c a
1

ref c
1

c
1

a b c
1

def a 1
error

d
error

ref d
error

let e (+ 1 2 3)
6

ref e
6

e
6

let f (` 2 3 4)
(` 2 3 4)

ref f
(` 2 3 4)

f
(` 2 3 4)
