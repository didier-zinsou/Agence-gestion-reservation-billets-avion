#include "Application.h"

#include <iostream>
#include <vector>

#include "Utils.h"

using std::cout;
using std::string;

Application::Application() : agence_("Agence de voyage de Cotonou") {}

// ----------------------------------------------------------------------------
//  Menu principal
// ----------------------------------------------------------------------------
void Application::afficherMenu() const {
    cout << "\n==================================================\n"
         << "   " << agence_.getNom() << "\n"
         << "==================================================\n"
         << "   1- Enregistrer un passager\n"
         << "   2- Ajouter un vol\n"
         << "   3- Afficher les vols disponibles\n"
         << "   4- Rechercher un vol\n"
         << "   5- Effectuer une réservation\n"
         << "   6- Enregistrer un paiement\n"
         << "   7- Annuler une réservation\n"
         << "   8- Consulter les réservations\n"
         << "   9- Afficher les infos d'une réservation\n"
         << "  10- Quitter\n"
         << "--------------------------------------------------\n";
}

void Application::afficherTitre(const string& titre) const {
    cout << "\n--- " << titre << " ---\n";
}

void Application::lancer() {
    int choix = 0;
    do {
        afficherMenu();
        choix = Utils::lireEntier("Votre choix : ", 1, 10);
        switch (choix) {
            case 1:  enregistrerPassager(); break;
            case 2:  ajouterVol(); break;
            case 3:  afficherVolsDisponibles(); break;
            case 4:  rechercherVol(); break;
            case 5:  effectuerReservation(); break;
            case 6:  enregistrerPaiement(); break;
            case 7:  annulerReservation(); break;
            case 8:  consulterReservations(); break;
            case 9:  afficherInfosReservation(); break;
            case 10: cout << "\nMerci d'avoir utilisé l'application. Au revoir !\n"; break;
        }
        if (choix != 10) Utils::pause();
    } while (choix != 10);
}

// ----------------------------------------------------------------------------
//  1- Passagers
// ----------------------------------------------------------------------------
void Application::enregistrerPassager() {
    afficherTitre("Enregistrement d'un passager");
    const string nom = Utils::lireTexte("Nom : ");
    const string prenom = Utils::lireTexte("Prénom : ");
    const string telephone = Utils::lireTelephone("Téléphone : ");
    const string email = Utils::lireEmail("E-mail : ");
    const string passeport = Utils::lireTexte("Numéro de passeport : ");

    string erreur;
    const Passager* p = agence_.enregistrerPassager(nom, prenom, telephone, email, passeport, erreur);
    if (p == nullptr) {
        cout << "\n  Échec : " << erreur << "\n";
        return;
    }
    cout << "\n  Passager enregistré avec succès. Identifiant : " << p->getId() << "\n";
}

// ----------------------------------------------------------------------------
//  2- 3- 4- Vols
// ----------------------------------------------------------------------------
void Application::ajouterVol() {
    afficherTitre("Ajout d'un vol");
    const string numero = Utils::lireTexte("Numéro du vol (ex. BJ101) : ");
    const string depart = Utils::lireTexte("Ville de départ : ");
    const string arrivee = Utils::lireTexte("Ville d'arrivée : ");
    const string date = Utils::lireDate("Date de départ (JJ/MM/AAAA) : ");
    const string heure = Utils::lireHeure("Heure de départ (HH:MM) : ");
    const int capacite = Utils::lireEntier("Capacité (nombre de places) : ", 1, 1000);
    const double prix = Utils::lireReel("Prix du billet (FCFA) : ", 1);

    string erreur;
    const Vol* v = agence_.ajouterVol(numero, depart, arrivee, date, heure, capacite, prix, erreur);
    if (v == nullptr) {
        cout << "\n  Échec : " << erreur << "\n";
        return;
    }
    cout << "\n  Vol ajouté avec succès :\n";
    v->afficher();
}

