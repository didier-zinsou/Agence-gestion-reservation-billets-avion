#pragma once
#include <string>

#include "Enums.h"
#include "Paiement.h"
#include "Passager.h"
#include "Vol.h"

// Une réservation relie UN passager à UN vol (règle 4) et possède au plus un paiement.
// Cycle de vie : EN_ATTENTE --payer()--> VALIDEE ; EN_ATTENTE/VALIDEE --annuler()--> ANNULEE.
class Reservation {
private:
    static int compteur;  // identifiants R001, R002, ...
    static constexpr double TAUX_REMBOURSEMENT = 0.80;  // 20 % de frais d'annulation

    std::string id_;
    std::string dateReservation_;
    std::string dateAnnulation_;
    StatutReservation statut_;
    Passager* passager_;    // association (le passager existe indépendamment)
    Vol* vol_;              // association (le vol existe indépendamment)
    Paiement* paiement_;    // composition : détruit avec la réservation

public:
    Reservation(Passager* passager, Vol* vol);
    ~Reservation();

    // La réservation possède un pointeur : on interdit la copie.
    Reservation(const Reservation&) = delete;
    Reservation& operator=(const Reservation&) = delete;

    const std::string& getId() const { return id_; }
    Passager* getPassager() const { return passager_; }
    Vol* getVol() const { return vol_; }
    Paiement* getPaiement() const { return paiement_; }
    StatutReservation getStatut() const { return statut_; }

    bool estActive() const { return statut_ != StatutReservation::ANNULEE; }
    bool estPayee() const { return paiement_ != nullptr; }
    double montantDu() const;

    // Vérifie les règles 5 : indique pourquoi un paiement serait refusé.
    bool peutEtrePayee(std::string& raison) const;
    // Enregistre le paiement, valide la réservation et confirme la place (règle 6).
    bool payer(double montant, ModePaiement mode, std::string& erreur);
    // Annule la réservation, libère la place et rembourse si elle était payée (règle 7).
    bool annuler(std::string& message);

    void afficherDetails() const;
    static void afficherEntete();
    void afficherLigne() const;
};
