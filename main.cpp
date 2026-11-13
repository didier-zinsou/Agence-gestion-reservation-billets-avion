#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

#include "Application.h"
#include "Utils.h"

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);  // affichage correct des accents sous Windows
#endif

    try {
        Application app;
        // L'option --demo pré-charge quelques passagers, vols et une réservation.
        if (argc > 1 && std::string(argv[1]) == "--demo") app.chargerDonneesDemo();
        app.lancer();
    } catch (const Utils::FinDeSaisie&) {
        std::cout << "\n\nFin de la saisie. Au revoir !\n";
    }
    return 0;
}
