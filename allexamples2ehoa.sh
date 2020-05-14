#!/bin/bash
TOUT=timeout

for filename in ./examples/*.tlsf; do
    ${TOUT} -k 600 540 ./tlsf2ehoa.sh "$filename"
done
