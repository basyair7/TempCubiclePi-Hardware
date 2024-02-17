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
</head>
<body>
    <table border="1">
        <thead>
            <caption>System Status</caption>
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
            <caption>WiFi Status</caption>
        </thead>
        <tbody>
            <!-- SSID -->
            <tr>
                <td>SSID </td>
                <td>%s</td>
            </tr>
            <!-- Password -->
            <tr>
                <td>Password </td>
                <td>%s</td>
            </tr>
        </tbody>
    </table>
    <!-- Link program -->
    <br /><h4>%s</h4>
    <!-- Link 1 -->
    %s
    <!-- Link 2 -->
    %s
    <!-- Link 3 -->
    %s
    <!-- Link 4 -->
    %s
    <!-- Link 5 -->
    %s
    <!-- Link 6 -->
    %s
    <!-- Link 7 -->
    %s
    <!-- Link 8 -->
    %s
    <!-- Link 9 -->
    %s
    <!-- Link 10 -->
    %s

    <br />
    <p><b>Powered By : <a href=https://github.com/basyair7 target='_blank'>Basyair7</a></b></p>
    
</body>
</html>
)rawliteral";