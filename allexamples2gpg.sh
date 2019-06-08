#!/bin/bash
TOUT=gtimeout

for filename in ./examples/*.tlsf; do
    ${TOUT} -k 600 540 ./tlsf2gpg.sh "$filename" > "${filename}.gpg"
done
