#include <iostream>
// #include "macmahon_game.h"
#include <thread>
#include "thread_pool_game.h"
#include "thread_normal.h"
#include <chrono>

using namespace std;

int main()
{

    MacMahon game; // Créez une instance de la classe MacMahon

    int algo;
    string fichier;
    cout << "Choisis Svp l'algorithme à utiliser : " << endl;
    cout << "==================================================" << endl;
    cout << "1 : Séquentiel, 2 : Thread normal ou 3 : Thread Pool " << endl;
    cout << "==================================================" << endl;
    cin >>algo;
    cout << "saisis le nom ou le chemin  du fichier" << endl;
    cin >> fichier;

    // Appelez la fonction pour lire le fichier et initialiser le jeu

    game.lireFichierEtInitialiser(fichier);

    // Vérifiez si les données ont été correctement chargées
    cout << "Rows: " << game.getRows() << ", Cols: " << game.getCols() << ", size matrice : " << game.getBoard().size() << "x" << game.getBoard()[0].size() << endl;

    // Affichez les tiles
    for (const Tile &tile : game.getTiles())
    {
        cout << "Tile : " << tile.getLeft() << " " << tile.getTop() << " " << tile.getRight() << " " << tile.getBottom() << " " << tile.getState() << endl;
        tile.displayTile();
        cout << endl;
    }
    cout << game.getTiles().size() << endl;
    int rows = game.getRows();
    int cols = game.getCols();

    // Appelez la fonction jouer pour placer les tiles dans la matrice
    auto start = chrono::high_resolution_clock::now();

    if (algo == 1)
    {
        // sequentielle
        if (game.jouer(0, 0))
        {
            cout << "Solution trouvée !" << endl;
            cout << "Toutes les tiles ont été placées avec succès !" << endl;
            // Affiche la matrice avec les tiles placées
            game.afficherMatrice();
        }
        else
        {
            cout << "Aucune solution trouvée." << endl;
        }
    }
    else if (algo == 2)
    {
        // Thread
        NormalThreadManager manager(game);
        manager.jouerAvecThreadsNormaux();
    }
    else if (algo == 3)
    {
        // Thread Pool
        ThreadPoolManager manager(game);
        manager.runThreadPool(5);
    }
    cout << "tu n'a pas choisis un bon numéro d'algorithme, reessaies" << endl;

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> d = end - start;
    cout << "Temps d'exécution: " << d.count() << endl;

    return 0;

    // g++ -g main.cpp -o main.exe
}
