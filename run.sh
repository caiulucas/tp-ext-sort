#!/bin/bash

# Function to run the code n times
run_n_times() {
    local command="$1"
    local n="$2"
    
    for ((i = 1; i <= n; i++)); do
        echo "Running iteration $i"
        eval "$command"
        sleep 1
    done
}

# Check if both code and n arguments are provided
if [[ $# -ne 2 ]]; then
    echo "Usage: $0 <code/command> <number of times>"
    exit 1
fi

# Get the code from the first argument
user_code="$1"

# Get the number of times from the second argument
n_times="$2"

# Validate n_times input to ensure it's a positive integer
if [[ ! $n_times =~ ^[1-9][0-9]*$ ]]; then
    echo "Invalid input. Please enter a positive integer for the number of times to run the code."
    exit 1
fi

# Call the function with the user-provided code and number of iterations
run_n_times "./ordena $user_code" "$n_times"
