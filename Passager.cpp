#include "Passager.h"

#include <iostream>

#include "Utils.h"

int Passager::compteur = 0;

Passager::Passager(const std::string& nom, const std::string& prenom, const std::string& telephone,
                   const std::string& email, const std::string& numeroPasseport)
    : id_(Utils::genererId("P", ++compteur)),
      nom_(nom),
      prenom_(prenom),
      telephone_(telephone),
      email_(email),
      numeroPasseport_(numeroPasseport) {}

// Aucune ressource dynamique à libérer : les membres std::string se détruisent seuls.
Passager::~Passager() {}

std::string Passager::nomComplet() const {
    return nom_ + " " + prenom_;
}

void Passager::afficherEntete() {
    std::cout << "  " << Utils::cadrer("ID", 7) << Utils::cadrer("Nom et prénom", 26)
              << Utils::cadrer("Téléphone", 17) << Utils::cadrer("Passeport", 12) << "E-mail\n";
    std::cout << "  " << std::string(86, '-') << "\n";
}

void Passager::afficherLigne() const {
    std::cout << "  " << Utils::cadrer(id_, 7) << Utils::cadrer(nomComplet(), 26)
              << Utils::cadrer(telephone_, 17) << Utils::cadrer(numeroPasseport_, 12) << email_
              << "\n";
}
