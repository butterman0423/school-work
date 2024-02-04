#/bin/bash
#lab1.sh

FLAGS=""

while getopts ":hlp" options; do
	case "${options}" in
		h)
			FLAGS="${FLAGS}h"
			;;
		p)
			FLAGS="${FLAGS}p"
			;;
		l)
			FLAGS="${FLAGS}l"
			;;
		*)
			echo "illegal option!" >&2
			exit 1
		esac
done

if [[ ${#FLAGS} -eq 0 ]]; then
	echo "No flags were inputed"
elif [[ ${#FLAGS} -eq 1 ]]; then
	echo "Flag ${FLAGS} is used"
else
	echo "Flags ${FLAGS} are used"
fi
exit 0
