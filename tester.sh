#!/bin/bash

PROGRAM="./ft_ssl"

# Format de chaque test :
# "description|commande|expected_output|string|expected_exit_code"
# Le "string" pour indiquer si expected_output est un texte brut ou un fichier (ici toujours 'string')

tests=(
	"Test md5 -s string|$PROGRAM md5 -s 'hello'|'5d41402abc4b2a76b9719d911017c592'|string|0"
	"Test invalid option|$PROGRAM md5 -z|'ft_ssl: illegal option '\''z'\'''|string|1"
	"Test no args (stdin)|echo 'test' | $PROGRAM md5|'098f6bcd4621d373cade4e832627b4f6'|string|0"
)

fail_count=0
pass_count=0

for test_case in "${tests[@]}"; do
	IFS='|' read -r description cmd expected exit_type expected_exit <<<"$test_case"

	echo "Running: $description"
	output=$(eval "$cmd" 2>&1)
	ret=$?

	# Trim spaces/newlines for comparison (optional)
	output=$(echo -n "$output" | tr -d '\r\n')
	expected=$(echo -n "$expected" | tr -d '\r\n')

	if [[ "$output" == "$expected" && "$ret" -eq "$expected_exit" ]]; then
		echo "✅ Passed"
		((pass_count++))
	else
		echo "❌ Failed"
		echo "Command:"
		echo "$cmd"
		echo "Exit code: got $ret, expected $expected_exit"
		echo "Output:"
		echo "$output"
		echo "Expected:"
		echo "$expected"
		((fail_count++))
	fi
	echo "-----------------------------------"
done

echo "Summary: $pass_count passed, $fail_count failed."

exit $fail_count
