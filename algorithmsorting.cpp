#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

// ================= KODE STATUS (angka) =================
enum Status {
    GAWAT_DARURAT = 0,
    DARURAT       = 1,
    SIAGA         = 2,
    WASPADA       = 3
};

string namaStatus(int s) {
    switch (s) {
        case GAWAT_DARURAT: return "Gawat Darurat";
        case DARURAT:       return "Darurat";
        case SIAGA:         return "Siaga";
        default:            return "Waspada";
    }
}

// ================= DATA WILAYAH =================
struct Wilayah {
    string kecamatan;
    string jenisBencana;   // "Longsor" / "Puting Beliung"
    bool   a, b, c, d, e;  // kondisi a-e (true = terpenuhi)
    int    jumlahSyarat;   // berapa kondisi terpenuhi
    int    status;         // kode angka status (0-3)
};

// ================= PENENTUAN STATUS =================
// Kondisi diisi lewat fungsi tiap jenis bencana (nilai ambang = contoh, silakan sesuaikan)

// Longsor:
// a = curah hujan > 100 mm/hari
// b = kemiringan lereng > 30%
// c = ada retakan / pergerakan tanah
// d = penduduk terdampak > 1000 jiwa
// e = akses jalan terputus
Wilayah buatLongsor(string kec, double hujan, double lereng,
                    bool retak, int pendudukTerdampak, bool aksesPutus) {
    Wilayah w;
    w.kecamatan = kec;
    w.jenisBencana = "Longsor";
    w.a = hujan > 100;
    w.b = lereng > 30;
    w.c = retak;
    w.d = pendudukTerdampak > 1000;
    w.e = aksesPutus;
    return w;
}

// Puting beliung:
// a = kecepatan angin > 60 km/jam
// b = ada kerusakan bangunan
// c = ada korban jiwa / luka
// d = penduduk terdampak > 1000 jiwa
// e = listrik padam / pohon tumbang menutup jalan
Wilayah buatPutingBeliung(string kec, double angin, bool rusak,
                          bool korban, int pendudukTerdampak, bool listrikPadam) {
    Wilayah w;
    w.kecamatan = kec;
    w.jenisBencana = "Puting Beliung";
    w.a = angin > 60;
    w.b = rusak;
    w.c = korban;
    w.d = pendudukTerdampak > 1000;
    w.e = listrikPadam;
    return w;
}

void hitungStatus(Wilayah &w) {
    w.jumlahSyarat = w.a + w.b + w.c + w.d + w.e;

    if (w.a && w.b && w.c && w.d && w.e) w.status = GAWAT_DARURAT; // harus a-e semua
    else if (w.jumlahSyarat >= 3)        w.status = DARURAT;
    else if (w.jumlahSyarat >= 1)        w.status = SIAGA;
    else                                 w.status = WASPADA;
}

// ================= INSERTION SORT =================
// Urut berdasarkan status (0 paling prioritas),
// jika status sama -> jumlah syarat terpenuhi terbanyak duluan
bool lebihPrioritas(const Wilayah &x, const Wilayah &y) {
    if (x.status != y.status) return x.status < y.status;
    return x.jumlahSyarat > y.jumlahSyarat;
}

void insertionSort(vector<Wilayah> &data) {
    for (size_t i = 1; i < data.size(); i++) {
        Wilayah key = data[i];
        int j = i - 1;
        while (j >= 0 && lebihPrioritas(key, data[j])) {
            data[j + 1] = data[j];
            j--;
        }
        data[j + 1] = key;
    }
}

