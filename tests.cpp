// -*- coding: utf-8 -*-
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include "coord.hpp"
#include "grille.hpp"
#include "termite.hpp"
#include "monde.hpp"

// VOIR LE PDF "Description Générale du Projet"

// Renvoie true si les constantes de probabilité sont comprises entre 0.01 et 1, false sinon
bool probas(float f) {
    return (f > 0.009999 and f <= 1);
}

bool probas(vector<float> t) {
    for (float f : t) {
        if (not(probas(f)))
            return false;
    }
    return true;
}

TEST_CASE("probas") {
    float f1 = 0.01; float f2 = 1;
    float f3 = 0.009999; float f4 = 1.000001;
    vector<float> t1 = {f1, f2};
    vector<float> t2 = {f3, f2};
    vector<float> t3 = {f1, f4};
    
    CHECK(probas(f1)); CHECK(probas(f2));
    CHECK_FALSE(probas(f3)); CHECK_FALSE(probas(f4));
    CHECK(probas(t1));CHECK_FALSE(probas(t2)); 
    CHECK_FALSE(probas(t3));
}

TEST_CASE("Constantes") {
    cout << "\33[31m" << "FAIRE UN 'make clean' AVANT DE LANCER LES TESTS, SOUS RISQUE DE SEG FAULT" << endl;
    cout << "UTILISER LES CONSTANTES DE TEST DANS 'coord.cpp' ET NON PAS CELLES DE LA SIMULATION" << "\33[0m" << endl;
    CHECK(tailleGrille > 0);
    CHECK((nbTermites > 0 and nbTermites <= tailleGrille*tailleGrille));
    CHECK(nbTermites % nbEquipes == 0);
    CHECK((nbEquipes >= 1 and nbEquipes <= 4));
    CHECK(probas(densiteBrindille));
    CHECK(probas(densiteToile));
    CHECK(dureeToile >= 2);
    CHECK(dureeSablier >= 0);
    CHECK(probas(probaTourner));
    CHECK(probas(probaCreuser));
}


// Création d'une grille 3x3 de test
Monde creeMondeTest() {
    // Initialisation d'un monde vide
    Monde m = Monde{};
    Grille *g = m.getGrille(); 
    vector<Termite> *vT = m.getVect();
    
    // On rempli le vecteur de termites
    vT->push_back(Termite{0, Coord{0, 1}, Direction::N, 1});
    vT->push_back(Termite{1, Coord{1, 2}, Direction::N, 1});
    vT->push_back(Termite{2, Coord{2, 1}, Direction::E, 1});
    vT->push_back(Termite{3, Coord{1, 1}, Direction::E, 1});
    vT->push_back(Termite{4, Coord{0, 0}, Direction::SE, 1});
    
    // On rempli la grille
    g->poseBrindille(Coord{1, 0}, -1, true); g->poseBrindille(Coord{0, 2}, 1, true);
    g->poseTermite(Coord{0, 1}, 0); g->poseTermite(Coord{1, 2}, 1);
    g->poseTermite(Coord{2, 1}, 2); g->poseTermite(Coord{1, 1}, 3);
    g->poseTermite(Coord{0, 0}, 4);
    return m;
}

TEST_CASE("creeMondeTest") {
    // Initialisation d'un monde vide
    Monde m = creeMondeTest();
    Grille *g = m.getGrille();
    vector<Termite> *vT = m.getVect();
    
    // Cases Vide
    CHECK(g->estVide(Coord{2, 2}));
    CHECK(g->estVide(Coord{2, 0}));
          
    // Cases Brindille
    CHECK(g->contientBrindille(Coord{1, 0}));
    CHECK(g->contientBrindille(Coord{0, 2}));
       
    // Cases Termite
    CHECK(g->contientTermite(Coord{0, 1}));
    CHECK(g->contientTermite(Coord{0, 0}));
    CHECK(g->contientTermite(Coord{1, 2}));
    CHECK(g->contientTermite(Coord{2, 1}));
    CHECK(g->contientTermite(Coord{1, 1}));
    
    // Vecteur Termite
    CHECK(vT->size() == 5);
    CHECK((*vT)[0] == Termite{0, Coord{0, 1}, Direction::N, 1});
    CHECK((*vT)[1] == Termite{1, Coord{1, 2}, Direction::N, 1});
    CHECK((*vT)[2] == Termite{2, Coord{2, 1}, Direction::E, 1});
    CHECK((*vT)[3] == Termite{3, Coord{1, 1}, Direction::E, 1});
    CHECK((*vT)[4] == Termite{4, Coord{0, 0}, Direction::SE, 1});
    
    // On vérifie que le vecteur et la grille sont cohérents
    CHECK(m.coherenceGrilleVect());
}


// Fonction unique(v, i), renvoie true si la valeur i est contenue exactement une fois dans v, false sinon
bool unique(vector<int> v, int i) {
    int s = 0;  // Compte les occurences de la valeur i dans v
    for (int j : v) {
        if (j == i)
            s++;
    }
    return (s == 1);
}

TEST_CASE("unique") {
    vector<int> v= {1, 1, 2, 3, 4, 5};
    
    CHECK(unique(v, 5));
    CHECK_FALSE(unique(v, 1));
    CHECK_FALSE(unique(v, 0));
}


// coord.cpp
TEST_CASE("Constructeur Coord") {
    Coord c1{1, 2}; Coord c2{0, 8}; Coord c3{19, 19};
    CHECK(c1.getLig() == 1); CHECK(c1.getCol() == 2);
    CHECK(c2.getLig() == 0); CHECK(c2.getCol() == 8);
    CHECK(c3.getLig() == 19); CHECK(c3.getCol() == 19);

    // Invariants
    CHECK_THROWS_AS((Coord {19, 20}), invalid_argument);
    CHECK_THROWS_AS((Coord {0, -1}), invalid_argument);
    CHECK_THROWS_AS((Coord {-1, 0}), invalid_argument);
    CHECK_THROWS_AS((Coord {20, 19}), invalid_argument);
    CHECK_THROWS_AS((Coord {20, 20}), invalid_argument);
}

TEST_CASE("Opérateur== Coord") {
    Coord c1{1, 2}; Coord c2{2, 1};
    Coord c3{1, 1}; Coord c4{1, 2};
    CHECK_FALSE(c1 == c2);
    CHECK_FALSE(c1 == c3);
    CHECK_FALSE(c3 == c2);
    CHECK(c1 == c4);
}

TEST_CASE("Opérateur== vector<int>") {
    vector<int> v1 = {1, 2, 3, 4};
    vector<int> v2 = {1, 2, 3};
    vector<int> v3 = {1, 2, 3, 4, 5};
    vector<int> v4 = {5, 4, 3, 2, 1};
    
    CHECK(v1 == v1);
    CHECK_FALSE(v1 == v2);
    CHECK_FALSE(v1 == v3);
    CHECK_FALSE(v1 == v4);
}

TEST_CASE("Méthodes Coord") {
    Coord c1{1, 2};
    CHECK(c1.getLig() == 1); 
    CHECK(c1.getCol() == 2);
    CHECK_FALSE(c1.getLig() == 2); 
    CHECK_FALSE(c1.getCol() == 1);
}

TEST_CASE("Fonctions membres Coord: aGauche et aDroite") {
    Direction d1 = Direction::NW; Direction d2 = Direction::W;
    
    // 1/8ème de tour
    CHECK(aGauche(d1) == Direction::W);
    CHECK(aDroite(d1) == Direction::N);
    CHECK(aGauche(d2) == Direction::SW);
    CHECK(aDroite(d2) == Direction::NW);
    
    // Tour complet
    for (int i = 0; i < 8; i++) {
        d1 = aGauche(d1); d2 = aDroite(d2);
    }
    CHECK(d1 == Direction::NW);
    CHECK(d2 == Direction::W);
    
    // 1/8ème de tour à droite puis à gauche; revient en position initiale
    d1 = aGauche(d1); d1 = aDroite(d1);
    CHECK(d1 == Direction::NW);
    d2 = aDroite(d2); d2 = aGauche(d2);
    CHECK(d2 == Direction::W);
}

