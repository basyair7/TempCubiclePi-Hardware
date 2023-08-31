"""
PROGRAM PROJECT MONITORING SUHU DAN KELEMBABABAN KUBIKEL 20 KV
NAMA    : FATHUL BASYAIR
NPM     : 1904105010004
PRODI   : TEKNIK ELEKTRO

FILE INI ADALAH LIBRARY TEMPAT MENGAMBIL DATA MELALUI IP ADDRESS NODEMCU DAN MEMBUAT PLOT GRAFIK REAL-TIME

!!! WARNING : DILARANG KERAS MENGUBAH ISI FILE INI / PLAGIARISME !!!
"""

from urllib.request import urlopen
from time import sleep
from pathlib import Path
import json, datetime

numberError = 0;
class server_esp:
    """
    masukan ip_address="https://198.168.x.x"
    """
    def __init__(self, ip_address="", kubikel=""):
        # ambil alamat ip_address
        self.ip_address = ip_address;
        if(self.ip_address == "" or self.ip_address == None or kubikel == "" or kubikel == None):
            raise TypeError("Parameter BackendProgram.server_esp() ip_address/kubikel tidak boleh kosong!");
        
        url = urlopen(f"{self.ip_address}/{kubikel}");
        documents = url.read().decode("utf-8");
        data = json.loads(documents);
        
        self.data_raw_4  = data['kubikel'];
        self.data_raw_5  = data['data_pzem']['tegangan_heater'];  self.data_raw_6  = data['data_pzem']['arus_heater'];
        self.data_raw_7  = data['data_pzem']['daya_heater'];      self.data_raw_8  = data['data_pzem']['energy_heater'];
        self.data_raw_9  = data['data_pzem']['frequency_heater']; self.data_raw_10  = data['data_pzem']['pf_heater'];
        self.data_raw_11 = data['dht']['suhu'];                   self.data_raw_12 = data['dht']['kelembapan'];
        self.data_raw_13 = data['atmega328p']['dimmer_heater'];   self.data_raw_14 = data['atmega328p']['fisOutputHeater'];
        self.data_raw_15 = data['atmega328p']['dimmer_fan'];      self.data_raw_16 = data['atmega328p']['fisOutputFan'];
        
        # jika nilai dimmer_heater atau fisOutputHeater 0, maka numberError ditambah 1
        if (self.data_raw_11 <= 0):
            numberError += 1;
            sleep(30);
        else:
            numberError = 0;
            
        # jika nilai numberError lebih dari 10, maka reset server esp
        if (numberError >= 10):
            urlopen(f"{self.ip_address}/resetesp");

    def get_data(self):
        """
        tools mengambil nilai dari server_esp
        """
        # get a real date and time
        now = datetime.datetime.now();
        day = now.day
        month = now.month
        year = now.year
        data_raw_3 = int(now.strftime("%S"));
        data_raw_2 = int(now.strftime("%M"));
        data_raw_1 = (now.strftime("%H:%M"));
        data_raw_0 = f"{day}/{month}/{year}"
        
        return {
            "date_time": {
                "date": data_raw_0, "time": data_raw_1, "minute": data_raw_2, "second": data_raw_3
            },
            "kode_kubikel": self.data_raw_4,
            "data_pzem": {
                "tegangan_heater": self.data_raw_5, "arus_heater": self.data_raw_6,
                "daya_heater": self.data_raw_7, "energy_heater": self.data_raw_8,
                "frequency_heater": self.data_raw_9, "pf_heater": self.data_raw_10
            },
            "data_dht": {
                "suhu": self.data_raw_11, "kelembapan": self.data_raw_12
            },
            "data_atmega": {
                "dimmer_heater": self.data_raw_13, "fisOutputHeater": self.data_raw_14,
                "dimmer_fan": self.data_raw_15, "fisOutputFan": self.data_raw_16
            }
        }