cmd_/mnt/shared/CS492/nescaro-pa2/module/nescaro.mod := printf '%s\n'   nescaro.o | awk '!x[$$0]++ { print("/mnt/shared/CS492/nescaro-pa2/module/"$$0) }' > /mnt/shared/CS492/nescaro-pa2/module/nescaro.mod