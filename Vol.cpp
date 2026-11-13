#include "Vol.h"

#include <iostream>

#include "Utils.h"

Vol::Vol(const std::string& numero, const std::string& villeDepart, const std::string& villeArrivee,
         const std::string& date, const std::string& heure, int capacite, double prix)
    : numero_(numero),
      villeDepart_(villeDepart),
      villeArrivee_(villeArrivee),
      date_(date),
      heure_(heure),
      capacite_(capacite),
      placesProvisoires_(0),
      placesConfirmees_(0),
      prix_(prix) {}

Vol::~Vol() {}

bool Vol::reserverProvisoirement() {
    if (estComplet()) return false;
    ++placesProvisoires_;
    return true;
}

void Vol::confirmerPlace() {
    if (placesProvisoires_ > 0) {
        --placesProvisoires_;
        ++placesConfirmees_;
    }
}

void Vol::libererPlace(bool etaitConfirmee) {
    if (etaitConfirmee) {
        if (placesConfirmees_ > 0) --placesConfirmees_;
    } else {
        if (placesProvisoires_ > 0) --placesProvisoires_;
    }
}

void Vol::afficher() const {
    std::cout << "  Vol " << numero_ << " : " << villeDepart_ << " -> " << villeArrivee_ << "\n"
              << "    Départ : " << date_ << " à " << heure_ << "\n"
              << "    Prix   : " << Utils::formaterMontant(prix_) << "\n"
              << "    Places : " << placesConfirmees_ << " confirmée(s), " << placesProvisoires_
              << " en attente, " << placesDisponibles() << " libre(s) sur " << capacite_ << "\n";
}

void Vol::afficherEntete() {
    std::cout << "  " << Utils::cadrer("N° vol", 9) << Utils::cadrer("Départ", 15)
              << Utils::cadrer("Arrivée", 15) << Utils::cadrer("Date", 12)
              << Utils::cadrer("Heure", 7) << Utils::cadrer("Prix", 15) << "Places libres\n";
    std::cout << "  " << std::string(86, '-') << "\n";
}

void Vol::afficherLigne() const {
    std::cout << "  " << Utils::cadrer(numero_, 9) << Utils::cadrer(villeDepart_, 15)
              << Utils::cadrer(villeArrivee_, 15) << Utils::cadrer(date_, 12)
              << Utils::cadrer(heure_, 7) << Utils::cadrer(Utils::formaterMontant(prix_), 15)
              << placesDisponibles() << "/" << capacite_ << "\n";
}
