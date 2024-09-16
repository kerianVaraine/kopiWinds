#!/bin/sh
file="csvTest.csv"
current_date=$(date +%Y-%m-%d)

cp "$file" "$current_date_$file"
