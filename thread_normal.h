#include <iostream>
#include <vector>
#include <thread>
#include "macmahon_game.h"

using namespace std;

class NormalThreadManager
{
private:
    MacMahon game; // Instance de la classe MacMahon représentant le jeu

public:
    NormalThreadManager(const MacMahon &base) : game(base) {}

    // Méthode pour jouer avec des threads normaux
    void jouerAvecThreadsNormaux()
    {
        int rows = game.getRows();
        int cols = game.getCols();
        int taille = rows * cols;
        vector<thread> Threads(taille);    // Vecteur de threads
        vector<MacMahon> plateaux(taille); // Vecteur de plateaux

        // Initialisation des instances de MacMahon
        for (int i = 0; i < taille; i++)
        {
            plateaux[i] = MacMahon(game); // Initialise chaque instance de MacMahon avec l'état de l'instance 'game'.
        }

        // Démarrage des threads pour résoudre le puzzle avec chaque instance de MacMahon
        for (int i = 0; i < taille; i++)
        {
            // Placer le premier carré dans le coin supérieur gauche de la matrice
            plateaux[i].getBoard()[0][0] = plateaux[i].getTiles()[i];
            plateaux[i].getTiles()[i].setState(true);
            // Démarrer un thread pour résoudre le puzzle avec cette instance
            Threads[i] = thread(&MacMahon::back_avec_thread, &plateaux[i], ref(plateaux[i]));
        }

        // Attendre la fin de tous les threads
        for (int i = 0; i < taille; i++)
        {
            Threads[i].join(); // Attend que chaque thread termine son exécution.
        }

        // Comparer les résultats des instances et mettre à jour la matrice principale du jeu
        int i = 0;
        while (i < taille && game.getBoard()[rows - 1][cols - 1].getBottom() == ' ')
        {
            // Si l'instance a une tuile placée dans le coin inférieur droit
            if (plateaux[i].getBoard()[rows - 1][cols - 1].getBottom() != ' ')
            {
                game.getBoard() = plateaux[i].getBoard(); // Mis à jour de la matrice principale si une solution valide est trouvée.
            }
            i++;
        }

        // Afficher la matrice finale
        game.afficherMatrice();
    }
};
