=? 1 2
0

=? 1 1
1

=? "a" "b"
0

=? "a" "a"
1

let a 1
1

let b 2
2

=? a 1
1

=? 1 a
1

=? a b
0

=?
error

=? 1
error

=? 1 ()
0

=? () ()
1

=? (1 2 3) (1 2 3)
1

=? (` 1 2 3) (` 1 2 3)
1

=? (` 1 2 3) (1 2 3)
0
