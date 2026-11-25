#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA 200
#define HASH_SIZE 10
#define MAX_TITIK 15

/* ==================== STRUCT DATA (TOPIK 2, 4, 6, 7) ==================== */
typedef struct {
    int id;
    char nama[60];
    int umur;
    char kategori[20];
    char rayon[40];
    char baptis[20];
} Jemaat;

typedef struct {
    int kode;
    char jenis[20];
    char tanggal[30];
    char waktu[20];
    char lokasi[60];
} Jadwal;

typedef struct NodeAntrean {
    Jemaat calon;
    struct NodeAntrean *next;
} NodeAntrean;

typedef struct NodeWarta {
    char tanggal[25];
    char judul[60];
    char isi[150];
    struct NodeWarta *next;
} NodeWarta;

typedef struct HashNode {
    Jemaat data;
    struct HashNode *next;
} HashNode;

typedef struct BSTNode {
    Jemaat data;
    struct BSTNode *kiri, *kanan;
} BSTNode;

/* ==================== FUNGSI BANTU LAYAR & INPUT (TOPIK 1) ==================== */
void bersihkanLayar(void) {
    fflush(stdout);
    system("cls");
}

void jeda(void) {
    char buf[16];
    printf("\nTekan ENTER untuk lanjut...");
    fflush(stdout);
    fgets(buf, sizeof(buf), stdin);
}

int bacaAngka(const char *tanya) {
    char buf[64];
    int angka;
    while (1) {
        printf("%s", tanya);
        fflush(stdout);
        if (fgets(buf, sizeof(buf), stdin) && sscanf(buf, "%d", &angka) == 1) {
            return angka;
        }
        printf("[!] Harap masukkan angka!\n");
    }
}

void bacaTeks(const char *tanya, char *hasil, int ukuran) {
    while (1) {
        printf("%s", tanya);
        fflush(stdout);
        if (fgets(hasil, ukuran, stdin)) {
            hasil[strcspn(hasil, "\r\n")] = '\0';
            if (strlen(hasil) > 0 && strchr(hasil, '|') == NULL) return;
        }
        printf("[!] Teks tidak boleh kosong atau memakai tanda '|'.\n");
    }
}

/* ==================== AUTENTIKASI LOGIN & REGISTER ==================== */
void registerAkun(void) {
    char user[40], pass[40], baris[120], uFile[40], pFile[40];
    bacaTeks("Username baru : ", user, 40);
    bacaTeks("Password baru : ", pass, 40);

    FILE *f = fopen("data/akun.txt", "r");
    if (f) {
        while (fgets(baris, sizeof(baris), f)) {
            baris[strcspn(baris, "\r\n")] = '\0';
            if (sscanf(baris, "%39[^|]|%39[^\n]", uFile, pFile) == 2 && strcmp(uFile, user) == 0) {
                printf("[!] Username sudah terdaftar!\n");
                fclose(f);
                return;
            }
        }
        fclose(f);
    }

    f = fopen("data/akun.txt", "a");
    if (f) {
        fprintf(f, "%s|%s\n", user, pass);
        fclose(f);
        printf("[+] Akun berhasil dibuat!\n");
    }
}

int loginAkun(char *userAktif) {
    char user[40], pass[40], baris[120], uFile[40], pFile[40];
    bacaTeks("Username : ", user, 40);
    bacaTeks("Password : ", pass, 40);

    FILE *f = fopen("data/akun.txt", "r");
    if (!f) {
        printf("[!] File data/akun.txt belum ada. Silakan pilih Register dulu.\n");
        return 0;
    }
    while (fgets(baris, sizeof(baris), f)) {
        baris[strcspn(baris, "\r\n")] = '\0';
        if (sscanf(baris, "%39[^|]|%39[^\n]", uFile, pFile) == 2) {
            if (strcmp(uFile, user) == 0 && strcmp(pFile, pass) == 0) {
                strcpy(userAktif, user);
                fclose(f);
                return 1;
            }
        }
    }
    fclose(f);
    printf("[!] Username atau password salah!\n");
    return 0;
}

