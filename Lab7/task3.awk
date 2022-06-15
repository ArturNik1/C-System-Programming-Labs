# BEGIN {FS=","} /spring/ {print $2 "---" $3 "---" $4}

#BEGIN {FS =","} /fun/||/fan/ {cnt++} END {print "count=" cnt}

BEGIN {FS=","} $2 ~ /happy/ {print $2 "---" $3 "---" $4}