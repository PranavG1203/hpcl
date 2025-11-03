#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <ctype.h>

#define MAX_SIZE 1000000   // Max characters in file

int is_separator(char c) {
    return (isspace(c) || c == ',' || c == '.' || c == '!' || c == '?' || c == ';' || c == ':');
}

int main() {
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        printf("Error opening file!\n");
        return 1;
    }

    char *buffer = (char*)malloc(MAX_SIZE);
    size_t len = fread(buffer, 1, MAX_SIZE, fp);
    fclose(fp);

    int total_words = 0;

    double start = omp_get_wtime();

    #pragma omp parallel
    {
        int local_count = 0;
        #pragma omp for nowait
        for (int i = 0; i < len; i++) {
            if (!is_separator(buffer[i]) && (i == 0 || is_separator(buffer[i - 1])))
                local_count++;
        }

        #pragma omp atomic
        total_words += local_count;
    }

    double end = omp_get_wtime();

    printf("Total Words: %d\n", total_words);
    printf("Time Taken: %f seconds\n", end - start);

    free(buffer);
    return 0;
}
