#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Fonction pour le modulo qui gère les valeurs négatives correctement
int mod(int a, int b) {
    return (a % b + b) % b;
}

// Convertir une lettre en son équivalent numérique
int letterToNumber(char letter) {
    if (letter == ' ') {
        return 26; // Espace
    } else {
        return toupper(letter) - 'A';
    }
}

// Convertir un numéro en sa lettre équivalente
char numberToLetter(int number) {
    if (number == 26) {
        return ' '; // Espace
    } else {
        return (char)(number + 'A');
    }
}

// Fonction de chiffrement
void encrypt(int cipherMatrix[10][1], int keyMatrix[10][10], int messageVector[10][1], int N) {
    for (int i = 0; i < N; i++) {
        cipherMatrix[i][0] = 0;
        for (int j = 0; j < N; j++) {
            cipherMatrix[i][0] += keyMatrix[i][j] * messageVector[j][0];
        }
        cipherMatrix[i][0] = mod(cipherMatrix[i][0], 27);
    }
}

// Fonction de déchiffrement
void decrypt(int plainMatrix[10][1], float invKeyMatrix[10][10], int cipherMatrix[10][1], int N) {
    for (int i = 0; i < N; i++) {
        plainMatrix[i][0] = 0;
        for (int j = 0; j < N; j++) {
            plainMatrix[i][0] += invKeyMatrix[i][j] * cipherMatrix[j][0];
        }
        plainMatrix[i][0] = mod((int)plainMatrix[i][0], 27);
    }
}

// Fonction pour obtenir le cofacteur d'une matrice
void getCofactor(int keyMatrix[10][10], int tempMatrix[10][10], int p, int q, int N) {
    int i = 0, j = 0;
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (row != p && col != q) {
                tempMatrix[i][j++] = keyMatrix[row][col];
                if (j == N - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

// Fonction pour calculer le déterminant d'une matrice
int determinantOfSubMatrix(int keyMatrix[10][10], int N) {
    int D = 0;
    if (N == 1) {
        return keyMatrix[0][0];
    }

    int tempMatrix[10][10];
    int sign = 1;

    for (int f = 0; f < N; f++) {
        getCofactor(keyMatrix, tempMatrix, 0, f, N);
        D += sign * keyMatrix[0][f] * determinantOfSubMatrix(tempMatrix, N - 1);
        sign = -sign;
    }
    return D;
}

// Fonction pour calculer l'inverse modulaire
int modInverse(int a, int m) {
    a = mod(a, m);
    for (int x = 1; x < m; x++) {
        if (mod(a * x, m) == 1)
            return x;
    }
    return -1;
}

// Fonction pour calculer l'inverse d'une matrice
int inverse(int keyMatrix[10][10], float invKeyMatrix[10][10], int N) {
    int det = determinantOfSubMatrix(keyMatrix, N);
    int detInv = modInverse(det, 27);

    if (det == 0 || detInv == -1) {
        printf("La matrice clé n'est pas inversible.\n");
        return 0;
    }

    int sign;
    int tempMatrix[10][10];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            sign = ((i + j) % 2 == 0) ? 1 : -1;
            getCofactor(keyMatrix, tempMatrix, i, j, N);
            invKeyMatrix[j][i] = mod(sign * determinantOfSubMatrix(tempMatrix, N - 1) * detInv, 27);
        }
    }
    return 1;
}

// Fonction pour afficher une matrice
void afficher_matrice(float mat[10][10], int ligne, int colonne) {
    for (int i = 0; i < ligne; i++) {
        for (int j = 0; j < colonne; j++) {
            printf("%5.2f ", mat[i][j]);
        }
        printf("\n");
    }
}

// Fonction de cryptanalyse (à compléter si nécessaire)
void cryptanalyse(int keyMatrix[10][10], int N) {
    printf("Matrice clé :\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", keyMatrix[i][j]);
        }
        printf("\n");
    }
}

// Afficher le menu
void afficher_menu() {
    printf("Menu:\n");
    printf("1. Chiffrer un message\n");
    printf("2. Déchiffrer un message\n");
    printf("3. Cryptanalyse\n");
    printf("4. Quitter\n");
}

// Afficher la table de correspondance
void afficher_table_correspondance() {
    printf("Table de correspondance :\n");
    for (int i = 0; i < 27; i++) {
        printf("%c = %d\n", numberToLetter(i), i);
    }
}

// Supprimer les espaces de padding
void remove_padding_spaces(char *message, int original_length) {
    int i;
    for (i = original_length; i > 0 && message[i] == ' '; i--);
    message[i + 1] = '\0';
}

