#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <rgb_lcd.h>

rgb_lcd lcd;

// ----------- Déclarations des pins et constantes -----------
#define solPin A1                  // Pin du capteur d'humidité du sol
#define sdCardPinChipSelect 53     // Pin de sélection de la carte SD
#define nomDuFichier "Humide2.xls" // Nom du fichier de stockage des données

// ----------- Variables globales -----------
File monFichier;                   // Objet de fichier pour la carte SD
int HR;                            // Valeur brute lue par le capteur
int HRp;                           // Humidité du sol en pourcentage
int compte = 0;                    // Compteur pour numéro de mesure

// ----------- Initialisation -----------
void setup() 
{
    Serial.begin(9600);

    pinMode(solPin, INPUT);         // Définit la pin du capteur comme entrée

    lcd.begin(16, 2);               // Initialisation de l'écran LCD
    lcd.setCursor(0, 0);
    lcd.print("Humidite du sol :");
    delay(1000);

    // Initialisation de la carte SD
    if (!SD.begin(sdCardPinChipSelect))
    {
        Serial.println(F("Échec de l'initialisation de la carte SD !"));
        while (1);  // Boucle infinie si échec
    }
   
    // Création du fichier et ajout de l'en-tête
    monFichier = SD.open(nomDuFichier, FILE_WRITE);
    if (monFichier)
    {
        Serial.println(F("Fichier ouvert avec succès."));
        monFichier.println("Numéro de mesure;Humidité (%)");  // Ajoute l'en-tête
        monFichier.close();  // Ferme le fichier après l'écriture de l'en-tête
    }
    else
    {
        Serial.println(F("Échec d'ouverture du fichier pour écriture."));
        while (1);  // Boucle infinie si échec d'ouverture
    }
}

// ----------- Boucle principale -----------
void loop() 
{
    // ---- Lecture et conversion de l'humidité du sol ----
    HR = analogRead(solPin);            // Lecture brute du capteur
    HRp = (1024 - HR) / 10;             // Conversion en pourcentage

    // ---- Affichage sur l'écran LCD ----
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Humidite du sol :");
    lcd.setCursor(7, 1);
    lcd.print(HRp);
    lcd.print(" %");

    // ---- Affichage sur le moniteur série ----
    Serial.print("Mesure n° ");
    Serial.print(compte);
    Serial.print(" - Humidité du sol = ");
    Serial.print(HRp);
    Serial.println(" %");

    // ---- Enregistrement des données sur la carte SD ----
    monFichier = SD.open(nomDuFichier, FILE_WRITE);
    if (monFichier)
    {
        monFichier.print(compte);         // Écrit le numéro de mesure
        monFichier.print(";");            // Séparateur
        monFichier.println(HRp);          // Écrit l'humidité en %
        monFichier.close();               // Ferme le fichier après écriture

        Serial.println(F("Données enregistrées avec succès."));  // Message de confirmation
        compte++;                         // Incrémente le compteur de mesures
    }
    else
    {
        Serial.println(F("Échec d'ouverture du fichier pour écriture."));  // Message d'erreur
    }

    delay(1000);  // Attente d'une seconde avant la prochaine mesure
}
