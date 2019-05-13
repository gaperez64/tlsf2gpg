#!/bin/bash
TOUT=gtimeout

for filename in ./examples/*.tlsf; do
    ${TOUT} -k 60 40 ./tlsf2gpg.sh "$filename" > "${filename}.gpg"
done