/* ==================== FILE I/O JEMAAT (TOPIK 2) ==================== */
int muatJemaat(Jemaat *list) {
    FILE *f = fopen("data/jemaat.txt", "r");
    if (!f) return 0;
    char baris[256];
    int n = 0;
    while (fgets(baris, sizeof(baris), f) && n < MAX_DATA) {
        baris[strcspn(baris, "\r\n")] = '\0';
        if (sscanf(baris, "%d|%59[^|]|%d|%19[^|]|%39[^|]|%19[^\n]",
                   &list[n].id, list[n].nama, &list[n].umur,
                   list[n].kategori, list[n].rayon, list[n].baptis) == 6) {
            n++;
        }
    }
    fclose(f);
    return n;
}

void simpanJemaat(Jemaat *list, int n) {
    FILE *f = fopen("data/jemaat.txt", "w");
    if (!f) return;
    for (int i = 0; i < n; i++) {
        fprintf(f, "%d|%s|%d|%s|%s|%s\n", list[i].id, list[i].nama,
                list[i].umur, list[i].kategori, list[i].rayon, list[i].baptis);
    }
    fclose(f);
}

void cetakTabelJemaat(Jemaat *list, int n) {
    printf("\n%-5s | %-22s | %-4s | %-8s | %-18s | %-12s\n",
           "ID", "Nama", "Umur", "Kategori", "Rayon", "Baptis");
    printf("--------------------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-5d | %-22s | %-4d | %-8s | %-18s | %-12s\n",
               list[i].id, list[i].nama, list[i].umur,
               list[i].kategori, list[i].rayon, list[i].baptis);
    }
}

/* ==================== SORTING & SEARCHING (TOPIK 3 & 5) ==================== */
void bubbleSortNama(Jemaat *list, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (strcmp(list[j].nama, list[j + 1].nama) > 0) {
                Jemaat temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }
}

void selectionSortUmur(Jemaat *list, int n) {
    for (int i = 0; i < n - 1; i++) {
        int terkecil = i;
        for (int j = i + 1; j < n; j++) {
            if (list[j].umur < list[terkecil].umur) terkecil = j;
        }
        Jemaat temp = list[i];
        list[i] = list[terkecil];
        list[terkecil] = temp;
    }
}

void sequentialSearchNama(Jemaat *list, int n) {
    char cari[60];
    bacaTeks("Ketik nama yang dicari: ", cari, 60);
    int ketemu = 0;
    for (int i = 0; i < n; i++) {
        if (strstr(list[i].nama, cari) != NULL) {
            printf("-> [%d] %s (%d thn, %s)\n", list[i].id, list[i].nama, list[i].umur, list[i].rayon);
            ketemu++;
        }
    }
    if (!ketemu) printf("[!] Nama tidak ditemukan.\n");
}

void binarySearchId(Jemaat *list, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (list[j].id < list[i].id) {
                Jemaat temp = list[i]; list[i] = list[j]; list[j] = temp;
            }
        }
    }
    int target = bacaAngka("Masukkan ID Jemaat yang dicari: ");
    int kiri = 0, kanan = n - 1;
    while (kiri <= kanan) {
        int tengah = (kiri + kanan) / 2;
        if (list[tengah].id == target) {
            printf("[+] Ketemu: %s | Umur %d | %s | %s\n",
                   list[tengah].nama, list[tengah].umur, list[tengah].kategori, list[tengah].rayon);
            return;
        } else if (list[tengah].id < target) {
            kiri = tengah + 1;
        } else {
            kanan = tengah - 1;
        }
    }
    printf("[!] ID %d tidak ada.\n", target);
}

