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
	input_str="pity those that aren't following baerista on spotify."
	expected_output="MD5 (\"$input_str\") = a3c990a1964705d9bf0e602f44572f5f"
	run_test "MD5 -s string" \
		"$PROGRAM md5 -s \"$input_str\"" \
		"$expected_output" \
		0

	expected_output=$'505211af1b713cc6fb137044df047c5d "test reverse string"'
	run_test "MD5 -r -s string" \
		"$PROGRAM md5 -r -s 'test reverse string'" \
		"$expected_output" \
		0

	run_test "MD5 -s sans argument" \
		"$PROGRAM md5 -s" \
		"ft_ssl: option requires an argument -- s" \
		1

	expected_output=$'MD5 ("") = d41d8cd98f00b204e9800998ecf8427e'
	run_test "MD5 -s argument vide" \
		"$PROGRAM md5 -s ''" \
		"$expected_output" \
		0

	expected_output=$'SHA256 ("42 is nice") = b7e44c7a40c5f80139f0a50f3650fb2bd8d00b0d24667c4c2ca32c88e13b758f'
	run_test "sha256 -s string" \
		"$PROGRAM sha256 -s '42 is nice'" \
		"$expected_output" \
		0

	input_str="pity those that aren't following baerista on spotify."
	expected_output="SHA256 (\"$input_str\") = 7838c25c9debff86c584245d67b429186d3850c89da31c0b49b8d0380a3e14bf"
	run_test "sha256 -s string" \
		"$PROGRAM sha256 -s \"$input_str\"" \
		"$expected_output" \
		0

	expected_output=$'98f43bc794778f436c1269798b6cf87bcba761cedadfb9f60454bcc163554e5c "test reverse string"'
	run_test "SHA256 -r -s string" \
		"$PROGRAM sha256 -r -s 'test reverse string'" \
		"$expected_output" \
		0

	run_test "SHA256 -s sans argument" \
		"$PROGRAM sha256 -s" \
		"ft_ssl: option requires an argument -- s" \
		1

	expected_output=$'SHA256 ("") = e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855'
	run_test "SHA256 -s argument vide" \
		"$PROGRAM sha256 -s ''" \
		"$expected_output" \
		0
}

# Test avec stdin pipe
test_stdin() {

	run_test "MD5 with stdin" \
		"$PROGRAM md5" \
		"(stdin)= 35f1d6de0302e2086a4e472266efb3a9" \
		0 \
		"42 is nice"

	run_test "MD5 with stdin" \
		"$PROGRAM md5" \
		"(stdin)= d8e8fca2dc0f896fd7cb4cb0031ba249" \
		0 \
		"test"

	run_test "MD5 with stdin -qr" \
		"$PROGRAM md5 -qr" \
		"e20c3b973f63482a778f3fd1869b7f25" \
		0 \
		"Pity the living."

	run_test "MD5 with stdin -q -r" \
		"$PROGRAM md5 -q -r" \
		"e20c3b973f63482a778f3fd1869b7f25" \
		0 \
		"Pity the living."

	run_test "MD5 with stdin -r -q" \
		"$PROGRAM md5 -r -q" \
		"e20c3b973f63482a778f3fd1869b7f25" \
		0 \
		"Pity the living."

	run_test "MD5 with stdin -rq" \
		"$PROGRAM md5 -rq" \
		"e20c3b973f63482a778f3fd1869b7f25" \
		0 \
		"Pity the living."

	expected_output='("42 is nice")= 35f1d6de0302e2086a4e472266efb3a9'
	run_test "MD5 -p with stdin" \
		"$PROGRAM md5 -p" \
		"$expected_output" \
		0 \
		"42 is nice"

	run_test "SHA256 with stdin" \
		"$PROGRAM sha256" \
		"(stdin)= a5482539287a4069ebd3eb45a13a47b1968316c442a7e69bc6b9c100b101d65d" \
		0 \
		"42 is nice"

	run_test "SHA256 with stdin" \
		"$PROGRAM sha256" \
		"(stdin)= f2ca1bb6c7e907d06dafe4687e579fce76b37e4e93b7605022da52e6ccc26fd2" \
		0 \
		"test"

	run_test "SHA256 with stdin -qr" \
		"$PROGRAM sha256 -qr" \
		"40133cfe543247c1cae0ffb0003c1179ce9fb0046bee19f9fca167380643ba45" \
		0 \
		"Pity the living."

	run_test "SHA256 with stdin -q -r" \
		"$PROGRAM sha256 -q -r" \
		"40133cfe543247c1cae0ffb0003c1179ce9fb0046bee19f9fca167380643ba45" \
		0 \
		"Pity the living."

	run_test "SHA256 with stdin -r -q" \
		"$PROGRAM sha256 -r -q" \
		"40133cfe543247c1cae0ffb0003c1179ce9fb0046bee19f9fca167380643ba45" \
		0 \
		"Pity the living."

	run_test "SHA256 with stdin -rq" \
		"$PROGRAM sha256 -rq" \
		"40133cfe543247c1cae0ffb0003c1179ce9fb0046bee19f9fca167380643ba45" \
		0 \
		"Pity the living."

	expected_output='("42 is nice")= a5482539287a4069ebd3eb45a13a47b1968316c442a7e69bc6b9c100b101d65d'
	run_test "SHA256 -p with stdin" \
		"$PROGRAM sha256 -p" \
		"$expected_output" \
		0 \
		"42 is nice"
}

