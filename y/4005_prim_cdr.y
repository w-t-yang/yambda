cdr 1 2 3
(cdr 1 2 3)
cdr (1 2 3)
cdr "a" "b" "c"

let c 3
cdr 1 2 c

cdr (1 2 3) (4 5 6) 7

cdr (` 1 2 3) 4 5
cdr 4 5 (` 1 2 3)
cdr 4 5 (1 2 3)

cdr (` 1 2 3)
(cdr (` 1 2 3))
(cdr (` 1 2 3) 4 5)
(cdr 4 5 (` 1 2 3))
car (cdr (` 1 2 3))
cdr (cdr (` 1 2 3 4))

cdr
cdr ()
