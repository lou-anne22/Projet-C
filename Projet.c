#include <stdio.h> // pour les fcts d'entrée/sortie
#include <stdlib.h> //fcts utilitaires (rand, srand...)
#include <time.h> // pour gérer l'heure
#include <string.h> // pour les chaînes de caractères

#define MAX_NAME 50 //taille max pour le nom
#define FILE_NAME "scores.txt" // fichier pour la sauvegardes des scores

int score = 0;
char nom[MAX_NAME];

//Génère un nb aléatoire entre min et max
int nombre_aleatoire(int min, int max) {
    return rand() % (max - min + 1) + min;
}

//sauvegarde le score atuel du joueur dans un fichier avec date et heure
void sauvegarder_score() {
    FILE *f = fopen(FILE_NAME, "a");
    if (f == NULL) return;
    time_t t = time(NULL); //recupère l'heure
    struct tm tm = *localtime(&t); //convertion en lecture lisible
    fprintf(f, "%s %02d/%02d/%d %02d:%02d:%02d Score: %d\n",
        nom, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
        tm.tm_hour, tm.tm_min, tm.tm_sec, score);
    fclose(f);
}

int verifier_reponse(int bonne_reponse) {
    int essai, tentative = 1;
    while (tentative <= 3) {
        printf("Entrez le résultat : ");
        scanf("%d", &essai);
        if (essai == bonne_reponse) {
            int points;
            switch (tentative) {
                case 1: points = 10; break;
                case 2: points = 5; break;
                case 3: points = 1; break;
            default: points = 0; break;
        }

            score = score + points;
            sauvegarder_score();
            return points;
        } else {
            if (tentative < 3)
                printf("Incorrect. Essayez encore...\n");
        }
        tentative++;
    }
    printf("Perdu ! La bonne réponse était %d\n", bonne_reponse);
    sauvegarder_score(); //même si erreur
    return 0;
}

//affiche les anciens scores du joueur
void charger_scores() {
    FILE *f = fopen(FILE_NAME, "r");
    if (f == NULL) return;
    char ligne[256];
    printf("\n--- Historique des scores de %s ---\n", nom);
    while (fgets(ligne, sizeof(ligne), f)) {
        if (strstr(ligne, nom)) {
            printf("%s", ligne);
        }
    }
    fclose(f);
}

// Jeux classiques
void jeu_addition() {
    int a = nombre_aleatoire(0, 100);
    int b = nombre_aleatoire(0, 100);
    printf("%d + %d = ?\n", a, b);
    verifier_reponse(a + b);
}

void jeu_soustraction() {
    int a = nombre_aleatoire(0, 100);
    int b = nombre_aleatoire(0, 100);
    if (b > a) { int temp = a; a = b; b = temp; } //toujours un résultat positif
    printf("%d - %d = ?\n", a, b);
    verifier_reponse(a - b);
}

void jeu_multiplication() {
    int a = nombre_aleatoire(1, 10);
    int b = nombre_aleatoire(1, 10);
    printf("%d * %d = ?\n", a, b);
    verifier_reponse(a * b);
}

void jeu_division() {
    int b = nombre_aleatoire(1, 10);
    int a = b * nombre_aleatoire(1, 10); //résultat en nb entier
    printf("%d / %d = ?\n", a, b);
    verifier_reponse(a / b);
}

// Tables
//afiche la table choisie
void afficher_table(int n) {
    printf("Table de %d :\n", n);
    for (int i = 1; i <= 10; i++) {
        printf("%d x %d = %d\n", n, i, n*i);
    }
}

//pose des questions sur la tble
void exercice_table(int n) {
    for (int i = 1; i <= 10; i++) {
        printf("%d x %d = ? ", n, i);
        verifier_reponse(n * i);
    }
}

// Exercices du site
void exercice_nombre_manquant() {
    int total = 100;
    int a = nombre_aleatoire(50, 99);
    printf("Il faut combien pour aller de %d à %d ?\n", a, total);
    verifier_reponse(total - a);
}

void exercice_trouver_facteur() {
    int b = nombre_aleatoire(2, 10);
    int result = b * nombre_aleatoire(2, 10);
    printf("? x %d = %d\n", b, result);
    verifier_reponse(result / b);
}

void exercice_plus_grand() {
    int a = nombre_aleatoire(10, 100);
    int b = nombre_aleatoire(10, 100);
    printf("Quel est le plus grand ? %d ou %d ?\n", a, b);
    verifier_reponse(a > b ? a : b);
}

// Exercices inventés
void exercice_pyramide_logique() {
    int a = nombre_aleatoire(1, 50);
    int b = a + nombre_aleatoire(1, 10);
    int c = b + nombre_aleatoire(1, 10);
    printf("Complétez la pyramide logique :\n");
    printf("1er nombre : %d\n", a);
    printf("2e nombre : ? (doit être > %d et < %d)\n", a, c);
    int reponse;
    scanf("%d", &reponse);
    if (reponse > a && reponse < c) {
        printf("Bravo, logique respectée !\n");
        score = score + 10;
    } else {
        printf("Raté, on attendait un nombre entre %d et %d.\n", a, c);
    }
    sauvegarder_score();
}

void afficher_menu() {
    printf("\n+-----------------------------------+\n");
    printf("|1 : Addition                       |\n");
    printf("|2 : Soustraction                   |\n");
    printf("|3 : Multiplication                 |\n");
    printf("|4 : Tables des multiplications     |\n");
    printf("|5 : Divisions                      |\n");
    printf("|6 : Combien manque-t-il ?          |\n");
    printf("|7 : Trouver le facteur             |\n");
    printf("|8 : Quel est le plus grand ?       |\n");
    printf("|9 : Logique pyramide               |\n");
    printf("|0 : Sortir du jeu                  |\n");
    printf("+-----------------------------------+\n");
    printf("Quel est votre choix ?\n");
}

int main() {
    srand(time(NULL)); //initialisation du générateur aléatoire
    printf("Bienvenue au Champion des Maths !\nEntrez votre nom : ");
    fgets(nom, MAX_NAME, stdin);
    nom[strcspn(nom, "\n")] = '\0'; // supp le retour à la ligne

    charger_scores(); //affiche ancien score du joueur

    int choix;
    do {
        afficher_menu();
        scanf("%d", &choix); //récup le choix fait
        switch (choix) {
            case 1: jeu_addition(); break;
            case 2: jeu_soustraction(); break;
            case 3: jeu_multiplication(); break;
            case 4: {
                int table;
                printf("Choisissez une table (1-10) : ");
                scanf("%d", &table);
                afficher_table(table);
                exercice_table(table);
                break;
            }
            case 5: jeu_division(); break;
            case 6: exercice_nombre_manquant(); break;
            case 7: exercice_trouver_facteur(); break;
            case 8: exercice_plus_grand(); break;
            case 9: exercice_pyramide_logique(); break;
            case 10: exercice_double_ou_rien(); break;
            case 0:
                printf("Merci de votre visite, %s ! Score final : %d\n", nom, score);
                break;
            default: printf("Choix invalide. Essayez encore.\n");
        }
    } while (choix != 0); // boucle tant que le joueur ne quitte pas

    return 0;
}

