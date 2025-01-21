// -*- coding: utf-8 -*-
#include "termite.hpp"

// Constructeurs
Termite::Termite(int idT, Coord c, int e) : idT{idT}, pos{c}, equipe{e} {
    if (idT < 0 or idT >= nbTermites)
        throw invalid_argument("Ce termite n'existe pas");
    if (e < 0 or e >= nbEquipes)
        throw invalid_argument("Cette équipe n'existe pas");
    dir = Direction(rand() % 8);  // Choisi aléatoirement une direction
    brind = false;
    sablier = 0;
    toile = 0;
    enVie = true;
}

Termite::Termite(int idT, Coord c, Direction d, int e) : idT{idT}, pos{c}, dir{d}, equipe{e} {
    if (idT < 0 or idT >= nbTermites)
        throw invalid_argument("Ce termite n'existe pas");
    if (e < 0 or e >= nbEquipes)
        throw invalid_argument("Cette équipe n'existe pas");
    brind = false;
    sablier = 0;
    toile = 0;
    enVie = true;
}

Termite::Termite(int idT, Coord c, Direction d, bool b, int s, int e, int t, bool v) : idT{idT}, pos{c}, dir{d}, brind{b}, sablier{s}, equipe{e}, toile{t}, enVie{v} {
    if (idT < 0 or idT >= nbTermites)
        throw invalid_argument("Ce termite n'existe pas");
    if (s < 0 or s > dureeSablier)
        throw invalid_argument("La durée du sablier n'est pas bonne");
    if (e < 0 or e >= nbEquipes)
        throw invalid_argument("Cette équipe n'existe pas");
    if (t < 0 or t > dureeToile)
        throw invalid_argument("La durée de ralentissement dans la toile n'est pas bonne");
}


// Prédicats
Direction Termite::directionTermite() const {return dir;}

Coord Termite::coordTermite() const {return pos;}

int Termite::idTTermite() const {return idT;}

int Termite::sablierTermite() const {return sablier;}

int Termite::equipeTermite() const {return equipe;}

int Termite::toileTermite() const {return toile;}

bool Termite::estEnVie() const {return enVie;}

bool Termite::porteBrindille() const {return brind;}


// Méthodes
string Termite::toString() const {
    string s;
    if (dir == Direction::NE or dir == Direction::SW)
        s = "/";
    else if (dir == Direction::NW or dir == Direction::SE)
        s = "\\";
    else if (dir == Direction::N or dir == Direction::S)
        s = "|";
    else  // (dir == Direction::E or dir == Direction::W)
        s = "-";
    return s;
}

Coord Termite::devant() const {return devantCoord(pos, dir);}

void Termite::tueTermite() {enVie = false;}

void Termite::tourneADroite() {dir = aDroite(dir);}

void Termite::tourneAGauche() {dir = aGauche(dir);}

void Termite::tourneAleat() {
    int i = rand() % 2;
    if (i == 0)
        tourneADroite();
    else
        tourneAGauche();
}


// Méthodes environnement
bool Termite::laVoieEstLibre(Grille &g) const {
    try {  // Vérifie que la case devant le termite existe
        Coord dev = devant();
    } catch(...) {
        return false;        
    }
    return g.estVide(devant());
}

bool Termite::brindilleEnFace(Grille &g) const {
    try {  // Vérifie que la case devant le termite existe
        Coord dev =  devant();
    } catch(...) {
        return false;
    }
    return g.contientBrindille(devant());
}


// Méthodes mouvement
void Termite::avance(Grille &g) {
    if (laVoieEstLibre(g)) {
        g.enleveTermite(pos);
        pos = devant();
        g.poseTermite(pos, idT);    
    } else
        throw invalid_argument("Le termite ne peut pas avancer");      
}

void Termite::creuseTunnel(Grille &g) {
    if (porteBrindille())
        throw invalid_argument("Le termite porte une brindille et ne peut pas creuser de tunnel");
    int x; int y;
    do {  // Téléporte aléatoirement le termite sur une case vide
            x = rand() % tailleGrille;
            y = rand() % tailleGrille;
        } while (not(g.estVide(Coord{x, y})));
    g.enleveTermite(coordTermite());
    pos = Coord{x, y};
    g.poseTermite(Coord{x, y}, idT);  
}