/* ==================== MENU 1 & MENU 3 ==================== */
void menuJemaat(void) {
    Jemaat *list = (Jemaat *)malloc(MAX_DATA * sizeof(Jemaat));
    int n = muatJemaat(list);
    int pilih;
    do {
        bersihkanLayar();
        printf("--- MENU DATA JEMAAT (TOPIK 2, 3, 5) ---\n");
        printf("1. Lihat Semua Jemaat\n2. Tambah Jemaat Baru\n");
        printf("3. Urutkan Nama A-Z (Bubble Sort)\n4. Urutkan Umur Muda-Tua (Selection Sort)\n");
        printf("5. Cari Nama (Sequential Search)\n6. Cari ID (Binary Search)\n0. Kembali\n");
        pilih = bacaAngka("Pilih: ");

        if (pilih == 1) { cetakTabelJemaat(list, n); jeda(); }
        else if (pilih == 2 && n < MAX_DATA) {
            list[n].id = bacaAngka("ID Jemaat : ");
            bacaTeks("Nama      : ", list[n].nama, 60);
            list[n].umur = bacaAngka("Umur      : ");
            bacaTeks("Kategori (Anak/Remaja/Pemuda/Umum): ", list[n].kategori, 20);
            bacaTeks("Rayon     : ", list[n].rayon, 40);
            bacaTeks("Baptis (Sudah/Belum): ", list[n].baptis, 20);
            n++;
            simpanJemaat(list, n);
            printf("[+] Tersimpan!\n");
            jeda();
        }
        else if (pilih == 3) { bubbleSortNama(list, n); cetakTabelJemaat(list, n); jeda(); }
        else if (pilih == 4) { selectionSortUmur(list, n); cetakTabelJemaat(list, n); jeda(); }
        else if (pilih == 5) { sequentialSearchNama(list, n); jeda(); }
        else if (pilih == 6) { binarySearchId(list, n); jeda(); }
    } while (pilih != 0);
    free(list);
}

void menuJadwal(void) {
    Jadwal *list = (Jadwal *)malloc(50 * sizeof(Jadwal));
    FILE *f = fopen("data/jadwal.txt", "r");
    int n = 0;
    char baris[256];
    if (f) {
        while (fgets(baris, sizeof(baris), f) && n < 50) {
            baris[strcspn(baris, "\r\n")] = '\0';
            if (sscanf(baris, "%d|%19[^|]|%29[^|]|%19[^|]|%59[^\n]",
                       &list[n].kode, list[n].jenis, list[n].tanggal,
                       list[n].waktu, list[n].lokasi) == 5) n++;
        }
        fclose(f);
    }

    int pilih;
    do {
        bersihkanLayar();
        printf("--- JADWAL IBADAH KATEGORIAL & UMUM ---\n");
        printf("1. Lihat Semua Jadwal\n2. Tambah Jadwal Baru\n0. Kembali\n");
        pilih = bacaAngka("Pilih: ");
        if (pilih == 1) {
            for (int i = 0; i < n; i++) {
                printf("[%d] %-8s | %-20s | %-10s | %s\n",
                       list[i].kode, list[i].jenis, list[i].tanggal, list[i].waktu, list[i].lokasi);
            }
            jeda();
        } else if (pilih == 2 && n < 50) {
            list[n].kode = bacaAngka("Kode Jadwal : ");
            bacaTeks("Jenis (Umum/Keluarga/Anak/Remaja/Pemuda): ", list[n].jenis, 20);
            bacaTeks("Hari & Tanggal : ", list[n].tanggal, 30);
            bacaTeks("Waktu          : ", list[n].waktu, 20);
            bacaTeks("Lokasi         : ", list[n].lokasi, 60);
            n++;
            f = fopen("data/jadwal.txt", "w");
            for (int i = 0; i < n; i++) {
                fprintf(f, "%d|%s|%s|%s|%s\n", list[i].kode, list[i].jenis,
                        list[i].tanggal, list[i].waktu, list[i].lokasi);
            }
            if (f) fclose(f);
            printf("[+] Jadwal ditambahkan!\n");
            jeda();
        }
    } while (pilih != 0);
    free(list);
}