# Test avec fichier
test_file() {
	echo "And above all," >file
	expected_output="MD5 (file) = 53d53ea94217b259c11a5a2d104ec58a"
	expected_output_r="53d53ea94217b259c11a5a2d104ec58a file"

	run_test "MD5 with file" \
		"$PROGRAM md5 file" \
		"$expected_output" \
		0

	run_test "MD5 with file" \
		"$PROGRAM md5 -r file" \
		"$expected_output_r" \
		0

	run_test "MD5 no file" \
		"$PROGRAM md5 noexist" \
		"ft_ssl: md5: noexist: No such file or directory" \
		1

	expected_output="SHA256 (file) = f9eb9a5a063eb386a18525c074e1065c316ec434f911e0d7d59ba2d9fd134705"
	expected_output_r="f9eb9a5a063eb386a18525c074e1065c316ec434f911e0d7d59ba2d9fd134705 file"

	run_test "SHA256 with file" \
		"$PROGRAM sha256 file" \
		"$expected_output" \
		0

	run_test "SHA256 with file" \
		"$PROGRAM sha256 -r file" \
		"$expected_output_r" \
		0

	run_test "SHA256 no file" \
		"$PROGRAM sha256 noexist" \
		"ft_ssl: sha256: noexist: No such file or directory" \
		1
}

