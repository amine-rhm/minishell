#ifndef _BUILTIN_H
#define _BUILTIN_H

#include "cmd.h"

// Vérifie si la commande est une commande interne
int is_builtin(const char* cmd);

// Exécute une commande interne
int builtin(cmd_t* cmd);

// Change le répertoire courant
int cd(const char* path, int fderr);

// Modifie ou ajoute une variable d'environnement
int export(const char* var, const char* value, int fderr);

// Quitte le minishell
int exit_shell(int ret, int fderr);

#endif /* _BUILTIN_H */

