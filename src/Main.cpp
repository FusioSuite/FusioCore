#include "../inc/Shell/Shell.hpp"
#include "../inc/Value/Value.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        auto& shell = FusioCore::Shell::getInstance();
        shell.printProjectInfo();
        
        // Exemples d'utilisation des classes Scalar, Vector et Matrix
        shell.print("=== Exemples d'utilisation des classes de valeurs ===", FusioCore::ShellType::INFO);
        
        // Exemples avec Scalar
        shell.print("\n--- Opérations avec Scalar ---", FusioCore::ShellType::INFO);
        FusioCore::Scalar a(3.14);
        FusioCore::Scalar b(2.0);
        shell.print("a = " + a.toString());
        shell.print("b = " + b.toString());
        shell.print("a + b = " + (a + b).toString());
        shell.print("a - b = " + (a - b).toString());
        shell.print("a * b = " + (a * b).toString());
        shell.print("a / b = " + (a / b).toString());
        
        // Exemples avec Vector
        shell.print("\n--- Opérations avec Vector ---", FusioCore::ShellType::INFO);
        FusioCore::Vector v1(3, 1.0);  // Vecteur [1, 1, 1]
        FusioCore::Vector v2(3, 2.0);  // Vecteur [2, 2, 2]
        shell.print("v1 = " + v1.toString());
        shell.print("v2 = " + v2.toString());
        shell.print("v1 + v2 = " + (v1 + v2).toString());
        shell.print("v1 - v2 = " + (v1 - v2).toString());
        shell.print("v1 * v2 (produit scalaire) = " + (v1 * v2).toString());
        shell.print("v1 * 3 = " + (v1 * a).toString());
        
        // Exemples avec Matrix
        shell.print("\n--- Opérations avec Matrix ---", FusioCore::ShellType::INFO);
        FusioCore::Matrix m1(2, 2, 1.0);  // Matrice [[1, 1], [1, 1]]
        m1(0, 0) = 2.0;  // Modifier un élément
        m1(1, 1) = 3.0;
        
        FusioCore::Matrix m2(2, 2, 2.0);  // Matrice [[2, 2], [2, 2]]
        shell.print("m1 = " + m1.toString());
        shell.print("m2 = " + m2.toString());
        shell.print("m1 + m2 = " + (m1 + m2).toString());
        shell.print("m1 - m2 = " + (m1 - m2).toString());
        shell.print("m1 * m2 = " + (m1 * m2).toString());
        shell.print("m1 * 3 = " + (m1 * a).toString());
        
        // Opérations matricielles avancées
        shell.print("\n--- Opérations matricielles avancées ---", FusioCore::ShellType::INFO);
        shell.print("m1 transpose = " + m1.transpose().toString());
        shell.print("m1 determinant = " + m1.determinant().toString());
        shell.print("m1 inverse = " + m1.inverse().toString());
        
        // Multiplication matrice-vecteur
        shell.print("\n--- Multiplication matrice-vecteur ---", FusioCore::ShellType::INFO);
        FusioCore::Vector v3(2, 1.0);  // Vecteur [1, 1]
        shell.print("v3 = " + v3.toString());
        shell.print("m1 * v3 = " + (m1 * v3).toString());
        
        // Boucle principale
        shell.print("\n=== Entrée en mode interactif ===", FusioCore::ShellType::INFO);
        while (true) {
            std::string input = shell.waitInput(">> ");
            if (input == "exit") {
                break;
            }
            // Traitement des commandes ici
            shell.print(input);
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
