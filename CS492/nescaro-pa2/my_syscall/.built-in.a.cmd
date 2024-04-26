cmd_my_syscall/built-in.a := rm -f my_syscall/built-in.a;  printf "my_syscall/%s " my_syscall.o | xargs ar cDPrST my_syscall/built-in.a
