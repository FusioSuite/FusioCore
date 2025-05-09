# Dépendances tierces utilisées dans FusioCore

Ce projet utilise les bibliothèques suivantes, dans le respect de leurs licences respectives.

---

## [Eigen](https://gitlab.com/libeigen/eigen)

- **Version** : 3.4.0 (forkée : [FusioSuite/fusio-eigen](https://github.com/FusioSuite/fusio-eigen))
- **Rôle** : Calcul matriciel et vectoriel (moteur linéaire)
- **Licence** : MPL 2.0
- **Méthode d’intégration** : `FetchContent` via CMake
- **Utilisation** :
  - Matrices (`Eigen::MatrixXd`)
  - Vecteurs (`Eigen::VectorXd`)
  - Solveurs linéaires (QR, LU, etc.)

---

## [ExprTk](https://github.com/ArashPartow/exprtk)

- **Version** : 3.0.0 (forkée : [FusioSuite/fusio-exprtk](https://github.com/FusioSuite/fusio-exprtk))
- **Rôle** : Évaluation d'expressions mathématiques
- **Licence** : MIT
- **Méthode d’intégration** : `FetchContent` via CMake
