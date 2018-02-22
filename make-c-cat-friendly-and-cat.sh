#!/bin/bash -e

dirName=src-before-cat
mkdir -pv "$dirName"
tmpfile="$(mktemp)"
cd src
for file in *.c; do
    outfile="../$dirName/$file"
    cp -v "$file" "$outfile"

    # Move the (supposedly constant) static variables from global scope into function scope
    sed -n -e '/\/\* Table of constant values \*\//,/^$/p' -e '/^static /,/^$/p' "$outfile" | sed 's@^.@    \0@' > "$tmpfile"
    sed -e '/\/\* Table of constant values \*\//,/^$/d' -e '/^static /,/^$/d' -e '/^{/{q;}' "$outfile" > "$outfile.edit"
    cat "$tmpfile" >> "$outfile.edit"
    sed '1,/^{/d' "$outfile" >> "$outfile.edit"
    mv "$outfile.edit" "$outfile"
done
rm "$tmpfile"

cd ..
# NOTE: converting dlamch_ declaration to dlamch_DISABLED instead of removing
# it because sometimes it appears in the same line as another declaration,
# which we don't want to remove.
echo '#include "f2c.h"' > src.cpp
cat "$dirName"/*.c | sed -e 's@\<char *\*@const char*@g' \
                         -e 's@\<TRUE_\>@ true @g' \
                         -e 's@\<FALSE_\>@ false @g' \
                         -e '/#ifdef __cplusplus/,/#endif$/d' \
                         -e '/#include "f2c.h"/d' \
                         -e '/disnan_(doublereal/d' \
                         -e 's@extern\(.*\)\<dlamch_(@extern\1dlamch_DISABLED(@' \
                         -e 's@dlamch_("@dlamch_<doublereal>("@g' \
                         -e '/^ *\/\* Subroutine \*\/.*;$/d' \
                         -e '/^ *\/\* Builtin functions \*\/$/,/^$/d' \
                         >> src.cpp
