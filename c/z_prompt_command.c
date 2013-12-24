// PROMPT_COMMAND for z.sh

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main() {
    char * _Z_DATA;
    char datafile[PATH_MAX];
    char tmpfile[PATH_MAX];
    FILE *read;
    FILE *write;
    char line[LINE_MAX];
    char cwd[PATH_MAX];
    char pth[PATH_MAX];
    float rank;
    int timestamp;
    time_t epoch = time(NULL);
    int found = 0;

    if (getcwd(cwd, sizeof(cwd)) == NULL) return EXIT_FAILURE;

    _Z_DATA = getenv("_Z_DATA");
    if( _Z_DATA == NULL ) {
        sprintf(datafile, "%s/.z", getenv("HOME"));
    } else {
        sprintf(datafile, "%s", _Z_DATA);
    }

    read = fopen(datafile, "r");
    if (read == NULL) return EXIT_FAILURE;

    srand(time(NULL));
    sprintf(tmpfile, "%s.%d", datafile, rand());

    write = fopen(tmpfile, "w");
    if (write == NULL) return EXIT_FAILURE;

    while (fgets(line, sizeof line, read) != NULL) {
        sscanf(line, "%[^|]|%f|%d\n", pth, &rank, &timestamp);
        if (strcmp(pth, cwd) == 0) {
            fprintf(write, "%s|%f|%d\n", pth, rank + 1, (int) epoch);
            found = 1;
        } else fprintf(write, "%s", line);
    }
    if (!found) fprintf(write, "%s|1|%d\n", cwd, (int) epoch);

    fclose(read);
    fclose(write);

    if (rename(tmpfile, datafile) == 0) return EXIT_SUCCESS;

    return EXIT_FAILURE;

}
