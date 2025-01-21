#ifndef TERMITE_HPP
#define TERMITE_HPP
#include "grille.hpp"

class Termite{ 
    // VOIR LE PDF "Description Générale du Projet" pour comprendre comment agissent les termites
    private:
        // Attributs
        Coord pos;  // Position du termite dans la grille
        int idT;  // ID du termite, compris entre 0 et nbTermites
        Direction dir;  // Direction du termite
        bool brind;  // true si le termite porte une brindille, false sinon
        int sablier;  // Valeur du temps d'attente de charge/décharge de brindille, vaut 0 à l'initialisation, 
        int equipe;  // Numéro de l'équipe du termite (entre 0 et 3)
        int toile;  // Nombre de tours restants à être bloqué dans une toile
        bool enVie;  // true si le termite est en vie, false sinon

    public:
        // Constructeurs
        Termite(int idT, Coord c, int e);  // idT correspond à l'attribut idT, c à pos et e à equipe
        Termite(int idT, Coord c, Direction d, int e);  // idT correspond à l'attribut idT, c à pos, dir à dir et e à equipe
        Termite(int idT, Coord c, Direction d, bool brind, int sablier, int e, int t, bool v);  // Attributs dans l'ordre ci-dessus
        
        // Prédicats
        bool porteBrindille() const;  // Renvoie l'attribut brind
        Direction directionTermite() const;  // Renvoie l'attribut dir
        Coord coordTermite() const;  // Renvoie l'attribut pos
        int idTTermite() const;  // Renvoie l'attribut idT
        int sablierTermite() const;  // Renvoie l'attribut sablier
        int equipeTermite() const;  // Renvoie l'attribut equipe
        int toileTermite() const;  // Renvoie l'attribut toile
        bool estEnVie() const;  // Renvoie l'attribut enVie
    
        // Méthodes
        string toString() const;  // Converti la direction du termite en string pour l'affichage
        Coord devant() const;  // Renvoie la coordonnée de la case située devant le termite
        void tueTermite();  // Tue le termite
        void tourneADroite();  // Fait tourner le termite à droite
        void tourneAGauche();  // Fait tourner le termite à gauche
        void tourneAleat();  // Fait tourner le termite à droite ou à gauche
        
        // Méthodes environnement
        bool laVoieEstLibre(Grille &g) const;  // Renvoie true si la case devant le termite est vide, false sinon
        bool brindilleEnFace(Grille &g) const;  // Renvoie true si la case devant le termite contient une brindille, false sinon
        
        // Méthodes mouvement
        void avance(Grille &g);  // Fait avancer le termite si cela est possible, renvoie une erreur sinon
        void creuseTunnel(Grille &g);  // Fait creuser un tunnel au termite s'il ne porte pas de brindille, renvoie une erreur sinon
        void chargeBrindille(Grille &g);  // Charge une brindille sur le termite si possible, renvoie une erreur sinon
        void dechargeBrindille(Grille &g);  // Décharge une brindille si possible, renvoie une erreur sinon
        void vieTermite(Grille &g);  // Effectue un tour pour le termite, se référer au PDF "ComportementTermites"
        void marcheAleatoire(Grille &g, int test);  // Fait aléatoirement tourner, avancer ou creuser le termite. On force le test de                                                          chaque cas avec 1 = tourne, 2 = tente d'avancer, 3 = creuse un tunnel avec int                                                          test. Utiliser 0 si l'on veut laisser l'aléatoire décider
};


// Opérateurs
ostream& operator<<(ostream& out, Termite t);  // Affiche un élément de type Termiteg
ostream& operator<<(ostream& out, vector<Termite> vT);  // Affiche un élément de type vector<Termite>
bool operator==(Termite t1, Termite t2);  // Compare deux éléments de type Termite
bool operator==(vector<Termite> vT1, vector<Termite> vT2);  // Compare deux éléments de type vector<Termite>

#endif