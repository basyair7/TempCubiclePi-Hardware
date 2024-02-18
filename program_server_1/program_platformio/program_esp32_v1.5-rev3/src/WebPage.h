#include <Arduino.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="configWiFi">
    <meta name="author" content="basyair7">
    <title>Helper Page - TempCubiclePi %s</title>
    <style>
    </style>
</head>
<body>
    <table border="1">
        <thead>
            <caption><b>System Status</b></caption>
        </thead>
        <tbody>
            <!-- Name Board -->
            <tr>
                <td>Chipset Master </td>
                <td>esp32doit-devkit-v1</td>
            <tr>
            <tr>
                <td>Chipset Slave </td>
                <td>Atmega328p</td>
            <tr>
            <!-- firmware version -->
            <!-- hardware version -->
            <tr>
                <td>HW Version </td>
                <td>%s</td>
            </tr>
            <!-- software version -->
            <tr>
                <td>SW Version </td>
                <td>%s</td>
            </tr>
            <!-- build time -->
            <tr>
                <td>Build Time </td>
                <td>%s</td>
            </tr>
            <!-- firmware region -->
            <tr>
                <td>Firmware Region </td>
                <td>%s</td>
            </tr>

            <!-- cubicle code -->
            <tr>
                <td>Kubikle Code </td>
                <td>%s</td>
            </tr>

            <!-- Auto Change Mode -->
            <tr>
                <td>Status Auto Change AP </td>
                <td>%s</td>
            </tr>
            <!-- Fuzzy state -->
            <tr>
                <td>Status Fuzzy </td>
                <td>%s</td>
            </tr>

            <!-- Buzzer state -->
            <tr>
                <td>Status Buzzer </td>
                <td>%s</td>
            </tr>
        </tbody>
    </table>
    <br />
    <table border="1">
        <thead>
            <caption><b>WiFi Status</b></caption>
        </thead>
        <tbody>
            <!-- Chipset -->
            <tr>
                <td>Chipset WiFi </td>
                <td>ESP32 Board</td>
            </tr>
            <!-- IP Address -->
            <tr>
                <td>IP Address </td>
                <td>%s</td>
            </tr>
            <!-- Mode WiFi -->
            <tr>
                <td>Mode WiFi </td>
                <td>%s</td>
            </tr>

            <!-- AP MODE -->
            <tr>
                <td>AP Name </td>
                <td>%s</td>
            </tr>
            <!-- Password -->
            <tr>
                <td>Password </td>
                <td>%s</td>
            </tr>

            <!-- SSID WiFi -->
            <tr>
                <td>SSID WiFi </td>
                <td>%s</td>
            </tr>
            <!-- Password -->
            <tr>
                <td>Password WiFi </td>
                <td>%s</td>
            </tr>
        </tbody>
    </table>
    <!-- Link program -->
    <br /><h4>%s</h4>
    <!-- Fuzzy Action -->
    %s
    <!-- Buzzer Action -->
    %s
    <!-- Mode WiFi Action -->
    %s
    <!-- Auto Change Mode WiFi Action -->
    %s
    <!-- Rename Kode Kubikel -->
    %s
    <!-- Configurate WiFi -->
    %s
    <!-- Configurate Kubikel Code -->
    %s
    <!-- Firmware Update -->
    %s
    <!-- Reset Hardware Action -->
    %s
    <!-- Reset PZEM Action -->
    %s
    
    <p><b>Powered By : <a href=https://github.com/basyair7 target='_blank'>Basyair7</a></b></p>
    
</body>
</html>
)rawliteral";

const char rename_kodekubikel[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
    <meta charset='utf-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <meta name='description' content='rename-kodekubikel'>
    <meta name='author' content='basyair7'>
    <title>Rename Kubikel Code - TempCubiclePi %s </title>
</head>
<body>
    <h4>Rename Kubikel Code - TempCubiclePi %s </h4>
    <form method='POST' action='/save-rename-kodekubikel'>
        <table>
            <thead>
                <caption><b>Old Kubikel Code : </b></caption>
            </thead>
            <tbody>
                <tr>
                    <td>Code : </td>
                    <td>%s</td>
                </tr>
                <br />
                <tr>
                    <caption><b>Rename : </b></caption>
                </tr>
                <tr>
                    <td>Code : </td>
                    <td><input type='text' name='newcode'></td>
                </tr>
                <tr>
                    <td> </td>
                    <td><button type='submit' value='save'>Save</button></td>
                </tr>
                <tr>
                    <td> </td>
                    <td><a href=http://%s/help>Kembali ke Menu</a></td>
                </tr>
            </tbody>
        </table>
    </form>

    <br><p><b>Powered by : <a href=https://github.com/basyair7 target='_blank'>Basyair7</a></b></p>
</body>
</html>
)rawliteral";

const char success_save_rename_kodekubikel[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset='utf-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <meta name='description' content='rename-kodekubikel'>
    <meta name='author' content='basyair7'>
    <title>Rename Kubikel Code - TempCubiclePi %s</title>
</head>
<body>
    <table>
        <thead>
            <caption><h4>Data telah di simpan</h4></caption>
        </thead>
        <tbody>
            <tr>
                <td>New Code : </td>
                <td>%s</td>
            </tr>
            <tr>
                <td> </td>
                <td><a href=http://%s/help>Kembali ke Menu</a></td>
            </tr>
        </tbody>
    </table>

    <br><p><b>Powered by : <a href=https://github.com/basyair7 target='_blank'>Basyair7</a></b></p>
</body>
</html>
)rawliteral";