TEST_CASE("Fonctions membres Coord: devantCoord") {
    Direction d1 = Direction::NW; Direction d2 = Direction::W;
    Direction d3 = Direction::N; Direction d4 = Direction::S;
    Direction d5 = Direction::SW; Direction d6 = Direction::SE;
    Direction d7 = Direction::E; Direction d8 = Direction::NE;
    Coord c1{1, 2}; Coord c2{2, 1}; Coord c3{0, 0}; Coord c4{19, 19}; 
    Coord c5{0, 19}; Coord c6{19, 0}; Coord c7{1, 0}; Coord c8{0, 1};
  
    // Cases existantes dans toutes les directions
    CHECK(devantCoord(c1, d1) == (Coord {0, 1}));
    CHECK(devantCoord(c2, d2) == (Coord {2, 0}));
    CHECK(devantCoord(c2, d3) == (Coord {1, 1}));
    CHECK(devantCoord(c3, d4) == (Coord {1, 0}));
    CHECK(devantCoord(c1, d5) == (Coord {2, 1}));
    CHECK(devantCoord(c3, d6) == (Coord {1, 1}));
    CHECK(devantCoord(c3, d7) == (Coord {0, 1}));
    CHECK(devantCoord(c2, d8) == (Coord {1, 2}));
  
    // Cases inexistantes
    CHECK_THROWS_AS(devantCoord(c4, d6), invalid_argument);  // Colonne et ligne incorrectes en diagonale
    CHECK_THROWS_AS(devantCoord(c6, d4), invalid_argument);  // Ligne incorrecte
    CHECK_THROWS_AS(devantCoord(c5, d7), invalid_argument);  // Colonne incorrecte
    CHECK_THROWS_AS(devantCoord(c7, d1), invalid_argument);  // Colonne incorrecte en diagonale
    CHECK_THROWS_AS(devantCoord(c8, d1), invalid_argument);  // Ligne incorrecte en diagonale
}


// grille.cpp
TEST_CASE("Constructeurs Case") {
    // Case Vide
    Case c = Case{};
    CHECK(c.type == 'V');
    CHECK(c.num == -1);
    CHECK(c.team == -1);
    CHECK_FALSE(c.toile);
    CHECK(c.moveable);
    
    // Case Brindille
    c = Case{0, true};
    CHECK(c.type == 'B');
    CHECK(c.num == -1);
    CHECK(c.team == 0);
    CHECK_FALSE(c.toile);
    CHECK(c.moveable);
    c = Case{0, false};
    CHECK_FALSE(c.moveable);
    
    // Case Termite
    c = Case{'T', 1, false};
    CHECK(c.type == 'T');
    CHECK(c.num == 1);
    CHECK(c.team == -1);
    CHECK_FALSE(c.toile);
    CHECK(c.moveable);
    
    // Case Toile
    c = Case{true};
    CHECK(c.type == 'V');
    CHECK(c.num == -1);
    CHECK(c.team == -1);
    CHECK(c.toile);
    CHECK(c.moveable);
    c = Case{false};
    CHECK_FALSE(c.toile);
    
    // Invariants
    CHECK_THROWS_AS((Case{'T', -1, false}), invalid_argument);  // ID invalide
    CHECK_THROWS_AS((Case{'T', 40, false}), invalid_argument);  // ID invalide
    CHECK_THROWS_AS((Case{4, true}), invalid_argument);  // Equipe invalide
    CHECK_THROWS_AS((Case{-2, true}), invalid_argument);  // Equipe invalide
}

TEST_CASE("Opérateur== Case") {
    Case c1 = Case{'T', 0, true};
    Case c2 = Case{'T', 0, false};
    Case c3 = Case{'T', 1, false};
    Case c4 = Case{2, true};
    Case c5 = Case{1, true};
    Case c6 = Case{2, false};
    Case c7 = Case{false};
    Case c8 = Case{true};
    
    CHECK(c1 == c1);
    CHECK(c2 == c2);
    CHECK(c3 == c3);
    CHECK(c4 == c4);
    CHECK(c5 == c5);
    CHECK(c6 == c6);
    CHECK(c7 == c7);
    CHECK(c8 == c8);
    CHECK_FALSE(c1 == c2);
    CHECK_FALSE(c1 == c3);
    CHECK_FALSE(c2 == c3);
    CHECK_FALSE(c4 == c5);
    CHECK_FALSE(c4 == c6);
    CHECK_FALSE(c5 == c6);
    CHECK_FALSE(c7 == c8);
    CHECK_FALSE(c1 == c4);
    CHECK_FALSE(c1 == c7);
    CHECK_FALSE(c5 == c8);
}

TEST_CASE("Constructeurs Grille") {
    // On vérifie que chaque case est bien vide
    Grille g = Grille{};
    for (int i = 0; i < tailleGrille; i++) {
        for (int j = 0; j < tailleGrille; j++)
            CHECK((g.estVide(Coord{i, j}) and not(g.contientToile(Coord{i, j}))));
    }
}


TEST_CASE("Méthodes Grille: Brindilles") {
    // On initialise le monde de test
    Monde m = creeMondeTest();
    Grille *g = m.getGrille();
    vector<Termite> *vT = m.getVect();
    
    // poseBrindille
    g->poseBrindille(Coord{2, 0}, 1, true);
    CHECK(g->contenuCase(Coord{2, 0}).type == 'B');
    CHECK(g->contientEquipe(Coord{2, 0}) == 1);
    CHECK(g->peutBouger(Coord{2, 0}));
    g->enleveBrindille(Coord{2, 0});
    g->poseBrindille(Coord{2, 0}, 1, false);
    g->poseToile(Coord{5, 6});
    CHECK_FALSE(g->peutBouger(Coord{2, 0})); 
    // Invariants
    CHECK_THROWS_AS(g->poseBrindille(Coord{5, 6}, 1, true), invalid_argument);  // Case Toile
    CHECK_THROWS_AS(g->poseBrindille(Coord{0, 0}, 1, true), invalid_argument);  // Case Termite
    CHECK_THROWS_AS(g->poseBrindille(Coord{2, 0}, 1, true), invalid_argument);  // Case Brindille
    CHECK_THROWS_AS(g->poseBrindille(Coord{20, 19}, 1, true), invalid_argument);  // Coordonnées invalides
    CHECK_THROWS_AS(g->poseBrindille(Coord{20, 19}, -2, true), invalid_argument);  // Equipe invalide
    CHECK_THROWS_AS(g->poseBrindille(Coord{20, 19}, 4, true), invalid_argument);  // Equipe invalide
    
    // enleveBrindille
    g->enleveBrindille(Coord{2, 0});
    CHECK_FALSE(g->contientBrindille(Coord{2, 0}));
    CHECK(g->contientEquipe(Coord{2, 0}) == -1);
    // Invariants
    CHECK_THROWS_AS(g->enleveBrindille(Coord{2, 0}), invalid_argument);  // Case Vide
    CHECK_THROWS_AS(g->enleveBrindille(Coord{2, 1}), invalid_argument);  // Case Termite
    CHECK_THROWS_AS(g->enleveBrindille(Coord{20, 19}), invalid_argument);  // Coordonnées invalides
    
    // contientBrindille
    CHECK_FALSE(g->contientBrindille(Coord{2, 0}));
    CHECK(g->contientBrindille(Coord{1, 0}));
    CHECK_FALSE(g->contientBrindille(Coord{2, 1}));
    // Invariants
    CHECK_THROWS_AS(g->contientBrindille(Coord{20, 19}), invalid_argument);  // Coordonnées invalides
          
    // contientEquipe
    CHECK(g->contientEquipe(Coord{1, 0}) == -1);
    CHECK(g->contientEquipe(Coord{0, 2}) == 1);
    
    // peutBouger
    g->poseBrindille(Coord{2, 0}, 1, false);
    CHECK(g->peutBouger(Coord{1, 0}));
    CHECK_FALSE(g->peutBouger(Coord{2, 0}));
}

