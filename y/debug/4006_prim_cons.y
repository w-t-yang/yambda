cons
ERROR

cons x
ERROR

cons 1 (1 2 3)
ERROR

cons 1 (` 1 2 3)
(` 1 1 2 3)

cdr (cons 1 (` 2 3 4))
(` 2 3 4)

car (cons 3 (` 1 2 3))
3

let x 1
1

cons x (` 2 3)
(` 1 2 3)
