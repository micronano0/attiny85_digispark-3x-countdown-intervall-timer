# attiny85_digispark-3x-countdown-intervall-timer  
macht das gleiche wie der esp8266 3x Countdown Intervall Timer  
// *****************************************************************  
// Projekt: Intervalltimer für Fitnesstraining  
// created 30.08.2025 by micronano (micronano@gmx-topmail.de)  
//  
// Beschreibung: 3 Timer mit je 2 Intervallen einstellbar  
// Timereinstellungen werden intern gespeichert und gehen somit nicht verloren  
// Intervallzeiten bis 245sek getrennt einstellbar  
// nach Ablauf von Intervall1 erfolgt 1 Piep, nach der Pausezeit (Intervall2) 2 Piepse.  
// Nach Ablauf des Timers erfolgen 5 Piepser.  
// Zweck: zB 1min trainieren, 20s Pause, 1min trainieren, 20s Pause ...  
// Pausezeiten reduzieren den Trainigstimer nicht  
// Taste rot: Timer start (länger drauf bleiben => stop) /  und Werte im Einstellungsmodus erhöhen  
// Taste blau: länger gedrückt: Timer einstellen / ansonsten Werte im Einstellungsmodus reduzieren  
// Taste grün: aktiven Timer einstellen  
// Einstellung bleiben nach Stromunterbrechungen erhalten  
//  
// siehe Wokwi: https://wokwi.com/projects/440283843368028161
