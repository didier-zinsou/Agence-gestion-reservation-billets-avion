#pragma once
#include <string>

#include "Agence.h"

// Interface console : affiche le menu et dialogue avec l'utilisateur.
class Application {
private:
    Agence agence_;

    void afficherMenu() const;
    void afficherTitre(const std::string& titre) const;

    void enregistrerPassager();
    void ajouterVol();
    void afficherVolsDisponibles() const;
    void rechercherVol() const;
    void effectuerReservation();
    void enregistrerPaiement();
    void annulerReservation();
    void consulterReservations() const;
    void afficherInfosReservation() const;

public:
    Application();

    void chargerDonneesDemo();  // jeu de données d'exemple (option --demo)
    void lancer();              // boucle du menu principal
};
