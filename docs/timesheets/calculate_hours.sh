#!/bin/bash

# Check if a file name was provided as an argument
if [ -z "$1" ]; then
  echo "Usage: $0 <csv_file>"
  exit 1
fi

# Initialize sum variable
total_hours=0

# Read the CSV file provided as an argument
while IFS=',' read -r date time duration activity
do
  # Skip the header line (assuming the header has "Duration" as a column name)
  if [[ "$duration" == "Duration(in hours)" ]]; then
    continue
  fi
  
  # Add the duration to the total hours, removing any leading/trailing spaces
  total_hours=$(echo "$total_hours + $duration" | bc)
done < "$1"

# Print the total sum of hours
echo "Total hours: $total_hours"
