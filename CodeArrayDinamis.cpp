#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// =====================================================================
//  ARRAY DINAMIS (generik) - alokasi dengan new[], membesar otomatis
// =====================================================================
template <typename T>
class ArrayDinamis {
private:
    T*  data;
    int ukuran;     // jumlah elemen terisi
    int kapasitas;  // kapasitas array saat ini

    void perbesar() {                       // kapasitas dikali 2
        int kapBaru = kapasitas * 2;
        T* baru = new T[kapBaru];
        for (int i = 0; i < ukuran; i++) baru[i] = data[i];
        delete[] data;                      // bebaskan array lama
        data = baru;
        kapasitas = kapBaru;
    }

public:
    ArrayDinamis(int kapAwal = 4) : data(new T[kapAwal]), ukuran(0), kapasitas(kapAwal) {}
    ~ArrayDinamis() { delete[] data; }

    // dilarang disalin agar tidak terjadi double delete
    ArrayDinamis(const ArrayDinamis&) = delete;
    ArrayDinamis& operator=(const ArrayDinamis&) = delete;

    void tambah(const T& nilai) {
        if (ukuran == kapasitas) perbesar();
        data[ukuran++] = nilai;
    }

    int banyak() const { return ukuran; }
    int kapasitasSaatIni() const { return kapasitas; }

    T& operator[](int i) { return data[i]; }
    const T& operator[](int i) const { return data[i]; }
};

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
    bool   a = false, b = false, c = false, d = false, e = false;
    int    jumlahSyarat = 0;
    int    status = WASPADA;
};

// Longsor:
// a = curah hujan > 100 mm/hari, b = lereng > 30%, c = ada retakan/pergerakan tanah
// d = penduduk terdampak > 1000 jiwa, e = akses jalan terputus
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
// a = angin > 60 km/jam, b = ada kerusakan bangunan, c = ada korban jiwa/luka
// d = penduduk terdampak > 1000 jiwa, e = listrik padam / pohon tumbang menutup jalan
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

    if (w.a && w.b && w.c && w.d && w.e) w.status = GAWAT_DARURAT;
    else if (w.jumlahSyarat >= 3)        w.status = DARURAT;
    else if (w.jumlahSyarat >= 1)        w.status = SIAGA;
    else                                 w.status = WASPADA;
}

// ================= ISI DATA (sesuai kolom DATA pada tabel) =================
void isiDataWilayah(ArrayDinamis<Wilayah> &data) {
    // Daerah rawan longsor (nilai dummy, ganti dengan data asli)
    data.tambah(buatLongsor("Pujon",              120, 35, true,  1500, true));
    data.tambah(buatLongsor("Ngantang",           110, 32, true,  1200, false));
    data.tambah(buatLongsor("Kasembon",            90, 28, true,   800, false));
    data.tambah(buatLongsor("Karangploso",         60, 15, false,  300, false));
    data.tambah(buatLongsor("Dau",                 70, 20, false,  500, false));
    data.tambah(buatLongsor("Singosari",           80, 18, false,  400, false));
    data.tambah(buatLongsor("Lawang",             105, 33, true,   900, false));
    data.tambah(buatLongsor("Poncokusumo",        115, 36, true,  1100, true));
    data.tambah(buatLongsor("Tumpang",             95, 25, false,  700, false));
    data.tambah(buatLongsor("Ampelgading",        100, 31, true,   600, false));
    data.tambah(buatLongsor("Tirtoyudo",          125, 40, true,  2000, true));
    data.tambah(buatLongsor("Dampit",             108, 34, true,  1300, false));
    data.tambah(buatLongsor("Sumbermanjing Wetan",130, 38, true,  1800, true));
    data.tambah(buatLongsor("Wagir",               85, 22, false,  500, false));
    data.tambah(buatLongsor("Kromengan",           50, 10, false,  200, false));
    data.tambah(buatLongsor("Ngajum",              55, 12, false,  250, false));
    data.tambah(buatLongsor("Wonosari",            75, 20, false,  350, false));

    // Daerah rawan angin kencang / puting beliung (nilai dummy)
    data.tambah(buatPutingBeliung("Karangploso",  70, true,  true,  1500, true));
    data.tambah(buatPutingBeliung("Dau",          65, true,  false, 1100, true));
    data.tambah(buatPutingBeliung("Singosari",    55, true,  false,  600, false));
    data.tambah(buatPutingBeliung("Lawang",       62, true,  true,   900, false));
    data.tambah(buatPutingBeliung("Pakis",        45, false, false,  200, false));
    data.tambah(buatPutingBeliung("Jabung",       68, true,  true,  1200, true));
    data.tambah(buatPutingBeliung("Tajinan",      40, false, false,  100, false));
    data.tambah(buatPutingBeliung("Poncokusumo",  58, true,  false,  500, false));
    data.tambah(buatPutingBeliung("Bululawang",   72, true,  true,  1600, true));
    data.tambah(buatPutingBeliung("Gondanglegi",  50, false, false,  300, false));
    data.tambah(buatPutingBeliung("Sumberpucung", 63, true,  false,  800, true));
    data.tambah(buatPutingBeliung("Kepanjen",     66, true,  true,  1400, false));
}

