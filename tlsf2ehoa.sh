#!/bin/bash
# AUTHOR: Guillermo A. Perez @ UAntwerp
# DESCRIPTION: A translator from TLSF to parity games encoded
#              in an extended version of HOA automata format
# arg1 = the TLSF specification file path

if [ ! -f $1 ]; then
    echo "Please provide a TLSF file as argument!"
    return 1
fi

syfcopath="."
ltl2tgbapath="."

# syfco will change propositions to lower case when using
# the ltlxba format, so we do the same here
inputs=$(${syfcopath}/syfco $1 --print-input-signals \
         | sed "s/,//g" \
         | tr "[:upper:]" "[:lower:]")
outputs=$(${syfcopath}/syfco $1 --print-output-signals \
          | sed "s/,//g" \
          | tr "[:upper:]" "[:lower:]")
spec=$(${syfcopath}/syfco $1 --format ltlxba)
# colored-parity makes every state be in exactly one acceptance set
# deterministic is self-explanatory
# complete is self-explanatory
${ltl2tgbapath}/ltl2tgba \
    --colored-parity"=max even" \
    --deterministic \
    --complete \
    -f "$spec" > "$1.ehoa"
# we now have to mark the controllable APs, using their indices,
# so we start by recovering the list of APs in the hoa file
allAPs=$(sed -n "s/^AP: [0-9]* \(.*\)$/\1/p" "$1.ehoa" | sed "s/\"//g")
outputIndices=$(python getindices.py "${outputs}" "${allAPs}")
sed -i "s/^\(AP:.*\)$/\1\ncontrollable-AP: ${outputIndices}/g" "$1.ehoa"
