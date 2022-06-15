# BEGIN{
#     for(i=0; i<ARGC; ++i){
#         printf "ARGV[%d]=\"%s\"\n",i,ARGV[i]
#     }
# }

# BEGIN{FS=",";OFS=" | "} {print $4,$3,$2}

#BEGIN {FS=","} $2>=1970 {print "---------------------------------\nActor Name: " $4 "\n" "Movie Name: " $5}

BEGIN {FS=","} $1==50,$1==70 {print $5}