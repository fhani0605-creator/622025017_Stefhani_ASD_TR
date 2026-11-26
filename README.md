# SiJemaat-C — Sistem Informasi Manajemen Jemaat Gereja

## Identitas Mahasiswa
* **Nama:** Stefhani Gloria Maleke
* **NIM:** 622025017
* **Program Studi:** Teknik Komputer — FTEK UKSW
* **Mata Kuliah:** Tugas Rancang Algoritma dan Struktur Data (ASD)

## Deskripsi Aplikasi
**SiJemaat-C** adalah aplikasi konsol berbasis bahasa C untuk membantu pengelolaan administrasi jemaat gereja secara terstruktur. Seluruh data operasional (akun petugas, anggota jemaat, jadwal ibadah, warta gereja, dan peta rayon) disimpan secara permanen dalam berkas teks (`.txt`) di dalam folder `data/` serta diolah menggunakan berbagai algoritma dan struktur data.

## Daftar Fitur
1. **Autentikasi Petugas & Validasi Input:** Fitur *Login* dan *Register* akun petugas dilengkapi proteksi masukan berbasis `fgets()` untuk mencegah kesalahan tipe input dan duplikasi username.
2. **Manajemen Data Jemaat (`File I/O`, `Sorting`, & `Searching`):**
   * Menampilkan daftar seluruh jemaat dan menambahkan data anggota baru ke `data/jemaat.txt`.
   * Pengurutan nama jemaat A–Z (**Bubble Sort**) dan pengurutan usia termuda–tertua (**Selection Sort**).
   * Pencarian nama parsial (**Sequential Search**) dan pencarian ID jemaat (**Binary Search**).
3. **Antrean Pendaftaran Jemaat (`Queue FIFO`):** Mengelola antrean calon jemaat baru secara *First-In First-Out* (*Enqueue* dan *Dequeue* langsung ke basis data jemaat).
4. **Jadwal Ibadah Kategorial & Umum:** Menampilkan dan menambahkan jadwal ibadah ke `data/jadwal.txt`.
5. **Papan Warta Gereja (`Stack LIFO`):** Menampilkan, menambahkan (*Push*), dan menghapus (*Pop*) pengumuman gereja dengan urutan warta terbaru di posisi paling atas.
6. **Cek Cepat ID Jemaat (`Hash Table`):** Pemetaan dan pencarian ID jemaat ke dalam 10 *bucket* (`id % 10`) menggunakan resolusi kolisi *Separate Chaining*.
7. **Pohon Data Jemaat (`Binary Search Tree`):** Penyisipan simpul, penelusuran arah cabang (`KIRI`/`KANAN`), tampilan data terurut (*In-Order*), dan penghapusan simpul pada BST.
8. **Peta Kunjungan Rayon (`Graph BFS & DFS`):** Pemetaan keterhubungan wilayah rayon menggunakan matriks ketetanggaan serta penelusuran rute kunjungan memakai algoritma **Breadth-First Search (BFS)** dan **Depth-First Search (DFS)**.

## Perintah Kompilasi dan Menjalankan Program
Buka terminal di direktori utama proyek (`622025017_Stefhani_ASD_TR`), lalu jalankan perintah berikut:

```bash
gcc src/main.c -o app
.\app.exe
