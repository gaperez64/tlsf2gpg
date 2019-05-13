#!/bin/bash
# AUTHOR: Guillermo A. Perez @ UAntwerp
# DESCRIPTION: A translator from TLSF to generalized parity games encoded
#              in an extended version of the PGSolver game format
# arg1 = the TLSF specification file path

syfcopath="."
ltl2tgbapath="."
tempfolder="./temp"

echo "Generating a generalized parity game for the TLSF file $1"
inputs=$(${syfcopath}/syfco $1 --print-input-signals | sed "s/,//g")
outputs=$(${syfcopath}/syfco $1 --print-output-signals | sed "s/,//g")
echo "(Uncontrollable) inputs: ${inputs}"
echo "Outputs: ${outputs}"
specs=$(${syfcopath}/syfco $1 --format ltlxba-decomp)
c=0
while read -r line; do
    c=$((c+1))
    echo "Working on a parity automaton for the LTL formula"
    echo "$line"
    # state-based-acceptance is self-explanatory
    # colored-parity makes every state be in exactly one acceptance set
    # deterministic is self-explanatory
    ${ltl2tgbapath}/ltl2tgba --state-based-acceptance \
        --colored-parity"=max even" \
        --deterministic \
        -f "$line" > "${tempfolder}/temp-spec${c}.hoaf"
done <<< "$specs"
hoafrange=$(seq 1 ${c})
allhoafs=$(printf "${tempfolder}/temp-spec%i.hoaf " ${hoafrange})
echo "Calling hoafs2gpg to read the parity automata and create the game"
./hoafs2gpg --files ${allhoafs} --inputs ${inputs} --outputs ${outputs}
