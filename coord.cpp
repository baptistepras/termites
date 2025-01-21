// -*- coding: utf-8 -*-
#include "coord.hpp"

// Constructeur
Coord::Coord(int l, int c) {
    if (l < 0)
        throw invalid_argument("La ligne est hors de la grille au Nord");
    if (l >= tailleGrille)
        throw invalid_argument("La ligne est hors de la grille au Sud");
    if (c < 0)
        throw invalid_argument("La colonne est hors de la grille à l'Ouest");
    if (c >= tailleGrille)
        throw invalid_argument("La colonne est hors de la grille à l'Est"); 
    ligne = l;
    colonne = c;
}


// Méthodes
int Coord::getLig() {return ligne;}
int Coord::getCol() {return colonne;}


// Fonctions membres
Direction aGauche(Direction d) {
    return Direction((int(d) + 7) % 8);
}

Direction aDroite(Direction d) {
    return Direction((int(d) + 1) % 8);
}

Coord devantCoord(Coord c, Direction d) {
    int x = c.getLig();
    int y = c.getCol();
    switch(d) {
        case Direction::N : x-- ; break;
        case Direction::S : x++ ; break;
        case Direction::E : y++ ; break;
        case Direction::W : y-- ; break;
        case Direction::NE : x-- ; y++ ;break;
        case Direction::NW : x-- ; y-- ;break;
        case Direction::SE : x++ ; y++ ;break;
        case Direction::SW : x++ ; y-- ;break;
    }
    return Coord {x, y};
}


// Opérateurs
ostream& operator<<(ostream& out, Direction d) {
    switch(d) {
        case Direction::NE : out << "NE"; break;
        case Direction::NW : out << "NW"; break;
        case Direction::SE : out << "SE"; break;
        case Direction::SW : out << "SW"; break;
        case Direction::E  : out << "E"; break;
        case Direction::N  : out << "N"; break;
        case Direction::W  : out << "W"; break;
        case Direction::S  : out << "S"; break;
    }
    return out;
}

ostream& operator<<(ostream& out, Coord c) {
    out << "(" << c.getLig() << "," << c.getCol() << ")";
    return out;
}

ostream& operator<<(ostream& out, vector<int> t) {
    out << "{";
    for (int i = 0; i < int(t.size()) - 1; i++)
        out << t[i] << ", ";
    out << t.back() << "}" << endl;
    return out;
}

bool operator==(Coord c1, Coord c2) {
    return (c1.getLig() == c2.getLig() && c1.getCol() == c2.getCol());
}

bool operator==(vector<int> v1, vector<int> v2) {
    if (v1.size() != v2.size())
        return false;
    for (int i = 0; i < int(v1.size()); i++) {
        if (v1[i] != v2[i])
            return false;
    }
    return true;
}