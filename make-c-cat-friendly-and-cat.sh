#!/bin/bash -e

dirName=src-before-cat
mkdir -pv "$dirName"
cd src
for file in *.c; do
    outfile="../$dirName/$file"
    cp -v "$file" "$outfile"

    globalVars=( $(sed -n 's@^static [^ ]\+ \(c[^ ]\+\) = .*@\1@p' "$file") )

    [[ ${#globalVars[@]} == 0 ]] && continue

    echo "Replacing $var in $outfile..." >&2
    for var in "${globalVars[@]}"; do
        sed -i "s@\<${var}\>@${file%.c}_${var}@g" "$outfile"
    done
done

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
                         -e '/^ *\/\* Builtin functions \*\/$/,/^$/d' >> src.cpp
