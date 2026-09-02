#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <type_traits>
#include <stdexcept>
#include <fstream>
#include <sstream>

// ============================================================================
// USER PROFILE TAGS
// Digunakan sebagai penanda tipe data (tag dispatching) untuk evaluasi promo.
// ============================================================================
struct NewUser {};
struct VIPUser {};
struct RegularUser {};

// ============================================================================
// CLASS: EcommerceAnalytics
// Mengelola agregasi perilaku pengguna (behavior tracking), evaluasi threshold 
// berbasis profil, dan algoritma penentuan kategori banner promosi.
// ============================================================================

struct UserBehavior {
    std::string name;
    std::string category;
    int duration;
};

class EcommerceAnalytics {
private:
    std::vector<UserBehavior> behavior_data; // Ganti map dengan vector struct

    static constexpr int THRESHOLD_MINAT_REG = 45;
    static constexpr int THRESHOLD_MINAT_NEW = 90;
    static constexpr int THRESHOLD_MINAT_VIP = 180;

    // Mengambil elemen dengan durasi tertinggi dari vektor peringkat.
    // Dilindungi oleh Concepts (Guard Level 1) untuk memastikan tipe numerik.
    template<typename T>
    decltype(auto) get_highest_duration(std::vector<T>& vec_rank) {
        if (vec_rank.empty()) {
            throw std::runtime_error("Error: Container is empty, cannot retrieve highest duration.");
        }
        return vec_rank.front();
    }

public:
    // Menerima aliran data real-time (data stream) secara variadic 
    // menggunakan Fold Expression untuk efisiensi kompilasi dan eksekusi.
    template <typename... T>
    void insert_behavior(T... events) {
        ((behavior_data.push_back(events)), ...);
    }

    // Mengevaluasi kelayakan promo secara dinamis saat kompilasi (Compile-Time Branching)
    // menggunakan if constexpr berdasarkan profil pengguna yang dikirimkan.
    template<typename UserType>
    void cek_promo_khusus() const {
        std::cout << "=== E-COMMERCE BEHAVIOR ANALYTICS (PROFILE: "
            << (std::is_same_v<UserType, VIPUser> ? "VIP" :
                std::is_same_v<UserType, NewUser> ? "NEW USER" : "REGULAR")
            << ") ===\n";

        int count = 0;
        for (const auto& [name, kategori, durasi] : behavior_data) {
            // Tentukan threshold berdasarkan tipe user
            int threshold = 0;
            std::string promo_msg = "";

            if constexpr (std::is_same_v<UserType, VIPUser>) {
                threshold = THRESHOLD_MINAT_VIP;
                promo_msg = "VIP EXCLUSIVE PROMO GRANTED!";
            }
            else if constexpr (std::is_same_v<UserType, NewUser>) {
                threshold = THRESHOLD_MINAT_NEW;
                promo_msg = "WELCOME VOUCHER GRANTED!";
            }
            else {
                threshold = THRESHOLD_MINAT_REG;
                promo_msg = "REGULAR PROMO GRANTED!";
            }

            // Lewati data yang tidak memenuhi threshold (hanya tampilkan yang layak)
            if (durasi < threshold) {
                continue;
            }

            if (count >= 5) break;

            std::cout << name << " - " << kategori << ": " << durasi << " detik -> " << promo_msg << "\n";
            ++count;
        }

        if (count == 0) {
            std::cout << "Tidak ada pengguna yang memenuhi syarat promo.\n";
        }
    }

    // Mengurutkan kategori berdasarkan durasi tertinggi, memberikan bobot loyalitas (+60s)
    // pada peringkat pertama, dan menyinkronkan kembali datanya ke struktur penyimpanan utama.
    void proses_kategori_unggulan() {
        std::cout << "\n=== PROMOTION BANNER RANKING & WEIGHTING ===\n";

        if (behavior_data.empty()) {
            std::cout << "Warning: Behavior dataset is empty.\n";
            return;
        }

        // 1. Alih-alih std::sort seluruh data, kita bisa cari max_element langsung untuk Juara 1
        auto it_juara = std::max_element(behavior_data.begin(), behavior_data.end(),
            [](const auto& a, const auto& b) {
                return a.duration < b.duration;
            });

        if (it_juara != behavior_data.end()) {
            // Berikan bonus loyalitas +60 detik langsung ke sumber data utama
            it_juara->duration += 60;
            std::cout << "Top Winner Found: " << it_juara->name << " - " << it_juara->category
                << " (Updated Total: " << it_juara->duration << " seconds)\n";
        }

        // 2. Jika tetap ingin mencetak daftar terurut untuk ditampilkan ke banner:
        std::vector<UserBehavior> high_interest_categories(behavior_data.begin(), behavior_data.end());
        std::sort(high_interest_categories.begin(), high_interest_categories.end(), [](const auto& a, const auto& b) {
            return a.duration > b.duration;
            });

        int count = 0;
        for (const auto& [name, category, duration] : high_interest_categories) {
            if (count >= 5) break;
            std::cout << "- " << name << " | " << category << " (" << duration << " seconds)\n";
            ++count;
        }
    }

    
    /*void hitung_rata_rata() const {
        if (behavior_data.empty()) return;

        double total_duration = 0;
        for (const auto& [name, category, duration] : behavior_data) {
            total_duration += duration;
        }

        double avg = total_duration / static_cast<double>(behavior_data.size());
        std::cout << "\nAverage user session duration: " << avg << " seconds\n";
    }*/

};

void load_behavior_from_csv(const std::string& filename, EcommerceAnalytics& analytics) {

    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Error: Gagal membuka file database CSV!");
    }

    std::string line;
    // Lewati baris pertama jika itu adalah header (Kategori,Durasi)
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string namaUser;
        std::string kategori;
        std::string durasi_str;

        
        if (std::getline(ss, namaUser, ',') &&
            std::getline(ss, kategori, ',') &&
            std::getline(ss, durasi_str, ',')) {
        
            if (!durasi_str.empty()) {
                int durasi = std::stoi(durasi_str);
                analytics.insert_behavior(UserBehavior{ namaUser, kategori, durasi });
            }
        }
    }

    file.close();
}

// ============================================================================
// MAIN APPLICATION ENTRY POINT
// ============================================================================
int main() {
    EcommerceAnalytics analytics;

    try {
        load_behavior_from_csv("D:/Visual Studio/repos/cysec/data/behavior_log.csv", analytics);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    // Evaluasi penawaran promo berdasarkan segmentasi profil user.
    analytics.cek_promo_khusus<NewUser>();
    std::cout << "\n--------------------------------------------------\n\n";
    analytics.cek_promo_khusus<VIPUser>();

    // Eksekusi pemrosesan banner utama dan kalkulasi metrik analitik.
    std::cout << "\n--------------------------------------------------\n";
    analytics.proses_kategori_unggulan();
    

    return 0;
}