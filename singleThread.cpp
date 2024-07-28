#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>

void apply_gaussian_blur(const cv::Mat &src, cv::Mat &dst, int kernel_size) {
    cv::GaussianBlur(src, dst, cv::Size(kernel_size, kernel_size), 0);
}

int main() {
    // Liste des noms de fichiers image
    std::vector<std::string> image_files = {
        "image1.jpg", "image2.jpg", "image3.jpg", "image4.jpg", "image5.jpg",
        "image6.jpg", "image7.jpg", "image8.jpg", "image9.jpg", "image10.jpg",
        "image11.jpg", "image12.jpg", "image13.jpg", "image14.jpg", "image15.jpg",
        "image16.jpg"
    };

    // Démarrer le chronomètre total
    auto total_start = std::chrono::high_resolution_clock::now();

    // Traitement de chaque image
    for (const auto& filename : image_files) {
        // Démarrer le chronomètre pour l'image courante
        auto start = std::chrono::high_resolution_clock::now();

        // Charger l'image
        cv::Mat src = cv::imread(filename, cv::IMREAD_COLOR);
        if (src.empty()) {
            std::cerr << "Erreur de chargement de l'image : " << filename << std::endl;
            continue; // Passer à l'image suivante
        }

        cv::Mat dst;
        apply_gaussian_blur(src, dst, 89);

        // Sauvegarder l'image traitée
        std::string output_filename = "output_" + filename;
        if (cv::imwrite(output_filename, dst)) {
            std::cout << "Filtre de flou gaussien appliqué avec succès à " << filename << " (single-thread)." << std::endl;
        } else {
            std::cerr << "Erreur lors de la sauvegarde de l'image : " << filename << std::endl;
            continue; // Passer à l'image suivante
        }

        // Arrêter le chronomètre pour l'image courante
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        // Afficher le temps d'exécution pour l'image courante
        std::cout << "Temps d'exécution pour " << filename << " : " << elapsed.count() << " secondes." << std::endl;
    }

    // Arrêter le chronomètre total
    auto total_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_elapsed = total_end - total_start;

    // Afficher le temps total d'exécution
    std::cout << "Temps total de traitement de toutes les images : " << total_elapsed.count() << " secondes." << std::endl;

    return 0;
}

