make ydebug

for t in y/debug/*
do
    echo "-------------------"
    echo "Test file $t"
    ./ydebug $t
done
