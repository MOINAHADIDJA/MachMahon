#ifndef TILE_H
#define TILE_H
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Tile
{
private:
    /* data */
    char left, top, right, bottom;
    bool state;

public:
    // Constructeur par défaut
    Tile() : left(' '), top(' '), right(' '), bottom(' '), state(false) {}
    // Constructeur
    Tile(char left, char top, char right, char bottom, bool state = false) : left(left), top(top), right(right), bottom(bottom), state(state) {}

    // Méthodes get pour obtenir les valeurs
    char getLeft() const
    {
        return left;
    }

    char getTop() const
    {
        return top;
    }

    char getRight() const
    {
        return right;
    }

    char getBottom() const
    {
        return bottom;
    }

    bool getState() const
    {
        return state;
    }

    // Méthode pour mettre à jour l'état
    void setState(bool newState)
    {
        state = newState;
    }
    // Méthode pour afficher les détails de la tuile
    void displayInfo() const
    {
        cout << "left : " << left << ", top : " << top << ", right : " << right << ", bottom : " << bottom << ", state : " << state << std::endl;
    }

    void displayTile() const
    {
        
        cout  << "  " << top << "   " <<   endl;
        cout  << right <<"   " << left << "  " << endl;
        cout << "  " << bottom << "   "  ;
    }
};

#endif // TILE_H
