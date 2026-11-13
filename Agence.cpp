#include "Agence.h"

#include "Utils.h"

Agence::Agence(const std::string& nom) : nom_(nom) {}

// Les r�serservations sont libérées en premier : elles référencent passagers et vols.
Agence::~Agence() {
    for (Reservation* r : reservations_) delete r;
    for (Vol* v : vols_) delete v;
    for (Passager* p : passagers_) delete p;
}

// ----------------------------------------------------------------------------
//  Passagers et vols
// ----------------------------------------------------------------------------
Passager* Agence::enregistrerPassager(const std::string& nom, const std::string& prenom,
                                      const std::string& telephone, const std::string& email,
                                      const std::string& passeport, std::string& erreur) {
    const std::string numero = Utils::enMajuscules(passeport);
    for (const Passager* p : passagers_) {
        if (p->getNumeroPasseport() == numero) {
            erreur = "Ce numéro de passeport est déjà enregistré (passager " + p->getId() + ").";
            return nullptr;
        }
    }
    Passager* p = new Passager(nom, prenom, telephone, email, numero);
    passagers_.push_back(p);
    return p;
}

Vol* Agence::ajouterVol(const std::string& numero, const std::string& depart,
                        const std::string& arrivee, const std::string& date,
                        const std::string& heure, int capacite, double prix, std::string& erreur) {
    const std::string num = Utils::enMajuscules(numero);
    if (trouverVol(num) != nullptr) {
        erreur = "Un vol portant le numéro " + num + " existe déjà.";
        return nullptr;
    }
    if (Utils::enMinuscules(depart) == Utils::enMinuscules(arrivee)) {
        erreur = "La ville de départ et la ville d'arrivée doivent être différentes.";
        return nullptr;
    }
    if (capacite <= 0) {
        erreur = "La capacité doit être strictement positive.";
        return nullptr;
    }
    if (prix <= 0) {
        erreur = "Le prix doit être strictement positif.";
        return nullptr;
    }
    Vol* v = new Vol(num, depart, arrivee, date, heure, capacite, prix);
    vols_.push_back(v);
    return v;
}

// ----------------------------------------------------------------------------
//  Recherches
// ----------------------------------------------------------------------------
Passager* Agence::trouverPassager(const std::string& id) const {
    const std::string cle = Utils::enMajuscules(id);
    for (Passager* p : passagers_)
        if (p->getId() == cle) return p;
    return nullptr;
}

Vol* Agence::trouverVol(const std::string& numero) const {
    const std::string cle = Utils::enMajuscules(numero);
    for (Vol* v : vols_)
        if (v->getNumero() == cle) return v;
    return nullptr;
}

Reservation* Agence::trouverReservation(const std::string& id) const {
    const std::string cle = Utils::enMajuscules(id);
    for (Reservation* r : reservations_)
        if (r->getId() == cle) return r;
    return nullptr;
}

std::vector<Vol*> Agence::volsDisponibles() const {
    std::vector<Vol*> resultat;
    for (Vol* v : vols_)
        if (!v->estComplet()) resultat.push_back(v);
    return resultat;
}

std::vector<Vol*> Agence::rechercherVols(CritereRecherche critere, const std::string& texte) const {
    const std::string cle = Utils::enMinuscules(texte);
    std::vector<Vol*> resultat;
    for (Vol* v : vols_) {
        bool correspond = false;
        switch (critere) {
            case CritereRecherche::NUMERO:
                correspond = Utils::enMinuscules(v->getNumero()).find(cle) != std::string::npos;
                break;
            case CritereRecherche::DEPART:
                correspond = Utils::enMinuscules(v->getVilleDepart()).find(cle) != std::string::npos;
                break;
            case CritereRecherche::ARRIVEE:
                correspond = Utils::enMinuscules(v->getVilleArrivee()).find(cle) != std::string::npos;
                break;
            case CritereRecherche::DATE:
                correspond = (v->getDate() == texte);
                break;
        }
        if (correspond) resultat.push_back(v);
    }
    return resultat;
}

// ----------------------------------------------------------------------------
//  Réservations, paiements, annulations
// ----------------------------------------------------------------------------
Reservation* Agence::reserver(const std::string& idPassager, const std::string& numeroVol,
                              std::string& erreur) {
    // Règle 1 : le passager doit être enregistré.
    Passager* passager = trouverPassager(idPassager);
    if (passager == nullptr) {
        erreur = "Passager introuvable : il doit d'abord être enregistré (menu 1).";
        return nullptr;
    }
    Vol* vol = trouverVol(numeroVol);
    if (vol == nullptr) {
        erreur = "Vol introuvable : vérifiez le numéro de vol.";
        return nullptr;
    }
    // Règle 3 : pas deux réservations actives du même passager sur le même vol.
    for (const Reservation* r : reservations_) {
        if (r->getPassager() == passager && r->getVol() == vol && r->estActive()) {
            erreur = "Ce passager a déjà une réservation (" + r->getId() + ") sur le vol " +
                     vol->getNumero() + ".";
            return nullptr;
        }
    }
    // Règle 2 : le vol ne peut pas dépasser sa capacité.
    if (!vol->reserverProvisoirement()) {
        erreur = "Le vol " + vol->getNumero() + " est complet.";
        return nullptr;
    }
    // Règle 4 : une réservation = un seul passager + un seul vol.
    Reservation* reservation = new Reservation(passager, vol);
    reservations_.push_back(reservation);
    return reservation;
}

bool Agence::enregistrerPaiement(const std::string& idReservation, double montant,
                                 ModePaiement mode, std::string& erreur) {
    Reservation* r = trouverReservation(idReservation);
    if (r == nullptr) {
        erreur = "Réservation introuvable.";
        return false;
    }
    return r->payer(montant, mode, erreur);  // règles 5 et 6 appliquées par la réservation
}

bool Agence::annulerReservation(const std::string& idReservation, std::string& message) {
    Reservation* r = trouverReservation(idReservation);
    if (r == nullptr) {
        message = "Réservation introuvable.";
        return false;
    }
    return r->annuler(message);  // règle 7 appliquée par la réservation
}
