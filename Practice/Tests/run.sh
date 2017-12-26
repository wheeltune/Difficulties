#!/bin/bash
for n in {30..70}
do
    for i in {0..10}
    do
        p=$(python -c "print $i/100.")
        .././main $n $p
    done
done
