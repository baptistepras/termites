#ifndef COORD_HPP
#define COORD_HPP
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <array>
#include <string>
#include <random>
#include <sstream>

// VOIR LE PDF "Description Générale du Projet" pour comprendre les deux jeux de constantes

using namespace std;
/*
// Constantes de Test: ne pas changer
const int tailleGrille = 20;  // Vaut 20
const int nbTermites = 40;  // Vaut 40
const int nbEquipes = 4;  // Vaut 4
const float densiteBrindille = 0.05;  // Vaut 0.05
const float densiteToile = 0.01;  // Vaut 0.01
const int dureeSablier = 6;  // Vaut 6
const int dureeToile = 2;  // Vaut 2
const vector<float> probaTourner = {0.1, 0.1, 0.1, 0.1};  // Vaut {0.01, 0.01, 0.01, O.01}
const vector<float> probaCreuser = {0.05, 0.05, 0.05, 0.05};  // Vaut {0.05, 0.05, 0.05, O.05}
*/
// Constantes de Simulation
const int tailleGrille = 40;  // Conserver un nombre tel que tailleGrille^2 >= nbTermites
const int nbTermites = 60;  // Nombre total de termites, ils seront équitablement répartis, conserver un multiple de nbEquipes
const int nbEquipes = 4;  // Conserver un nombre compris entre 1 et 4
const float densiteBrindille = 0.05;  // % de brindilles sur la grille, conserver un nombre compris entre 0.01 et 1
const float densiteToile = 0.01;  // % de toiles sur la grille, conserver un nombre compris entre 0.01 et 1
const int dureeSablier = 6;  // Temps d'attente d'un termite avant de pouvoir charger/décharger à nouveau une brindille
const int dureeToile = 2;  // Nombre de tours bloqués si un termite marche sur une toile
const vector<float> probaTourner = {0.1, 0.2, 0.1, 0.2};  // Conserver un nombre compris entre 0.01 et 1, valeur 0 = équipe 0 etc...
const vector<float> probaCreuser = {0.05, 0.05, 0.09, 0.09};  // Probabilité d'un termite de creuser un tunnel à chaque tour joué,                                                                      conserver un nombre compris entre 0.01 et 1. Entrer la racine de la                                                                    probabilité voulue (0,05 = 0, 0025 = 0,25%), valeur 0 = équipe 0 ...

// Classes
enum class Direction{NW, N, NE, E, SE, S, SW, W};


class Coord{
    private:
        // Attributs
        int ligne;
        int colonne;

    public:
        // Constructeur
        Coord(int l, int c);  // Fabrique la structure coord avec l la ligne et c la colonne
        
        // Méthodes
        int getLig();  // Fournit l'attribut ligne de la structure
        int getCol();  // Fournit l'attribut colonne de la structure
};


// Opérateurs
ostream& operator<<(ostream& out, Coord c);  // Affiche un élément de type Coord
ostream& operator<<(ostream& out, Direction d);  // Affiche un élément de type Direction
ostream& operator<<(ostream& out, vector<int> t);  // Affiche un élément de type vector<int>
bool operator==(Coord c1, Coord c2);  // Compare deux éléments de type Coord
bool operator==(vector<int> v1, vector<int> v2);  // Compare deux éléments de type vector<int>


// Fonctions membres
Direction aGauche(Direction d);  // Donne la direction située à 1/8e de tour à gauche de d
Direction aDroite(Direction d);  // Donne la direction située à 1/8e de tour à droite de d
Coord devantCoord(Coord c, Direction d);  // Renvoie la coordonnée situé en face de c dans la direction d

#endif