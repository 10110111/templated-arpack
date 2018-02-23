#!/bin/bash -e

indent() { sed 's@^.@    \0@' "$@"; }

dirName=src-before-cat
mkdir -pv "$dirName"
tmpfile="$(mktemp)"
cd src
for file in *.c; do
    outfile="../$dirName/$file"
    cp -v "$file" "$outfile"

    # Move the (supposedly constant) static variables from global scope into function scope
    sed -n -e '/\/\* Table of constant values \*\//,/^$/p' -e '/^static /,/^$/p' "$outfile" | indent > "$tmpfile"
    sed -e '/\/\* Table of constant values \*\//,/^$/d' -e '/^static /,/^$/d' -e '/^{/q' "$outfile" > "$outfile.edit"
    cat "$tmpfile" >> "$outfile.edit"
    sed '1,/^{/d' "$outfile" >> "$outfile.edit"
    mv "$outfile.edit" "$outfile"
done
rm "$tmpfile"

cd ..
srcFile=templated-arpack.hxx
cp includes-begin.h "$srcFile"
cat >> "$srcFile" <<'EOF'
template<typename Real>
class ARPACK
{
EOF
indent funcs.cpp >> "$srcFile"
echo -e "public:\n    // Edited ARPACK functions\n" >> "$srcFile"
cat "$dirName"/*.c | sed -e 's@\<char *\*@const char* @g' \
                         -e 's@\<\(doublereal\|integer\|char\) *\*@\1*__restrict__ @g' \
                         -e 's@\<TRUE_\>@ true @g' \
                         -e 's@\<FALSE_\>@ false @g' \
                         -e '/#ifdef __cplusplus/,/#endif$/d' \
                         -e '/#include "f2c.h"/d' \
                         -e '/^ *\/\* Builtin functions \*\/$/,/^$/d' \
                         -e '/ \+extern\> .*;$/d' \
                         -e '/ \+extern\> .*[^;]$/,/;$/d' \
                         -e 's@\<doublereal\>@Real@g' \
                         | indent >> "$srcFile"
echo -e '};\n\n#endif // include guard' >> "$srcFile"
