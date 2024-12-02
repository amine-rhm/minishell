/*
  Projet minishell - Licence 3 Info - PSI 2024

  Nom :
  Prénom :
  Num. étudiant :
  Groupe de projet :
  Date :

  Parsing de la ligne de commandes utilisateur.
*/

#ifndef _PARSER_H
#define _PARSER_H
#define LOGICAL_AND_PARSER 1
#define LOGICAL_OR_PARSER 2
#define LOGICAL_AND_CONST 3


// Définition unique de la taille maximale de commande
#ifndef MAX_CMD_SIZE
#define MAX_CMD_SIZE 256
#endif

#define MAX_LINE_SIZE   1024

// Inclusions des bibliothèques nécessaires
#include <stddef.h>  // Définir size_t
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // Pour les fonctions de gestion des chaînes de caractères, comme strdup

/*
  Fonctions de parsing pour le minishell.
  
  - `parse_command` : Analyse la ligne de commande.
  - `trim` : Retire les espaces inutiles au début et à la fin d'une chaîne.
  - `clean` : Enlève les caractères indésirables (comme les retours à la ligne).
  - `separate_s` : Sépare une chaîne par un séparateur spécifique.
  - `substenv` : Remplace les variables d'environnement dans une chaîne de caractères.
  - `strcut` : Coupe une chaîne en utilisant un séparateur et place les tokens dans un tableau.
  - `strcut_s` (Optionnel) : Variante de `strcut` pour gérer les chaînes de manière plus sûre.
*/

// Prototypes des fonctions
int parse_command(char *input, char **command, char ***args);
int trim(char* str);
int clean(char* str);
int separate_s(char* str, char* s, size_t max);
int substenv(char* str, size_t max);
int strcut(char* str, char sep, char** tokens, size_t max);

// Optionnel
int strcut_s(char* str, char sep, char** tokens, size_t max);
// Déclarez la fonction trim_quotes
void trim_quotes(char* str);

#endif  // _PARSER_H

