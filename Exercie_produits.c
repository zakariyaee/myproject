#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct produit{
    char NomProduit[20];
    float PrixProduit;
    int QuantiteProduit;
}Prod;
void creer_fichier_txt (int Nbr, Prod *E){
    FILE *fichier=fopen("produits.txt","w");
    int i;
    if (fichier == NULL){
        perror ("Erreur d'ouverture du fichier");
        return ;
    }
    fprintf(fichier, "Nom\t| Prix\t| Quantite\n");
    fprintf(fichier, "------------------------\n");
    for (i=0;i<Nbr;i++){
        fprintf(fichier,"%s\t| %.2f\t| %d\n",E[i].NomProduit,E[i].PrixProduit,E[i].QuantiteProduit);
    }
    fclose(fichier);
    printf("Le fichier a ete cree avec succes\n");
}
Prod* lire_fichier_txt (int *Nbr) {
    FILE *F=fopen("produits.txt","r");
    if ((F)==NULL) {
        perror ("Erreur d'ouverture du fichier");
        return NULL;
    }
    char buffer[100];
    fgets(buffer, sizeof(buffer), F); // Première ligne (Nom, Prix, Quantité)
    fgets(buffer, sizeof(buffer), F); // Ligne de séparation
    int T=10;
    Prod *P=(Prod*)malloc(T*sizeof(Prod));
    *Nbr=0;
    Prod product;
    while (fgets(buffer, sizeof(buffer), F) != NULL){
        if (sscanf(buffer, "%s\t| %f\t| %d", product.NomProduit, &product.PrixProduit, &product.QuantiteProduit) == 3) {
            P[*Nbr]=product;
            (*Nbr)++;
            if (*Nbr>=T) {
                T*=2;
                P=realloc(P,T*sizeof(Prod));
                if (P == NULL) {
                    perror("Erreur de realloc");
                    free(P); // Libérer la mémoire précédemment allouée
                    fclose(F);
                    return NULL;
                }
            }
        }
    }
    fclose(F);
    return P;
}
void afficher_produits (Prod* PL, int Nbr) {
    if (PL != NULL) {
        printf("\nListe des produits lus depuis le fichie:\n");
        printf("Nom\t| Prix\t| Quantite\n");
        printf("----------------------------\n");
        for(int i=0;i<Nbr;i++){
            printf("%s\t| %.2f\t| %d\n",PL[i].NomProduit,PL[i].PrixProduit,PL[i].QuantiteProduit);
        }
    }else{
        printf("Erreur de lecture de fichier");
    }
}
void ajouter_produit (int *Nbr) {
    FILE *F=fopen("produits.txt","a");
    Prod A;
    if ((F)==NULL) {
        perror ("Erreur d'ouverture du fichier");
    }
    printf("Veuillez ajouter un produit:\n");
    printf("Nom: ");
    scanf("%s", A.NomProduit);
    printf("Prix: ");
    scanf("%f", &A.PrixProduit);
    printf("Quantite: ");
    scanf("%d", &A.QuantiteProduit);
    fprintf(F,"%s\t| %.2f\t| %d\n", A.NomProduit, A.PrixProduit, A.QuantiteProduit);
    (*Nbr)++;
    fclose(F);
}
float calculer_valeur_totale (int N, Prod* P){
    float VT=0;
    for(int i=0; i<N; i++){
        VT += P[i].PrixProduit*P[i].QuantiteProduit;
    }
    return VT;
}
Prod* trouver_produit_cher(Prod* P, int N) { 
    Prod* produit_cher = &P[0]; 
    for (int i = 1; i < N; i++) { 
        if (P[i].PrixProduit > produit_cher->PrixProduit) { 
            produit_cher = &P[i]; 
        } 
    } 
    return produit_cher; 
}
void afficher_menu() {
    printf("\nMenu:\n");
    printf("1. Ajouter un produit\n");
    printf("2. Afficher tous les produits\n");
    printf("3. Calculer la valeur totale\n");
    printf("4. Trouver le produit plus chere\n");
    printf("5. Quitter\n");
    printf("Choisissez une option: ");
}
int main() {
    Prod E[5] = {{"Stylo", 1.50, 20}, {"Carnet", 3.20, 15}, {"Gomme", 0.80, 50}, {"Règle", 2.00, 10}, {"Crayon", 1.0, 30}};
    int Nbr = 5;
    creer_fichier_txt(Nbr, E);
    int choix;
    Prod* PA = NULL;
    do {
        afficher_menu();
        scanf("%d", &choix);
        switch (choix) {
            case 1:
                ajouter_produit(&Nbr);
                PA = lire_fichier_txt(&Nbr);
                break;
            case 2:
                PA = lire_fichier_txt(&Nbr);
                afficher_produits(PA, Nbr);
                break;
            case 3:
                PA = lire_fichier_txt(&Nbr);
                printf("La valeur totale est: %.2f\n", calculer_valeur_totale(Nbr, PA));
                break;
            case 4:
                printf("trouvons le produits plus chere\n");
                Prod* produit_cher = trouver_produit_cher(PA, Nbr); 
                printf("Le produit le plus cher est : %s avec un prix de %.2f€\n", produit_cher->NomProduit, produit_cher->PrixProduit);
                break;
            case 5:
                printf("Quitter le programme.\n");
                break;
            default:
                printf("Option invalide, veuillez réessayer.\n");
        }
    } while (choix != 5);
    if (PA != NULL) {
        free(PA);
    }
    return 0;
}