TEST_CASE("Méthodes Grille: Termites") {
    // On initialise le monde de test
    Monde m = creeMondeTest();
    Grille *g = m.getGrille();
    vector<Termite> *vT = m.getVect();
    
    // poseTermite
    g->poseTermite(Coord{2, 0}, 10);
    g->poseToile(Coord{5, 6});
    g->poseTermite(Coord{5, 6}, 11);
    CHECK(g->contenuCase(Coord{5, 6}).num == 11);
    CHECK(g->contenuCase(Coord{2, 0}).num == 10);
    // Invariants
    CHECK_THROWS_AS(g->poseTermite(Coord{0, 0}, 4), invalid_argument);  // Case Termite
    CHECK_THROWS_AS(g->poseTermite(Coord{1, 0}, 5), invalid_argument);  // Case Brindille
    CHECK_THROWS_AS(g->poseTermite(Coord{20, 19}, 6), invalid_argument);  // Coordonnées invalides
    CHECK_THROWS_AS(g->poseTermite(Coord{1, 0}, -1), invalid_argument);  // ID invalide
    CHECK_THROWS_AS(g->poseTermite(Coord{1, 0}, 40), invalid_argument);  // ID invalide
    
    // enleveTermite
    g->enleveTermite(Coord{2, 0});
    CHECK(g->estVide(Coord{2, 0}));
    // Invariants
    CHECK_THROWS_AS(g->enleveTermite(Coord{2, 0}), invalid_argument);  // Case Vide
    CHECK_THROWS_AS(g->enleveTermite(Coord{1, 0}), invalid_argument);  // Case Termite
    CHECK_THROWS_AS(g->enleveTermite(Coord{20, 19}), invalid_argument);  // Coordonnées invalides
    
    // numeroTermite
    CHECK(g->numeroTermite(Coord{0, 1}) == 0);
    CHECK(g->numeroTermite(Coord{1, 2}) == 1);
    CHECK(g->numeroTermite(Coord{2, 1}) == 2);
    // Invariants
    CHECK_THROWS_AS(g->numeroTermite(Coord{2, 0}), invalid_argument);  // Case Vide
    CHECK_THROWS_AS(g->numeroTermite(Coord{1, 0}), invalid_argument);  // Case Brindille
    CHECK_THROWS_AS(g->numeroTermite(Coord{20, 19}), invalid_argument);  // Coordonnées invalides
    
    // contientTermite
    CHECK_FALSE(g->contientTermite(Coord{2, 0}));
    CHECK(g->contientTermite(Coord{1, 2}));
    CHECK_FALSE(g->contientTermite(Coord{0, 2}));
    // Invariants
    CHECK_THROWS_AS(g->contientTermite(Coord{20, 19}), invalid_argument);  // Coordonnées invalides
}

TEST_CASE("Méthode Grille: estVide") {
    // On initialise le monde de test
    Monde m = creeMondeTest();
    Grille *g = m.getGrille();
    
    CHECK(g->estVide(Coord{2, 0}));
    CHECK_FALSE(g->estVide(Coord{1, 2}));
    CHECK_FALSE(g->estVide(Coord{0, 2}));
    
    // Invariants
    CHECK_THROWS_AS(g->estVide(Coord{20, 19}), invalid_argument);  // Coordonnées invalides
}

TEST_CASE("Méthodes Grille: Toile") {
    // On initialise le monde de test
    Monde m = creeMondeTest();
    Grille *g = m.getGrille();  
    
    // poseToile
    g->poseToile(Coord{5, 5});
    CHECK(g->contenuCase(Coord{5, 5}).toile);
    // Invariants
    CHECK_THROWS_AS(g->poseToile(Coord{1, 0}), invalid_argument);
    CHECK_THROWS_AS(g->poseToile(Coord{0, 0}), invalid_argument);
    CHECK_THROWS_AS(g->poseToile(Coord{20, 19}), invalid_argument);  // Coordonnées invalides
          
    // contientToile
    CHECK(g->contientToile(Coord{5, 5}));
    CHECK_FALSE(g->contientToile(Coord{0, 0}));
    CHECK_FALSE(g->contientToile(Coord{1, 0}));
    CHECK_FALSE(g->contientToile(Coord{2, 0}));
    // Invariants
    CHECK_THROWS_AS(g->contientToile(Coord{20, 19}), invalid_argument);  // Coordonnées invalides
}

TEST_CASE("Méthode Grille: contenuCase") {
    // On initialise le monde de test
    Monde m = creeMondeTest();
    Grille *g = m.getGrille();
    Case c1 = g->contenuCase(Coord{0, 0});
    Case c2 = g->contenuCase(Coord{0, 2});
    Case c3 = g->contenuCase(Coord{2, 2});
    Case c4 = g->contenuCase(Coord{1, 0});
    g->poseToile(Coord{5, 5}); g->poseBrindille(Coord{5, 6}, 1, false);
    Case c5 = g->contenuCase(Coord{5, 5});
    Case c6 = g->contenuCase(Coord{5, 6});
    
    // Vérification du contenu des cases
    CHECK((c1.num == 4 and c1.type == 'T' and c1.team == -1 and not(c1.toile) and c1.moveable));
    CHECK((c2.num == -1 and c2.type == 'B' and c2.team == 1 and not(c2.toile) and c2.moveable));
    CHECK((c3.num == -1 and c3.type == 'V' and c3.team == -1 and not(c3.toile) and c3.moveable));
    CHECK((c4.num == -1 and c4.type == 'B' and c4.team == -1 and not(c4.toile) and c4.moveable));
    CHECK((c5.num == -1 and c5.type == 'V' and c5.team == -1 and c5.toile and c5.moveable));
    CHECK((c6.num == -1 and c6.type == 'B' and c6.team == 1 and not(c6.toile) and not(c6.moveable)));
    
    // Invariants
    CHECK_THROWS_AS(g->contenuCase(Coord{20, 19}), invalid_argument);  // Coordonnées invalides
}

TEST_CASE("Méthode Grille: voisinsLibres") {
    // On initialise le monde de test
    Monde m = creeMondeTest();
    Grille *g = m.getGrille();
    vector<Termite> *vT = m.getVect();
    vT->push_back(Termite{5, Coord{9, 9}, 1});
    g->poseTermite(Coord{9, 9}, 5);
    Termite t = (*vT)[0]; Termite t1 = (*vT)[1];
    Termite t2 = (*vT)[2]; Termite t3 = (*vT)[3];
    Termite t4 = (*vT)[4]; Termite t5 = (*vT)[5];
    g->poseToile(Coord{4, 5}); g->poseToile(Coord{5, 5});
    g->poseToile(Coord{6, 5}); g->poseToile(Coord{6, 6});
    g->poseToile(Coord{6, 7}); g->poseToile(Coord{5, 7});
    g->poseToile(Coord{4, 7}); g->poseToile(Coord{4, 6});

    // voisinsLibre
    CHECK(g->voisinsLibres(t.coordTermite()) == 0);
    CHECK(g->voisinsLibres(t1.coordTermite()) == 4);
    CHECK(g->voisinsLibres(t2.coordTermite()) == 5);
    CHECK(g->voisinsLibres(t3.coordTermite()) == 2);
    CHECK(g->voisinsLibres(t4.coordTermite()) == 0);
    CHECK(g->voisinsLibres(t5.coordTermite()) == 8);
    CHECK(g->voisinsLibres(Coord{5, 6}) == 0);  // 8 toiles autour de la case
    CHECK(g->voisinsLibres(Coord{12, 12}) == 8);
    CHECK(g->voisinsLibres(Coord{19, 19}) == 3);
    //Invariants
    CHECK_THROWS_AS(g->voisinsLibres(Coord{20, 19}), invalid_argument);
    
    //findVoisinsLibres
    CHECK(g->findVoisinsLibres(t3.coordTermite()) == Coord{2, 2});
    CHECK(g->findVoisinsLibres(t5.coordTermite()) == Coord{8, 8});
    CHECK(g->findVoisinsLibres(Coord{0, 19}) == Coord{1, 19});  // Coordonnées invalides
    //Invariants
    CHECK_THROWS_AS(g->findVoisinsLibres(t4.coordTermite()), invalid_argument);  // Pas de voisin libre
    CHECK_THROWS_AS(g->voisinsLibres(Coord{20, 19}), invalid_argument);  // Coordonnées invalides
}

