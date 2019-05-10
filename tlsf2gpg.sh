#!/bin/bash
# AUTHOR: Guillermo A. Perez @ UAntwerp
# DESCRIPTION: A translator from TLSF to generalized parity games encoded
#              in an extended version of the PGSolver game format
# arg1 = the TLSF specification file path

syfcopath="."
ltl2tgbapath="."
tempfolder="./temp"

echo "Generating a generalized parity game for the TLSF file $1"
inputs=$(${syfcopath}/syfco $1 --print-input-signals)
outputs=$(${syfcopath}/syfco $1 --print-output-signals)
echo "(Uncontrollable) inputs: ${inputs}"
echo "Outputs: ${outputs}"
specs=$(${syfcopath}/syfco $1 --format acacia-specs)
c=1
singletonspec=0
while read -r line; do
    if [[ ! "$line" = "[spec_unit"* ]]; then
        if [ "$singletonspec" -eq "1" ]; then
            echo "There is more there one line per spec!"
            exit
        fi
        noscolon=$(echo "$line" | sed "s/;*$//g")
        echo "Working on a parity automaton for the (Wring format) LTL"
        echo "$noscolon"
        # state-based-acceptance is self-explanatory
        # colored-parity (-p) makes every state be in exactly one acceptance set
        # deterministic is self-explanatory
        ${ltl2tgbapath}/ltl2tgba --state-based-acceptance \
            --colored-parity"=max even" \
            --deterministic \
            -f "$noscolon" > "${tempfolder}/temp-spec${c}.hoa"
        c=$((c+1))
        singletonspec=1
    else
        singletonspec=0
    fi
done <<< "$specs"