void Termite::marcheAleatoire(Grille &g, int test) {
    bool creuser = true;
    int r = rand() % 100; int a = rand() % 100;
    if ((r < probaTourner[equipeTermite()]*100 and test != 2 and test != 3) or test == 1) {  // Calcule la probabilité de tourner
        tourneAleat();
    } else {  // Si le termite n'a pas tourné
        r = rand() % 100;  // Ne pas effacer ! Ne marche pas si on garde le r au-dessus
        if ((r < probaCreuser[equipeTermite()]*100 and a < probaCreuser[equipeTermite()]*100 and test != 2) or test == 3) {  
        // Calcule la probabilité de creuser
            try {  // Essaye de creuser un tunnel
                creuseTunnel(g);
            } catch(...) {
                creuser = false;
            }
        } if (not(creuser) or not(r < probaCreuser[equipeTermite()]*100 and a < probaCreuser[equipeTermite()]*100 and test != 3) or test == 2) {  // Si le termite n'a pas creusé
            try {  // Essaye d'avancer
                avance(g);
            } catch(...) {  // Tourne si ne peut pas
                tourneAleat();
            }
        }
    }
    if (g.contientToile(coordTermite()))  // Si le termite s'est coincé dans une toile avec son mouvement, on le notifie au termite
        toile = dureeToile;
}

void Termite::chargeBrindille(Grille &g) {
    if (sablier != 0)
        throw invalid_argument("Le termite ne peut pas prendre de brindille");
    if (brind)
        throw invalid_argument("Le termite a déjà une brindille");
    Coord c = devant();
    if (brindilleEnFace(g) and g.contientEquipe(c) != equipeTermite() and g.peutBouger(c)) {  
    // Si la case devant le termite contient une brindille d'une équipe adverse ou non marquée et que cette brindille ne symbolise pas        un nid, alors il la récupère
        brind = true;
        g.enleveBrindille(c);
        sablier = dureeSablier;
    } else
        throw invalid_argument("Le termite ne peut pas prendre de brindille");
}

void Termite::dechargeBrindille(Grille &g) {
    if (sablier != 0)
        throw invalid_argument("Le termite ne peut pas poser la brindille");
    if (not(brind))
        throw invalid_argument("Le termite n'a pas de brindille");
    Coord c = devant();
    if (brindilleEnFace(g) and g.voisinsLibres(c) > 0 and g.contientEquipe(c) == equipeTermite()) {  
    // Si la case devant le termite contient une brindille de son équipe et que cette brindille possède au moins une case voisine vide,        alors le termite pose sa brindille
        Coord c1 = g.findVoisinsLibres(c);
        brind = false;
        g.poseBrindille(c1, equipeTermite(), true);
        sablier = dureeSablier;
    } else
        throw invalid_argument("Le termite ne peut pas poser la brindille");
}

void Termite::vieTermite(Grille &g) {
    // On vérifie que le termite est encore en vie
    if (estEnVie()) {
        // On vérifie que le termite ne soit pas embourbé dans une toile d'araignée
        if (toileTermite() == 0) {
            // Si le termite peut charger ou décharger une brindille, il le fait en priorité
            bool charge = true;
            bool decharge = true;
            try {
                chargeBrindille(g);
            } catch(...) {
                charge = false;
            }
            try {
                dechargeBrindille(g);
            } catch(...) {
                decharge = false;
            } 
            if (not(charge or decharge)) {  // Si le termite n'a pas encore fait d'action, il fait une action de mouvement
                if (sablier > 0)
                    sablier--;
                marcheAleatoire(g, 0);
            }
        } else  // S'il a passé son tour, on réduit son temps de blocage dans la toile
            toile--;
    }
}


// Opérateurs
ostream& operator<<(ostream& out, Termite t) {
    out << "ID: " << setw(2) << t.idTTermite() << " | Coordonnées: " << t.coordTermite() << " | Direction: " << t.directionTermite() << " | Brindille: " << t.porteBrindille() << " | Sablier: " << t.sablierTermite() << " | Equipe: " << t.equipeTermite() << " | Toile: " << t.toileTermite() << " | En vie: " << t.estEnVie() << endl;
    return out;
}

ostream& operator<<(ostream& out, vector<Termite> vT) {
    for (Termite t : vT)
        out << t;
    return out;
}

bool operator==(Termite t1, Termite t2) {
    return (t1.coordTermite() == t2.coordTermite() and t1.directionTermite() == t2.directionTermite() and t1.idTTermite() == t2.idTTermite() and t1.sablierTermite() == t2.sablierTermite() and t1.porteBrindille() == t2.porteBrindille() and t1.equipeTermite() == t2.equipeTermite() and t1.toileTermite() == t2.toileTermite() and t1.estEnVie() == t2.estEnVie());
}

bool operator==(vector<Termite> vT1, vector<Termite> vT2) {
    if (vT1.size() != vT2.size())
        return false;
    for (int i = 0; i < int(vT1.size()); i++) {
        if (not(vT1[i] == vT2[i]))
            return false;
    }
    return true;
}