void Application::afficherVolsDisponibles() const {
    afficherTitre("Vols disponibles");
    const std::vector<Vol*> vols = agence_.volsDisponibles();
    if (vols.empty()) {
        cout << "  Aucun vol disponible pour le moment.\n";
        return;
    }
    Vol::afficherEntete();
    for (const Vol* v : vols) v->afficherLigne();
    cout << "\n  " << vols.size() << " vol(s) disponible(s).\n";
}

void Application::rechercherVol() const {
    afficherTitre("Recherche d'un vol");
    cout << "  1- Par numéro de vol\n"
         << "  2- Par ville de départ\n"
         << "  3- Par ville d'arrivée\n"
         << "  4- Par date\n";
    const int choix = Utils::lireEntier("Critère : ", 1, 4);

    std::vector<Vol*> resultats;
    switch (choix) {
        case 1: resultats = agence_.rechercherVols(CritereRecherche::NUMERO, Utils::lireTexte("Numéro : ")); break;
        case 2: resultats = agence_.rechercherVols(CritereRecherche::DEPART, Utils::lireTexte("Ville de départ : ")); break;
        case 3: resultats = agence_.rechercherVols(CritereRecherche::ARRIVEE, Utils::lireTexte("Ville d'arrivée : ")); break;
        case 4: resultats = agence_.rechercherVols(CritereRecherche::DATE, Utils::lireDate("Date (JJ/MM/AAAA) : ")); break;
    }

    cout << "\n";
    if (resultats.empty()) {
        cout << "  Aucun vol ne correspond à votre recherche.\n";
        return;
    }
    Vol::afficherEntete();
    for (const Vol* v : resultats) v->afficherLigne();
    cout << "\n  " << resultats.size() << " vol(s) trouvé(s).\n";
}

// ----------------------------------------------------------------------------
//  5- Réservation
// ----------------------------------------------------------------------------
void Application::effectuerReservation() {
    afficherTitre("Nouvelle réservation");
    if (agence_.getPassagers().empty()) {
        cout << "  Aucun passager enregistré. Enregistrez d'abord un passager (menu 1).\n";
        return;
    }
    if (agence_.volsDisponibles().empty()) {
        cout << "  Aucun vol disponible pour le moment.\n";
        return;
    }

    cout << "Passagers enregistrés :\n";
    Passager::afficherEntete();
    for (const Passager* p : agence_.getPassagers()) p->afficherLigne();

    const string idPassager = Utils::lireTexte("\nIdentifiant du passager (ex. P001) : ");

    cout << "\nVols disponibles :\n";
    Vol::afficherEntete();
    for (const Vol* v : agence_.volsDisponibles()) v->afficherLigne();

    const string numeroVol = Utils::lireTexte("\nNuméro du vol : ");

    string erreur;
    const Reservation* r = agence_.reserver(idPassager, numeroVol, erreur);
    if (r == nullptr) {
        cout << "\n  Réservation refusée : " << erreur << "\n";
        return;
    }
    cout << "\n  Réservation " << r->getId() << " créée (statut : " << versTexte(r->getStatut()) << ").\n"
         << "  Montant à payer : " << Utils::formaterMontant(r->montantDu()) << "\n"
         << "  La place ne sera définitive qu'après le paiement (menu 6).\n";
}

// ----------------------------------------------------------------------------
//  6- Paiement
// ----------------------------------------------------------------------------
void Application::enregistrerPaiement() {
    afficherTitre("Enregistrement d'un paiement");
    const string id = Utils::lireTexte("Identifiant de la réservation (ex. R001) : ");
    const Reservation* r = agence_.trouverReservation(id);
    if (r == nullptr) {
        cout << "\n  Échec : réservation introuvable.\n";
        return;
    }

    string raison;
    if (!r->peutEtrePayee(raison)) {
        cout << "\n  Paiement impossible : " << raison << "\n";
        return;
    }

    cout << "\n";
    r->afficherDetails();
    cout << "\nMode de paiement :\n  1- Espèces\n  2- Mobile Money\n  3- Carte bancaire\n";
    const int choixMode = Utils::lireEntier("Mode : ", 1, 3);
    const ModePaiement mode = (choixMode == 1)   ? ModePaiement::ESPECES
                              : (choixMode == 2) ? ModePaiement::MOBILE_MONEY
                                                 : ModePaiement::CARTE_BANCAIRE;
    cout << "Montant à payer : " << Utils::formaterMontant(r->montantDu()) << "\n";
    const double montant = Utils::lireReel("Montant versé (FCFA) : ", 0);

    string erreur;
    if (!agence_.enregistrerPaiement(id, montant, mode, erreur)) {
        cout << "\n  Paiement refusé : " << erreur << "\n";
        return;
    }
    cout << "\n  Paiement enregistré. La réservation " << r->getId()
         << " est validée : la place est définitivement réservée.\n";
}

