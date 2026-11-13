#pragma once
#include <string>

// Un client de l'agence. Doit être enregistré avant de pouvoir réserver (règle 1).
class Passager {
private:
    static int compteur;  // sert à générer les identifiants P001, P002, ...

    std::string id_;
    std::string nom_;
    std::string prenom_;
    std::string telephone_;
    std::string email_;
    std::string numeroPasseport_;

public:
    Passager(const std::string& nom, const std::string& prenom, const std::string& telephone,
             const std::string& email, const std::string& numeroPasseport);
    ~Passager();

    const std::string& getId() const { return id_; }
    const std::string& getNom() const { return nom_; }
    const std::string& getPrenom() const { return prenom_; }
    const std::string& getTelephone() const { return telephone_; }
    const std::string& getEmail() const { return email_; }
    const std::string& getNumeroPasseport() const { return numeroPasseport_; }

    std::string nomComplet() const;
    static void afficherEntete();
    void afficherLigne() const;
};