TEST_CASE("Opérateur== Grille") {
    // On initialise le monde de test et une grille vide
    Grille g1 = Grille{};
    Monde m = creeMondeTest();
    Grille *g2 = m.getGrille();
    
    CHECK(*g2 == *g2);
    CHECK(g1 == g1);
    CHECK_FALSE(g1 == *g2);
}


// termite.cpp
TEST_CASE("Constructeurs Termite et prédicats") {
    srand(time(NULL));  // Pour la part d'aléatoire dans le constructeur de t
    Termite t{0, Coord{0, 0}, 0};
    Termite t1{1, Coord{1, 1}, Direction::S, 1};
    Termite t2{2, Coord{2, 2}, Direction::N, true, 5, 0, dureeToile, false};
    
    // porteBrindille
    CHECK(t2.porteBrindille());
    CHECK_FALSE(t.porteBrindille());
    CHECK_FALSE(t1.porteBrindille());
    
    // directionTermite
    CHECK(t1.directionTermite() == Direction::S);
    CHECK(t2.directionTermite() == Direction::N);
    
    // idTTermite
    CHECK(t.idTTermite() == 0);
    CHECK(t1.idTTermite() == 1);
    CHECK(t2.idTTermite() == 2);
    
    // sablierTermite
    CHECK(t.sablierTermite() == 0);
    CHECK(t1.sablierTermite() == 0);
    CHECK(t2.sablierTermite() == 5);
    
    // coordTermite
    CHECK(t.coordTermite() == Coord{0, 0});
    CHECK(t1.coordTermite() == Coord{1, 1});
    CHECK(t2.coordTermite() == Coord{2, 2});
    
    // equipeTermite
    CHECK(t.equipeTermite() == 0);
    CHECK(t1.equipeTermite() == 1);
    CHECK(t2.equipeTermite() == 0);
    
    // toileTermite
    CHECK(t.toileTermite() == 0);
    CHECK(t1.toileTermite() == 0);
    CHECK(t2.toileTermite() == 2);
    
    // estEnVie
    CHECK(t.estEnVie());
    CHECK(t1.estEnVie());
    CHECK_FALSE(t2.estEnVie());
    
    // Invariants
    CHECK_THROWS_AS((Termite{-1, Coord{1, 0}, 1}), invalid_argument);  // ID invalide
    CHECK_THROWS_AS((Termite{40, Coord{1, 0}, 1}), invalid_argument);  // ID invalide
    CHECK_THROWS_AS((Termite{1, Coord{1, 0}, -1}), invalid_argument);  // Equipe invalide
    CHECK_THROWS_AS((Termite{4, Coord{1, 0}, 4}), invalid_argument);  // Equipe invalide
    CHECK_THROWS_AS((Termite{0, Coord{20, 0}, 1}), invalid_argument);  // Coordonnées invalides
    CHECK_THROWS_AS((Termite{0, Coord{1, -1}, 1}), invalid_argument);  // Coordonnées invalides
    CHECK_THROWS_AS((Termite{-1, Coord{1, 0}, Direction::N, 1}), invalid_argument);  // ID invalide
    CHECK_THROWS_AS((Termite{40, Coord{1, 0}, Direction::N, 1}), invalid_argument);  // ID invalide
    CHECK_THROWS_AS((Termite{1, Coord{1, 0}, Direction::N, -1}), invalid_argument);  // Equipe invalide
    CHECK_THROWS_AS((Termite{4, Coord{1, 0}, Direction::N, 4}), invalid_argument);  // Equipe invalide
    CHECK_THROWS_AS((Termite{0, Coord{20, 0}, Direction::N, 1}), invalid_argument);  // Coordonnées invalides
    CHECK_THROWS_AS((Termite{0, Coord{1, -1}, Direction::N, 1}), invalid_argument);  // Coordonnées invalides
    CHECK_THROWS_AS((Termite{2, Coord{2, 2}, Direction::N, true, 7, 1, 0, false}), invalid_argument);  // Valeur sablier invalide
    CHECK_THROWS_AS((Termite{2, Coord{2, 2}, Direction::N, true, -1, 1, 0, false}), invalid_argument);  // Valeur sablier invalide
    CHECK_THROWS_AS((Termite{2, Coord{2, 2}, Direction::N, true, 1, -1, 0, false}), invalid_argument);  // Equipe invalide
    CHECK_THROWS_AS((Termite{2, Coord{2, 2}, Direction::N, true, 1, 4, 0, false}), invalid_argument);  // Equipe invalide
    CHECK_THROWS_AS((Termite{2, Coord{2, 2}, Direction::N, true, 1, 1, -1, false}), invalid_argument);  // Valeur toile invalide
    CHECK_THROWS_AS((Termite{2, Coord{2, 2}, Direction::N, true, 1, 1, 3, false}), invalid_argument);  // Valeur toile invalide
    CHECK_THROWS_AS((Termite{2, Coord{20, 2}, Direction::N, true, 1, 1, 0, false}), invalid_argument);  // Coordonnées invalides
    CHECK_THROWS_AS((Termite{2, Coord{2, -1}, Direction::N, true, 1, 1, 0, false}), invalid_argument);  // Coordonnées invalides
}

TEST_CASE("Méthodes Termite") {
    srand(time(NULL));  // Pour la part d'aléatoire dans le constructeur de t
    Termite t{0, Coord{1, 1}, Direction::NE, 1};
    Termite t1{1, Coord{1, 2}, Direction::S, 1};
    Termite t2{2, Coord{0, 0}, Direction::W, 1};
    
    // toString
    CHECK(t.toString() == "/");
    CHECK(t1.toString() == "|");
    CHECK(t2.toString() == "-");
    
    // devant
    CHECK(t.devant() == Coord{0, 2});
    CHECK(t1.devant() == Coord{2, 2});
    // Invariants
    CHECK_THROWS_AS(t2.devant(), invalid_argument);  // Case inexistante
    
    // tueTermite
    Termite t9{2, Coord{0, 0}, Direction::W, 1};
    t9.tueTermite();
    CHECK_FALSE(t9.estEnVie());

    // tourneADroite
    t.tourneADroite(); t1.tourneADroite(); t2.tourneADroite();
    CHECK(t.directionTermite() == Direction::E);
    CHECK(t1.directionTermite() == Direction::SW);
    CHECK(t2.directionTermite() == Direction::NW);
    
    // tourneAGauche
    Termite t3{0, Coord{1, 1}, Direction::NE, 1};
    Termite t4{1, Coord{1, 2}, Direction::S, 1};
    Termite t5{2, Coord{0, 0}, Direction::W, 1};
    t3.tourneAGauche(); t4.tourneAGauche(); t5.tourneAGauche();
    CHECK(t3.directionTermite() == Direction::N);
    CHECK(t4.directionTermite() == Direction::SE);
    CHECK(t5.directionTermite() == Direction::SW);
    
    // tourneAleat
    Termite t6{0, Coord{1, 1}, Direction::NE, 1};
    Termite t7{1, Coord{1, 2}, Direction::S, 1};
    Termite t8{2, Coord{0, 0}, Direction::W, 1};
    t6.tourneAleat(); t7.tourneAleat(); t8.tourneAleat();
    Direction d = t6.directionTermite();
    Direction d1 = t7.directionTermite();
    Direction d2 = t8.directionTermite();
    CHECK((d == Direction::N || d == Direction::E));
    CHECK((d1 == Direction::SE || d1 == Direction::SW));
    CHECK((d2 == Direction::SW || d2 == Direction::NW));
}

