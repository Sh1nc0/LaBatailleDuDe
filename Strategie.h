#pragma once 
#include <vector>
#include <memory>


// Représente une action du joueur.
// avec le territoire d'origine et le territoire de destination.
// Position est la position d'une cellule dans la carte. Qui est un tuple d'entier (x, y).
struct ActionDeJeu
{
    Position celluleOrigine;
    Position celluleDestination;
};

// Méthode appelée à chaque tour de jeu par l'arbitre pour demander 
// la stratégie du joueur.
// La Carte est passé en paramètre pour permettre au joueur de
// prendre sa décision.
// La méthode retourne un unique pointer vers un vecteur d'actions.
// car sinon la copie du vecteur serait trop coûteuse. et le vecteur ne peut pas être passé par référence car il est temporaire.
std::unique_ptr<std::vector<ActionDeJeu>> play(const Carte& carte);