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
cat "$dirName"/*.c | sed -e 's@\<char *\*@const char*@g' \
                         -e '/#ifdef __cplusplus/,/#endif$/d' \
                         -e '/disnan_(doublereal/d' \
                         -e '/^ *\/\* Subroutine \*\/.*;$/d' \
                         -e '/^ *\/\* Builtin functions \*\/$/,/^$/d' > src.cpp
