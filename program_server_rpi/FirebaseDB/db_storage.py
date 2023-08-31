import json, os
import firebase_admin as FDB
from firebase_admin import credentials, firestore, storage
from firebase_admin import db
from pathlib import Path
import google.auth.exceptions as firebase_err


# Upload dan Download data dari Firebase Database Real-Time
class db_storage:
    #  buat inisialisasi objek kelas db_storage
    def __init__(self):
        if (os.name == 'nt'): 
            # buka file alamat firebase
            with open('./FirebaseDB/firebaseHttps.json') as file_1:
                databaseURL_1 = json.load(file_1)["https"];
                
            with open('./FirebaseDB/firebaseHttps.json') as file_2:
                databaseURL_2 = json.load(file_2)["https_storage"];

            # buka file sertifikat database
            cred = credentials.Certificate("./FirebaseDB/serviceAccountKey.json");
            if not FDB._apps:
                FDB.initialize_app(cred, {
                    'databaseURL': databaseURL_1,
                    'storageBucket': databaseURL_2
                });
            
            self.storageDB = firestore.client();
            self.bucket = storage.bucket();

        if (os.name == 'posix'):
            # buka file alamat firebase
            with open('./FirebaseDB/firebaseHttps.json') as file_1:
                databaseURL_1 = json.load(file_1)["https"];
                
            with open('./FirebaseDB/firebaseHttps.json') as file_2:
                databaseURL_2 = json.load(file_2)["https_storage"];

            # buka file sertifikat database
            cred = credentials.Certificate("./FirebaseDB/serviceAccountKey.json");
            if not FDB._apps:
                FDB.initialize_app(cred, {
                    'databaseURL': databaseURL_1,
                    'storageBucket': databaseURL_2
                });
            
            self.storageDB = firestore.client();
            self.bucket = storage.bucket();
            
    # Upload file ke database
    def upload_data(self, path, data):
        """
            masukan parameter db_storage.upload_data(__path__, __data__);
            path = string; data = string
        """
        blob = self.bucket.blob(f"{path}/{data}.mp4");
        blob.upload_from_filename(f"{path}/{data}.mp4");

    # Download file dari database
    def download_data(self, path, data):
        """
            masukan parameter db_storage.download_data(__path__, __data__);
            path = string; data = string
        """
        blob = self.bucket.blob(f"{path}/{data}");
        blob.download_to_filename(f"{path}/{data}.mp4");