// ----------------------------------------------------------------------------
//  7- Annulation
// ----------------------------------------------------------------------------
void Application::annulerReservation() {
    afficherTitre("Annulation d'une réservation");
    const string id = Utils::lireTexte("Identifiant de la réservation (ex. R001) : ");
    const Reservation* r = agence_.trouverReservation(id);
    if (r == nullptr) {
        cout << "\n  Échec : réservation introuvable.\n";
        return;
    }
    if (!r->estActive()) {
        cout << "\n  Cette réservation est déjà annulée.\n";
        return;
    }

    cout << "\n";
    r->afficherDetails();
    cout << "\n";
    if (!Utils::confirmer("Confirmer l'annulation")) {
        cout << "\n  Annulation abandonnée.\n";
        return;
    }

    string message;
    const bool ok = agence_.annulerReservation(id, message);
    cout << "\n  " << (ok ? "" : "Échec : ") << message << "\n";
}

// ----------------------------------------------------------------------------
//  8- 9- Consultation
// ----------------------------------------------------------------------------
void Application::consulterReservations() const {
    afficherTitre("Liste des réservations");
    const std::vector<Reservation*>& liste = agence_.getReservations();
    if (liste.empty()) {
        cout << "  Aucune réservation enregistrée.\n";
        return;
    }
    int attente = 0, validees = 0, annulees = 0;
    Reservation::afficherEntete();
    for (const Reservation* r : liste) {
        r->afficherLigne();
        switch (r->getStatut()) {
            case StatutReservation::EN_ATTENTE: ++attente; break;
            case StatutReservation::VALIDEE:    ++validees; break;
            case StatutReservation::ANNULEE:    ++annulees; break;
        }
    }
    cout << "\n  Total : " << liste.size() << " réservation(s) - " << attente << " en attente, "
         << validees << " validée(s), " << annulees << " annulée(s).\n";
}

void Application::afficherInfosReservation() const {
    afficherTitre("Informations d'une réservation");
    const string id = Utils::lireTexte("Identifiant de la réservation (ex. R001) : ");
    const Reservation* r = agence_.trouverReservation(id);
    if (r == nullptr) {
        cout << "\n  Réservation introuvable.\n";
        return;
    }
    cout << "\n";
    r->afficherDetails();
}

// ----------------------------------------------------------------------------
//  Données de démonstration
// ----------------------------------------------------------------------------
void Application::chargerDonneesDemo() {
    string e;
    agence_.enregistrerPassager("DOSSOU", "Marie", "+229 97 12 34 56", "marie.dossou@exemple.com", "BJ1234567", e);
    agence_.enregistrerPassager("AGBODJAN", "Kévin", "+229 96 65 43 21", "kevin.agbodjan@exemple.com", "BJ7654321", e);
    agence_.enregistrerPassager("HOUNGBO", "Sarah", "+229 95 11 22 33", "sarah.houngbo@exemple.com", "BJ2468135", e);

    agence_.ajouterVol("BJ101", "Cotonou", "Paris", "20/12/2026", "08:30", 3, 350000, e);
    agence_.ajouterVol("BJ202", "Cotonou", "Abidjan", "18/11/2026", "14:15", 2, 120000, e);
    agence_.ajouterVol("BJ303", "Cotonou", "Ouagadougou", "25/11/2026", "10:00", 50, 95000, e);

    agence_.reserver("P001", "BJ101", e);
    agence_.enregistrerPaiement("R001", 350000, ModePaiement::MOBILE_MONEY, e);
}
