import json, os
import firebase_admin as FDB
from firebase_admin import credentials, firestore, storage
from firebase_admin import db
from pathlib import Path
import google.auth.exceptions as firebase_err

class db_configServer:
    # buat inisialisasi object class db_configServer
    def __init__(self, reference=""):
        """
            Masukan parameter db_configServer(reference="reference")
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
            
    # buat fungsi check configurate esp server di database
    def read_config_server(self, configServer, http_default, server_default):
        """            
        Masukan parameter db_configServer(__reference__).read_config_server(__configServer__, __http_default__, __server_default__)
        
        configServer -> yang tersedia di dalam console.firebase.google.com
        """
        config = self.ref.child("config-ipaddress-esp-server").child(configServer).get();
        dataConfig = [];
        if (config != None):
            for value in config.values():
                dataConfig.append(value);

        else:
            self.ref.child("config-ipaddress-esp-server").child(configServer).child("ip-address").set(http_default);
            self.ref.child("config-ipaddress-esp-server").child(configServer).child("kubikel").set(server_default);
            
            newconfig = self.ref.child("config-ipaddress-esp-server").child(configServer).get();
            if (newconfig != None):
                for value in newconfig.values():
                    dataConfig.append(value);

        return dataConfig;