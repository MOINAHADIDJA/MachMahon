#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include "macmahon_game.h"

using namespace std;

class ThreadPoolManager
{
    MacMahon game;              // Instance de la classe MacMahon représentant le jeu
    atomic<bool> solutionFound; // Variable atomique indiquant si une solution a été trouvée
    queue<MacMahon> plateaux;   // File de plateaux à traiter
    mutex mtx;                  // Mutex pour la synchronisation

public:
    ThreadPoolManager(const MacMahon &base) : game(base), solutionFound(false) {}

    // Fonction pour coordonner le jeu avec plusieurs threads
    void runThreadPool(int numThreads)
    {
        vector<thread> threads;

        // Remplir la file de plateaux
        int taille = game.getRows() * game.getCols();
        for (int i = 0; i < taille; ++i)
        {
            plateaux.push(MacMahon(game)); // Ajouter une copie  du plateau à la file
        }

        // Créer les threads et les ajouter au pool
        for (int i = 0; i < numThreads; ++i)
        {
            
            threads.emplace_back([this]()
                                 { processPlateaus(); });
        }

        // Attendre que tous les threads aient terminé leur exécution
        for (auto &thread : threads)
        {
            thread.join();
        }
    }

private:
    // Fonction exécutée par chaque thread pour rechercher la solution dans un plateau.
    void processPlateaus()
    {
        int i = 0;
        while (!solutionFound)
        {
            lock_guard<mutex> lock(mtx); // Verrou unique pour synchroniser l'accès à la file de plateaux
            // Vérifier si la solution a déjà été trouvée ou si la file de plateaux est vide
            if (solutionFound || plateaux.empty())
            {
                break; // Sortir de la boucle si aucune tâche n'est disponible
            }
            MacMahon plateau = plateaux.front(); // Obtenir le prochain plateau à traiter depuis la file
            plateaux.pop();                      // Retirer le plateau de la file

            // Initialisez le plateau avec la tuile correspondante à l'index
            plateau.getBoard()[0][0] = plateau.getTiles()[i];
            plateau.getTiles()[i].setState(true);
            // Tenter de trouver une solution en commençant à jouer à partir de la position (0, 0)
            if (plateau.jouer(0, 0))
            {
                // Si la solution est trouvée, afficher un message et le plateau solution
                cout << "Solution trouvée !" << endl;
                plateau.afficherMatrice();
                solutionFound = true; // Marquer la solution comme trouvée et sortir de la boucle
                break;
            }
            i++;
        }
    }

    MacMahon getGame() const { return game; }
};
