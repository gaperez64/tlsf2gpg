#!/bin/bash
TOUT=gtimeout

for filename in ./examples/*.tlsf; do
    ${TOUT} -k 60 40 ./tlsf2pg.sh "$filename" > "${filename}.pg"
done
