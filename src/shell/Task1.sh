#!/bin/sh

# shuffle the wordlist with a fixed seed
# shuffling not necessary according to discussion forum
# shuf --random-source=/dev/urandom --output=wlist_match1_shuffled.txt wlist_match1.txt

# find words with 3 or more consecutive letters
# find words that aren't alphanumeric
# invert the output
# pipe into grep that selects words between 3 to 15 characters
# -a required to suppress "binary file matches" warning (?)

grep -a -v -e '\([a-zA-Z]\)\1\{2,\}\|[^a-zA-Z]' wlist_match1_shuffled.txt \
    | grep '^.\{3,15\}$' > wlist_match1_shuffled_cleaned.txt

# sort file
sort wlist_match1_shuffled_cleaned.txt > wlist_match1_shuffled_cleaned_sorted.txt