/* ==================== MENU 2: QUEUE ANTREAN (TOPIK 4) ==================== */
void menuQueueAntrean(void) {
    static NodeAntrean *front = NULL, *rear = NULL;
    int pilih;
    do {
        bersihkanLayar();
        printf("--- ANTREAN PENDAFTARAN JEMAAT (QUEUE FIFO) ---\n");
        printf("1. Daftar Masuk Antrean (Enqueue)\n2. Lihat Antrean\n");
        printf("3. Proses Antrean Terdepan ke Data Jemaat (Dequeue)\n0. Kembali\n");
        pilih = bacaAngka("Pilih: ");

        if (pilih == 1) {
            NodeAntrean *baru = (NodeAntrean *)malloc(sizeof(NodeAntrean));
            bacaTeks("Nama Calon Jemaat : ", baru->calon.nama, 60);
            baru->calon.umur = bacaAngka("Umur              : ");
            bacaTeks("Kategori          : ", baru->calon.kategori, 20);
            bacaTeks("Rayon             : ", baru->calon.rayon, 40);
            bacaTeks("Baptis (Sudah/Belum): ", baru->calon.baptis, 20);
            baru->next = NULL;
            if (!rear) front = rear = baru;
            else { rear->next = baru; rear = baru; }
            printf("[+] Masuk antrean!\n");
            jeda();
        } else if (pilih == 2) {
            NodeAntrean *bantu = front;
            int no = 1;
            if (!bantu) printf("[Info] Antrean kosong.\n");
            while (bantu) {
                printf("%d. %s (%d thn - %s)\n", no++, bantu->calon.nama,
                       bantu->calon.umur, bantu->calon.rayon);
                bantu = bantu->next;
            }
            jeda();
        } else if (pilih == 3) {
            if (!front) { printf("[Info] Antrean kosong.\n"); jeda(); continue; }
            NodeAntrean *keluar = front;
            printf("Memproses: %s\n", keluar->calon.nama);
            keluar->calon.id = bacaAngka("Beri ID Jemaat baru: ");

            Jemaat *list = (Jemaat *)malloc(MAX_DATA * sizeof(Jemaat));
            int n = muatJemaat(list);
            list[n++] = keluar->calon;
            simpanJemaat(list, n);
            free(list);

            front = front->next;
            if (!front) rear = NULL;
            free(keluar);
            printf("[+] Sah menjadi anggota jemaat!\n");
            jeda();
        }
    } while (pilih != 0);
}

/* ==================== MENU 4: STACK PAPAN WARTA (TOPIK 4) ==================== */
void pushWarta(NodeWarta **top, const char *tgl, const char *judul, const char *isi) {
    NodeWarta *baru = (NodeWarta *)malloc(sizeof(NodeWarta));
    strcpy(baru->tanggal, tgl);
    strcpy(baru->judul, judul);
    strcpy(baru->isi, isi);
    baru->next = *top;
    *top = baru;
}

void simpanStackRekursif(FILE *f, NodeWarta *node) {
    if (!node) return;
    simpanStackRekursif(f, node->next);
    fprintf(f, "%s|%s|%s\n", node->tanggal, node->judul, node->isi);
}

