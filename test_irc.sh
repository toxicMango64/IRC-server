#!/bin/bash

port=$1
password=$2

output=$(./ircserv "$port" "$password" 2>&1)
exit_code=$?

if [[ $exit_code -eq 0 && "$output" == "Server running" ]]; then
    echo -e "\033[32mPASS\033[0m: Port: $port, Password: '$password'"
    echo "Output: $output"  # Added output display for passing cases
    exit 0
else
    echo -e "\033[31mFAIL\033[0m: Port: $port, Password: '$password'"
    echo "Output: $output"
    echo "Exit code: $exit_code"
    exit 1
fi

# #!/bin/bash

# # Initialize counters
# passed=0
# failed=0

# # Function to run a single test case
# run_test() {
#     local port="$1"
#     local password="$2"
#     local expected_output="$3"
#     local expected_error="$4"

#     # Run the server and capture output
#     output=$(./ircserv "$port" "$password" 2>&1)
#     exit_code=$?

#     # Check results
#     if [[ $exit_code -eq 0 && "$output" == *"$expected_output"* ]]; then
#         echo -e "\033[1;32m[ PASS ]\033[0m Port: $port | Password: '$password'"
#         ((passed++))
#     elif [[ $exit_code -ne 0 && "$output" == *"$expected_error"* ]]; then
#         echo -e "\033[1;32m[ PASS ]\033[0m Port: $port | Password: '$password' (Expected error)"
#         ((passed++))
#     else
#         echo -e "\033[1;31m[ FAIL ]\033[0m Port: $port | Password: '$password'"
#         echo "Expected: ${expected_output:-$expected_error}"
#         echo "Got:      $output"
#         ((failed++))
#     fi
# }

# # Main loop for test cases
# echo "=== Running Test Cases ==="
# while [[ $# -gt 0 ]]; do
#     run_test "$1" "$2" "$3" "$4"
#     shift 4  # Move to the next test case
# done

# # Summary
# echo -e "\n=== Results ==="
# echo -e "\033[1;32mPassed: $passed\033[0m"
# echo -e "\033[1;31mFailed: $failed\033[0m"
# exit $((failed > 0))