# Test combinaisons, flags -p, -r, etc
test_combined() {
	expected_output=$'("be sure to handle edge cases carefully")= 3553dc7dc5963b583c056d1b9fa3349c\nMD5 (file) = 53d53ea94217b259c11a5a2d104ec58a'
	run_test "MD5 -p with stdin + file" \
		"$PROGRAM md5 -p file" \
		"$expected_output" \
		0 \
		"be sure to handle edge cases carefully"

	run_test "MD5 with stdin + file" \
		"$PROGRAM md5 file" \
		"MD5 (file) = 53d53ea94217b259c11a5a2d104ec58a" \
		0 \
		"some of this will not make sense at first"

	expected_output=$'("but eventually you will understand")= dcdd84e0f635694d2a943fa8d3905281\n53d53ea94217b259c11a5a2d104ec58a file'
	run_test "MD5 -p -r + file" \
		"$PROGRAM md5 -p -r file" \
		"$expected_output" \
		0 \
		"but eventually you will understand"

	run_test "MD5 -r -p + file" \
		"$PROGRAM md5 -r -p file" \
		"$expected_output" \
		0 \
		"but eventually you will understand"

	run_test "MD5 -rp + file" \
		"$PROGRAM md5 -rp file" \
		"$expected_output" \
		0 \
		"but eventually you will understand"

	run_test "MD5 -pr + file" \
		"$PROGRAM md5 -rp file" \
		"$expected_output" \
		0 \
		"but eventually you will understand"

	expected_output=$'("GL HF let\'s go")= d1e3cc342b6da09480b27ec57ff243e2\nMD5 ("foo") = acbd18db4cc2f85cedef654fccc4a4d8\nMD5 (file) = 53d53ea94217b259c11a5a2d104ec58a'
	run_test "MD5 with stdin -p + string + file" \
		"$PROGRAM md5 -p -s 'foo' file" \
		"$expected_output" \
		0 \
		"GL HF let's go"

	expected_output=$'("one more thing")= a0bd1876c6f011dd50fae52827f445f5\nacbd18db4cc2f85cedef654fccc4a4d8 "foo"\n53d53ea94217b259c11a5a2d104ec58a file\nft_ssl: md5: -s: No such file or directory\nft_ssl: md5: bar: No such file or directory'
	run_test "MD5 with stdin -r -p + string + file + string" \
		"$PROGRAM md5 -r -p -s 'foo' file -s 'bar'" \
		"$expected_output" \
		1 \
		"one more thing"

	expected_output=$'just to be extra clear\n3ba35f1ea0d170cb3b9a752e3360286c\nacbd18db4cc2f85cedef654fccc4a4d8\n53d53ea94217b259c11a5a2d104ec58a'
	run_test "MD5 whith all flags" \
		"$PROGRAM md5 -r -q -p -s 'foo' file" \
		"$expected_output" \
		0 \
		"just to be extra clear"

	expected_output=$'just to be extra clear\n3ba35f1ea0d170cb3b9a752e3360286c\nacbd18db4cc2f85cedef654fccc4a4d8\n53d53ea94217b259c11a5a2d104ec58a'
	run_test "MD5 whith all flags" \
		"$PROGRAM md5 -rqps 'foo' file" \
		"$expected_output" \
		0 \
		"just to be extra clear"

	expected_output=$'("be sure to handle edge cases carefully")= ef9241f878a1da676104a81239792a2817bc0390a427ca20bad1a59030fd20c2\nSHA256 (file) = f9eb9a5a063eb386a18525c074e1065c316ec434f911e0d7d59ba2d9fd134705'
	run_test "SHA256 -p with stdin + file" \
		"$PROGRAM sha256 -p file" \
		"$expected_output" \
		0 \
		"be sure to handle edge cases carefully"

	run_test "SHA256 with stdin + file" \
		"$PROGRAM sha256 file" \
		"SHA256 (file) = f9eb9a5a063eb386a18525c074e1065c316ec434f911e0d7d59ba2d9fd134705" \
		0 \
		"some of this will not make sense at first"

	expected_output=$'("but eventually you will understand")= 43da940057fd3b7453ee91b3a056a41343e6f0bce315570ed27e06c993a539da\nf9eb9a5a063eb386a18525c074e1065c316ec434f911e0d7d59ba2d9fd134705 file'
	run_test "SHA256 -p -r + file" \
		"$PROGRAM sha256 -p -r file" \
		"$expected_output" \
		0 \
		"but eventually you will understand"

	run_test "SHA256 -r -p + file" \
		"$PROGRAM sha256 -r -p file" \
		"$expected_output" \
		0 \
		"but eventually you will understand"

	run_test "SHA256 -rp + file" \
		"$PROGRAM sha256 -rp file" \
		"$expected_output" \
		0 \
		"but eventually you will understand"

	run_test "SHA256 -pr + file" \
		"$PROGRAM sha256 -rp file" \
		"$expected_output" \
		0 \
		"but eventually you will understand"

	expected_output=$'("GL HF let\'s go")= f33201f3d70c9dccccec022e2ff0df2006e016f153f600407917d14955fbec22\nSHA256 ("foo") = 2c26b46b68ffc68ff99b453c1d30413413422d706483bfa0f98a5e886266e7ae\nSHA256 (file) = f9eb9a5a063eb386a18525c074e1065c316ec434f911e0d7d59ba2d9fd134705'

	run_test "SHA256 with stdin -p + string + file" \
		"$PROGRAM sha256 -p -s 'foo' file" \
		"$expected_output" \
		0 \
		"GL HF let's go"

	expected_output=$'("one more thing")= 720bbf63077e0bea3b70c87954123daa6fcf32f973f4d646622bd016b140ec75\n2c26b46b68ffc68ff99b453c1d30413413422d706483bfa0f98a5e886266e7ae "foo"\nf9eb9a5a063eb386a18525c074e1065c316ec434f911e0d7d59ba2d9fd134705 file\nft_ssl: sha256: -s: No such file or directory\nft_ssl: sha256: bar: No such file or directory'
	run_test "SHA256 with stdin -r -p + string + file + string" \
		"$PROGRAM sha256 -r -p -s 'foo' file -s 'bar'" \
		"$expected_output" \
		1 \
		"one more thing"

	expected_output=$'just to be extra clear\n41c3da28172faf72bb777d6a428b6d801427d02513c56cd9e3672f44383f8eee\n2c26b46b68ffc68ff99b453c1d30413413422d706483bfa0f98a5e886266e7ae\nf9eb9a5a063eb386a18525c074e1065c316ec434f911e0d7d59ba2d9fd134705'
	run_test "SHA256 whith all flags" \
		"$PROGRAM sha256 -r -q -p -s 'foo' file" \
		"$expected_output" \
		0 \
		"just to be extra clear"

	expected_output=$'just to be extra clear\n41c3da28172faf72bb777d6a428b6d801427d02513c56cd9e3672f44383f8eee\n2c26b46b68ffc68ff99b453c1d30413413422d706483bfa0f98a5e886266e7ae\nf9eb9a5a063eb386a18525c074e1065c316ec434f911e0d7d59ba2d9fd134705'
	run_test "SHA256 whith all flags" \
		"$PROGRAM sha256 -rqps 'foo' file" \
		"$expected_output" \
		0 \
		"just to be extra clear"
}

# Test option invalide
test_invalid() {
	run_test "Option invalide" \
		"$PROGRAM md5 -z" \
		"_IGNORE_" \
		1

	run_test "Option invalide" \
		"$PROGRAM sha256 -z" \
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

rm -f file

echo
echo "Résumé : $pass OK, $fail FAIL"
exit $fail