TEST_CASE("Méthodes environnement Termite") {
    // On initialise le monde de test
    Monde m = creeMondeTest();
    Grille *g = m.getGrille();
    vector<Termite> *vT = m.getVect();
    vT->push_back(Termite{5, Coord{4, 4}, Direction::N, 1});
    g->poseTermite(Coord{4, 4}, 5);
    g->poseToile(Coord{3, 4});
    Termite t = (*vT)[0]; Termite t1 = (*vT)[1];
    Termite t2 = (*vT)[2]; Termite t3 = (*vT)[3];
    Termite t4 = (*vT)[4]; Termite t5 = (*vT)[5];
    
    // laVoieEstLibre
    CHECK(t2.laVoieEstLibre(*g));
    CHECK(t5.laVoieEstLibre(*g));  // Le termite peut avancer sur une toile
    CHECK_FALSE(t.laVoieEstLibre(*g));  // Case inexistante
    CHECK_FALSE(t1.laVoieEstLibre(*g));  // Case brindille
    CHECK_FALSE(t3.laVoieEstLibre(*g));  // Case Termite
    CHECK_FALSE(t4.laVoieEstLibre(*g));  // Case Termite diagonale
    
    // brindilleEnFace
    CHECK(t1.brindilleEnFace(*g));
    CHECK_FALSE(t.brindilleEnFace(*g));  // Case inexistante
    CHECK_FALSE(t2.brindilleEnFace(*g));  // Case Vide
    CHECK_FALSE(t3.brindilleEnFace(*g));  // Case Termite
    CHECK_FALSE(t4.brindilleEnFace(*g));  // Case termite diagonale
}

TEST_CASE("Méthodes mouvement Termite") {
    // On initialise le monde de test
    srand(time(NULL));
    Monde m = creeMondeTest();
    Grille *g = m.getGrille();
    vector<Termite> *vT = m.getVect();
    
    // avance
    (*vT)[2].avance(*g);
    CHECK((*vT)[2].coordTermite() == Coord{2, 2});
    CHECK(g->estVide(Coord{2, 1}));
    CHECK(g->contientTermite(Coord{2,2}));
    // Invariants
    CHECK_THROWS_AS((*vT)[1].avance(*g), invalid_argument);  // Le termite ne peut pas avancer
    // On vérifie la cohérence entre le vecteur et la grille après les mouvements
    CHECK(m.coherenceGrilleVect());
    
    // creuseTunnel
    (*vT)[2].creuseTunnel(*g);
    CHECK(g->estVide(Coord{2,2}));
    CHECK(g->contientTermite((*vT)[2].coordTermite()));
    CHECK(g->numeroTermite((*vT)[2].coordTermite()) == 2);
    g->enleveTermite((*vT)[2].coordTermite());
    g->poseTermite(Coord{2, 2}, 2);
    (*vT)[2] = Termite{2, Coord{2, 2}, Direction::N, true, 0, 1, 0, false};  // Porte une brindille
    // Invariants
    CHECK_THROWS_AS((*vT)[2].creuseTunnel(*g), invalid_argument);  // Le termite ne peut pas creuser de tunnel
    // On vérifie la cohérence entre le vecteur et la grille après les mouvements
    CHECK(m.coherenceGrilleVect());
    
    // marcheAleatoire
    // On réinitialise un monde de test neuf
    Monde m1 = creeMondeTest();
    Grille *g1 = m1.getGrille();
    vector<Termite> *vT1 = m1.getVect();
    // On force le test de chaque cas avec 1 = tourne, 2 = tente d'avancer, 3 = creuse un tunnel
    for (int i = 1; i < 4; i++) {  
        (*vT1)[2].marcheAleatoire(*g1, i);
        if (g1->estVide(Coord{2, 1})) {
            if (((*vT1)[2].coordTermite() == Coord{2, 2}))  // Le termite a avancé
                CHECK(g1->contientTermite(Coord{2,2}));
            else  // Le termite a creusé un tunnel
                CHECK(g1->contientTermite((*vT1)[2].coordTermite()));
        } else {  // Le termite a tourné
            CHECK(((*vT1)[2].directionTermite() == Direction::SE or (*vT1)[2].directionTermite() == Direction::NE));
            CHECK((*vT1)[2].coordTermite() == Coord{2, 1});
            CHECK(g1->estVide(Coord{2, 2}));
            CHECK(g1->contientTermite(Coord{2, 1}));
        }
        // On remet la grille en l'état pour le cas suivant
        g1->enleveTermite((*vT1)[2].coordTermite());
        g1->poseTermite(Coord{2, 1}, 2);
        (*vT1)[2] = Termite{2, Coord{2, 1}, Direction::E, false, 0, 1, 0, false};

    }
    // On vérifie la cohérence entre le vecteur et la grille après les mouvements
    CHECK(m1.coherenceGrilleVect());
    
    // chargeBrindille
    // On réinitialise un monde de test neuf
    Monde m2 = creeMondeTest();
    Grille *g2 = m2.getGrille();
    vector<Termite> *vT2 = m2.getVect();
    g2->enleveBrindille(Coord{0, 2});
    g2->poseBrindille(Coord{0, 2}, -1, true);

    // On peut charger la brindille
    (*vT2)[1].chargeBrindille(*g2);  // Brindille non marquée
    CHECK((*vT2)[1].porteBrindille());
    CHECK((*vT2)[1].sablierTermite() == 6);
    CHECK(g2->estVide(Coord{0, 2}));
    g2->poseBrindille(Coord{0, 2}, 3, true);
    g2->enleveTermite((*vT2)[1].coordTermite());
    g2->poseTermite(Coord{1, 2}, 1);
    (*vT2)[1] = Termite{1, Coord{1, 2}, Direction::N, false, 0, 1, 0, false};
    (*vT2)[1].chargeBrindille(*g2);  // Brindille d'une autre équipe
    CHECK((*vT2)[1].porteBrindille());
    CHECK((*vT2)[1].sablierTermite() == 6);
    CHECK(g2->estVide(Coord{0, 2}));
    
    // Invariants
    // Brindille non transportable
    CHECK_THROWS_AS((*vT2)[1].chargeBrindille(*g2), invalid_argument);  // La brindille est de la même équipe que le termite
    g2->poseBrindille(Coord{0, 2}, -1, false);
    CHECK_THROWS_AS((*vT2)[1].chargeBrindille(*g2), invalid_argument);  // La brindille n'est pas transportable (=nid)
    
    // La case cible est vide
    g2->enleveTermite((*vT2)[1].coordTermite());
    g2->poseTermite(Coord{1, 2}, 1);   
    (*vT2)[1] = Termite{1, Coord{1, 2}, Direction::N, false, 0, 1, 0, false};
    CHECK_THROWS_AS((*vT2)[1].chargeBrindille(*g2), invalid_argument);
    
    // La durée du sablier n'est pas à 0
    g2->enleveBrindille(Coord{0, 2});
    g2->poseBrindille(Coord{0, 2}, 1, true);
    g2->enleveTermite((*vT2)[1].coordTermite());
    g2->poseTermite(Coord{1, 2}, 1);    
    (*vT2)[1] = Termite{1, Coord{1, 2}, Direction::N, false, 1, 1, 0, false};
    CHECK_THROWS_AS((*vT2)[1].chargeBrindille(*g2), invalid_argument);
    
    // Le termite porte déjà une brindille
    g2->enleveTermite((*vT2)[1].coordTermite());
    g2->poseTermite(Coord{1, 2}, 1);    
    (*vT2)[1] = Termite{1, Coord{1, 2}, Direction::N, true, 0, 1, 0, false};
    CHECK_THROWS_AS((*vT2)[1].chargeBrindille(*g2), invalid_argument);
    
    // On vérifie la cohérence entre le vecteur et la grille après les mouvements
    CHECK(m2.coherenceGrilleVect());
    
    // dechargeBrindille
    // on peut décharger la brindille
    (*vT2)[1].dechargeBrindille(*g2);
    CHECK_FALSE((*vT2)[1].porteBrindille());
    CHECK((*vT2)[1].sablierTermite() == 6);
    CHECK(g2->contientBrindille(Coord{0, 3}));
    CHECK(g2->contientEquipe(Coord{0, 3}) == (*vT2)[1].equipeTermite());
    
    // Le termite n'est pas au bon nid
    g2->enleveBrindille(Coord{0, 2});
    g2->poseBrindille(Coord{0, 2}, -1, true);
    g2->enleveTermite((*vT2)[1].coordTermite());
    g2->poseTermite(Coord{1, 2}, 1);
    (*vT2)[1] = Termite{1, Coord{1, 2}, Direction::N, true, 0, 1, 0, false};
    CHECK_THROWS_AS((*vT2)[1].dechargeBrindille(*g2), invalid_argument);  // La brindille est une brindille neutre
    g2->enleveBrindille(Coord{0, 2});
    g2->poseBrindille(Coord{0, 2}, 0, true);
    CHECK_THROWS_AS((*vT2)[1].dechargeBrindille(*g2), invalid_argument);  // La brindille est une brindille adverse
    
    // La case cible ne contient pas de brindille
    g2->enleveBrindille(Coord{0, 2});
    g2->enleveTermite((*vT2)[1].coordTermite());
    g2->poseTermite(Coord{1, 2}, 1);    
    (*vT2)[1] = Termite{1, Coord{1, 2}, Direction::N, true, 0, 1, 0, false};
    CHECK_THROWS_AS((*vT2)[1].dechargeBrindille(*g2), invalid_argument);
    
    // La durée du sablier n'est pas terminée
    g2->poseBrindille(Coord{0, 2}, 1, true);
    g2->enleveTermite((*vT2)[1].coordTermite());
    g2->poseTermite(Coord{1, 2}, 1);    
    (*vT2)[1] = Termite{1, Coord{1, 2}, Direction::N, true, 1, 1, 0, false};
    CHECK_THROWS_AS((*vT2)[1].dechargeBrindille(*g2), invalid_argument);
    
    // Le termite ne porte aucune brindille
    g2->enleveTermite((*vT2)[1].coordTermite());
    g2->poseTermite(Coord{1, 2}, 1);
    (*vT2)[1] = Termite{1, Coord{1, 2}, Direction::N, false, 0, 1, 0, false};
    CHECK_THROWS_AS((*vT2)[1].dechargeBrindille(*g2), invalid_argument); 
    
    // Aucune case vide autour de la brindille cible
    g2->enleveTermite((*vT2)[2].coordTermite());
    g2->poseTermite(Coord{16, 16}, 2);  
    (*vT2)[2] = Termite{2, Coord{16, 16}, Direction::N, true, 0, 1, 0, false};
    g2->poseBrindille(Coord{15, 16}, 1, true);
    g2->poseToile(Coord{15, 15}); g2->poseToile(Coord{15, 17});
    g2->poseToile(Coord{14, 15}); g2->poseToile(Coord{14, 16});
    g2->poseToile(Coord{14, 17}); g2->poseToile(Coord{16, 15});
    g2->poseToile(Coord{16, 17});
    CHECK_THROWS_AS((*vT2)[2].dechargeBrindille(*g2), invalid_argument); 
    
    // On vérifie la cohérence entre le vecteur et la grille après les mouvements
    CHECK(m2.coherenceGrilleVect());
}

