#{print $0}       
#{print $3 "  " $4}
#/a/ {print $2 , $4}
#{print $1,$4,$3,$2}
#/English/ {cnt++} END {print "count=" cnt}
$4>87 {print $0}