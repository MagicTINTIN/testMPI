#!/usr/bin/env bash

# Initialize sum to 0
sum=0

# Read the file line by line
while IFS= read -r $1; do
  # Extract the slot_value using pattern matching
  slot_value=$(echo "$line" | grep -oP '(?<=#)\d+')

  # Add the slot_value to the sum
  if [[ ! -z "$slot_value" ]]; then
    sum=$((sum + slot_value))
  fi
done < "$1"  # First argument is the file name

# Output the total sum
echo "Total sum of slot values: $sum"