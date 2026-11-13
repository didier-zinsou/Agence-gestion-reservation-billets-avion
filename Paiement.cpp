#include "Paiement.h"

#include <iostream>

#include "Utils.h"

int Paiement::compteur = 0;

Paiement::Paiement(double montant, ModePaiement mode)
    : id_(Utils::genererId("PAY", ++compteur)),
      montant_(montant),
      mode_(mode),
      datePaiement_(Utils::maintenant()),
      statutRemboursement_(StatutRemboursement::AUCUN),
      montantRembourse_(0.0) {}

Paiement::~Paiement() {}

void Paiement::rembourser(double taux) {
    if (statutRemboursement_ == StatutRemboursement::REMBOURSE) return;  // déjà traité
    montantRembourse_ = montant_ * taux;
    statutRemboursement_ = StatutRemboursement::REMBOURSE;
}

void Paiement::afficher() const {
    std::cout << "  Paiement      : " << id_ << " - " << Utils::formaterMontant(montant_) << " ("
              << versTexte(mode_) << ") le " << datePaiement_ << "\n"
              << "  Remboursement : " << versTexte(statutRemboursement_);
    if (statutRemboursement_ == StatutRemboursement::REMBOURSE)
        std::cout << " (" << Utils::formaterMontant(montantRembourse_) << ")";
    std::cout << "\n";
}
