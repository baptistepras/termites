// -*- coding: utf-8 -*-
#include "grille.hpp"

// Constructeurs Case

Case::Case() {
    type = 'V';
    num = -1;
    team = -1;
    toile = false;
    moveable = true;
}

Case::Case(bool b) : toile{b} {
    type = 'V';
    num = -1;
    team = -1;
    moveable = true;
}

Case::Case(int T, bool m) : team{T}, moveable{m} {
    if (T < -1 or T > 3)
        throw invalid_argument("Cette équipe n'existe pas");
    type = 'B';
    num = -1;
    toile = false;
}

Case::Case(char c, int idT, bool b) : num{idT}, toile{b}  {
    if (idT < 0 or idT >= nbTermites)
        throw invalid_argument("Ce termite n'existe pas");
    type = 'T';
    team = -1;
    moveable = true;
}


// Constructeur Grille
Grille::Grille() : grille{Case{}} {}


// Méthodes Grille
void Grille::poseBrindille(Coord c, int i, bool m) {
    if (not(estVide(c)) or contientToile(c))
        throw invalid_argument("La case n'est pas vide ou contient une toile");
    if (i < -1 or i > 3)
        throw invalid_argument("Cette équipe n'existe pas");
    grille[c.getLig()][c.getCol()] = Case{i, m};
}

void Grille::enleveBrindille(Coord c) {
    if (not(contientBrindille(c)))
        throw invalid_argument("La case ne contient pas de brindille");
    grille[c.getLig()][c.getCol()] = Case{}; 
}

bool Grille::contientBrindille(Coord c) const {return (contenuCase(c).type == 'B');}

void Grille::poseTermite(Coord c, int idT) {
    if (not(estVide(c)))
        throw invalid_argument("La case n'est pas vide");
    if (idT < 0 or idT >= nbTermites)
        throw invalid_argument("Ce termite n'existe pas");
    grille[c.getLig()][c.getCol()] = Case{'B', idT, contientToile(c)};
}

void Grille::enleveTermite(Coord c) {
    if (not(contientTermite(c)))
        throw invalid_argument("La case ne contient pas de termite");
    grille[c.getLig()][c.getCol()] = Case{contientToile(c)};
}

int Grille::numeroTermite(Coord c) const {
    if (not(contientTermite(c)))
        throw invalid_argument("La case ne contient pas de termite");
    return contenuCase(c).num;
}

bool Grille::contientTermite(Coord c) const {return (contenuCase(c).type == 'T');}

bool Grille::estVide(Coord c) const {return (contenuCase(c).type == 'V');}

int Grille::contientEquipe(Coord c) const {return contenuCase(c).team;}

void Grille::poseToile(Coord c) {
    if (not(estVide(c)))
        throw invalid_argument("La case n'est pas vide");
    grille[c.getLig()][c.getCol()] = Case{true};
}

bool Grille::contientToile(Coord c) const {return contenuCase(c).toile;}

bool Grille::peutBouger(Coord c) const {return contenuCase(c).moveable;}

Case Grille::contenuCase(Coord c) const {return grille[c.getLig()][c.getCol()];}

int Grille::voisinsLibres(Coord c) const {
    Direction d; Coord c1{0, 0};
    int compteur = 0;
    for (int i = 0; i < 8; i++){
        try {
            c1 = devantCoord(c, Direction(i));
        } catch(...) {
            if (estVide(c))  // Rééquilibre avec l'instruction suivante
                compteur--;
            c1 = c; 
        }
        if (estVide(c1) and not(contientToile(c1)))
            compteur++;
    } return compteur;
}

Coord Grille::findVoisinsLibres(Coord c) const {
    Direction d; Coord c1{0, 0}; bool b;
    for (int i = 0; i < 8; i++){
        b = true;
        try {
            c1 = devantCoord(c, Direction(i));
        } catch(...) {
            c1 = c;
            b = false;  // empêche de renvoyer la case c à l'instruction suivante
        }
        if (estVide(c1) and b)
            return c1;
    } throw invalid_argument("Pas de voisin libre");
}


// Opérateurs
bool operator==(Case c1, Case c2) {
    return (c1.type == c2.type and c1.num == c2.num and c1.team == c2.team and c1.toile == c2.toile and c1.moveable == c2.moveable);
}

bool operator==(Grille g1, Grille g2) {
    for (int i = 0; i < tailleGrille; i++) {
        for (int j = 0; j < tailleGrille; j++) {
            if (not(g1.contenuCase(Coord{i, j}) == g2.contenuCase(Coord{i, j})))
                return false;
        }
    }
    return true;
}