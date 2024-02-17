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
    <h4>Helper Page - TempCubiclePi %s <br /> %s </h4>
    <table border="1">
        <caption>System Status</caption>
        <tbody>
            <!-- firmware version -->
            <tr>
                <td>Firmware Version : </td>
                <td>%s</td>
            </tr>
            <!-- build time -->
            <tr>
                <td>Build Time : </td>
                <td>%s</td>
            </tr>
            <!-- firmware region -->
            <tr>
                <th>
                    <td>Firmware Region : </td>
                    <td>%s</td>
                </th>
            </tr>
            <!-- Auto Change Mode -->
            <tr>
                <td>Status Auto Change AP : </td>
                <td>%s</td>
            </tr>

            <!-- Fuzzy state -->
            <tr>
                <td>Status Fuzzy  : </td>
                <td>%s</td>
            </tr>

            <!-- Buzzer state -->
            <tr>
                <td>Status Buzzer : </td>
                <td>%s</td>
            </tr>
        </tbody>
    </table>
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