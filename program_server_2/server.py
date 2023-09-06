"""
PROGRAM PROJECT MONITORING SUHU DAN KELEMBABAN KUBIKEL 20 KV
NAMA    : FATHUL BASYAIR
NPM     : 1904105010004
PRODI   : TEKNIK ELEKTRO

FILE INI ADALAH TEMPAT PROGRAM UTAMA UPLOAD DATA DARI SERVER (RASPBERRY PI) KE FIREBASE DATABASE REAL-TIME

!!! WARNING: DILARANG KERAS MENGUBAH ISI FILE INI / PLAGIARISME !!!
"""

# library program
from BackendProgram import server_esp, datetime, urlopen, json
from DatabaseSQLite import db_connect
from FirebaseDB import db_ref, os
from time import sleep
from pathlib import Path
import FirebaseDB
import google.auth.exceptions as firebase_err

# masukan nama Gardu Hubung
GH = 'GH_PKA';

upload_data     = 60; # 60 menit
reload_upload   = 1;  # 1 menit


# buat variabel tampung nilai error program
numberErr_restart_server = 0;
numberErr_program_main_1 = 0;
numberErr_program_main_2 = 0;

# list nama bulan untuk file database
list_month = ["januari", "februari", "maret", 
              "april", "mei", "juni", 
              "juli", "agustus", "september",
              "oktober", "november", "desember"];

def program_main(ip_server="", kubikel=""):
    # ambil tanggal dan waktu di dalam server
    now = datetime.datetime.now();
    day = now.day
    month = now.month
    year = now.year
    date_time_error = f"{day}/{month}/{year} ({(now.strftime('%H:%M:%S'))})"
    # buat variabel global data
    global date, time, minute, second, kode_kubikel
    global tegangan_heater, arus_heater, daya_heater, energy_heater
    global freq_heater, pf_heater, suhu, kelembapan, dimmer_heater
    global dimmer_fan, fisOutputHeater, fisOutputFan
    try:
        # mengambil data dari rest api server ESP32
        data                 = server_esp(ip_server, kubikel);
        date                 = data.get_data()['date_time']['date'];
        time                 = data.get_data()['date_time']['time'];
        minute               = int(data.get_data()['date_time']['minute'])
        second               = int(data.get_data()['date_time']['second'])
        kode_kubikel         = data.get_data()['kode_kubikel'];
        tegangan_heater      = round(float(data.get_data()['data_pzem']['tegangan_heater']), 2);
        arus_heater          = round(float(data.get_data()['data_pzem']['arus_heater']), 2);
        daya_heater          = round(float(data.get_data()['data_pzem']['daya_heater']), 2);
        energy_heater        = round(float(data.get_data()['data_pzem']['energy_heater']), 2);
        freq_heater          = round(float(data.get_data()['data_pzem']['frequency_heater']), 2);
        pf_heater            = round(float(data.get_data()['data_pzem']['pf_heater']), 2);
        suhu                 = round((float(data.get_data()['data_dht']['suhu'])), 2);
        kelembapan           = round(float(data.get_data()['data_dht']['kelembapan']), 2);
        dimmer_heater        = data.get_data()['data_atmega']['dimmer_heater'];
        dimmer_fan           = data.get_data()['data_atmega']['dimmer_fan'];
        fisOutputHeater      = round(float(data.get_data()['data_atmega']['fisOutputHeater']), 2);
        fisOutputFan         = round(float(data.get_data()['data_atmega']['fisOutputFan']), 2);
        
        # nama database lokal (sqlite3)
        nama_db = f"{Path.home()}/Desktop/data_server_{GH}_{list_month[month - 1]}_{year}_tester.db";
        # buat file database lokal otomatis (jika belum tersedia)
        sql_local = db_connect(nama_db);
        sql_local.create();
        # masukan data ke dalam database lokal
        sql_local.insert_data_kubikel(gardu_hubung=GH, kubikel=kode_kubikel, date=date, time=time, 
            tegangan_heater=tegangan_heater, arus_heater=arus_heater, daya_heater=daya_heater, energy_heater=energy_heater, 
            freq_heater=freq_heater, pf_heater=pf_heater, suhu=suhu, kelembapan=kelembapan, dimmer_heater=dimmer_heater, dimmer_fan=dimmer_fan, 
            fisOutputHeater=fisOutputHeater, fisOutputFan=fisOutputFan
        );
        
    except:
        global numberErr_program_main_1;
        numberErr_program_main_1 += 1;
        print(f"{date_time_error}: No.{numberErr_program_main_1} Error -> tidak bisa input data ke database lokal {ip_server}/{kubikel}!");