TEST_CASE("Méthode Termite: vieTermite") {
    // On initialise le monde de test
    srand(time(NULL));
    Monde m = creeMondeTest();
    Grille *g = m.getGrille();
    vector<Termite> *vT = m.getVect();
    
    // On sauvegarde la valeur avant le tour de la grille et des termites
    vector<Termite> saveVT = *vT;
    Grille saveG = *g;
    
    // On fait bouger chaque termite une fois
    (*vT)[0].vieTermite(*g);
    (*vT)[1].vieTermite(*g);
    (*vT)[2].vieTermite(*g);
    (*vT)[3].vieTermite(*g);
    (*vT)[4].vieTermite(*g);
    
    // On vérifie qu'uniquement un seul paramètre pour chaque termite a été modifié (soit pos, soit dir, soit brind)
    for (int i = 0; i < saveVT.size(); i++) {
        CHECK(((
               not(saveVT[i].coordTermite() == (*vT)[i].coordTermite())
               and saveVT[i].directionTermite() == (*vT)[i].directionTermite()
               and saveVT[i].porteBrindille() == (*vT)[i].porteBrindille())
               or (
               saveVT[i].coordTermite() == (*vT)[i].coordTermite()
               and saveVT[i].directionTermite() != (*vT)[i].directionTermite()
               and saveVT[i].porteBrindille() == (*vT)[i].porteBrindille())
               or (
               saveVT[i].coordTermite() == (*vT)[i].coordTermite() 
               and saveVT[i].directionTermite() == (*vT)[i].directionTermite()
               and saveVT[i].porteBrindille() != (*vT)[i].porteBrindille()
               )));
    }
    
    // On vérifie le bon fonctionnement des toiles
    vT->push_back(Termite{5, Coord{5, 5}, Direction::E, false, 0, 1, 0, true});
    g->poseToile(Coord{5, 6}); g->poseBrindille(Coord{5, 7}, -1, true);
    g->poseTermite(Coord{5, 5}, 5);
    (*vT)[5].marcheAleatoire(*g, 2);
    CHECK((*vT)[5].coordTermite() == Coord{5, 6});
    CHECK((*vT)[5].toileTermite() == 2);
    (*vT)[5].vieTermite(*g);  // 1er tour de blocage
    CHECK((*vT)[5].coordTermite() == Coord{5, 6});
    CHECK((*vT)[5].directionTermite() == Direction::E);
    CHECK_FALSE((*vT)[5].porteBrindille());
    CHECK((*vT)[5].toileTermite() == 1);
    (*vT)[5].vieTermite(*g);  // 2ème tours de blocage
    CHECK((*vT)[5].coordTermite() == Coord{5, 6});
    CHECK((*vT)[5].directionTermite() == Direction::E);
    CHECK_FALSE((*vT)[5].porteBrindille());
    CHECK((*vT)[5].toileTermite() == 0);
    (*vT)[5].vieTermite(*g);  // Termite débloqué, récupère une brindille
    CHECK((*vT)[5].coordTermite() == Coord{5, 6});
    CHECK((*vT)[5].directionTermite() == Direction::E);
    CHECK((*vT)[5].porteBrindille());
    CHECK((*vT)[5].toileTermite() == 0);
    // Soit tourne et se rebloque pour deux tours, soit avance ou creuse un tunnel, on teste les deux cas en forcant les tests:
    // Cas avance (<=>creuse)
    (*vT)[5].marcheAleatoire(*g, 2);
    CHECK(((*vT)[5].coordTermite() == Coord{5, 7}));
    CHECK((*vT)[5].directionTermite() == Direction::E);
    CHECK((*vT)[5].porteBrindille());
    CHECK((*vT)[5].toileTermite() == 0);
    // Cas tourne
    (*vT)[5] = Termite{5, Coord{5, 5}, Direction::E, false, 0, 1, 0, true};
    g->enleveTermite(Coord{5, 7}); g->poseTermite(Coord{5, 5}, 5); 
    g->poseBrindille(Coord{5, 7}, -1, true);  g->poseToile(Coord{5, 6});
    (*vT)[5].marcheAleatoire(*g, 2); (*vT)[5].vieTermite(*g); 
    (*vT)[5].vieTermite(*g); (*vT)[5].vieTermite(*g);
    (*vT)[5].marcheAleatoire(*g, 1);
    CHECK(((*vT)[5].coordTermite() == Coord{5, 6}));
    CHECK(((*vT)[5].directionTermite() == Direction::SE or (*vT)[5].directionTermite() == Direction::NE));
    CHECK((*vT)[5].porteBrindille());
    CHECK((*vT)[5].toileTermite() == 2);
    
    // On vérifie qu'un termite mort n'a pas bougé
    vT->push_back(Termite{6, Coord{10, 10}, Direction::E, false, 0, 1, 0, false});
    g->poseTermite(Coord{10, 10}, 6);
    // On sauvegarde la valeur avant le tour de la grille et des termites
    saveVT = *vT;
    saveG = *g;
    // On fait bouger le termite mort
    (*vT)[6].vieTermite(*g);
    CHECK((*vT == saveVT and *g == saveG)); 
    
    // On vérifie la cohérence entre le vecteur et la grille après les mouvements
    CHECK(m.coherenceGrilleVect());
}