void menuStackWarta(void) {
    NodeWarta *top = NULL;
    FILE *f = fopen("data/warta.txt", "r");
    char baris[256], tgl[25], judul[60], isi[150];
    if (f) {
        while (fgets(baris, sizeof(baris), f)) {
            baris[strcspn(baris, "\r\n")] = '\0';
            if (sscanf(baris, "%24[^|]|%59[^|]|%149[^\n]", tgl, judul, isi) == 3)
                pushWarta(&top, tgl, judul, isi);
        }
        fclose(f);
    }

    int pilih;
    do {
        bersihkanLayar();
        printf("--- PAPAN WARTA GEREJA (STACK LIFO - TERBARU DI ATAS) ---\n");
        printf("1. Baca Semua Warta\n2. Tambah Warta Baru (Push)\n3. Hapus Warta Teratas (Pop)\n0. Kembali\n");
        pilih = bacaAngka("Pilih: ");

        if (pilih == 1) {
            NodeWarta *b = top;
            if (!b) printf("[Info] Warta kosong.\n");
            while (b) {
                printf("[%s] %s -> %s\n", b->tanggal, b->judul, b->isi);
                b = b->next;
            }
            jeda();
        } else if (pilih == 2) {
            bacaTeks("Tanggal : ", tgl, 25);
            bacaTeks("Judul   : ", judul, 60);
            bacaTeks("Isi     : ", isi, 150);
            pushWarta(&top, tgl, judul, isi);
            f = fopen("data/warta.txt", "w");
            if (f) { simpanStackRekursif(f, top); fclose(f); }
            printf("[+] Warta ditempel di paling atas!\n");
            jeda();
        } else if (pilih == 3) {
            if (top) {
                NodeWarta *hapus = top;
                printf("[-] Warta '%s' dihapus dari tumpukan atas.\n", hapus->judul);
                top = top->next;
                free(hapus);
                f = fopen("data/warta.txt", "w");
                if (f) { simpanStackRekursif(f, top); fclose(f); }
            } else {
                printf("[Info] Warta sudah kosong.\n");
            }
            jeda();
        }
    } while (pilih != 0);

    while (top) { NodeWarta *h = top; top = top->next; free(h); }
}

/* ==================== MENU 5: HASH TABLE (TOPIK 6) ==================== */
void menuHashTable(void) {
    HashNode *laci[HASH_SIZE] = {NULL};
    Jemaat *list = (Jemaat *)malloc(MAX_DATA * sizeof(Jemaat));
    int n = muatJemaat(list);

    for (int i = 0; i < n; i++) {
        int idx = abs(list[i].id) % HASH_SIZE;
        HashNode *baru = (HashNode *)malloc(sizeof(HashNode));
        baru->data = list[i];
        baru->next = laci[idx];
        laci[idx] = baru;
    }
    free(list);

    int pilih;
    do {
        bersihkanLayar();
        printf("--- CEK CEPAT ID JEMAAT (HASH TABLE MOD 10) ---\n");
        printf("1. Lihat Semua Laci (Bucket 0-9)\n2. Cari ID Jemaat\n0. Kembali\n");
        pilih = bacaAngka("Pilih: ");

        if (pilih == 1) {
            for (int i = 0; i < HASH_SIZE; i++) {
                printf("Laci [%d]: ", i);
                for (HashNode *b = laci[i]; b; b = b->next) printf("[%d: %s] -> ", b->data.id, b->data.nama);
                printf("NULL\n");
            }
            jeda();
        } else if (pilih == 2) {
            int cari = bacaAngka("Masukkan ID: ");
            int idx = abs(cari) % HASH_SIZE;
            HashNode *b = laci[idx];
            while (b && b->data.id != cari) b = b->next;
            if (b) printf("[+] Ketemu di Laci [%d]: %s (%s)\n", idx, b->data.nama, b->data.rayon);
            else printf("[!] Tidak ditemukan di Laci [%d].\n", idx);
            jeda();
        }
    } while (pilih != 0);

    for (int i = 0; i < HASH_SIZE; i++) {
        while (laci[i]) { HashNode *h = laci[i]; laci[i] = laci[i]->next; free(h); }
    }
}

/* ==================== MENU 6: BINARY SEARCH TREE (TOPIK 7) ==================== */
BSTNode *insertBST(BSTNode *root, Jemaat d) {
    if (!root) {
        BSTNode *baru = (BSTNode *)malloc(sizeof(BSTNode));
        baru->data = d; baru->kiri = baru->kanan = NULL;
        return baru;
    }
    if (d.id < root->data.id) root->kiri = insertBST(root->kiri, d);
    else if (d.id > root->data.id) root->kanan = insertBST(root->kanan, d);
    return root;
}

