#!/usr/bin/env bash

# Check if the file is provided as an argument
if [ -z "$1" ]; then
    echo "Usage: $0 <file_name>"
    exit 1
fi

# Check if the file exists
if [ ! -f "$1" ]; then
    echo "File not found!"
    exit 1
fi

# Initialize sum variable
sum=0

# Read each line of the file
while read -r line; do
    if [[ "$line" =~ ^# ]]; then
        continue
    fi

    # Extract the slot_value using grep and sed
    slot_value=$(echo "$line" | grep -oP '(?<=#)\d{1,2}')
    
    # Add slot_value to sum if it's a valid integer
    if [[ $slot_value -ge 1 && $slot_value -le 20 ]]; then
        sum=$((sum + slot_value))
    fi
    # echo "Total sum of slot values: $sum"
done < "$1"
# SLOT_SUMS=$sum
# Output the sum
# echo "Total sum of slot values: $sum"
echo $sum
