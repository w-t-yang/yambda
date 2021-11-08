1 2 3# 4 5 6
3


(1 2 3) 4 # 5
    6 (` 7 8)
(` 7 8)

print "The next test is expected to fail. Explanation is in the script comments."
1

# The test below is expected to fail
# Because once it has an unclosed parenthesis,
# the program will try to read until the end of file
# TODO: find a fix for this
(1 2 3 # 4)
ERROR