const char config_wifi_1[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
    <meta charset='utf-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <meta name='description' content='configWiFi'>
    <meta name='author' content='basyair7'>
    <title>WiFi Configuration - TempCubiclePi %s </title>
</head>
<body>
    <h4>WiFi Configuration - TempCubiclePi %s </h4>
    <p>Server kubikel %s akan di restart dan ganti mode WIFI AP terlebih dahulu... <br />
        silahkan koneksikan WiFi : %s dan Password %s
    </p>
    <p>Setelah terkoneksi ke WiFi %s silahkan klik <a href=http://192.168.4.1 target='_blank'> Helper Page </a></p>
    
    <br><p><b>Powered by : <a href=https://github.com/basyair7 target='_blank'>Basyair7</a></b></p>
</body>
</html>
)rawliteral";

const char config_wifi_2[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
    <meta charset='utf-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <meta name='description' content='configWiFi'>
    <meta name='author' content='basyair7'>
    <title>WiFi Configuration - TempCubiclePi %s </title>
</head>
<body>
    <h4>WiFi Configuration - TempCubiclePi %s </h4>
    <form method='POST' action='/config-wifi-save'>
        <table>
            <thead>
                <caption><b>Setup WiFi Client : </b></caption>
            </thead>
            <tbody>
                <tr>
                    <caption>Old Configuration : </caption>
                </tr>
                <tr>
                    <td>SSID : </td>
                    <td>%s</td>
                </tr>
                <tr>
                    <td>Password : </td>
                    <td>%s</td>
                </tr>
                <br />
                <tr>
                    <caption>New Configuration : </caption>
                </tr>
                <tr>
                    <td>New SSID : </td>
                    <td><input type='text' name='newssid'></td>
                </tr>
                <tr>
                    <td>New Password : </td>
                    <td><input type='password' name='newpassword'></td>
                </tr>
                <tr>
                    <td> </td>
                    <td><button type='submit' value='save'>Save</button></td>
                </tr>
                <tr>
                    <td> </td>
                    <td><a href=http://%s/help>Kembali ke Menu</a></td>
                </tr>
            </tbody>
        </table>
    </form>

    <br><p><b>Powered by : <a href=https://github.com/basyair7 target='_blank'>Basyair7</a></b></p>
</body>
</html>
)rawliteral";

const char success_save_config_wifi[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset='utf-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <meta name='description' content='configWiFi'>
    <meta name='author' content='basyair7'>
    <title>WiFi Configuration - TempCubiclePi %s</title>
</head>
<body>
    <table>
        <thead>
            <caption><h4>Data telah di simpan</h4></caption>
        </thead>
        <tbody>
            <tr>
                <td>New WiFi SSID : </td>
                <td>%s</td>
            </tr>
            <tr>
                <td>New WiFi Password : </td>
                <td>%s</td>
            </tr>
            <tr>
                <td> </td>
                <td><a href=http://%s/help>Kembali ke Menu</a></td>
            </tr>
        </tbody>
    </table>

    <br><p><b>Powered by : <a href=https://github.com/basyair7 target='_blank'>Basyair7</a></b></p>
</body>
</html>
)rawliteral";

const char config_ap[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
    <meta charset='utf-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <meta name='description' content='configAP'>
    <meta name='author' content='basyair7'>
    <title>Access Point WiFi Configuration - TempCubiclePi %s </title>
</head>
<body>
    <h4>Access Point WiFi Configuration - TempCubiclePi %s </h4>
    <form method='POST' action='/config-ap-save'>
        <table>
            <thead>
                <caption><b>Setup Access Point WiFi : </b></caption>
            </thead>
            <tbody>
                <tr>
                    <caption>Old Configuration : </caption>
                </tr>
                <tr>
                    <td>Access Point Name : </td>
                    <td>%s</td>
                </tr>
                <tr>
                    <td>Password : </td>
                    <td>%s</td>
                </tr>
                <br />
                <tr>
                    <caption>New Configuration : </caption>
                </tr>
                <tr>
                    <td>New AP Name : </td>
                    <td><input type='text' name='newap'></td>
                </tr>
                <tr>
                    <td>New Password : </td>
                    <td><input type='password' name='newpassword'></td>
                </tr>
                <tr>
                    <td> </td>
                    <td><button type='submit' value='save'>Save</button></td>
                </tr>
                <tr>
                    <td> </td>
                    <td><a href=http://%s/help>Kembali ke Menu</a></td>
                </tr>
            </tbody>
        </table>
    </form>

    <br><p><b>Powered by : <a href=https://github.com/basyair7 target='_blank'>Basyair7</a></b></p>
</body>
</html>
)rawliteral";

const char success_save_config_ap[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset='utf-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <meta name='description' content='configAP'>
    <meta name='author' content='basyair7'>
    <title>Access Point WiFi Configuration - TempCubiclePi %s</title>
</head>
<body>
    <table>
        <thead>
            <caption><h4>Data telah di simpan</h4></caption>
        </thead>
        <tbody>
            <tr>
                <td>New AP Name : </td>
                <td>%s</td>
            </tr>
            <tr>
                <td>New AP Password : </td>
                <td>%s</td>
            </tr>
            <tr>
                <td> </td>
                <td><a href=http://%s/help>Kembali ke Menu</a></td>
            </tr>
        </tbody>
    </table>

    <br><p><b>Powered by : <a href=https://github.com/basyair7 target='_blank'>Basyair7</a></b></p>
</body>
</html>
)rawliteral";
