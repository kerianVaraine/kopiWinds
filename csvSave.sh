#!/bin/sh
file="csvTest.csv"
current_date=$(date +%Y-%m-%d)

cp "$file" "csv/${current_date}_$file"