// ================= TAMPILAN =================
void garis(char ch = '-', int n = 75) { cout << string(n, ch) << endl; }

void tampilkanProblem(const string &problem) {
    cout << "[PROBLEM]" << endl;
    garis();
    cout << problem << endl << endl;
}

void tampilkanDaftarKecamatan(const ArrayDinamis<Wilayah> &data) {
    cout << "[DATA]" << endl;
    garis();
    const string jenis[2] = {"Longsor", "Puting Beliung"};
    for (int k = 0; k < 2; k++) {
        cout << "Daerah rawan " << jenis[k] << ":" << endl << "  ";
        bool pertama = true;
        for (int i = 0; i < data.banyak(); i++) {
            if (data[i].jenisBencana == jenis[k]) {
                if (!pertama) cout << ", ";
                cout << data[i].kecamatan;
                pertama = false;
            }
        }
        cout << endl << endl;
    }
    cout << "Jumlah data: " << data.banyak()
         << " (kapasitas array: " << data.kapasitasSaatIni() << ")" << endl << endl;
}

void tampilkanTabel(const ArrayDinamis<Wilayah> &data) {
    cout << left << setw(4)  << "No"
         << setw(22) << "Kecamatan"
         << setw(16) << "Bencana"
         << setw(8)  << "Kode"
         << setw(15) << "Status"
         << "Syarat(a-e)" << endl;
    garis();
    for (int i = 0; i < data.banyak(); i++) {
        const Wilayah &w = data[i];
        cout << left << setw(4)  << i + 1
             << setw(22) << w.kecamatan
             << setw(16) << w.jenisBencana
             << setw(8)  << w.status
             << setw(15) << namaStatus(w.status)
             << w.a << w.b << w.c << w.d << w.e
             << " (" << w.jumlahSyarat << "/5)" << endl;
    }
    cout << endl;
}

void tampilkanList(const string &judul, const ArrayDinamis<string> &arr) {
    cout << "[" << judul << "]" << endl;
    garis();
    for (int i = 0; i < arr.banyak(); i++)
        cout << i + 1 << ". " << arr[i] << endl;
    cout << endl;
}

int main() {
    // ---- PROBLEM ----
    string problem = "Keterbatasan Metode Konvensional dalam Mengidentifikasi, "
                     "Memilih, dan Menentukan Wilayah Prioritas Penanganan Bencana";

    // ---- DATA (array dinamis) ----
    ArrayDinamis<Wilayah> data;
    isiDataWilayah(data);
    for (int i = 0; i < data.banyak(); i++) hitungStatus(data[i]);

    // ---- SOLUSI (array dinamis) ----
    ArrayDinamis<string> solusi;
    solusi.tambah("Level risiko dan imbauan untuk tiap laporan.");
    solusi.tambah("Tabel peringkat lokasi paling berbahaya.");
    solusi.tambah("Menggunakan sorting untuk menentukan daerah mana yang lebih urgent ditangani.");

    // ---- FEEDBACK (array dinamis) ----
    ArrayDinamis<string> feedback;
    feedback.tambah("ACC reference datanya sangat valid.");
    feedback.tambah("Penggunaan algoritma sorting untuk menentukan prioritas daerah yang "
                    "ditangani sangat pas dengan latar belakang masalah. Keep Up!");

    // ---- OUTPUT ----
    garis('=');
    cout << "  KELOMPOK 6 - PRIORITAS PENANGANAN BENCANA (ARRAY DINAMIS)" << endl;
    garis('=');
    cout << endl;

    tampilkanProblem(problem);
    tampilkanDaftarKecamatan(data);

    cout << "Tabel data wilayah beserta level risiko (belum diurutkan):" << endl;
    tampilkanTabel(data);

    tampilkanList("SOLUSI", solusi);
    tampilkanList("FEEDBACK 1", feedback);

    return 0;   // memori dibebaskan otomatis oleh destruktor
}
