rm stdlib.out 2>/dev/null
for f in $(find stdlib -type f); do
	content=$(cat "$f" \
		| sed 's/%/%%/g' \
		| sed 's/"/\\"/g' \
		| sed 's/\\/\\\\/g' \
		| sed 's/^/"/g' \
		| sed 's/$/\\\\n"/g' \
		| sed 's/\t//g')
	printf "$content\n" >> stdlib.out
done
