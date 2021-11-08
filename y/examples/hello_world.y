let x "Hello"

let if cond
let eq? =?

def test_function
    let n $1
    foreach (` 1 2 3)
        let n =: (+ n $_)

    print "Number n:" n

    if (eq? n 6)
        print (cons x (` $2 "value of n is 6"))
        otherwise, print x $2 "value of n is" n

test_function 0 "world"
test_function n: 1 user: "stranger"
