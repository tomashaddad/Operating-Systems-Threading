#!/bin/sh

if [ "$1" = "help" ]
then
    echo "Usage: $0 N T <input> <output>"
    echo "Randomly selects N words from <input> and writes them to <output> T times."
    exit 1
fi

if [ $# -ne 4 ]
then
    echo "Script requires 4 arguments: N, T, input, output. Only $# arguments provided."
    exit 1
fi

if [ ! -e $3 ]
then
    echo "Error: Input file $3 could not be found."
    exit 1
fi

if [ ! -e $4 ]
then
    touch $3
fi

for i in $(seq 1 $2)
do
    shuf -n $1 $3 >> $4
done
