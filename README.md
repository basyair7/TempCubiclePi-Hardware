# TempCubiclePi-Hardware
Project Tugas Akhir Fathul Basyair (Mahasiswa Teknik Elektro USK 2019)

## Keterangan Direktori
- program_server_1 : berisi program C++ untuk arduino uno (atmega328p), esp32 board, dan file matlab (fuzzy fis)
- program_server_2 : berisi program Python untuk server Raspberry Pi atau Mini PC

## Pembaruan Program
- Memperbaiki program arduino sebagai input PWM dimmer
- Memperbaiki dan menambahkan fungsi enable/disable program fuzzy
- Memindahkan program fuzzy dari arduino ke ESP32 server
- Menambahan uploader OTA
- Memperbaiki help page dan menambahkan system info

## Tujuan program
Tujuan program ini untuk menjaga suhu dan kelembapan pada kubikel 20kv secara otomatis dengan logika fuzzy

## Opsional
Silahkan download <a href="https://sqlitebrowser.org/dl/">DB Browser for SQLite</a> agar lebih leluasa untuk operasi aplikasi TempCubiclePi

## Spesifikasi
- Program ini berjalan pada raspberry 3b, raspberry 4b, mini server, ESP32, dan arduino uno (Atmega328p)
- Hanya bisa pada aplikasi TempCubiclePi versi 1.7.2, versi 1.7.3, hingga terbaru.
# Lisensi Aplikasi
Aplikasi ini memiliki lisensi BSD 3-Clause License, dengan catatan
1. Harus memiliki Database Config untuk menjalankan program
2. Memiliki izin dari developer untuk memodifikasi aplikasi
3. Developer TempCubiclePi tidak bertanggung jawab jika pengguna meng-unduh aplikasi selain dari https://github.com/basyair7/TempCubiclePi-Hardware (aplikasi bajakan)

# Powered By
<p align="center">
    <a href="https://firebase.google.com/" target="_blank">
        <img src="https://www.gstatic.com/devrel-devsite/prod/v4adef427db21a4cd79f489fce8da23c25ef3f53705a17ddcb0611ee166b2e610/firebase/images/lockup.svg" width="30%">
    </a>
</p>

<h3 align="center"><a href="https://firebase.google.com/" target="_blank">Firebase Database</a></h3>

