let a 1
1

def test-lambda
    let b 2
    let res (+ a b)
    let res (+ res $1 $2)
    res
ref test-lambda

test-lambda (4) 5
12

test-lambda 4 (` 1 2 3)
ERROR

test-lambda (` 1 2 3)
ERROR

def lambda
    let b 2
    let res 0
    foreach $1 :
        let res (+ res $_)
    let res (+ res a b $2)
    res
ref lambda

lambda (1 2 3) 4 5
10

lambda (` 1 2 3) 4 5
13

let l lambda
ref lambda

l (` 1 2 3) 4
13
