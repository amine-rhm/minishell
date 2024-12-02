#include <stddef.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "builtin.h"
#include "parser.h"

// Supprime les espaces en début et fin de chaîne
int trim(char* str) {
    if (str == NULL) return -1;

    char *start = str;
    while (isspace((unsigned char)*start)) start++;

    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';

    memmove(str, start, strlen(start) + 1);
    return 0;
}

// Nettoie les espaces multiples dans une chaîne
int clean(char* str) {
    if (str == NULL) return -1;

    int j = 0, in_space = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (isspace((unsigned char)str[i])) {
            if (!in_space) {
                str[j++] = ' ';
                in_space = 1;
            }
        } else {
            str[j++] = str[i];
            in_space = 0;
        }
    }
    if (j > 0 && str[j - 1] == ' ') j--;
    str[j] = '\0';
    return 0;
}

// Ajoute des espaces autour des caractères spéciaux
int separate_s(char* str, char* s, size_t max) {
    if (str == NULL || s == NULL) return -1;

    size_t len = strlen(s);
    size_t new_len = strlen(str) + len + 1;
    if (new_len > max) return -1;

    char* new_str = malloc(new_len);
    if (new_str == NULL) return -1;

    size_t j = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (strchr(s, str[i])) {
            new_str[j++] = ' ';
            new_str[j++] = str[i];
            new_str[j++] = ' ';
        } else {
            new_str[j++] = str[i];
        }
    }
    new_str[j] = '\0';
    strncpy(str, new_str, max);
    free(new_str);
    return 0;
}

// Substitue les variables d'environnement
// Example of substenv
// Assurez-vous qu'il n'y a pas de ligne erronée comme `cd` avant cette déclaration
int substenv(char *str, size_t max) {
    char temp[max];    // Temporary buffer to store the result
    char *out = temp;  // Pointer to the output buffer
    const char *env_val;  // To store the value of the environment variable
    char *start = str;    // Pointer to traverse the input string

    while (*start) {
        if (*start == '$' && isalpha((unsigned char)*(start + 1))) {
            // Variable found: skip '$' and extract variable name
            char var[64];
            size_t i = 0;
            start++;  // Skip '$'
            
            // Extract variable name
            while ((isalnum((unsigned char)*start) || *start == '_') && i < sizeof(var) - 1) {
                var[i++] = *start++;
            }
            var[i] = '\0';  // Null-terminate the variable name

            // Lookup the environment variable
            env_val = getenv(var);
            if (env_val) {
                size_t len = strlen(env_val);
                if ((out - temp) + len >= max) {
                    fprintf(stderr, "substenv: buffer overflow\n");
                    return -1;  // Return failure on overflow
                }
                strcpy(out, env_val);  // Append the environment variable value
                out += len;
            } else {
                // If environment variable is not found, preserve the `$` and variable name
                *out++ = '$';  // Preserve the '$'
                size_t remaining_space = max - (out - temp);
                if (snprintf(out, remaining_space, "%s", var) >= remaining_space) {
                    fprintf(stderr, "substenv: buffer overflow while appending variable name\n");
                    return -1;  // Return failure on overflow
                }
                out += strlen(var);  // Move pointer to the end of the appended string
            }
        } else {
            *out++ = *start++;  // Copy normal characters directly
        }
    }

    *out = '\0';  // Null-terminate the output string

    // Copy the result back to the original string, ensuring no overflow
    if ((out - temp) >= max) {
        fprintf(stderr, "substenv: final output buffer overflow\n");
        return -1;
    }
    strncpy(str, temp, max);  // Copy the processed string back
    str[max - 1] = '\0';  // Ensure the original string is null-terminated

    return 0;  // Success
}

// Découpe une chaîne en fonction d'un séparateur unique, avec des ajustements pour les séparateurs spéciaux
int strcut_s(char* str, char sep, char** tokens, size_t max) {
    if (str == NULL || tokens == NULL) return -1;

    size_t count = 0;
    char* start = str;
    while (*start && count < max) {
        tokens[count++] = start;
        while (*start && *start != sep) start++;
        if (*start) *start++ = '\0';
    }
    return count;
}

