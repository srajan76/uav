cd bin/

for t in `seq 25 25 100`
do 
    for i in `seq 0 1 9`
    do 
        for s in `seq 20 20 100`
        do 
            ./main -f $t-1-10-$i.txt -s $s >> $s.txt
        done 
    done 
done  