// ================= TAMPILKAN =================
void tampilkan(const vector<Wilayah> &data) {
    cout << left << setw(4)  << "No"
         << setw(20) << "Kecamatan"
         << setw(16) << "Bencana"
         << setw(8)  << "Kode"
         << setw(15) << "Status"
         << "Syarat(a-e)" << endl;
    cout << string(75, '-') << endl;
    for (size_t i = 0; i < data.size(); i++) {
        const Wilayah &w = data[i];
        cout << left << setw(4)  << i + 1
             << setw(20) << w.kecamatan
             << setw(16) << w.jenisBencana
             << setw(8)  << w.status
             << setw(15) << namaStatus(w.status)
             << w.a << w.b << w.c << w.d << w.e
             << " (" << w.jumlahSyarat << "/5)" << endl;
    }
}

int main() {
    vector<Wilayah> data;

    // ---- Contoh data LONGSOR (nilai hanya dummy, ganti dengan data asli) ----
    // (kecamatan, hujan mm, lereng %, retak, penduduk terdampak, akses putus)
    data.push_back(buatLongsor("Pujon",              120, 35, true,  1500, true));
    data.push_back(buatLongsor("Ngantang",           110, 32, true,  1200, false));
    data.push_back(buatLongsor("Kasembon",            90, 28, true,   800, false));
    data.push_back(buatLongsor("Karangploso",         60, 15, false,  300, false));
    data.push_back(buatLongsor("Dau",                 70, 20, false,  500, false));
    data.push_back(buatLongsor("Singosari",           80, 18, false,  400, false));
    data.push_back(buatLongsor("Lawang",             105, 33, true,   900, false));
    data.push_back(buatLongsor("Poncokusumo",        115, 36, true,  1100, true));
    data.push_back(buatLongsor("Tumpang",             95, 25, false,  700, false));
    data.push_back(buatLongsor("Ampelgading",        100, 31, true,   600, false));
    data.push_back(buatLongsor("Tirtoyudo",          125, 40, true,  2000, true));
    data.push_back(buatLongsor("Dampit",             108, 34, true,  1300, false));
    data.push_back(buatLongsor("Sumbermanjing Wetan",130, 38, true,  1800, true));
    data.push_back(buatLongsor("Wagir",               85, 22, false,  500, false));
    data.push_back(buatLongsor("Kromengan",           50, 10, false,  200, false));
    data.push_back(buatLongsor("Ngajum",              55, 12, false,  250, false));
    data.push_back(buatLongsor("Wonosari",            75, 20, false,  350, false));

    // ---- Contoh data PUTING BELIUNG (nilai dummy) ----
    // (kecamatan, kecepatan angin km/jam, rusak, korban, penduduk terdampak, listrik padam)
    data.push_back(buatPutingBeliung("Karangploso",  70, true,  true,  1500, true));
    data.push_back(buatPutingBeliung("Dau",          65, true,  false, 1100, true));
    data.push_back(buatPutingBeliung("Singosari",    55, true,  false,  600, false));
    data.push_back(buatPutingBeliung("Lawang",       62, true,  true,   900, false));
    data.push_back(buatPutingBeliung("Pakis",        45, false, false,  200, false));
    data.push_back(buatPutingBeliung("Jabung",       68, true,  true,  1200, true));
    data.push_back(buatPutingBeliung("Tajinan",      40, false, false,  100, false));
    data.push_back(buatPutingBeliung("Poncokusumo",  58, true,  false,  500, false));
    data.push_back(buatPutingBeliung("Bululawang",   72, true,  true,  1600, true));
    data.push_back(buatPutingBeliung("Gondanglegi",  50, false, false,  300, false));
    data.push_back(buatPutingBeliung("Sumberpucung", 63, true,  false,  800, true));
    data.push_back(buatPutingBeliung("Kepanjen",     66, true,  true,  1400, false));

    // Hitung status tiap wilayah
    for (size_t i = 0; i < data.size(); i++) hitungStatus(data[i]);

    cout << "=== SEBELUM DIURUTKAN ===" << endl;
    tampilkan(data);

    insertionSort(data);

    cout << "\n=== SESUDAH INSERTION SORT (PRIORITAS PENANGANAN) ===" << endl;
    tampilkan(data);

    return 0;
}
