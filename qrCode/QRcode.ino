#include <qrcodegen.h>

void setup() {
    Serial.begin(9600); // Inizializza la comunicazione seriale

    // Testo da convertire in QR Code
    const char *text = "prova1234";

    // Buffer per il QR Code
    uint8_t qrcode[qrcodegen_BUFFER_LEN_FOR_VERSION(3)];
    uint8_t tempBuffer[qrcodegen_BUFFER_LEN_FOR_VERSION(3)];

    // Genera il QR Code (versione 3, livello di correzione errori basso)
    bool ok = qrcodegen_encodeText(text, tempBuffer, qrcode,
                               qrcodegen_Ecc_LOW, // Livello ECC
                               qrcodegen_VERSION_MIN, // Versione minima
                               qrcodegen_VERSION_MAX, // Versione massima
                               qrcodegen_Mask_AUTO,   // Maschera automatica
                               true);

    if (ok) {
        // Stampa il QR Code in formato ASCII sulla seriale
        //printQr(qrcode);
        debugQrCode(qrcode);

    } else {
        Serial.println("Errore nella generazione del QR Code!");
    }
}

void loop() {
    // Non fare nulla nel loop
}

// Funzione per stampare il QR Code in formato ASCII
void printQr(const uint8_t qrcode[]) {
    int size = qrcodegen_getSize(qrcode);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            Serial.print(qrcodegen_getModule(qrcode, x, y) ? "⬛" : "⬜");
        }
        Serial.println();
    }
}

void debugQrCode(const uint8_t qrcode[]) {
    int size = qrcodegen_getSize(qrcode);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            Serial.print(qrcodegen_getModule(qrcode, x, y) ? "1" : "0");
            Serial.print(", ");
        }
        Serial.println();
    }
}