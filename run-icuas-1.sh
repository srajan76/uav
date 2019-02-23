cd bin/

for t in `seq 25 25 100`
do 
    for i in `seq 0 1 9`
    do 
        for p in `seq 0.2 0.3 0.8`
        do 
            ./main -f $t-1-10-$i.txt -s 100 -r $p >> 100-$p.txt
        done 
    done 
done  