void cetakInOrder(BSTNode *root) {
    if (!root) return;
    cetakInOrder(root->kiri);
    printf("  ID %-5d | %s (%s)\n", root->data.id, root->data.nama, root->data.rayon);
    cetakInOrder(root->kanan);
}

BSTNode *hapusBST(BSTNode *root, int id) {
    if (!root) return NULL;
    if (id < root->data.id) root->kiri = hapusBST(root->kiri, id);
    else if (id > root->data.id) root->kanan = hapusBST(root->kanan, id);
    else {
        if (!root->kiri) { BSTNode *t = root->kanan; free(root); return t; }
        if (!root->kanan) { BSTNode *t = root->kiri; free(root); return t; }
        BSTNode *pengganti = root->kanan;
        while (pengganti->kiri) pengganti = pengganti->kiri;
        root->data = pengganti->data;
        root->kanan = hapusBST(root->kanan, pengganti->data.id);
    }
    return root;
}

void bebasBST(BSTNode *root) {
    if (root) { bebasBST(root->kiri); bebasBST(root->kanan); free(root); }
}

void menuBST(void) {
    BSTNode *root = NULL;
    Jemaat *list = (Jemaat *)malloc(MAX_DATA * sizeof(Jemaat));
    int n = muatJemaat(list);
    for (int i = 0; i < n; i++) root = insertBST(root, list[i]);
    free(list);

    int pilih;
    do {
        bersihkanLayar();
        printf("--- POHON DATA JEMAAT (BINARY SEARCH TREE) ---\n");
        printf("1. Tampilkan Urut ID (In-Order)\n2. Cari ID di Pohon\n");
        printf("3. Tambah Node ke Pohon\n4. Hapus Node dari Pohon\n0. Kembali\n");
        pilih = bacaAngka("Pilih: ");

        if (pilih == 1) { cetakInOrder(root); jeda(); }
        else if (pilih == 2) {
            int cari = bacaAngka("Cari ID: ");
            BSTNode *b = root;
            while (b && b->data.id != cari) {
                printf("  Lewati ID %d -> belok %s\n", b->data.id, (cari < b->data.id) ? "KIRI" : "KANAN");
                b = (cari < b->data.id) ? b->kiri : b->kanan;
            }
            if (b) printf("[+] Ketemu: %s (%s)\n", b->data.nama, b->data.rayon);
            else printf("[!] ID tidak ada di pohon.\n");
            jeda();
        } else if (pilih == 3) {
            Jemaat d;
            d.id = bacaAngka("ID Baru : ");
            bacaTeks("Nama    : ", d.nama, 60);
            bacaTeks("Rayon   : ", d.rayon, 40);
            root = insertBST(root, d);
            printf("[+] Masuk ke pohon BST!\n");
            jeda();
        } else if (pilih == 4) {
            int id = bacaAngka("ID yang dihapus: ");
            root = hapusBST(root, id);
            printf("[+] Proses hapus selesai.\n");
            jeda();
        }
    } while (pilih != 0);
    bebasBST(root);
}

/* ==================== MENU 7: GRAPH BFS & DFS (TOPIK 8) ==================== */
void dfs(int u, int vTotal, int adj[MAX_TITIK][MAX_TITIK], char nama[MAX_TITIK][40], int vis[]) {
    vis[u] = 1;
    printf(" -> [%d] %s\n", u, nama[u]);
    for (int v = 0; v < vTotal; v++) {
        if (adj[u][v] && !vis[v]) dfs(v, vTotal, adj, nama, vis);
    }
}

