#!/usr/bin/env bash

set -e
set -u

BIN=$1
EXPERIMENT=$2 # this is the csv file that contains the experiments to run
PREFIX="====>"

echo "proof_system,base_field,m,n,prover_time,verifier_time,proof_size"

sed 1d "$EXPERIMENT" | while IFS=, read -r proof_system base_field _ m n; do
  if [[ "$proof_system" == "limbo" ]]; then
    continue
  fi
  out=$($BIN --proof_system "$proof_system" --base_field "$base_field" -m "$m" -n "$n" | grep "$PREFIX" | sed "s/$PREFIX//")
  echo "$proof_system,$base_field,$m,$n,$out"
done
