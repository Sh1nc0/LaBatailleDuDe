#pragma once

// Contient toutes les informations sur comment générer une carte.
// Avoir une structure permet de ne pas avoir une fonction avec trop de paramètres.
// et de pouvoir ajouter des paramètres sans avoir à modifier la signature de la fonction.
struct CarteOptions
{
    int longueur;
    int hauteur;
    int nombreDeJoueurs = 2;
    int nombreDeCellulesParJoueur = 10;
    int nombreDeCellulesNeutres = 10;
};

// Une Carte est composée de cellules.
// Soit une cellule peut appartenir à un joueur soit être vide.
// La Carte est passé en paramètre car elle peut posséder des informations autre que celle générer par le générateur.
// La Carte est passée par référence pour éviter la copie.
void GenérerCarte(Carte& carte, CarteOptions options);