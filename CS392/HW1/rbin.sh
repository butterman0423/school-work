# /bin/bash

# *******************************************************************************
#  Author  : Nathaniel Escaro
#  Date    : 2/8/2024
#  Description: CS392 - Homework 1
#  Pledge  : I pledge my honor that I have abided by the Stevens Honor System.
# *******************************************************************************

FLAG_h=0
FLAG_l=0
FLAG_p=0

function print_usage {
	cat << EOF
Usage: rbin.sh [-hlp] [list of files]
   -h: Display this help;
   -l: List files in the recycle bin;
   -p: Empty all files in the recycle bin;
   [list of files] with no other flags,
        these files will be moved to the
        recycle bin.
EOF
}

while getopts ":hlp" opts; do
	case "${opts}" in 
		h)
			FLAG_h=1
			;;
		l)
			FLAG_l=1
			;;
		p)
			FLAG_p=1
			;;
		*)
			echo "Error: Unknown option '-${OPTARG}'." >&2
			print_usage
			exit 1
	esac
done

ARG_SUM=$(( $FLAG_h + $FLAG_l + $FLAG_p ))

# No arguments supplied
if [[ $ARG_SUM -eq 0 && $# -eq 0 ]]; then
	print_usage
	exit 1
fi

# Too many flags or flag(s) + file(s)
if [[ $ARG_SUM -gt 1 ]] || [[ $ARG_SUM -eq 1 && $# -gt 1 ]]; then
	echo "Error: Too many options enabled." >&2
	print_usage
	exit 1
fi

# Help msg
if [[ $FLAG_h -eq 1 ]]; then
	print_usage
	exit 0
fi

readonly RPATH=~/.recycle

# .recycle check
if [[ ! -d $RPATH ]]; then
	mkdir $RPATH
fi

# "Clears" .recycle
if [[ $FLAG_p -eq 1 ]]; then
	rm -r $RPATH
	mkdir $RPATH
	exit 0
fi

# -l option
if [[ $FLAG_l -eq 1 ]]; then
	ls -lAF $RPATH
	exit 0
fi

# Otherwise files were inputed
for fpath in $@; do
	if [[ -e $fpath ]]; then
		mv $fpath $RPATH
	else
		echo "Warning: '${fpath}' not found." >&2
	fi
done