# buat fungsi program_main server
def program_upload():
    # ambil tanggal dan waktu di dalam server
    now = datetime.datetime.now();
    day = now.day
    month = now.month
    year = now.year
    date_time_error = f"{day}/{month}/{year} ({(now.strftime('%H:%M:%S'))})"

    try:
    # unggah data ke firebase database real-time
        if(minute%reload_upload == 0 and second <= 15):
            database = db_ref(GH)
            database.insert_data(
                date=date, time=time, kubikel=kode_kubikel,
                tegangan_heater=tegangan_heater, arus_heater=arus_heater, daya_heater=daya_heater,
                energy_heater=energy_heater, freq_heater=freq_heater, pf_heater=pf_heater,
                suhu=suhu, kelembapan=kelembapan, dimmer_heater=dimmer_heater, dimmer_fan=dimmer_fan,
                fisOutputHeater=fisOutputHeater, fisOutputFan=fisOutputFan
            );
            
            if (minute%upload_data == 0 and second <= 1): 
                database.save_data(
                    date=date, time=time, kubikel=kode_kubikel,
                    tegangan_heater=tegangan_heater, arus_heater=arus_heater, daya_heater=daya_heater,
                    energy_heater=energy_heater, freq_heater=freq_heater, pf_heater=pf_heater,
                    suhu=suhu, kelembapan=kelembapan, dimmer_heater=dimmer_heater, dimmer_fan=dimmer_fan,
                    fisOutputHeater=fisOutputHeater, fisOutputFan=fisOutputFan
                );
    
            if(database.read_reset_esp_state(kubikel=kubikel) == 1):
                url = urlopen(f"{ip_server}/resetesp");
                documents = url.read().decode("utf-8");
                state = json.loads(documents);
                database.set_reset_esp_state(kubikel=kode_kubikel, state=int(state["esp_state"]));
                sleep(30);
            
            else:
                database.set_reset_esp_state(kubikel=kode_kubikel, state=0);
    
    except firebase_err.TransportError as err:
        global numberErr_program_main_2
        numberErr_program_main_2 += 1;
        print(f"{date_time_error}: No.{numberErr_program_main_2} Error -> tidak bisa input data ke firebase {ip_server}/{kubikel}!\n{err}");

# buat program restart server remote
def restart_server():
    # ambil tanggal dan waktu di dalam server
    now = datetime.datetime.now();
    day = now.day
    month = now.month
    year = now.year
    date_time_error = f"{day}/{month}/{year} ({(now.strftime('%H:%M:%S'))})"

    try:
        database = db_ref(GH);
        if (database.read_reset_server_state() == 1):
            database.set_reset_server_state(0);
            sleep(10)
            os.system("reboot");
        
        else:
            database.set_reset_server_state(0);
    
    except:
        global numberErr_restart_server;
        numberErr_restart_server += 1;
        print(f"{date_time_error}: No.{numberErr_restart_server} Error -> tidak bisa restart server raspberry {GH}!");
    

# jalankan program_main
def program_online(second, delay):
    try :
        if (second%delay == 0):
            configServerMain = FirebaseDB.db_configServer(GH);
            config_esp_1 = configServerMain.read_config_server("esp-server-1", "192.168.154.39", "PA01")
            program_main(f"http://{config_esp_1[0]}", f"{config_esp_1[1]}");
            program_upload();
            # restart_server();
        
    except:
        if (second%delay == 0):
            print ("Error __main__ : pastikan koneksi internet ada!");
            program_main(f"http://192.168.154.39", "PA01");
            
def program_offline(second, delay):
    if (second%delay == 0):
        program_main(f"http://192.168.8.103", "PA01");
        # program_upload();

if (__name__ == "__main__"):
    while True:
        second = int(datetime.datetime.now().second)
        delay = 10 # 5 detik
        
        # program_online(second=second, delay=delay)
        program_offline(second=second, delay=delay)
