#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <number-of-processes> <matrix-size>"
    exit 1
fi

num_processes=$1
matrix_size=$2

if ! [[ "$num_processes" =~ ^[1-9][0-9]*$ ]] ||
   ! [[ "$matrix_size" =~ ^[1-9][0-9]*$ ]]; then
    echo "Error: both arguments must be positive integers."
    exit 1
fi

output_dir="data/bench-nice-${matrix_size}"

echo "$(date)"
echo "Starting ${num_processes} concurrent ${matrix_size}x${matrix_size} matrix multiplications"

mkdir -p "$output_dir"

pids=()

halfway=$((num_processes / 2))

for i in $(seq 1 "$num_processes")
do
    if [ "$i" -le "$halfway" ]; then 
        priority=0
    else
        priority=7
    fi

    /usr/bin/time \
        -f "CPU: %P" \
        -o "${output_dir}/mm-${i}-cpu.out" \
        nice -n "$priority" ./bench "$matrix_size" "$matrix_size" "$matrix_size" 0 \
        > "${output_dir}/mm-${i}.out" &

    pids+=($!)

    echo "Started process ${i}"
done

echo "Waiting for matrix multiplications"

for pid in "${pids[@]}"
do
    wait "$pid"
done

echo "Benchmark complete"
echo "$(date)"