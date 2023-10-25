#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// encode char c according to start,mod,add and return the encoded char
char encodeChar(char c, char start, int mod, int add) {
    if (add != 0) {
        c = (c - start) + add;
        if (c < 0)
            c = c + mod;
        c = (c % mod) + start;
    }
    return c;
}

// read from File in, encode according to key and keyType and write to File out.
// return the current key index
int encoder(FILE *in, FILE *out, char *key, int keyI, int keyType) {
    int add;
    char input;
    while ((input = fgetc(in)) != EOF) { // read from in
        if (keyType == 0) add = 0;
        else {
            add = (key[keyI] - '0') * keyType; // calculate add
            keyI++;
            if (key[keyI] == '\0') keyI = 2; // key index from restart
        }
        // encode
        if ('A' <= input && input <= 'Z')  
            input = encodeChar(input, 'A', 26, add);
        else if ('a' <= input && input <= 'z') 
            input = encodeChar(input, 'a', 26, add);
        else if ('0' <= input && input <= '9') 
            input = encodeChar(input, '0', 10, add);
        fputc(input,out); // write to out
    }
    return keyI;
}

int main(int argc, char **argv) {
    int keyType = 0, keyInArgv = 0, isDebugMode = 0, keyI = 2;
    FILE *in = stdin, *out = stdout;
    int i;
    // parse commandLine
    for(i = 1; i < argc; i++){
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'D' && argv[i][2] == '\0') { // debug mode off
                isDebugMode = 0;
            }
            if (argv[i][1] == 'e') { // key for encoder
                if (argv[i][2] == '\0') {
                    fprintf(stderr,"invalid parameter - %s. there is no key for encoder.\n",argv[i]);
	                return 1;
                }
                keyType = -1;
                keyInArgv = i;
            }
            if (argv[i][1] == 'i') { // file in name
                if (argv[i][2] == '\0') {
                    fprintf(stderr,"invalid parameter - %s. there is no name of file.\n",argv[i]);
	                return 1;
                }
                char* fileName = &argv[i][2]; 
                in = fopen(fileName, "r");
                if (in == NULL) {
                    fprintf(stderr,"cannot open file - %s.\n",fileName);
	                return 1;
                }
            }
            if (argv[i][1] == 'o') { // file out name
                if (argv[i][2] == '\0') {
                    fprintf(stderr,"invalid parameter - %s. there is no name of file.\n",argv[i]);
	                return 1;
                }
                char* fileName = &argv[i][2];
                out = fopen(fileName, "w");
                if (out == NULL) {
                    fprintf(stderr,"cannot open file - %s.\n",fileName);
	                return 1;
                }
            }
        }
        if (isDebugMode) // print to stderr
            fprintf(stderr, "%s\n", argv[i]);
        if (argv[i][0] == '+') {
            if (argv[i][1] == 'D' && argv[i][2] == '\0') { // debug mode on
                isDebugMode = 1;
            }
            if (argv[i][1] == 'e') { // key for encoder
                if (argv[i][2] == '\0') {
                    fprintf(stderr,"invalid parameter - %s. there is no key for encoder.\n",argv[i]);
	                return 1;
                }
                keyType = 1;
                keyInArgv = i;
            }
        }
    }
    keyI = encoder(in, out, argv[keyInArgv], keyI, keyType);
    fclose(in);
    fclose(out);
    return 1;
}
