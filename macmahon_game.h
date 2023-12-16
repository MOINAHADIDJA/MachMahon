#ifndef MACMAHON_GAME_H
#define MACMAHON_GAME_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "tile.h"
#include <thread>

using namespace std;

class MacMahon
{
private:
    int rows;
    int cols;
    vector<vector<Tile>> board;
    vector<Tile> tiles;

public:
    MacMahon(int r, int c) : rows(r), cols(c)
    {
        // Initialisez le plateau avec des tiles vides .
        board.resize(rows, vector<Tile>(cols, Tile(' ', ' ', ' ', ' ')));
    }
    MacMahon() : rows(0), cols(0)
    {
        // Initialisez le plateau avec des tiles vides  et le vecteur de tiles est vide.
        board.resize(rows, vector<Tile>(cols, Tile(' ', ' ', ' ', ' ')));
    }
   

    MacMahon(const MacMahon &nouveau_jeu)
    {
        rows = nouveau_jeu.rows;
        cols = nouveau_jeu.cols;
        tiles = nouveau_jeu.tiles;
        board = nouveau_jeu.board;
    };

    // methode pour lire le fichier et initialiser le plateau du jeu et recuperer la liste des tuiles
    void lireFichierEtInitialiser(const string &nomFichier)
    {

        ifstream fichier(nomFichier);
        if (!fichier)
        {
            cout << "Impossible d'ouvrir le fichier !" << endl;
            return;
        }
        // Lecture des dimensions du plateau
        fichier >> rows >> cols;
        // Réinitialisation du plateau et du vecteur de tiles
        board.resize(rows, vector<Tile>(cols, Tile()));
        
        //  Remplir la liste des tiles avec les differents tiles du fichier
        char left, top, right, bottom;
        while (fichier >> left >> top >> right >> bottom)
        {
            Tile tile{left, top, right, bottom};
            tiles.push_back(tile);
        }
        fichier.close();
    }

    // méthode pour verifier la couleur des tiles voisines
    bool verifierConditionVoisinage(const Tile &tile, int x, int y)
    {
        // Vérifie si les couleurs adjacentes correspondent

        if (x > 0 && board[x - 1][y].getBottom() != tile.getTop())
        {
            return false;
        }

        if (y > 0 && board[x][y - 1].getRight() != tile.getLeft())
        {
            return false;
        }
        return true;
    }

    // méthode pour verifier la couleur des tiles du contour
    bool verifierConditionCouleurBord(Tile &tile, int x, int y)
    {
        // 1ere ligne
        if (x == 0)
        {
            // Bord supérieur de la 1ere colonne
            if (y == 0)
            {
                // Coin supérieur et gauche
                return tile.getLeft() == tile.getTop();
            }
            else
            {
                // Bord supérieur
                return board[0][0].getTop() == tile.getTop();
            }
        }
        else if (y == 0) // ligne > 0 1ere colonne
        {
            // Bord gauche
            return board[0][y].getLeft() == tile.getLeft();
        }

        if (x == board.size() - 1) // derniere ligne
        {
            // bord inférieur
            return board[0][0].getLeft() == tile.getBottom();
        }

        if (y == board[x].size() - 1) // derniere colonne
        {
            return board[0][y - 1].getTop() == tile.getRight(); // Bord droit
        }
        return true;
    }

    bool placerCarre(Tile &tile, int x, int y)
    {
        if (x < 0 || y < 0 || x >= board.size() || y >= board[0].size())
        {
            // Les coordonnées (x, y) sont hors de la matrice
            return false;
        }
        if (!tile.getState()) // si la tile n'a pas encore été placée
        {
            // Verification des conditions du jeu
            if (verifierConditionCouleurBord(tile, x, y) && verifierConditionVoisinage(tile, x, y))
            {
                // Toutes les conditions sont satisfaites
                board[x][y] = tile;  // la tile est placée dans la matrice
                tile.setState(true); // mis à jour de l'état de la tile
                return true;
            }
        }
        return false;
    }

    bool jouer(int x, int y)
    {
        // Vérifier si la ligne actuelle est la dernière (la solution est trouvée si c'est le cas)
        if (x == rows)
        {
            return true;
        }
        // Vérifier si la colonne actuelle est la dernière, passer à la ligne suivante
        if (y == cols)
        {
            return jouer(x + 1, 0);
        }
        // Parcourir toutes les tuiles disponibles
        for (Tile &tile : tiles)
        {
            // Vérifier si la tuile peut être placée à la position actuelle
            if (placerCarre(tile, x, y))
            {
                // Marquer la tuile comme placée
                tile.setState(true);
                // Récursivement passer à la colonne suivante
                if (jouer(x, y + 1))
                {
                    return true; // La tuile suivante a été placée avec succès
                }
                // Si le placement de la tuile n'a pas abouti, remettre à l'état non placé et essayer la suivante
                tile.setState(false);
            }
        }
        return false; // Aucune tuile n'a pu être placée ici, retourner false
    }

    void afficherMatrice()
    {
        for (const vector<Tile> &row : board)
        {
            for (const Tile &tile : row)
            {
                cout << getColor(tile.getTop()) << "  " << tile.getTop() << "    "
                     << "\033[0m";
            }
            cout << endl;
            for (const Tile &tile : row)
            {
                cout << getColor(tile.getLeft()) << tile.getLeft() << "   "
                     << "\033[0m";
                cout << getColor(tile.getRight()) << tile.getRight() << "  "
                     << "\033[0m";
            }
            cout << endl;
            for (const Tile &tile : row)
            {
                cout << getColor(tile.getBottom()) << "  " << tile.getBottom() << "    "
                     << "\033[0m";
            }
            cout << endl;
            cout << endl;
        }
    }

    int getRows() const { return rows; }

    int getCols() const { return cols; }

    vector<vector<Tile>> &getBoard() { return board; }

    vector<Tile> getTiles() const { return tiles; }

    string getColor(char c)
    {
        switch (c)
        {
        case 'B':
            return "\033[34m";
        case 'R':
            return "\033[31m";
        case 'G':
            return "\033[32m";
        default:
            return "\033[0m";
        }
    }

    void back_avec_thread(MacMahon &jeu)
    {
        jeu.jouer(0, 0);
    }
};

#endif // MACMAHON_GAME_H
