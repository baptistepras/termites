#ifndef GRILLE_HPP
#define GRILLE_HPP
#include "coord.hpp"

// Classes
class Case{
    public:
        // Attributs
        char type;  // 'T' pour termite, 'B' pour brindille, 'V' pour vide
        int num;  // Si la case est un termite, numéro du termite (entre 0 et nbTermites) ou -1
        int team; // Si la case est une brindille, numéro de l'équipe (entre 0 et 3) possédant la brindille ou -1
        bool toile;  // -1 si pas de toile, 0 sinon, présence de toile seulement sur case Vide ou Termite
        bool moveable;  // false si la case est une brindille fixe symbolisant un nid, true sinon
    
        // Constructeurs
        Case();  // Constructeur vide pour l'initialisation de la grille
        Case(bool b);  // Case vide, b est la présence d'une toile
        Case(int T, bool m);  // Case brindille, T est l'équipe, m la possibilité de bouger la brindille ou pas
        Case(char c, int idT, bool b);  // Case termite, entrer 'T' pour c, idT est l'ID du termite, b la présence d'une toile
};


class Grille{
    private:
        // Attributs
        array<array<Case, tailleGrille>, tailleGrille> grille;  // Initialise automatiquement toutes les cases sur vide
        
    public:    
        // Constructeur
        Grille();
    
        // Méthodes
        void poseBrindille(Coord c, int i, bool m);  // Dépose une brindille si la case de coordonnées c est libre, i étant l'équipe du                                                         ayant marqué la brindille (entre 0 et 3, -1 si non marquée) et m la possibilité                                                         de la récupérer ou non
        void enleveBrindille(Coord c);  // Retire une brindille si la case de coordonnées c en contient une
        bool contientBrindille(Coord c) const;  // Renvoie true si la case de coordonnées c contient une brindille, false sinon
        void poseTermite(Coord c, int idT);  // Dépose le termite numéro idT si la case de coordonnées c est libre
        void enleveTermite(Coord c);  // Retire un termite si la case de coordonnées c en contient un
        int numeroTermite(Coord c) const;  // Renvoie le numéro du termite si la case de coordonnées c en contient un, false sinon
        bool contientTermite(Coord c) const;  // Renvoie true si la case de coordonnées c contient un termite, false sinon
        bool estVide(Coord c) const;  // Renvoie true si la case de coordonnées c est vide (ni brindille ni termite), false sinon
        int contientEquipe(Coord c) const;  // Renvoie l'attribut équipe de la case de coordonnées c
        void poseToile(Coord c);  // Pose une toile sur une case vide
        bool contientToile(Coord c) const;  // Renvoie l'attribut toile de la case de coordonnées c
        bool peutBouger(Coord c) const;  // Renvoie l'attribut moveable de la case de coordonnées c
        Case contenuCase(Coord c) const;  // Renvoie le contenu de la case de coordonnées c
        int voisinsLibres(Coord c) const;  // Renvoie le nombre de cases voisines vides sans toile de la case de coordonnées c
        Coord findVoisinsLibres(Coord c) const;  // Renvoie la première case voisine vide de la case de coordonnées c en partant du NW                                                     vers la droite
};


// Opérateurs
bool operator==(Case c1, Case c2);  // Compare deux éléments de type Case
bool operator==(Grille g1, Grille g2);  // Compare deux éléments de type Grille

#endif