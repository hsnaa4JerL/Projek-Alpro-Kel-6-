#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <cctype>
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

// ================= KETERANGAN SYARAT a-e =================
// Longsor
const string SYARAT_LONGSOR[5] = {
    "Curah hujan > 100 mm/hari",
    "Kemiringan lereng > 30%",
    "Ada retakan / pergerakan tanah",
    "Penduduk terdampak > 1000 jiwa",
    "Akses jalan terputus"
};
// Puting beliung
const string SYARAT_BELIUNG[5] = {
    "Kecepatan angin > 60 km/jam",
    "Ada kerusakan bangunan",
    "Ada korban jiwa / luka",
    "Penduduk terdampak > 1000 jiwa",
    "Listrik padam / pohon tumbang menutup jalan"
};

void hitungStatus(Wilayah &w) {
    w.jumlahSyarat = w.a + w.b + w.c + w.d + w.e;

    if (w.a && w.b && w.c && w.d && w.e) w.status = GAWAT_DARURAT;
    else if (w.jumlahSyarat >= 3)        w.status = DARURAT;
    else if (w.jumlahSyarat >= 1)        w.status = SIAGA;
    else                                 w.status = WASPADA;
}

// ================= FUNGSI BANTU INPUT =================
string bacaBaris(const string &prompt) {
    string s;
    cout << prompt;
    if (!getline(cin, s)) {            // input habis / dihentikan
        cout << "\nInput berakhir." << endl;
        exit(0);
    }
    return s;
}

int bacaInt(const string &prompt, int minimum, int maksimum) {
    while (true) {
        istringstream iss(bacaBaris(prompt));
        int v; char sisa;
        if (iss >> v && !(iss >> sisa) && v >= minimum && v <= maksimum) return v;
        cout << "  Input tidak valid, masukkan angka " << minimum
             << " - " << maksimum << "." << endl;
    }
}

string bacaNama(const string &prompt) {
    while (true) {
        string s = bacaBaris(prompt);
        size_t awal = s.find_first_not_of(" \t");
        if (awal != string::npos) {
            size_t akhir = s.find_last_not_of(" \t");
            return s.substr(awal, akhir - awal + 1);
        }
        cout << "  Nama tidak boleh kosong." << endl;
    }
}

bool tanyaYaTidak(const string &pertanyaan) {
    while (true) {
        string s = bacaBaris(pertanyaan + " (y/t): ");
        for (size_t i = 0; i < s.size(); i++) s[i] = tolower(s[i]);
        if (s == "y" || s == "ya")                    return true;
        if (s == "t" || s == "tidak" || s == "n")     return false;
        cout << "  Jawab dengan y (ya) atau t (tidak)." << endl;
    }
}

// ================= INPUT DATA WILAYAH DARI USER =================
void inputDataWilayah(ArrayDinamis<Wilayah> &data) {
    int n = bacaInt("Jumlah daerah yang akan diinput: ", 1, 1000);

    for (int i = 1; i <= n; i++) {
        cout << "\n--- Daerah ke-" << i << " dari " << n << " ---" << endl;
        Wilayah w;
        w.kecamatan = bacaNama("Nama kecamatan : ");
        int jenis = bacaInt("Jenis bencana (1 = Longsor, 2 = Puting Beliung): ", 1, 2);

        const string *syarat;
        if (jenis == 1) { w.jenisBencana = "Longsor";        syarat = SYARAT_LONGSOR; }
        else            { w.jenisBencana = "Puting Beliung"; syarat = SYARAT_BELIUNG; }

        cout << "Apakah daerah ini sudah memenuhi syarat berikut?" << endl;
        w.a = tanyaYaTidak("  a. " + syarat[0]);
        w.b = tanyaYaTidak("  b. " + syarat[1]);
        w.c = tanyaYaTidak("  c. " + syarat[2]);
        w.d = tanyaYaTidak("  d. " + syarat[3]);
        w.e = tanyaYaTidak("  e. " + syarat[4]);

        hitungStatus(w);     // tentukan kode & status
        data.tambah(w);      // masuk ke array dinamis
    }
    cout << endl;
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
        if (pertama) cout << "(belum ada data)";
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

// =====================================================================
//  INSERTION SORT pada ARRAY DINAMIS
//  Urut berdasarkan status (0 paling prioritas),
//  jika status sama -> jumlah syarat terpenuhi terbanyak duluan
// =====================================================================
bool lebihPrioritas(const Wilayah &x, const Wilayah &y) {
    if (x.status != y.status) return x.status < y.status;
    return x.jumlahSyarat > y.jumlahSyarat;
}

void insertionSort(ArrayDinamis<Wilayah> &data) {
    for (int i = 1; i < data.banyak(); i++) {
        Wilayah key = data[i];
        int j = i - 1;
        while (j >= 0 && lebihPrioritas(key, data[j])) {
            data[j + 1] = data[j];
            j--;
        }
        data[j + 1] = key;
    }
}

int main() {
    // ---- PROBLEM ----
    string problem = "Keterbatasan Metode Konvensional dalam Mengidentifikasi, "
                     "Memilih, dan Menentukan Wilayah Prioritas Penanganan Bencana";

    // ---- DATA (array dinamis) ----
    ArrayDinamis<Wilayah> data;
    inputDataWilayah(data);   // data diisi oleh user

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
    cout << "  KELOMPOK 6 - PRIORITAS PENANGANAN BENCANA (ARRAY DINAMIS + SORTING + MANIPULATOR + POINTER + ENUM)" << endl;
    garis('=');
    cout << endl;

    tampilkanProblem(problem);
    tampilkanDaftarKecamatan(data);

    insertionSort(data);   // solusi: sorting untuk menentukan daerah paling urgent

    cout << "=== HASIL (PRIORITAS PENANGANAN) ===" << endl;
    tampilkanTabel(data);

    tampilkanList("SOLUSI", solusi);
    tampilkanList("FEEDBACK 1", feedback);

    return 0;   // memori dibebaskan otomatis oleh destruktor
}
