#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CMD_SIZE 10

#include "parser.h"
#include "cmd.h"

#define MAX_LINE_SIZE 1024

int main(int argc, char* argv[]) {
    char cmdline[MAX_LINE_SIZE];  // buffer des lignes de commandes
    char* cmdtoks[MAX_CMD_SIZE];  // "mots" de la ligne de commandes
    cmd_t cmds[MAX_CMD_SIZE];     // Tableau pour stocker les commandes
    cmd_t* current;

    while (1) {
        // Effacer les contenus de cmdline, cmdtoks et cmds
        memset(cmdline, 0, sizeof(cmdline));
        memset(cmdtoks, 0, sizeof(cmdtoks));
        memset(cmds, 0, sizeof(cmds));

        // Afficher un prompt
        printf("$minishellamine ");
        fflush(stdout);

        // Lire une ligne dans cmdline - Attention fgets enregistre le \n final
        if (fgets(cmdline, MAX_LINE_SIZE, stdin) == NULL) break;

        // Supprimer le caractère de nouvelle ligne ('\n') à la fin
        cmdline[strlen(cmdline) - 1] = '\0';

        // Traiter la ligne de commande
        // Découper la ligne en "tokens"
        int i = 0;
        char *token = strtok(cmdline, " ");
        while (token != NULL && i < MAX_CMD_SIZE) {
            cmdtoks[i++] = token;
            token = strtok(NULL, " ");
        }
        cmdtoks[i] = NULL;  // Terminer le tableau avec NULL

        // Traduire la ligne en structures cmd_t dans cmds
        int cmd_count = parse_cmd(cmdtoks, cmds, MAX_CMD_SIZE);

        // Exécuter les commandes dans l'ordre
        for (int i = 0; i < cmd_count; i++) {
            current = &cmds[i]; // Sélectionner la commande courante
            if (current->args[0] != NULL) {
                // Lancer la commande
                exec_cmd(current);
            }
        }
    }

    fprintf(stderr, "\nGood bye!\n");
    return 0;
}

