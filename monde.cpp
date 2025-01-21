// -*- coding: utf-8 -*-
#include "monde.hpp"

// Constructeur
Monde::Monde() {g = Grille{};}


// Méthodes
Grille *Monde::getGrille() {return &g;}

vector<Termite> *Monde::getVect() {return &vT;}

bool Monde::coherenceGrilleVect() const {
    // On vérifie que chaque termite est bien placée dans la grille
    // Permet de vérifier que l'on n'a pas perdu des termites
    for (Termite t : vT) {
        Case c = g.contenuCase(t.coordTermite());
        if ((c.type != 'T' or c.num != t.idTTermite()) and t.estEnVie())
            return false;
    }
    
    // On vérifie que la grille a bien placé tous les termites
    // Permet de vérifier que l'on n'a pas dédoubler des termites
    for (int i = 0; i < tailleGrille; i++) {
        for (int j = 0; j < tailleGrille; j++) {
            Case c = g.contenuCase(Coord{i, j});
            if (c.type == 'T') {
                if (not(vT[c.num].coordTermite() == Coord{i, j}))
                    return false;
            }
        }
    }

    return true;
}

bool Monde::coherenceFichiers(Grille g1, vector<Termite> vT1) const {return (g == g1 and vT == vT1);}

bool Monde::brindillesLibres() const {
    int s = 0;
    
    for (int i = 0; i < tailleGrille; i++) {  // Vérifie la présence de brindilles libre dans la grille
        for (int j = 0; j < tailleGrille; j++) {
            if (g.contientEquipe(Coord{i, j}) == -1 and g.contientBrindille(Coord{i, j}))
                s++;
        }
    }
    
    for (Termite t: vT) {  // Vérifie que toutes les termites ont posé leur brindille
        if (t.porteBrindille())
            s++;
    }  
    
    return (s > 0);
}

vector<int> Monde::compteBrindilles() const {
    vector<int> v = {0, 0, 0, 0};
    
    // Il faut que la brindille soit posée et marquée pour la compter
    for (int i = 0; i < tailleGrille; i++) {
        for (int j = 0; j < tailleGrille; j++) {
            if (g.contientEquipe(Coord{i, j}) != -1 and g.contientBrindille(Coord{i, j}))
                v[g.contientEquipe(Coord{i, j})]++;
        }
    }
    return v;
}

bool Monde::termitesVivants() {
    for (Termite T : vT) {
        if (T.estEnVie())
            return true;
        }
    return false;
}

void Monde::insecticide(int i) {
    if (not(termitesVivants()))
        throw invalid_argument("Il n'y a aucun termite en vie");
    int r;
    if (i % int(2000 / (nbTermites*0.05)) == 0) {
        do {
            r = rand() % nbTermites;  // Choisis aléatoirement un pauvre termite encore en vie (plus pour longtemps)
        } while (not(vT[r].estEnVie()));
        vT[r].tueTermite();
        g.enleveTermite(vT[r].coordTermite());
    }
}

void Monde::monde() {
    int numT = 0; int r;
    int x; int y; int nbCases = nbEquipes;
    
    // Placement d'un nid pour chacune des équipes
    for (int i = 0; i < nbEquipes; i++) {
        do {
            x = rand() % tailleGrille;
            y = rand() % tailleGrille;
        } while (not(g.estVide(Coord{x, y})));
        g.poseBrindille(Coord{x, y}, i, false);
    }
    
    // Placement des brindilles non marquées
    for (int i = 0; i < tailleGrille; i++) {
        for (int j = 0; j < tailleGrille; j++) {
            r = rand() % 100;
            if (r < densiteBrindille*100 and g.estVide(Coord{i, j})) {
                g.poseBrindille(Coord{i, j}, -1, true);
                nbCases++;  // Compte le nombre de cases
            }
        }
    }
    
    // Placement des toiles
    for (int i = 0; i < tailleGrille; i++) {
        for (int j = 0; j < tailleGrille; j++) {
            r = rand() % 100;
            if (r < densiteToile*100 and g.estVide(Coord{i, j})) {
                g.poseToile(Coord{i, j});
                nbCases++;
            }
        }
    }

    // Placement des termites
    if (nbTermites + nbCases > tailleGrille*tailleGrille)  // S'il n'y a plus assez de cases vides
        throw invalid_argument("Il y a trop de termites à placer et trop peu de cases, la simulation pourrait ne pas marcher. Arrêt automatique, veuillez réduire la densité des brindilles, des toiles ou le nombre de termites");
    else {
        for (int e = 0; e < nbEquipes; e++) {
            for (int i = 0; i < nbTermites/nbEquipes; i++) {
                do {
                    x = rand() % tailleGrille;
                    y = rand() % tailleGrille;
                } while (not(g.estVide(Coord{x, y})) or g.contientToile(Coord{x, y}));  // On ne place pas de termite sur des toiles
                vT.push_back(Termite{numT, Coord{x, y}, e});
                g.poseTermite(Coord{x, y}, numT);
                numT++;  // ID du termite
            }
        }
    }
}


// Opérateur
ostream& operator<<(ostream& out, Monde m) {
    vector<Termite> *vT = m.getVect();
    Grille *g = m.getGrille();
    for (int i = 0; i < tailleGrille + 2; i++)  // Affiche la bordure supérieure de la grille
        out << "\33[40m" << "  " << "\33[0m";
    out << endl;
    for (int i = 0; i < tailleGrille; i++) {
        out << "\33[40m" << "  " << "\33[0m";  // Affiche la bordure gauche de la grille
        for (int j = 0; j < tailleGrille; j++) {
            if (g->estVide(Coord{i, j})) {
                if (g->contientToile(Coord{i, j}))  // Affiche une case vide contenant une toile
                    out << "# ";
                else  // Affiche une case vide sans toile
                    out << "  ";
            }
            else if (g->contientBrindille(Coord{i, j})) {
                Case c = g->contenuCase(Coord{i, j});
                switch (c.team) {  // Affiche la brindille avec la couleur de son équipe, noire si non-marquée
                    case -1 : out << "* "; break;
                    case 0 : out << "\33[91m" << "* " << "\33[0m"; break;  // Rouge, équipe 0
                    case 1 : out << "\33[92m" << "* " << "\33[0m"; break;  // Vert, équipe 1
                    case 2 : out << "\33[93m" << "* " << "\33[0m"; break;  // Jaune, équipe 2
                    case 3 : out << "\33[94m" << "* " << "\33[0m"; break;  // Bleu, équipe 3
                }
            }
            else {
                Termite t = (*vT)[g->numeroTermite(Coord{i, j})];
                switch (t.equipeTermite()) {  // Affiche le Termite avec la couleur de son équipe
                    case 0 : out << "\33[91m" << t.toString() << " " << "\33[0m"; break;  // Rouge, équipe 0
                    case 1 : out << "\33[92m" << t.toString() << " " << "\33[0m"; break;  // Vert, équipe 1
                    case 2 : out << "\33[93m" << t.toString() << " " << "\33[0m"; break;  // Jaune, équipe 2
                    case 3 : out << "\33[94m" << t.toString() << " " << "\33[0m"; break;  // Bleu, équipe 3
                }
            }
        }
        out << "\33[40m" << "  " << "\33[0m" << endl;  // Affiche la bordure droite de la grille
    }
    for (int i = 0; i < tailleGrille + 2; i++)  // Affiche la bordure inférieure de la grille
        out << "\33[40m" << "  " << "\33[0m";
    return out;
}