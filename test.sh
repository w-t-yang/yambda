for t in y/*
do
    echo "-------------------"
    echo "Test file $t"
    cat $t | ./yambda
done
