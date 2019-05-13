#!/bin/bash
# AUTHOR: Guillermo A. Perez @ UAntwerp
# DESCRIPTION: A translator from TLSF to generalized parity games encoded
#              in an extended version of the PGSolver game format
# arg1 = the TLSF specification file path

syfcopath="."
ltl2tgbapath="."
tempfolder="./temp"

inputs=$(${syfcopath}/syfco $1 --print-input-signals | sed "s/,//g")
outputs=$(${syfcopath}/syfco $1 --print-output-signals | sed "s/,//g")
specs=$(${syfcopath}/syfco $1 --format ltlxba-decomp)
c=0
while read -r line; do
    c=$((c+1))
    # state-based-acceptance is self-explanatory
    # colored-parity makes every state be in exactly one acceptance set
    # deterministic is self-explanatory
    # complete is self-explanatory
    ${ltl2tgbapath}/ltl2tgba --state-based-acceptance \
        --colored-parity"=max even" \
        --deterministic \
        --complete \
        -f "$line" > "${tempfolder}/temp-spec${c}.hoaf"
done <<< "$specs"
hoafrange=$(seq 1 ${c})
allhoafs=$(printf "${tempfolder}/temp-spec%i.hoaf " ${hoafrange})
./hoafs2gpg --files ${allhoafs} --inputs ${inputs} --outputs ${outputs}