TEST_CASE("Opérateur== Termite") {
    Termite t1{0, Coord{2, 2}, Direction::N, true, 5, 1, 0, false};
    Termite t2{0, Coord{2, 2}, Direction::N, true, 5, 1, 0, false};
    Termite t3{1, Coord{2, 2}, Direction::N, true, 5, 1, 0, false};
    Termite t4{0, Coord{2, 1}, Direction::N, true, 5, 1, 0, false};
    Termite t5{0, Coord{2, 2}, Direction::S, true, 5, 1, 0, false};
    Termite t6{0, Coord{2, 2}, Direction::N, false, 5, 1, 0, false};
    Termite t7{0, Coord{2, 2}, Direction::N, true, 4, 1, 0, false};
    Termite t8{0, Coord{2, 2}, Direction::N, true, 5, 0, 0, false};
    Termite t9{0, Coord{2, 2}, Direction::N, true, 4, 1, 0, false};
    Termite t10{0, Coord{2, 2}, Direction::N, true, 5, 0, dureeToile, false};
    Termite t11{0, Coord{2, 2}, Direction::N, true, 5, 0, dureeToile, true};
    
    CHECK(t1 == t2);
    CHECK_FALSE(t1 == t3); CHECK_FALSE(t1 == t4);
    CHECK_FALSE(t1 == t5); CHECK_FALSE(t1 == t6);
    CHECK_FALSE(t1 == t7); CHECK_FALSE(t1 == t8);
    CHECK_FALSE(t9 == t10); CHECK_FALSE(t10 == t11);
}

TEST_CASE("Opérateur== vector<Termite>") {
    Termite t1{0, Coord{2, 2}, Direction::N, true, 5, 1, 0, false};
    Termite t2{0, Coord{2, 2}, Direction::N, true, 5, 1, 0, false};
    Termite t3{1, Coord{2, 2}, Direction::N, true, 5, 1, 0, false};
    vector<Termite> vT1 = {t1, t1, t1};
    vector<Termite> vT2 = {t1, t1};
    vector<Termite> vT3 = {t1, t3, t1};
    
    CHECK(vT1 == vT1);
    CHECK_FALSE(vT1 == vT2); CHECK_FALSE(vT1 == vT3);
}


// monde.cpp
TEST_CASE("Constructeur Monde") {
    // On initialise un monde vide
    Monde m = Monde{};
    Grille *g = m.getGrille();
    vector<Termite> *vT = m.getVect(); 
    
    // Vérification de la grille vide
    for (int i = 0; i < tailleGrille; i++) {
        for (int j = 0; j < tailleGrille; j++)
            CHECK(g->estVide(Coord{i, j}));
    }
    
    // Vérification du vecteur vide
    CHECK(vT->size() == 0);
}

TEST_CASE("coherenceGrilleVect") {
     // On initialise le monde de test
    srand(time(NULL));
    Monde m = creeMondeTest();
    Grille *g = m.getGrille(); 
    vector<Termite> *vT = m.getVect();
    
    // On vérifie la cohérence entre la grille et le vecteur à l'initialisation
    CHECK(m.coherenceGrilleVect());
    
    // On fait bouger chaque termite une fois
    (*vT)[0].vieTermite(*g);
    (*vT)[1].vieTermite(*g);
    (*vT)[2].vieTermite(*g);
    (*vT)[3].vieTermite(*g);
    (*vT)[4].vieTermite(*g);
    
    // On vérifie la cohérence entre la grille et le vecteur après le mouvement
    CHECK(m.coherenceGrilleVect());
    // Le main vérifiera plus en profondeur la cohérence entre la grille et le vecteur
}

TEST_CASE("coherenceFichiers") {
    // On crée le monde comme on le ferait dans projet.cpp
    srand(time(NULL));
    Monde m = creeMondeTest();
    Grille *g = m.getGrille(); 
    vector<Termite> *vT = m.getVect();
    
    // On vérifie la cohérence entre le monde dans ce fichier et dans la structure monde à l'initialisation
    CHECK(m.coherenceFichiers(*g, *vT));
    
    // On fait bouger chaque termite une fois
    (*vT)[0].vieTermite(*g);
    (*vT)[1].vieTermite(*g);
    (*vT)[2].vieTermite(*g);
    (*vT)[3].vieTermite(*g);
    (*vT)[4].vieTermite(*g);
    
    // On vérifie la cohérence entre le monde dans ce fichier et dans la structure monde après le mouvement
    CHECK(m.coherenceFichiers(*g, *vT));
}

TEST_CASE("brindillesLibres") {
    // On initialise un monde vide
    Monde m = creeMondeTest();
    Grille *g = m.getGrille(); 
    vector<Termite> *vT = m.getVect();
    
    CHECK(m.brindillesLibres());
    g->enleveBrindille(Coord{1, 0});
    CHECK_FALSE(m.brindillesLibres());  // Aucune brindille non-marquée
    g->enleveBrindille(Coord{0, 2});
    CHECK_FALSE(m.brindillesLibres());  // Aucune brindille sur la grille
}

TEST_CASE("compteBrindilles") {
    // On initialise un monde vide
    Monde m = creeMondeTest();
    Grille *g = m.getGrille(); 
    vector<Termite> *vT = m.getVect();
    
    CHECK(m.compteBrindilles() == vector<int>{0, 1, 0, 0});
    g->poseBrindille(Coord{2, 2}, 1, true);
    g->poseBrindille(Coord{2, 0}, 3, true);
    CHECK(m.compteBrindilles() == vector<int>{0, 2, 0, 1});
    g->enleveBrindille(Coord{2, 2});
    g->enleveBrindille(Coord{2, 0});
    g->enleveBrindille(Coord{0, 2});
    g->poseBrindille(Coord{0, 2}, -1, true);
    CHECK(m.compteBrindilles() == vector<int>{0, 0, 0, 0});
    g->poseBrindille(Coord{2, 0}, 0, true);
    g->enleveBrindille(Coord{0, 2});
    CHECK(m.compteBrindilles() == vector<int>{1, 0, 0, 0});
}

