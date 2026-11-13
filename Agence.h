#pragma once
#include <string>
#include <vector>

#include "Enums.h"
#include "Passager.h"
#include "Reservation.h"
#include "Vol.h"

// Classe de gestion : elle possède tous les passagers, vols et réservations (dans des vector)
// et applique les règles de gestion de l'agence.
class Agence {
private:
    std::string nom_;
    std::vector<Passager*> passagers_;
    std::vector<Vol*> vols_;
    std::vector<Reservation*> reservations_;

public:
    explicit Agence(const std::string& nom);
    ~Agence();  // libère passagers, vols et réservations

    // L'agence possède des pointeurs : on interdit la copie.
    Agence(const Agence&) = delete;
    Agence& operator=(const Agence&) = delete;

    const std::string& getNom() const { return nom_; }
    const std::vector<Passager*>& getPassagers() const { return passagers_; }
    const std::vector<Vol*>& getVols() const { return vols_; }
    const std::vector<Reservation*>& getReservations() const { return reservations_; }

    // Les opérations qui peuvent échouer retournent nullptr/false et renseignent 'erreur'.
    Passager* enregistrerPassager(const std::string& nom, const std::string& prenom,
                                  const std::string& telephone, const std::string& email,
                                  const std::string& passeport, std::string& erreur);
    Vol* ajouterVol(const std::string& numero, const std::string& depart, const std::string& arrivee,
                    const std::string& date, const std::string& heure, int capacite, double prix,
                    std::string& erreur);

    Passager* trouverPassager(const std::string& id) const;
    Vol* trouverVol(const std::string& numero) const;
    Reservation* trouverReservation(const std::string& id) const;

    std::vector<Vol*> volsDisponibles() const;
    std::vector<Vol*> rechercherVols(CritereRecherche critere, const std::string& texte) const;

    Reservation* reserver(const std::string& idPassager, const std::string& numeroVol,
                          std::string& erreur);
    bool enregistrerPaiement(const std::string& idReservation, double montant, ModePaiement mode,
                             std::string& erreur);
    bool annulerReservation(const std::string& idReservation, std::string& message);
};
