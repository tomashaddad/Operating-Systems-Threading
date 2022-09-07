#!/bin/sh

echo "Current directory: ${PWD}"

# $# is the number of arguments supplied
if [ $# -ne 2 ]
then
    echo "Usage: $0 <input> <output>"
    echo "Exiting ..."
    exit 1
fi

# Check if file exists
if [ ! -e $1 ]
then
    echo "Error: $1 could not be found."
    exit 1
fi

echo "Note that initial shuffling is disabled by default."

# shuffle the wordlist with a fixed seed
# shuffling not necessary according to discussion forum
# shuf --random-source=/dev/urandom $1

echo "Removing words with: 
    - more than 3 consecutive letters
    - non-alphanumeric characters
    - less than 3 characters
    - more than 15 characters
    - duplicates"

echo "Using sort to sort the output on the third letter onwards ..."

grep -a -v -e '\([a-zA-Z]\)\1\{2,\}\|[^a-zA-Z]' $1 \
    | grep '^.\{3,15\}$' \
    | awk '!seen[$0]++' \
    | sort -k1.3 > $2

echo "Operation complete. Exiting ..."

# | sort -u \
# the above could replace the awk line but ruins consistency,
# see report for rationale