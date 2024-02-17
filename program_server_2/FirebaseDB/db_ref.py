import json, os
import firebase_admin as FDB
from firebase_admin import credentials, firestore, storage
from firebase_admin import db
from pathlib import Path
import google.auth.exceptions as firebase_err

class db_ref:
    # buat inisialisasi object class db_ref
    def __init__ (self, reference=""):
        """
            Masukan parameter db_ref(reference)
        """
        if (os.name == 'nt'):
            # buka file alamat database 
            with open('./FirebaseDB/FirebaseConfig/firebaseHttps.json') as file_1:
                databaseURL_1 = json.load(file_1)["https"];
                
            with open('./FirebaseDB/FirebaseConfig/firebaseHttps.json') as file_2:
                databaseURL_2 = json.load(file_2)["https_storage"];

            # buka file sertifikat database
            cred = credentials.Certificate("./FirebaseDB/FirebaseConfig/serviceAccountKey.json");
            if not FDB._apps:
                FDB.initialize_app(cred, {
                    'databaseURL': databaseURL_1,
                    'storageBucket': databaseURL_2
                });
            # ambil reference root firebase
            if (reference == "" or reference == None):
                raise TypeError("FirebaseDB.db_ref() : Silahkan isi nama reference database");

            self.ref = db.reference(reference);
            self.reference = reference;
        
        if (os.name == 'posix'):
            # buka file alamat database 
            with open(f'{Path.home()}/monitor_kubikel_iot/FirebaseDB/FirebaseConfig/firebaseHttps.json') as file_1:
                databaseURL_1 = json.load(file_1)["https"];
                
            with open(f'{Path.home()}/monitor_kubikel_iot/FirebaseDB/FirebaseConfig/firebaseHttps.json') as file_2:
                databaseURL_2 = json.load(file_2)["https_storage"];

            # buka file sertifikat database
            cred = credentials.Certificate(f"{Path.home()}/monitor_kubikel_iot/FirebaseDB/FirebaseConfig/serviceAccountKey.json");

            if not FDB._apps:
                FDB.initialize_app(cred, {
                    'databaseURL': databaseURL_1,
                    'storageBucket': databaseURL_2
                });
            # ambil reference root firebase
            if (reference == "" or reference == None):
                raise TypeError("FirebaseDB.db_ref() : Silahkan isi nama reference database");

            self.ref = db.reference(reference);
            self.reference = reference;

    # buat fungsi insert data dalam object db_ref
    def insert_data (self, 
        kubikel, date, time, 
        tegangan_heater, arus_heater, daya_heater, 
        energy_heater, freq_heater, pf_heater, 
        suhu, kelembapan, dimmer_heater, dimmer_fan, 
        fisOutputHeater, fisOutputFan):
        """
        masukan parameter 

        insert_data(kubikel="nama_kubikel", date="tanggal_server", time="waktu_server", 
        tegangan_heater=int(tegangan_heater), arus_heater=float(arus_heater), daya_heater=float(daya_heater), energy_heater=float(energy_heater), freq_heater=float(freq_heater), pf_heater=float(pf_heater),
        suhu=float(suhu), kelembapan=float(kelembapan), dimmer_heater=float(dimmer_heater), dimmer_fan=float(dimmer_fan),
        fisOutputHeater=float(fisOutputHeater), fisOutputFan=float(fisOutputFan));    
        """
        # ambil data sensor
        self.date = date; self.time = time; self.kubikel = kubikel;
        self.arus_heater = arus_heater; self.tegangan_heater = tegangan_heater; self.daya_heater = daya_heater;
        self.energy_heater = energy_heater; self.freq_heater = freq_heater; self.pf_heater = pf_heater;
        self.suhu = suhu; self.kelembapan = kelembapan; self.dimmer_heater = dimmer_heater; self.dimmer_fan = dimmer_fan;
        self.fisOutputHeater = fisOutputHeater; self.fisOutputFan = fisOutputFan;

        # proses memasukkan data ke database real-time
        # upload tanpa tanggal (untuk dikirim ke aplikasi)
        self.ref.child(self.kubikel).child("tanggal").set(self.date); 
        self.ref.child(self.kubikel).child("waktu").set(time);
        self.ref.child(self.kubikel).child("GH").set(self.reference); 
        self.ref.child(self.kubikel).child("kubikel").set(self.kubikel);
        self.ref.child(self.kubikel).child("arus_heater").set(self.arus_heater); 
        self.ref.child(self.kubikel).child("tegangan_heater").set(self.tegangan_heater);
        self.ref.child(self.kubikel).child("daya_heater").set(self.daya_heater); 
        self.ref.child(self.kubikel).child("energy_heater").set(self.energy_heater);
        self.ref.child(self.kubikel).child("freq_heater").set(self.freq_heater); 
        self.ref.child(self.kubikel).child("pf_heater").set(self.pf_heater);
        self.ref.child(self.kubikel).child("suhu").set(self.suhu); 
        self.ref.child(self.kubikel).child("kelembapan").set(self.kelembapan);
        self.ref.child(self.kubikel).child("dimmer_heater").set(self.dimmer_heater); 
        self.ref.child(self.kubikel).child("dimmer_fan").set(self.dimmer_fan);
        self.ref.child(self.kubikel).child("fisOutputHeater").set(self.fisOutputHeater); 
        self.ref.child(self.kubikel).child("fisOutputFan").set(self.fisOutputFan);
        
    # buat fungsi save_data dalam object db_ref
    def save_data(self, 
        kubikel, date, time, 
        tegangan_heater, arus_heater, daya_heater, 
        energy_heater, freq_heater, pf_heater, 
        suhu, kelembapan, dimmer_heater, dimmer_fan, 
        fisOutputHeater, fisOutputFan):
        """
        masukan parameter 

        insert_data(kubikel="nama_kubikel", date="tanggal_server", time="waktu_server", 
        tegangan_heater=int(tegangan_heater), arus_heater=float(arus_heater), daya_heater=float(daya_heater), energy_heater=float(energy_heater), freq_heater=float(freq_heater), pf_heater=float(pf_heater),
        suhu=float(suhu), kelembapan=float(kelembapan), dimmer_heater=float(dimmer_heater), dimmer_fan=float(dimmer_fan),
        fisOutputHeater=float(fisOutputHeater), fisOutputFan=float(fisOutputFan));    
        """
        # ambil data sensor
        self.date = date; self.time = time; self.kubikel = kubikel;
        self.arus_heater = arus_heater; self.tegangan_heater = tegangan_heater; self.daya_heater = daya_heater;
        self.energy_heater = energy_heater; self.freq_heater = freq_heater; self.pf_heater = pf_heater;
        self.suhu = suhu; self.kelembapan = kelembapan; self.dimmer_heater = dimmer_heater; self.dimmer_fan = dimmer_fan;
        self.fisOutputHeater = fisOutputHeater; self.fisOutputFan = fisOutputFan;
        
        value_dict = {
            "datetimes": {
                "date": self.date,
                "time": self.time
            },
            "data_pzem": {
                "arus_heater": self.arus_heater, 
                "tegangan_heater": self.tegangan_heater, 
                "daya_heater": self.daya_heater, 
                "energy_heater": self.energy_heater, 
                "freq_heater": self.freq_heater, 
                "pf_heater": self.pf_heater
            },
            "data_dht": {
                "suhu": self.suhu, 
                "kelembapan": self.kelembapan
            },
            "data_atmega": {
                "dimmer_heater": self.dimmer_heater, 
                "dimmer_fan": self.dimmer_fan,
                "fisOutputHeater": self.fisOutputHeater, 
                "fisOutputFan": self.fisOutputFan
            }
        }
        
        self.ref.child("save_data").child(self.kubikel).push(value_dict);
        
    # buat fungsi read data dalam object db_ref
    def read_data(self, GH, kubikel):
        # ambil nama Gardu Hubung
        if (GH == "" or GH == None):
            print("Informasi -> Silahkan isi nama Gardu Hubung!");
            return exit();
        
        # ambil kode kubikel
        if (kubikel == "" or kubikel == None):
            print("Informasi -> Silahkan isi kode kubikel");
            return exit();
        
        db_GH = db.reference(GH)
        tanggal =  db_GH.child(kubikel).child("tanggal").get();
        waktu = db_GH.child(kubikel).child("waktu").get();
        arus_heater = db_GH.child(kubikel).child("arus_heater").get();
        tegangan_heater = db_GH.child(kubikel).child("tegangan_heater").get();
        daya_heater = db_GH.child(kubikel).child("daya_heater").get();
        suhu = db_GH.child(kubikel).child("suhu").get();
        kelembapan = db_GH.child(kubikel).child("kelembapan").get();
        dimmer_heater = db_GH.child(kubikel).child("dimmer_heater").get();
        dimmer_fan = db_GH.child(kubikel).child("dimmer_fan").get();
        fisOutputHeater = db_GH.child(kubikel).child("fisOutputHeater").get();
        fisOutputFan = db_GH.child(kubikel).child("fisOutputFan").get();
        

        return {
            "tanggal": tanggal, "waktu": waktu, "GH": GH, "kubikel": kubikel, "arus_heater": arus_heater, "tegangan_heater": tegangan_heater, "daya_heater": daya_heater,
            "suhu": suhu, "kelembapan": kelembapan, "dimmer_heater": dimmer_heater, "dimmer_fan": dimmer_fan, 
            "fisOutputHeater": fisOutputHeater, "fisOutputFan": fisOutputFan
        }
                    
    def set_order_record_db(self, kubikel):
        self.ref.child(kubikel).child("order_record_video").set(False);
            
    def read_order(self, order_record):
        val = self.ref.child(order_record).child("order_record_video").get();
        return val;
    
    def set_reset_esp_state(self, kubikel, state):
        """
        masukan parameter set_reset_esp_state(kubikel="kubikel", state=int(nilai_state));

        note : nilai_state berupa 0 atau 1
        """
        self.ref.child(kubikel).child("reset_esp_state").set(int(state));

    def read_reset_esp_state(self, kubikel):
        """
        masukan parameter read_reset_esp_state(kubikel="kubikel");
        """
        state = int(self.ref.child(kubikel).child("reset_esp_state").get());
        return state;
    
    def set_reset_server_state(self, state):
        """
        masukan parameter set_reset_server_state(state=int(nilai_state));

        note : nilai_state berupa 0 atau 1
        """
        self.ref.child("reset_server_state").set(int(state));

    def read_reset_server_state(self):
        state = int(self.ref.child("reset_server_state").get());
        return state;
        
    def remove_data(self, path_data):
        self.ref.child(path_data).remove();
        