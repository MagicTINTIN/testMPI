#!/bin/env bash
echo -n "Prefix: "
read PREF
echo -n "Start: "
read START
echo -n "End: "
read END
for ((i = START; i <= END; i++)); do
    echo $PREF.$i
done
echo -n "Start ? (y/N):"
read startProg
if [[ $startProg == "y" ]]; then
    echo "Starting..."
    for ((i = START; i <= END; i++)); do
        ssh-copy-id $PREF.$i
    done
    echo "END. Exiting..."
else
    echo "Cancelled. Exiting..."
fi
