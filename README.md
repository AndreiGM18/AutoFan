Nume: Mitran Andrei-Gabriel
Grupă: 333CA

# AutoFan v3

## Descriere
Acest proiect este o implementare a unui sistem de control al temperaturii și al ventilatorului, folosind o placă Arduino și componente hardware adiționale. Scopul sistemului este de a menține temperatura într-un anumit interval, activând sau dezactivând ventilatorul în funcție de setările utilizatorului și de condițiile de mediu.

## Caracteristici
- Monitorizarea temperaturii folosind un senzor de temperatură
- Controlul ventilatorului în funcție de temperatura detectată și de preferințele utilizatorului
- Opțiune pentru setarea pragurilor de temperatură și de timp folosind telecomanda IR
- Funcție de oprire automată a sistemului după un anumit interval de timp
- Afișaj LCD pentru vizualizarea temperaturii, stării sistemului și a altor informații relevante

## Cum să folosești
1. Conectează placa Arduino la componentele hardware necesare conform diagramei de cablare.
2. Încarcă codul din fișierul "AutoFan_version3.ino" pe placa Arduino folosind Arduino IDE sau un alt mediu de dezvoltare.
3. Asigură-te că toate componentele sunt conectate corect și porniți sistemul.
4. Utilizați telecomanda IR pentru a seta pragurile de temperatură și de timp și pentru a controla sistemul în funcție de preferințele dvs.
5. Urmați instrucțiunile de pe afișajul LCD pentru a monitoriza starea sistemului și a temperaturii.

## Cerințe Hardware
- 1 x Arduino Uno (Plusivo Uno R3) - Placa principală, ce alimentează toate componentele (cu excepția motorului)
- 1 x placă de cablaj
- 1 x termorezistor - Senzor de temperatură
- 1 x modul LCD1602 - Afișează diferite informații utile (temperatura curentă, temperatura de threshold, modul de funcționare etc.)
- 1 x potențiometru (10K) - Reglează contrastul LCD-ului
- 1 x L293D IC - Controlează motorul
- 1 x lamă de ventilator
- 1 x motor 3-6V - Folosit ca ventilator împreună cu lama
- 1 x modul de alimentare
- 1 x baterie 9V - Alimentează ventilatorul împreună cu modulul și adaptorul
- 1 x adaptor 9V 1A
- 1 x fotodiodă - Senzor de lumină
- 2 x LEDs (1 x roșu, 1 x albastru) - Indică modul de funcționare
- 1 x modul receptor pentru telecomandă infraroșu
- 1 x telecomandă infraroșu - Facilitează controlul temperaturii de threshold și a modului de funcționare
- rezistori (220 Ω, 1K etc.) și fire
