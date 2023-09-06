"""
PROGRAM PROJECT MONITORING SUHU DAN KELEMBABABAN KUBIKEL 20 KV
NAMA    : FATHUL BASYAIR
NPM     : 1904105010004
PRODI   : TEKNIK ELEKTRO

FILE INI ADALAH LIBRARY TEMPAT MENYIMPAN DATA KE DALAM DATABASE LOKAL
WARNING : DILARANG KERAS MENGUBAH PROGRAM INI / PLAGIARISME
"""

import os
from sqlite3 import connect, IntegrityError, OperationalError
from pathlib import Path

class db_connect:
    # buat inisialisasi object class db_connect
    def __init__ (self, namadb=""):
        # ambil nama database
        if namadb == None or namadb == "":
            return print("Informasi -> Silahkan isi nama database!")
        
        self.namadb = namadb

    def create(self):
        conn = connect(self.namadb);
        c = conn.cursor();

        # commit 
        conn.commit();
        # tutup database
        conn.close();

    # buat fungsi insert data kubikel dalam object db_connect
    def insert_data_kubikel (self, 
        gardu_hubung, kubikel, date, time, 
        tegangan_heater, arus_heater, daya_heater, 
        energy_heater, freq_heater, pf_heater, 
        suhu, kelembapan, dimmer_heater, dimmer_fan, 
        fisOutputHeater, fisOutputFan):
        """
        masukan parameter 

        insert_data_kubikel(gardu_hubung="gardu_hubung", kubikel="nama_kubikel", date="tanggal_server", time="waktu_server", 
        tegangan_heater=int(tegangan_heater), arus_heater=float(arus_heater), daya_heater=float(daya_heater), energy_heater=float(energy_heater), freq_heater=float(freq_heater), pf_heater=float(pf_heater),
        suhu=float(suhu), kelembapan=float(kelembapan), dimmer_heater=float(dimmer_heater), dimmer_fan=float(dimmer_fan),
        fisOutputHeater=float(fisOutputHeater), fisOutputFan=float(fisOutputFan));    
        """
        
        try:
            # ambil nama gardu hubung dan kubikel
            if (kubikel == None or kubikel == ""):
                raise print("Informasi -> Silahkan isi nama Kubikel!");
            
            if (gardu_hubung == None or gardu_hubung == ""):
                raise print("Informasi -> Silahkan isi nama Gardu Hubung!");

            # buat database lokal
            conn = connect(self.namadb);
            c = conn.cursor();

            # buat table database jika tidak tersedia
            c.execute(f"""CREATE TABLE IF NOT EXISTS {kubikel} (
                date TEXT, time TEXT, gardu_hubung TEXT, kubikel TEXT, 
                tegangan_heater REAL, arus_heater REAL, daya_heater REAL, energy_heater REAL, 
                freq_heater REAL, pf_heater REAL, suhu REAL, kelembapan REAL, 
                dimmer_heater REAL, dimmer_fan REAL, fisOutputHeater REAL, fisOutputFan REAL);
            """);

            # commit
            conn.commit();

            # isi data ke dalam database lokal
            c.execute(f"""INSERT INTO {kubikel} VALUES (
                :date, :time, :gardu_hubung, :kubikel, 
                :tegangan_heater, :arus_heater, :daya_heater, :energy_heater, :freq_heater, :pf_heater,
                :suhu, :kelembaban, :dimmer_heater, :dimmer_fan, :fisOutputHeater, :fisOutputFan)""", 
            { 
                'date': date, 'time': time, 'gardu_hubung': gardu_hubung, 'kubikel': kubikel, 
                'tegangan_heater': tegangan_heater, 'arus_heater': arus_heater, 'daya_heater': daya_heater, 
                'energy_heater': energy_heater, 'freq_heater': freq_heater, 'pf_heater': pf_heater, 
                'suhu': suhu, 'kelembaban': kelembapan, 'dimmer_heater': dimmer_heater, 'dimmer_fan': dimmer_fan, 
                'fisOutputHeater': fisOutputHeater, 'fisOutputFan': fisOutputFan 
            });

            # commit 
            conn.commit();
            # tutup database
            conn.close();
        
        except:
            print("Error SQLITE Database");