void menuGraphRayon(void) {
    FILE *f = fopen("data/rayon.txt", "r");
    if (!f) { printf("[!] File data/rayon.txt tidak ditemukan.\n"); jeda(); return; }

    int V = 0, E = 0, adj[MAX_TITIK][MAX_TITIK] = {{0}};
    char nama[MAX_TITIK][40], baris[120];
    fscanf(f, "%d %d\n", &V, &E);
    for (int i = 0; i < V; i++) {
        int idx;
        if (fgets(baris, sizeof(baris), f)) {
            baris[strcspn(baris, "\r\n")] = '\0';
            sscanf(baris, "%d|%39[^\n]", &idx, nama[i]);
        }
    }
    for (int i = 0; i < E; i++) {
        int u, v;
        if (fscanf(f, "%d %d", &u, &v) == 2) {
            adj[u][v] = adj[v][u] = 1;
        }
    }
    fclose(f);

    int pilih;
    do {
        bersihkanLayar();
        printf("--- PETA KUNJUNGAN RAYON (GRAPH BFS & DFS) ---\n");
        for (int i = 0; i < V; i++) printf("  [%d] %s\n", i, nama[i]);
        printf("1. Sebaran Warta Terdekat (BFS)\n2. Rute Kunjungan Sampai Ujung (DFS)\n0. Kembali\n");
        pilih = bacaAngka("Pilih: ");

        if (pilih == 1) {
            int awal = bacaAngka("Mulai dari titik (0-5): ");
            if (awal < 0 || awal >= V) { printf("[!] Titik tidak valid.\n"); jeda(); continue; }
            int vis[MAX_TITIK] = {0}, q[MAX_TITIK], fIdx = 0, rIdx = 0;
            vis[awal] = 1; q[rIdx++] = awal;
            while (fIdx < rIdx) {
                int u = q[fIdx++];
                printf(" -> [%d] %s\n", u, nama[u]);
                for (int v = 0; v < V; v++) {
                    if (adj[u][v] && !vis[v]) { vis[v] = 1; q[rIdx++] = v; }
                }
            }
            jeda();
        } else if (pilih == 2) {
            int awal = bacaAngka("Mulai dari titik (0-5): ");
            if (awal < 0 || awal >= V) { printf("[!] Titik tidak valid.\n"); jeda(); continue; }
            int vis[MAX_TITIK] = {0};
            dfs(awal, V, adj, nama, vis);
            jeda();
        }
    } while (pilih != 0);
}

/* ==================== PROGRAM UTAMA ==================== */
int main(void) {
    char userAktif[40];
    int pilihAwal, pilihMenu;

    while (1) {
        bersihkanLayar();
        printf("==============================================\n");
        printf("   SISTEM INFORMASI GEREJA (SiJemaat-C)\n");
        printf("==============================================\n");
        printf("1. Login\n2. Register Akun\n3. Keluar\n");
        pilihAwal = bacaAngka("Pilih (1-3): ");

        if (pilihAwal == 3) break;
        if (pilihAwal == 2) { registerAkun(); jeda(); continue; }
        if (pilihAwal != 1) continue;
        if (!loginAkun(userAktif)) { jeda(); continue; }

        do {
            bersihkanLayar();
            printf("=== MENU UTAMA (Petugas: %s) ===\n", userAktif);
            printf("1. Data Anggota Jemaat (File, Sorting, Searching)\n");
            printf("2. Antrean Pendaftaran Jemaat (Queue FIFO)\n");
            printf("3. Jadwal Ibadah Kategorial & Umum\n");
            printf("4. Papan Warta Gereja (Stack LIFO)\n");
            printf("5. Cek Cepat ID Jemaat (Hash Table)\n");
            printf("6. Pohon Data Jemaat (Binary Search Tree)\n");
            printf("7. Peta Kunjungan Rayon (Graph BFS & DFS)\n");
            printf("0. Logout\n");
            pilihMenu = bacaAngka("Pilih (0-7): ");

            switch (pilihMenu) {
                case 1: menuJemaat(); break;
                case 2: menuQueueAntrean(); break;
                case 3: menuJadwal(); break;
                case 4: menuStackWarta(); break;
                case 5: menuHashTable(); break;
                case 6: menuBST(); break;
                case 7: menuGraphRayon(); break;
            }
        } while (pilihMenu != 0);
    }
    return 0;
}
