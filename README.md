# yambda

## Quickstart

- Clone the repository
- `make yambda`
- `./yambda y/examples/hello_world.y`

## Why yambda?

[WIP]

## Basic Concepts

1. Everything is a list.
2. When evaluating a list, the last element will be returned.
    ```
    (1 2 3)   #returns 3
    (` 1 2 3) #returns (1 2 3)
    ```
3. Similar to python, by default, identation must be 4 whitespaces. With proper identation
the next line will be considered as an element and appended to previous list
    ```
    1 2 3
        4 5 6
            7 8
            9
        10
    #is equivalent to
    1 2 3 ( 4 5 6 (7 8) 9 ) 10

    #another example
    def test
        let res 1
        dosomething
        return res
    #is equivalent to
    def test (let res 1) (dosomething) (return res)
    ```
[WIP]

## Keywords

- `let`, defines a variable.
- `def`, defines a lambda function. Use `$1 $2 $3 ...` to reference the arguments
- `ref`, references a symbol.
- `+`, math function sum.
- `*`, math function multiply.
- `\`, math function devide.
- `` ` ``, defines a list. e.g.
    ```
    (` 1 2 3) #returns (1 2 3)
    (1 2 3)   #returns 3
    ```
- `a?`, returns true if the given argument is an atom.
- `=?`, returns true if the given arguments are equal to each other.
- `car`, returns the first item of a list.
- `cdr`, returns the list after the first item.
- `cons`, expects an atom and a list, returns the combined list. e.g.
    ```
    cons 1 (` 2 3) #returns (1 2 3)
    ```
- `cond`, expects a list. Return the (2i+1)th element if the (2i)th element is true.
  ```
  cond 0 x 1 y 1 z #returns y
  cond 0 x 0 y z   #returns z
  cond 1 x 1 y 1 z #returnx x
  ```
- `foreach`, expects a list and a block. Evaluate the block for each of the element in the list.
use `$_` to reference each element.
- `print`, evaluates the given list and print the result to stdout.
- `return`, expects to be used inside any lambda function to return the result without reaching the end.

## Example
```
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
#Expected result: Hello world value of n is 6

test_function n: 1 user: "stranger"
#Expected result: Hello stranger value of n is 7
```

```

```
