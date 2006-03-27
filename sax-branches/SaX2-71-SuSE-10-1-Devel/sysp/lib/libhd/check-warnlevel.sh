echo "int main(void){return 0;}" > $0.c
gcc -c -Wno-pointer-sign $0.c && rm -f $0.* && echo -n "-Wno-pointer-sign"

