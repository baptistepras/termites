#ifndef MONDE_HPP
#define MONDE_HPP
#include "termite.hpp"

class Monde{
    private:
        // Attributs
        Grille g;  // Initialement une grille vide
        vector<Termite> vT;  // Initialement un vecteur vide
    
    public:
        // Constructeur
        Monde();  // Fabrique un monde vide
    
        // Méthodes
        Grille *getGrille();  // Renvoie un pointeur vers l'attribut g
        vector<Termite> *getVect();  // Renvoie un pointeur vers l'attribut vT
        bool coherenceGrilleVect() const;  // Assure la cohérence entre le vecteur de termites et la grille
        bool coherenceFichiers(Grille g1, vector<Termite> vT1) const;  // Assure le bon fonctionnement des pointeurs
        bool brindillesLibres() const;  // Compte le nombre de brindilles libres (brindilles non marquées ou portées par une termite)
        vector<int> compteBrindilles() const;  // Compte le nombre de brindilles marquées par chaque équipe
        bool termitesVivants();  // Renvoie true s'il reste au moins un termite vivant, false sinon
        void monde();  // Initialise aléatoirement un monde selon les constantes
        void insecticide(int i);  // Tue aléatoirement un termite, i représente le numéro actuel du                                                                         tour. Tuera au maximum 5% des termites. Fonctionne que si                                                                               nbTermites >= 20 (20 à 39 = 1 mort, 40 à 59 = 2 morts etc...)
};


// Opérateur
ostream& operator<<(ostream& out, Monde m);  // Affiche un élément de type Monde

#endif