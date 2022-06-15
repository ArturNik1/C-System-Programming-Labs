#BEGIN {FS=","} /standard/{sum1+=$7;sum2+=$8} END {print sum1/NR " " sum2/NR}

#/bachelor's/ {cnt++} END {print "count=" cnt}

BEGIN {FS =",";print "===================\nSuccess Student List\n===================\n"} {if ($6>=80 || $7>=80 || $8>=90) {
    print $0;
    cnt += 1;
    }
}
END {print cnt}