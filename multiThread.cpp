#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <thread>
#include <mutex>

// Mutex pour synchroniser l'accès à la sortie standard
std::mutex mtx;

void apply_gaussian_blur(const cv::Mat &src, cv::Mat &dst, int kernel_size) {
    cv::GaussianBlur(src, dst, cv::Size(kernel_size, kernel_size), 0);
}

void process_image(const std::string &filename) {
    // Démarrer le chronomètre pour l'image courante
    auto start = std::chrono::high_resolution_clock::now();

    // Charger l'image
    cv::Mat src = cv::imread(filename, cv::IMREAD_COLOR);
    if (src.empty()) {
        std::lock_guard<std::mutex> lock(mtx);
        std::cerr << "Erreur de chargement de l'image : " << filename << std::endl;
        return;
    }

    cv::Mat dst;
    apply_gaussian_blur(src, dst, 89);

    // Sauvegarder l'image traitée
    std::string output_filename = "output_" + filename;
    if (cv::imwrite(output_filename, dst)) {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Filtre de flou gaussien appliqué avec succès à " << filename << " (multi-thread)." << std::endl;
    } else {
        std::lock_guard<std::mutex> lock(mtx);
        std::cerr << "Erreur lors de la sauvegarde de l'image : " << filename << std::endl;
    }

    // Arrêter le chronomètre pour l'image courante
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Afficher le temps d'exécution pour l'image courante
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Temps d'exécution pour " << filename << " : " << elapsed.count() << " secondes." << std::endl;
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

    // Création et lancement des threads
    std::vector<std::thread> threads;
    for (const auto& filename : image_files) {
        threads.emplace_back(process_image, filename);
    }

    // Attendre la fin de tous les threads
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    // Arrêter le chronomètre total
    auto total_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_elapsed = total_end - total_start;

    // Afficher le temps total d'exécution
    std::cout << "Temps total de traitement de toutes les images : " << total_elapsed.count() << " secondes." << std::endl;

    return 0;
}
