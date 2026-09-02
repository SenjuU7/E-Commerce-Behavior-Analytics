#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

// 1. Gunakan constexpr untuk menentukan KKM/Batas Kelulusan (Compile-time constant)
constexpr int KKM = 75;

// Fungsi advanced untuk mengambil referensi data siswa peringkat pertama
// Tipe kembalian otomatis mengikat referensi asli vector (std::pair<const std::string, int>&)
decltype(auto) dapatkan_juara_kelas(std::vector<std::pair<std::string, int>>& vec_rank) {
    return vec_rank[0]; // Mengembalikan elemen pertama (Juara 1)
}

int main() {
    // 2. Inisialisasi Eksplisit menggunakan gaya modern {}
    std::map<std::string, int> daftar_nilai{
        {"Alice", 90},
        {"Bob", 65},
        {"Charlie", 85},
        {"Delta", 72}
    };

    std::cout << "--- SISTEM MANAJEMEN NILAI CYSEC ---" << std::endl;

    // TUGAS KAMU ADA DI SINI LINGKUNGAN NYATA
    for (const auto& [k, v] : daftar_nilai) {
        if (v >= KKM)
        {
			std::cout << k << ": " << v << " (LULUS)" << std::endl;
        }
        else
        {
            std::cout << k << ": " << v << " (REMEDI)" << std::endl;
        }
    }

    std::cout << "\n--- PERINGKAT KELAS (TERTINGGI -> TERENDAH) ---" << std::endl;
    std::vector<std::pair<std::string, int>> ranking(daftar_nilai.begin(), daftar_nilai.end());

    std::sort(ranking.begin(), ranking.end(), [](const auto& a, const auto& b) {
		return a.second > b.second; // Urutkan dari nilai tertinggi ke terendah
    });

    decltype(auto) juara = dapatkan_juara_kelas(ranking);
    juara.second = 100;

    int peringkat = 1;
    for (const auto& [k,v]: ranking) {
		std::cout << peringkat << ". " << k << ": " << v << std::endl;
        peringkat++;
    }

    return 0;
}



