#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

// ⚙️ Inserisci le tue credenziali WiFi
const char* ssid = "TUA_RETE_WIFI";
const char* password = "TUA_PASSWORD_WIFI";

// Web server sulla porta 80
WebServer server(80);

// HTML completo (codice abbreviato per chiarezza visiva)
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html lang="it">
<head>
  <meta charset="UTF-8">
  <title>Generatore di QR code</title>
  <style>
    html, body {
      margin: 0; padding: 0; height: 100%;
      background-color: #eeeeee;
      font-family: Arial, sans-serif;
    }
    .full-height {
      display: flex;
      flex-direction: column;
      height: 100vh;
      align-items: center;
      justify-content: center;
      text-align: center;
    }
    h1 { margin-bottom: 40px; font-size: 36px; color: #333; }
    .wrapper {
      display: flex;
      gap: 40px;
      align-items: center;
      flex-wrap: wrap;
      justify-content: center;
    }
    .container {
      background-color: #ccc;
      border-radius: 20px;
      padding: 20px;
      overflow: auto;
      max-height: 80vh;
    }
    table { border-collapse: collapse; }
    td {
      width: 40px; height: 40px; padding: 0; margin: 0;
    }
    .white { background-color: white; }
    .black { background-color: black; }
    .input-area {
      display: flex; align-items: center;
    }
    input[type="text"] {
      padding: 10px;
      font-size: 16px;
      border: 1px solid #ccc;
      border-right: none;
      border-radius: 10px 0 0 10px;
      outline: none;
      width: 200px;
    }
    button {
      width: 50px; height: 50px;
      font-size: 16px;
      background-color: #444;
      color: white;
      border: none;
      border-radius: 0 10px 10px 0;
      cursor: pointer;
    }
    button:hover { background-color: #222; }
  </style>
</head>
<body>
  <div class="full-height">
    <h1>Generatore di QR code</h1>
    <div class="wrapper">
      <div class="container">
        <table id="scacchiera"></table>
      </div>
      <div class="input-area">
        <input type="text" id="inputParole" placeholder="Inserisci parole">
        <button onclick="inviaParole()">➤</button>
      </div>
    </div>
  </div>

  <script>
    const pattern = [
      0, 1, 0, 1,
      1, 0, 1, 0,
      0, 1, 0, 1,
      1, 0, 1, 0
    ];

    function creaScacchiera(array) {
      const n = Math.sqrt(array.length);
      const table = document.getElementById("scacchiera");
      table.innerHTML = "";
      if (!Number.isInteger(n)) {
        table.innerHTML = "<tr><td>Errore: array non quadrato.</td></tr>";
        return;
      }
      let index = 0;
      for (let i = 0; i < n; i++) {
        const row = document.createElement("tr");
        for (let j = 0; j < n; j++) {
          const cell = document.createElement("td");
          cell.className = array[index] === 1 ? "black" : "white";
          row.appendChild(cell);
          index++;
        }
        table.appendChild(row);
      }
    }

    creaScacchiera(pattern);

    function inviaParole() {
      const parole = document.getElementById("inputParole").value;
      alert("Hai scritto: " + parole);
    }
  </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);

  // Connessione a WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connessione a WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connesso con IP: ");
  Serial.println(WiFi.localIP());

  // Avvio mDNS (dominio: esp32.local)
  if (MDNS.begin("esp32")) {
    Serial.println("mDNS avviato: http://esp32.local");
  } else {
    Serial.println("Errore mDNS");
  }

  // Gestione root (pagina HTML)
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", htmlPage);
  });

  // Avvio del server
  server.begin();
  Serial.println("Server HTTP avviato");
}

void loop() {
  server.handleClient();
}
