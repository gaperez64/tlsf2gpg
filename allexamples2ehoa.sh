#!/bin/bash
TOUT=timeout

for filename in ./examples/*.tlsf; do
    ${TOUT} -k 5 10 ./tlsf2ehoa.sh "$filename"
done