TEST_CASE("termitesVivants") {
    // On initialise un monde aléatoire
    Monde m = Monde{};
    m.monde();
    
    CHECK(m.termitesVivants());
    for (int i = 0; i < nbTermites; i++)
        m.insecticide(2000);
    CHECK_FALSE(m.termitesVivants());  // On a tué tous les termites
}

TEST_CASE("insecticide") {
    // On initialise un monde aléatoire
    Monde m = Monde{};
    m.monde();
    vector<Termite> *vT = m.getVect();
    bool mort = false;
    
    // On vérifie qu'aucun termite n'a été tué (le tour 200 n'est pas un tour de mort pour 40 termites)
    m.insecticide(200);
    for (Termite t : *vT) {
        if (not(t.estEnVie())) {
            mort = true;
            break;
        }
    }
    CHECK_FALSE(mort);
    
    // On vérifie qu'un termite est mort
    m.insecticide(1000);
    for (Termite t : *vT) {
        if (not(t.estEnVie())) {
            mort = true;
            break;
        }
    }
    CHECK(mort);
    
    // Invariants
    for (int i = 0; i < nbTermites - 1; i++)
        m.insecticide(2000);  // On tue tous les termites de la grille
    CHECK_THROWS_AS(m.insecticide(2000), invalid_argument);  // Plus aucun termite en vie
        
    // On vérifie la cohérence entre le vecteur et la grille après les mouvements
    CHECK(m.coherenceGrilleVect());
}

TEST_CASE("Initialisation de la simulation") {
    // On initialise un monde aléatoire
    Monde m = Monde{};
    Grille *g = m.getGrille();
    vector<Termite> *vT = m.getVect(); 
    m.monde();
    
    // Vérification du nombre de termites dans la grille
    int t = 0; vector<int> v;
    for (int i = 0; i < tailleGrille; i++) {
        for (int j = 0; j < tailleGrille; j++)
            if (g->contientTermite(Coord{i, j})) {
                t++; v.push_back(g->numeroTermite(Coord{i, j}));
            }
    }
    CHECK(t == vT->size());
    CHECK(t == nbTermites);
    
    // On vérifie que tous les termites soient uniques
    int s = 0;
    for (int i : v) {
        s += i;
        CHECK(unique(v, i));
    }
    CHECK(s == ((vT->size()-1) * ((vT->size()-1) + 1)) / 2);
    // On vérifie que l'on a bien la somme des indices des termites qui vaut la somme des entiers de 0 à vt->size()-1 (formule              mathématique de la somme des entiers de 0 à n), car l'indice des termites doit aller de 0 à vt->size()-1. On vérifie également        que tous les éléments de v (les indices des termites) sont uniques
    
    // On vérifie la cohérence entre le vecteur et la grille après les mouvements
    CHECK(m.coherenceGrilleVect());
}


// main
int main(int argc, const char **argv) {
    doctest::Context context(argc, argv);
    int test_result = context.run();
    if (context.shouldExit()) return test_result;
    srand(time(NULL));
      
    // Opérateur d'affichage de Coord
    Coord c1{1, 2};
    cout << "Test opérateur d'affichage Coord: doit afficher (1,2)" << endl;
    cout << c1 << endl << endl;
      
    // Opérateur d'affichage de Direction
    Direction d1 = Direction::NW; Direction d2 = Direction::W;
    cout << "Test opérateur d'affichage Direction: doit afficher NW, W" << endl;
    cout << d1 << ", " << d2 << endl << endl;
    
     // Opérateur d'affichage de vector<int>
    vector<int> v = {1, 2, 3, 4};
    cout << "Test opérateur d'affichage vector<int>: doit afficher {1, 2, 3, 4}" << endl;
    cout << v << endl << endl;
    
    // Opérateur d'affichage de Monde
    Monde m = Monde{};
    m.monde();
    cout << "Test opérateur d'affichage Monde:" << endl;
    cout << m << endl << endl;
    
    // Opérateur d'affichage de Termite
    Termite t = Termite{0, Coord{0, 0}, 1};
    cout << "Test opérateur d'affichage Termite: doit afficher" << endl;
    cout << "ID: " << setw(2) << 0 << " | Coordonnées: (0,0)" << " | Direction: " << t.directionTermite() << " | Brindille: " << 0 << " | Sablier: " << 0 << " | Equipe: " << 1 << " | Toile:" << 0 << " | En vie: " << 1 << endl;
    cout << "Résultat:" << endl << t << endl;
    
    // Opérateur d'affichage de vector<Termite>
    vector<Termite> vT = {Termite{0, Coord{0, 0}, 1}, Termite{1, Coord{1, 0}, 1}, Termite{2, Coord{0, 1}, 1}};
    cout << "Test opérateur d'affichage vector<Termite>: doit afficher" << endl;
    cout << "ID: " << setw(2) << 0 << " | Coordonnées: (0,0)" << " | Direction: " << vT[0].directionTermite() << " | Brindille: " << 0 << " | Sablier: " << 0 << " | Equipe: " << 1 << " | Toile:" << 0 << " | En vie: " << 1 << endl;
    cout << "ID: " << setw(2) << 1 << " | Coordonnées: (1,0)" << " | Direction: " << vT[1].directionTermite() << " | Brindille: " << 0 << " | Sablier: " << 0 << " | Equipe: " << 1 << " | Toile:" << 0 << " | En vie: " << 1 << endl;
    cout << "ID: " << setw(2) << 2 << " | Coordonnées: (0,1)" << " | Direction: " << vT[2].directionTermite() << " | Brindille: " << 0 << " | Sablier: " << 0 << " | Equipe: " << 1 << " | Toile:" << 0 << " | En vie: " << 1 << endl;
    cout << "Résultat:" << endl << vT << endl;
    
    // Test fonction toString()
    cout << "Test toString()" << endl;
    Termite t1{0, Coord{0, 0}, 1}; Termite t2{0, Coord{0, 0}, 1};
    cout << t1.toString() << " | " << t1.directionTermite() << endl;
    cout << t2.toString() << " | " << t2.directionTermite() << endl;
    
    // Test de la cohérence entre projet.cpp et monde.cpp
    cout << endl << "Test de la cohérence entre projet.cpp et monde.cpp:" << endl;
    Monde m1 = creeMondeTest();
    Grille *g1 = m1.getGrille(); vector<Termite> *vT1 = m1.getVect();
    cout << endl << "Grille de base: " << endl << m1 << endl;
    cout << "vecteur<Termite> associé:" << endl << *vT1 << endl;
    
    // Changement de l'état d'un termite
    cout << "On tourne la termite 0 d'un cran à droite:" << endl;
    cout << "Nouvelle grille: " << endl;
    ((*vT1)[0]).tourneADroite();
    cout << m1 << endl;
    cout << "Nouveau vecteur<Termite>:" << endl << *vT1 << endl; 
    
    // Vérification de la cohérence entre m et vT1/g1
    cout << "On vérifie que les valeurs stockées dans m soient cohérentes:" << endl;
    cout << "Grille de m:" << endl << m1 << endl;
    vector<Termite> *vT2 = m1.getVect(); cout << "vecteur<Termite> de m:" << endl << *vT2 << endl;
    if (not(*vT1 == *vT2))
        throw invalid_argument("Plus de cohérence entre les vecteurs Termite de projet.cpp et monde.cpp");
    Grille *g2 = m1.getGrille();
    if (not(*g2 == *g1))
        throw invalid_argument("Plus de cohérence entre les grilles de projet.cpp et monde.cpp");

    return 0;
}