#!/usr/bin/env bash

usage() {
    echo "Submodule Builder"
    echo "$0 [-s UNIX|WINDOWS] [-m lua|collections] [-h]"
}

error_os() {
    echo "Invalid OS"
    echo
    usage
    exit 1
}

error_module() {
    echo "Invalid module"
    echo
    usage
    exit 1
}

error_nofind() {
    echo "Cannot find module relative to $0"
    echo
    usage
    exit 1
}

##
# @param $1
#   operating system: UNIX / WINDOWS
# @param $2
#   module to build: lua / collections
main() {
    os="$1"
    mod="$2"

    # root of project
    top="$(pwd)"
    mypath='submodules/'
    if [ "$top" == submodules ]; then
        mypath=''
    fi

    if [ "$mod" == "lua" ]; then
        if [ ! -d "${mypath}lua" ]; then
            error_modules
        fi

        # generate makefiles for their respective OS
        cd "${mypath}lua"
        if [ ! -e 'makefile.bak' ]; then
            cp makefile makefile.bak

            cat makefile.bak | \
                sed 's/ -DLUA_USE_READLINE//g' | \
                sed 's/ -lreadline//g' \
                > makefile.unix

            # Note: MinGW needs '-D__need___off64_t' for MYCFLAGS
            cat makefile.bak | \
                sed 's/ -DLUA_USE_LINUX -DLUA_USE_READLINE//g' | \
                sed 's/,-E//g' | \
                sed 's/ -ldl -lreadline//g' | \
                sed 's/CC= gcc/CC= tcc/g' | \
                sed 's/ -lm//g' \
                > makefile.windows
        fi
        cd "$top"

        # build lua
        if [ "$os" == "UNIX" ]; then
            cp "${mypath}lua/makefile.unix" "${mypath}lua/makefile"
        elif [ "$os" == "WINDOWS" ]; then
            cp "${mypath}lua/makefile.windows" "${mypath}lua/makefile"
        else
            error_os
        fi

        make -C "${mypath}lua"

    elif [ "$mod" == "collections" ]; then
        if [ ! -d "${mypath}collections" ]; then
            error_modules
        fi

        make -C "${mypath}collections" release
    else
        error_module
    fi

    return 0
}

# main

MYOS=''
MYMOD=''

while getopts ":s:m:h" o; do
    case "${o}" in
        s)
            MYOS="$OPTARG"
            if [ "$MYOS" != "UNIX" ] && [ "$MYOS" != "WINDOWS" ]; then
                error_os
            fi
            ;;
        m)
            MYMOD="$OPTARG"
            if [ "$MYMOD" != "lua" ] && [ "$MYMOD" != "collections" ]; then
                error_module
            fi
            ;;
        h)
            usage
            exit 0
            ;;
        *)
            usage
            exit 1
            ;;
    esac
done
shift $((OPTIND-1))

if [ -z "$MYOS" ] || [ -z "$MYMOD" ]; then
    echo "Missing one of: -s,-m"
    echo
    usage
    exit 1
fi

main "$MYOS" "$MYMOD"
exit $?