// Programme principal
int main() {
    int N;
    printf("Entrez la taille de la matrice clé : ");
    scanf("%d", &N);
    int keyMatrix[10][10];
    printf("Entrez la matrice clé %dx%d :\n", N, N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%d", &keyMatrix[i][j]);
            keyMatrix[i][j] = mod(keyMatrix[i][j], 27);
        }
    }
    printf("Matrice clé entrée :\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", keyMatrix[i][j]);
        }
        printf("\n");
    }

    int choix;
    do {
        afficher_menu();
        printf("Entrez votre choix : ");
        while (scanf("%d", &choix) != 1 || (choix < 1 || choix > 4)) {
            printf("Veuillez entrer un choix valide (1-4).\n");
            while (getchar() != '\n');
        }
        printf("\n");

        if (choix == 1) {
            char message[100];
            printf("Entrez le message à chiffrer : ");
            getchar();
            fgets(message, sizeof(message), stdin);
            message[strcspn(message, "\n")] = '\0';

            printf("Texte en clair : %s\n", message);
            afficher_table_correspondance();
            printf("\n");

            int messageLength = strlen(message);
            int paddedLength = messageLength;
            if (messageLength % N != 0) {
                paddedLength = messageLength + (N - (messageLength % N));
            }

            int messageVector[10][1];
            for (int i = 0; i < paddedLength; i++) {
                if (i < messageLength) {
                    messageVector[i][0] = letterToNumber(message[i]);
                } else {
                    messageVector[i][0] = 23; // 'X' pour le padding
                }
            }

            int cipherMatrix[10][1];
            for (int i = 0; i < paddedLength; i += N) {
                encrypt(cipherMatrix + i, keyMatrix, messageVector + i, N);
            }

            printf("Message Chiffré: ");
            for (int i = 0; i < paddedLength; i++) {
                printf("%c", numberToLetter(cipherMatrix[i][0]));
            }
            printf("\n");

        } else if (choix == 2) {
            char message[100];
            printf("Entrez le message chiffré à déchiffrer : ");
            getchar();
            fgets(message, sizeof(message), stdin);
            message[strcspn(message, "\n")] = '\0';

            printf("Message chiffré : %s\n", message);

            int messageLength = strlen(message);
            int cipherMatrix[10][1];
            for (int i = 0; i < messageLength; i++) {
                cipherMatrix[i][0] = letterToNumber(message[i]);
            }

            float invKeyMatrix[10][10];
            if (!inverse(keyMatrix, invKeyMatrix, N)) {
                printf("Erreur : La matrice clé n'est pas inversible.\n");
                continue;
            }

            printf("Matrice de la clé inverse :\n");
            afficher_matrice(invKeyMatrix, N, N);

            int plainMatrix[10][1];
            for (int i = 0; i < messageLength; i += N) {
                decrypt(plainMatrix + i, invKeyMatrix, cipherMatrix + i, N);
            }

            // Supprimer le padding (caractère 'X') à la fin du message déchiffré
            int len = messageLength;
            while (len > 0 && plainMatrix[len - 1][0] == 23) {
                len--;
            }

            printf("Message déchiffré : ");
            for (int i = 0; i < len; i++) {
                printf("%c", numberToLetter(plainMatrix[i][0]));
            }
            printf("\n");

        } else if (choix == 3) {
            char message[100];
            printf("Entrez le message chiffré à analyser : ");
            getchar();
            fgets(message, sizeof(message), stdin);
            message[strcspn(message, "\n")] = '\0';

            printf("Message chiffré : %s\n", message);

            cryptanalyse(keyMatrix, N);

            int messageLength = strlen(message);
            int cipherMatrix[10][1];
            for (int i = 0; i < messageLength; i++) {
                cipherMatrix[i][0] = letterToNumber(message[i]);
            }

            float invKeyMatrix[10][10];
            if (!inverse(keyMatrix, invKeyMatrix, N)) {
                printf("Erreur : La matrice clé n'est pas inversible.\n");
                continue;
            }

            int plainMatrix[10][1];
            for (int i = 0; i < messageLength; i += N) {
                decrypt(plainMatrix + i, invKeyMatrix, cipherMatrix + i, N);
            }

            printf("Texte en clair retrouvé : ");
            for (int i = 0; i < messageLength; i++) {
                printf("%c", numberToLetter(plainMatrix[i][0]));
            }
            printf("\n");
        }
    } while (choix != 4);

    printf("Au revoir !\n");
    return 0;
}
