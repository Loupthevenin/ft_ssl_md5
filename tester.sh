#!/bin/bash

PROGRAM="./ft_ssl"

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

fail=0
pass=0

run_test() {
	local description="$1"
	local cmd="$2"
	local expected="$3"
	local expected_exit="$4"
	local stdin_data="${5:-}"

	echo -e "\n${YELLOW}[Test]${NC} $description"
	echo "Command: $cmd"

	if [[ -n "$stdin_data" ]]; then
		output=$(echo -e "$stdin_data" | eval "$cmd" 2>&1)
	else
		output=$(eval "$cmd" 2>&1)
	fi
	ret=$?

	# Nettoyage des retours chariots Windows
	output=$(echo "$output" | tr -d '\r')
	expected=$(echo "$expected" | tr -d '\r')

	if [[ "$expected" == "_IGNORE_" ]]; then
		if [[ $ret -eq $expected_exit ]]; then
			echo -e "  ${GREEN}✅ OK${NC} (exit code $ret)"
			((pass++))
		else
			echo -e "  ${RED}❌ FAIL${NC} (exit code $ret, attendu $expected_exit)"
			((fail++))
		fi
	else
		if [[ "$output" == "$expected" && $ret -eq $expected_exit ]]; then
			echo -e "  ${GREEN}✅ OK${NC}"
			((pass++))
		else
			echo -e "  ${RED}❌ FAIL${NC}"
			echo -e "    Exit code: $ret (attendu $expected_exit)"
			echo -e "    Sortie réelle :"
			echo "---------------------------------"
			printf '%s\n' "$output"
			echo "---------------------------------"
			echo -e "    Sortie attendue :"
			echo "---------------------------------"
			printf '%s\n' "$expected"
			echo "---------------------------------"
			((fail++))
		fi
	fi
}

# --------------------------
# DÉFINITION DES TESTS
# --------------------------

test_string() {
	run_test "MD5 -s string" \
		"$PROGRAM md5 -s '42 is nice'" \
		"(stdin)= 0029a98ee90fdb85d70924d44d3c9e75" \
		0

	run_test "MD5 -s string" \
		"$PROGRAM md5 -s ''" \
		"(stdin)= d41d8cd98f00b204e9800998ecf8427e" \
		0

	run_test "MD5 -s string" \
		"$PROGRAM md5 -s 'test'" \
		"(stdin)= 098f6bcd4621d373cade4e832627b4f6" \
		0
}

# Test avec stdin pipe
test_stdin() {
	run_test "MD5 with stdin" \
		"$PROGRAM md5" \
		"(stdin)= 35f1d6de0302e2086a4e472266efb3a9" \
		0 \
		"42 is nice"
}

# Test avec fichier
test_file() {
	# Prépare fichier de test
	echo "And above all," >file
	expected_output="MD5 (file) = 53d53ea94217b259c11a5a2d104ec58a"

	run_test "MD5 with file" \
		"$PROGRAM md5 file" \
		"$expected_output" \
		0
	rm -f file
}

# Test combinaisons, flags -p, -r, etc
test_combined() {
	expected_output='("42 is nice")= 35f1d6de0302e2086a4e472266efb3a9'
	run_test "MD5 -p with stdin" \
		"$PROGRAM md5 -p" \
		"$expected_output" \
		0 \
		"42 is nice"

}

# Test option invalide
test_invalid() {
	run_test "Option invalide" \
		"$PROGRAM md5 -z" \
		"_IGNORE_" \
		1
}
# --------------------------
# RÉSULTATS
# --------------------------
test_string
test_stdin
test_file
test_combined
test_invalid
echo
echo "Résumé : $pass OK, $fail FAIL"
exit $fail
