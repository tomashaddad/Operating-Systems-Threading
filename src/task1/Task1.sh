#!/bin/sh

echo "Current directory: ${PWD}"

# $# is the number of arguments supplied
if [ $# -ne 1 ]
then
    echo "Usage: $0 <textfile>"
    echo "Assumes text file is in the same directory as script."
    echo "Exiting ..."
    exit 1
fi

name=${1%%.*}
extension=${1#*.}
output="output"
prefix="$output/$name"

if [ $extension != "txt" ]
then
    echo "Error: $1 is not a text file"
    exit 1
fi

# Check if file exists
if [ ! -e $1 ]
then
    echo "Error: $1 could not be found."
    exit 1
fi

if [ -d ${output} ]
then
    echo "Files will be placed in existing output directory:
    ${PWD}/output"
else
    echo "An output folder does not exist in this directory. Creating ..."
    mkdir output
fi

echo "Note that initial shuffling is disabled by default."

# shuffle the wordlist with a fixed seed
# shuffling not necessary according to discussion forum
# shuf --random-source=/dev/urandom --output=wlist_match1_shuffled.txt wlist_match1.txt

# find words with 3 or more consecutive letters
# find words that aren't alphanumeric
# invert the output
# pipe into grep that selects words between 3 to 15 characters
# -a required to suppress "binary file matches" warning (?)

echo "Using grep to remove words with: 
    - more than 3 consecutive letters
    - non-alphanumeric characters
    - less than 3 characters
    - more than 15 characters"

grep -a -v -e '\([a-zA-Z]\)\1\{2,\}\|[^a-zA-Z]' $1 \
    | grep '^.\{3,15\}$' \
    | sort \
    | uniq > ${prefix}_cleaned_sorted_uniq.txt

# Uncomment to examine outputs individually

# echo "Sorting file, which is required for uniq to remove duplicate words."

# sort ${prefix}_cleaned.txt > ${prefix}_cleaned_sorted.txt

# echo "Removing duplicate words."

# uniq ${prefix}_cleaned_sorted.txt > ${prefix}_cleaned_sorted_uniq.txt

# echo "Shell script complete. Output files can be found in .${output}/ folder."