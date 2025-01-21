// -*- coding: utf-8 -*-
#include <unistd.h>
#include "coord.hpp"
#include "grille.hpp"
#include "termite.hpp"
#include "monde.hpp"

// VOIR LE PDF "Description Générale du Projet"
// TEST RETOUR VERS LE FUTUR

int main() {
    srand(time(NULL));
    
    
    // On fabrique une grille aléatoirement et on l'affiche pour voir l'état de base de la simulation
    Monde m = Monde{};
    Grille *g = m.getGrille();  // On récupère un pointeur vers la grille du monde
    vector<Termite> *vT = m.getVect();  // On récupère un pointeur vers le vecteur de termites du monde
    
    
    // Initialisation
    cout << *vT; m.monde();
    cout << endl << "Grille de départ: " << endl << m << endl;
    cout << *vT << endl << endl;
    
    cout << "Souhaitez vous jouer le mode 1, 2 ou 3 ? Tapez 1 puis Entrée et ainsi passer vous-même les tours, tapez 2 puis Entrée afin qu'ils passent     automatiquement. Tapez 3 puis appuyez sur Entrée pour lancer 600 simulations et obtenir le nombre de brindilles par équipe. Le mode 2 dure 30 secondes. Le mode 3 dure 10 minutes. Attention, le mode 2 n'offre aucune possibilité d'afficher l'état du vecteur de termites, et le mode 3 nevous laissera ni le temps d'afficher le vecteur, ni d'afficher la grille. Vous pouvez arrêter un mode avec Ctrl+C" << endl;
    string s;
    do {
        cout << endl << "Tapez 1, 2 ou 3: ";
        s = getchar();  // Pour ne récupérer que le premier caractère et les caractères '\n' et ' '
    } while (s != "1" and s != "2" and s != "3"); 
    cout << endl;
    int i = 1;
    
    
    // Lancement de la simulation
    // On impose une limite de 2000 tours, suffisants pour laisser le système se stabiliser
    if (s == "1") {
        // Bug et passe directement au tour 2, fonctionne normalement après
        s = "\n";  // NE PAS ENLEVER
        cout << "Suite à un bug inconnu, l'affichage ira directement au tour 2" << endl;
        cout << "A tout moment, appuyez sur espace puis Entrée pour afficher le vecteur de termites. Attention, le tour suivant sera joué et affiché, il faudra donc remonter pour voir le vecteur" << endl;
        cout << "Appuyez sur Entrée pour passer au tour suivant" << endl;
        cout << "Appuyez sur une touche quelconque puis Entrée pour sortir de la simulation" << endl << endl;
    
         do {
            // On joue le tour puis on affiche la grille
            cout << "Tour " << i << ":" << endl; i++;
            for (int j = 0; j < nbTermites; j++) {
                (*vT)[j].vieTermite(*g);
            }
            try {
                m.insecticide(i);
            } catch(...) {
                cout << "";
            }
            cout << m << endl;
            
            // Vérification de la cohérence des données
            if (not(m.coherenceGrilleVect())) {
                cout << "Incohérence entre les données de la grille et du vecteur, arrêt de la simulation" << endl << endl;
                s = "B";  // Pour le rapport d'erreur
            } else if (not(m.coherenceFichiers(*g, *vT))) {
                cout << "Incohérence entre les fichiers projet.cpp et monde.cpp, arrêt de la simulation" << endl << endl;
                s = "A";  // Pour le rapport d'erreur
            }
        
            // On demande la prochaine étape à l'utilisateur
            cout << "Appuyez sur Entrée pour passer au tour suivant, sur Espace puis  Entrée pour afficher le vecteur de termites ou sur une touche quelconque puis Entrée pour quitter: ";
            s = getchar();  // Pour ne récupérer que le premier caractère et les caractères '\n' et ' '
            if (s == " ") {
                // Le tour 12 est directement joué, le vecteur s'affiche au-dessus
                cout << *vT << endl;
                s = getchar();  // Pour ne récupérer que le premier caractère et les caractères '\n' et ' '
            }
            
        } while (s == "\n" and i <= 2000);
    } else if (s == "2") {
        s = "\n";
        do {
            // On joue le tour puis on l'affiche
            cout << "Tour " << i << ":" << endl; i++;
            for (int j = 0; j < nbTermites; j++) {
                (*vT)[j].vieTermite(*g);
            }
            try {
                m.insecticide(i);
            } catch(...) {
                cout << "";
            }
            cout << m << endl << endl;
        
            // Vérification de la cohérence des données
            if (not(m.coherenceGrilleVect())) {
                cout << "Incohérence entre les données de la grille et du vecteur, arrêt de la simulation" << endl << endl;
                s = "B";  // Pour le rapport d'erreur
            } else if (not(m.coherenceFichiers(*g, *vT))) {
                cout << "Incohérence entre les fichiers projet.cpp et monde.cpp, arrêt de la simulation" << endl << endl;
                s = "A";  // Pour le rapport d'erreur
            }
            
            usleep(15000);  // 15 000 microsecondes = 0.015 seconde de pause entre chaque tour
        } while (i <= 2000);
    } else {
        vector<int> v;
        int A = 0; int B = 0; int C = 0; int D = 0;
        // On joue 600 parties
        for (int k = 0; k < 600; k++) {
            do {
                // On joue le tour sans l'afficher
                for (int j = 0; j < nbTermites; j++) {
                    (*vT)[j].vieTermite(*g);
                }
                try {
                    m.insecticide(i);
                } catch(...) {
                    cout << "";
                }
            
                // Vérification de la cohérence des données
                if (not(m.coherenceGrilleVect())) {
                    cout << "Incohérence entre les données de la grille et du vecteur, arrêt de la simulation" << endl << endl;
                    s = "B";  // Pour le rapport d'erreur
                } else if (not(m.coherenceFichiers(*g, *vT))) {
                    cout << "Incohérence entre les fichiers projet.cpp et monde.cpp, arrêt de la simulation" << endl << endl;
                    s = "A";  // Pour le rapport d'erreur
                }
                
                i++;
            } while (i <= 2000);
            
            // On compte les résultats par équipe
            v = m.compteBrindilles();
            A += v[0]; B += v[1]; C += v[2]; D += v[3];
            
            // On crée une nouvelle grille
            m = Monde{};  // On réinitialise la grille
            *vT = {};  // On réinitialise le vecteur de termites
            m.monde(); i = 0;
            
            cout << "Simulation " << k << endl;
            // On affiche les résultats en cours
            cout << "Equipe 0: " << A << endl;
            cout << "Equipe 1: " << B << endl;
            cout << "Equipe 2: " << C << endl;
            cout << "Equipe 3: " << D << endl << endl;
        }
        // On affiche les résultats finaux
        cout << "Affichage des résultats:" << endl;
        cout << "Equipe 0: " << A << endl;
        cout << "Equipe 1: " << B << endl;
        cout << "Equipe 2: " << C << endl;
        cout << "Equipe 3: " << D << endl << endl;
    }
    
    
    // Messages de fin de partie
    if (s == "A") {
        cout << "La simulation a rencontré une incohérence entre les données stockées dans projet.cpp et monde.cpp" << endl;
        cout << "Affichage de la grille et des deux vecteurs" << endl;
        cout << "Grille: " << endl << m << endl << "Vecteur dans monde.cpp" << m.getVect() << endl << *vT << endl << endl;
    } else if (s == "B") {
        cout << "La simulation a rencontré une incohérence entre la grille et le vecteur de termites" << endl;
        cout << "Affichage du monde dans monde.cpp et de son vecteur" << endl;
        cout << "Grille:" << m << "Vecteur" << m.getVect() << endl << endl;
    } else if (s != "3") {
        if (i < 2000)
            cout << "Vous avez volontairement quitté la simulation" << endl << endl;
        else
            cout << "Limite de tours atteinte, la simulation est stabilisée" << endl << endl; 
        
        vector<int> v = m.compteBrindilles();
        cout << "Affichage des résultats:" << endl;
        cout << "Equipe 0: " << v[0] << endl;
        cout << "Equipe 1: " << v[1] << endl;
        cout << "Equipe 2: " << v[2] << endl;
        cout << "Equipe 3: " << v[3] << endl << endl;
    }
    
    while (s == "\n") {
        cout << "Appuyez sur une touche quelconque puis Entrée pour terminer le jeu, appuyez sur Espace puis Entrée pour afficher le vecteur de termites" << endl;
        s = getchar();  // Pour ne récupérer que le premier caractère et les caractères '\n' et ' '
        if (s == " ")
            cout << *vT << endl;        
    }
    
    cout << endl << "Fin de la simulation, nettoyage des données en mémoire et arrêt du programme" << endl;

    return 0;
}