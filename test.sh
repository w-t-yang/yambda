make ydebug

for t in y/*
do
    echo "-------------------"
    echo "Test file $t"
    ./ydebug $t
done
