#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int outputBinary = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--outputBinary") == 0) {
            outputBinary = 1;
            break;
        }
    }

    if (outputBinary) {
        FILE *fp = fopen("compositeTable.bin", "wb");
        // Write your data to the file here
        fclose(fp);
    } else {
        // Output data to stdout or do something else
    }

    return 0;
}