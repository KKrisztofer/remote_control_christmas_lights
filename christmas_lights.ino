#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FastLED.h>

#define LED_PIN_SZAM           5
#define NUM_LEDS_SZAM          10

#define LED_PIN_KERET          4
#define NUM_LEDS_KERET         144

#define LED_PIN_KARACSONYFA    0
#define NUM_LEDS_KARACSONYFA   9

#define LED_PIN_RENSZARVAS     2
#define NUM_LEDS_RENSZARVAS    6

#define FUZER_PIN              14

#define VILLOGAS_SPEED         200

byte rSzam = 255;
byte gSzam = 0;
byte bSzam = 0;
byte szamAnim = 0;

byte rKeret = 255;
byte gKeret = 0;
byte bKeret = 0;
byte keretAnim = 0;

byte rKaracsonyfa = 255;
byte gKaracsonyfa = 0;
byte bKaracsonyfa = 0;
byte karacsonyfaAnim = 0;

byte rRenszarvas = 255;
byte gRenszarvas = 0;
byte bRenszarvas = 0;
byte renszarvasAnim = 0;

byte rCsillag = 255;
byte gCsillag = 255;
byte bCsillag = 255;

bool brightnessPulse = false;

CRGB leds_szam[NUM_LEDS_SZAM];
CRGB leds_keret[NUM_LEDS_KERET];
CRGB leds_karacsonyfa[NUM_LEDS_KARACSONYFA];
CRGB leds_renszarvas[NUM_LEDS_RENSZARVAS];



//---------------------------------------------------------------
//Our HTML webpage contents in program memory
const char MAIN_page[] PROGMEM = R"=====(
<!doctype html>
<html lang="hu">
    <head>
        <title>LED vezérlő</title>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <style>
            *{
                padding: 0;
                margin: 0;
                font-family:'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif;
                color: #fff;
                text-shadow: 0 0 10px rgba(0,0,0,0.3);
            }
            body{
                background: rgb(206,138,116);
                background: linear-gradient(0deg, rgba(206,138,116,1) 0%, rgba(174,63,27,1) 100%);
                background-attachment: fixed;
            }
            .frame{
                width: 100%;
                display: flex;
                flex-direction: column;
                
                
            }
            .title{
                width: 100%;
                display: flex;
                flex-direction: column;
                justify-content: center;
                align-items: center;
                margin-bottom: 10px;
            }
            .title h1{
                font-size: 55px;
            }
            .title h2{
                font-size: 30px;
            }
            h1, h2{
                padding: 10px 0;
            }
            .button-frame{
                display: flex;
                flex-direction: column;
                align-items: center;
            }
            span{
                
                font-size: 30px;
                
            }
            .button{
                width: 98%;
                height: 80px;
                margin: 5px 0;
                display: flex;
                justify-content: center;
                align-items: center;
                text-decoration: none;
                border-radius: 15px;
                box-shadow: 0 0 10px rgba(0,0,0,0.3);
            }
            .buttons-title{
                display: flex;
                justify-content: center;
                align-items: center;
                padding: 5px 0;
                margin-top: 20px;
            }
            .buttons-title p{
                font-size: 35px;
            }

            .color-on{
                background: rgb(168,212,111);
                background: linear-gradient(50deg, rgba(168,212,111,1) 0%, rgba(103,163,22,1) 100%);
            }
            .color-off{
                background: rgb(149,149,149);
                background: linear-gradient(190deg, rgba(149,149,149,1) 0%, rgba(0,0,0,1) 100%);
            }

            .color-frame{
                display: flex;
                flex-direction: row;
                justify-content: center;
                align-items: center;
            }
            .color{
                height: 80px;
                width: 100%;
                margin: 5px;
                border-radius: 10px;
                box-shadow: 0 0 10px rgba(0,0,0,0.3);
            }

            .color01{
                /*background: rgba(255,0,0);*/
                background: rgb(255,2,2);
                background: linear-gradient(31deg, rgba(255,2,2,1) 0%, rgba(255,82,52,1) 100%);
            }
            .color02{
                /*background: rgba(0,255,0);*/
                background: rgb(71,255,60);
                background: linear-gradient(310deg, rgba(71,255,60,1) 0%, rgba(31,231,20,1) 100%);
            }
            .color03{
                /*background: rgba(0,0,255);*/
                background: rgb(61,71,255);
                background: linear-gradient(211deg, rgba(61,71,255,1) 0%, rgba(44,20,231,1) 100%);
            }
            .color04{
                /*background: rgba(255,255,255);*/
                background: rgb(255,255,255);
                background: linear-gradient(130deg, rgba(255,255,255,1) 0%, rgba(242,242,242,1) 100%);
            }
            .color05{
                background: rgba(255,0,255);
            }
            .color06{
                background: rgba(153,0,153);
            }

            .color-random{
                background: rgb(251,0,0);
                background: linear-gradient(90deg, rgba(251,0,0,1) 0%, rgba(227,5,226,1) 50%, rgba(233,238,3,1) 100%);
            }

            .color-random-villogas{
                background: rgb(238,98,250);
                background: linear-gradient(319deg, rgba(238,98,250,1) 0%, rgba(115,0,119,1) 100%);
            }
            .color-villogas{
                background: rgb(207,21,59);
                background: linear-gradient(36deg, rgba(207,21,59,1) 0%, rgba(184,235,8,1) 100%);
            }
            .color-uszas{
                background: rgb(21,37,207);
                background: linear-gradient(36deg, rgba(21,37,207,1) 0%, rgba(235,8,62,1) 100%);
            }
            .color-random-uszas{
                background: rgb(8,193,51);
                background: linear-gradient(266deg, rgba(8,193,51,1) 0%, rgba(181,185,0,1) 100%);
            }
            .color-uszas2{
                background: rgb(251,0,0);
                background: linear-gradient(138deg, rgba(251,0,0,1) 0%, rgba(3,238,208,1) 100%);
            }
            .color-random-uszas2{
                background: rgb(0,34,251);
                background: linear-gradient(47deg, rgba(0,34,251,1) 0%, rgba(237,238,3,1) 100%);
            }
            .color-fenyero, .color-nincs{
                background: rgb(47,47,47);
                background: linear-gradient(90deg, rgba(47,47,47,1) 0%, rgba(133,133,133,1) 100%);
            }
            .color-fenyero-pulse{
                background: rgb(132,0,138);
                background: linear-gradient(47deg, rgba(132,0,138,1) 0%, rgba(129,130,0,1) 100%);
            }
            .color-szines{
                background: rgb(255,163,2);
                background: linear-gradient(90deg, rgba(255,163,2,1) 0%, rgba(208,209,0,1) 100%);
            }
            
            .color-szinvaltas{
                background: rgb(26,8,193);
                background: linear-gradient(196deg, rgba(26,8,193,1) 0%, rgba(0,185,177,1) 100%);
            }

            
        </style>
    </head>
    <body>
        <div class="frame">
            <div class="title">
                <h1>Élő kalendárium</h1>
                <h2>Jó szórakozást!</h2>
            </div>
            <div class="button-frame">
                <!--<a href="button=on" class="button color-on"><span>Bekapcs</span></a>-->
                <a href="button=off" class="button color-off"><span>Sötétség</span></a>
            </div>

            <div class="buttons-title">
                <p>Szám színe</p>
            </div>
            <div class="color-frame">
                <a href="button=szam-color01" class="color color01"><span></span></a>
                <a href="button=szam-color02" class="color color02"><span></span></a>
                <a href="button=szam-color03" class="color color03"><span></span></a>
                <a href="button=szam-color04" class="color color04"><span></span></a>
            </div>
            <div class="color-frame">
                <a href="button=szam-color-random" class="button color color-random"><span>Random</span></a>
            </div>
            <div class="buttons-title">
                <p>Szám animációja</p>
            </div>
            <div class="button-frame">
                <a href="button=kilences-nincs" class="button color-nincs"><span>Nincs</span></a>
                <a href="button=kilences-villogas" class="button color-villogas"><span>Villogás</span></a>
                <a href="button=kilences-random-villogas" class="button color-random-villogas"><span>Random villogás</span></a>
                <a href="button=kilences-uszas" class="button color-uszas"><span>Úszás</span></a>
                <a href="button=kilences-random-uszas" class="button color-random-uszas"><span>Random úszás</span></a>
            </div>
            
            <div class="buttons-title">
                <p>Karácsonyfa színe</p>
            </div>
            <div class="color-frame">
                <a href="button=karacsonyfa-color01" class="color color01"><span></span></a>
                <a href="button=karacsonyfa-color02" class="color color02"><span></span></a>
                <a href="button=karacsonyfa-color03" class="color color03"><span></span></a>
                <a href="button=karacsonyfa-color04" class="color color04"><span></span></a>
            </div>
            <div class="color-frame">
                <a href="button=karacsonyfa-color-random" class="button color color-random"><span>Random</span></a>
            </div>
            <div class="buttons-title">
                <p>Karácsonyfa animációja</p>
            </div>
            <div class="button-frame">
                <a href="button=karacsonyfa-nincs" class="button color-nincs"><span>Nincs</span></a>
                <a href="button=karacsonyfa-villogas" class="button color-villogas"><span>Villogás</span></a>
                <a href="button=karacsonyfa-random-villogas" class="button color-random-villogas"><span>Random villogás</span></a>
                <a href="button=karacsonyfa-szines-villogas" class="button color-szines"><span>Színes villogás</span></a>
                <a href="button=karacsonyfa-uszas" class="button color-uszas"><span>Úszás</span></a>
                <a href="button=karacsonyfa-random-uszas" class="button color-random-uszas"><span>Random úszás</span></a>
            </div>

            <div class="buttons-title">
                <p>Csillag színe</p>
            </div>
            <div class="color-frame">
                <a href="button=csillag-color01" class="color color01"><span></span></a>
                <a href="button=csillag-color02" class="color color02"><span></span></a>
                <a href="button=csillag-color03" class="color color03"><span></span></a>
                <a href="button=csillag-color04" class="color color04"><span></span></a>
            </div>
            <div class="color-frame">
                <a href="button=csillag-color-random" class="button color color-random"><span>Random</span></a>
            </div>

            <div class="buttons-title">
                <p>Rénszarvasok színe</p>
            </div>
            <div class="color-frame">
                <a href="button=renszarvas-color01" class="color color01"><span></span></a>
                <a href="button=renszarvas-color02" class="color color02"><span></span></a>
                <a href="button=renszarvas-color03" class="color color03"><span></span></a>
                <a href="button=renszarvas-color04" class="color color04"><span></span></a>
            </div>
            <div class="color-frame">
                <a href="button=renszarvas-color-random" class="button color color-random"><span>Random</span></a>
            </div>
            <div class="buttons-title">
                <p>Rénszarvasok animációja</p>
            </div>
            <div class="button-frame">
                <a href="button=renszarvas-nincs" class="button color-nincs"><span>Nincs</span></a>
                <a href="button=renszarvas-villogas" class="button color-villogas"><span>Villogás</span></a>
                <a href="button=renszarvas-random-villogas" class="button color-random-villogas"><span>Random villogás</span></a>
                <a href="button=renszarvas-szines-villogas" class="button color-szines"><span>Színes villogás</span></a>
                <a href="button=renszarvas-uszas" class="button color-uszas"><span>Úszás</span></a>
                <a href="button=renszarvas-random-uszas" class="button color-random-uszas"><span>Random úszás</span></a>
            </div>

            <div class="buttons-title">
                <p>Háttér színe</p>
            </div>
            <div class="color-frame">
                <a href="button=keret-color01" class="color color01"><span></span></a>
                <a href="button=keret-color02" class="color color02"><span></span></a>
                <a href="button=keret-color03" class="color color03"><span></span></a>
                <a href="button=keret-color04" class="color color04"><span></span></a>
            </div>
            <div class="color-frame">
                <a href="button=keret-color-random" class="button color color-random"><span>Random</span></a>
            </div>
            <div class="buttons-title">
                <p>Háttér animációja</p>
            </div>
            <div class="button-frame">
                <a href="button=keret-nincs" class="button color-nincs"><span>Nincs</span></a>
                <a href="button=keret-villogas" class="button color-villogas"><span>Villogás</span></a>
                <a href="button=keret-random-villogas" class="button color-random-villogas"><span>Random villogás</span></a>
                <a href="button=keret-szines-villogas" class="button color-uszas"><span>Színes villogás</span></a>
                <a href="button=keret-uszas" class="button color-random-uszas"><span>Úszás</span></a>
                <a href="button=keret-random-uszas" class="button color-uszas2"><span>Random úszás</span></a>
            </div>

            <div class="buttons-title">
                <p>Fényfüzér</p>
            </div>
            <div class="button-frame">
                <a href="button=fuzer-be" class="button color-on"><span>Be</span></a>
                <a href="button=fuzer-ki" class="button color-off"><span>Ki</span></a>
            </div>

            <div class="buttons-title">
                <p>Fényerő</p>
            </div>
            <div class="button-frame">
                <a href="button=25" class="button color-fenyero"><span>25%</span></a>
                <a href="button=50" class="button color-fenyero"><span>50%</span></a>
                <a href="button=75" class="button color-fenyero"><span>75%</span></a>
                <a href="button=100" class="button color-fenyero"><span>100%</span></a>
                <a href="button=pulse" class="button color-fenyero-pulse"><span>Pulzálás</span></a>
            </div>
        </div>
    </body>
</html>
)=====";
//---------------------------------------------------------------

//WIFI SSID and Password
const char* ssid = "KaracsonyiAblak2022";
const char* password = "";

ESP8266WebServer server(80); //Server on port 80

void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
void allOffMethod(){
  Serial.println("All LED off");
    
    szamAnim = 0;
    keretAnim = 0;
    karacsonyfaAnim = 0;
    renszarvasAnim = 0;
    
    brightnessPulse = false;

    setSzamColorsOff();
    setKeretColorsOff();
    setKaracsonyfaColorsOff();
    setCsillagOff();
    setRenszarvasColorsOff();
    
    digitalWrite(FUZER_PIN, HIGH);
    
    FastLED.show();
}
void allOff() { 
    allOffMethod();
    server.send(200, "text/html", "<script>history.back();</script>"); //Send ADC value only to client ajax request
}

void szamColorRed() {
    rSzam = 255;
    gSzam = 0;
    bSzam = 0;
    setSzamColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void szamColorGreen() {
    rSzam = 0;
    gSzam = 255;
    bSzam = 0;
    setSzamColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void szamColorBlue() {
    rSzam = 0;
    gSzam = 0;
    bSzam = 255;
    setSzamColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void szamColorWhite() {
    rSzam = 255;
    gSzam = 255;
    bSzam = 255;
    setSzamColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void szamColorRandom() {
    rSzam = random(0,255);
    gSzam = random(0,255);
    bSzam = random(0,255);
    setSzamColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}

void szamAnimNo() {
    szamAnim = 0;
    server.send(200, "text/html", "<script>history.back();</script>");
}
void szamAnimVillogas() {
    szamAnim = 1;
    server.send(200, "text/html", "<script>history.back();</script>");
}
void szamAnimRandomVillogas() {
    szamAnim = 2;
    server.send(200, "text/html", "<script>history.back();</script>");
}
void szamAnimUszas() {
    szamAnim = 3;
    server.send(200, "text/html", "<script>history.back();</script>");
}
void szamAnimRandomUszas() {
    szamAnim = 4;
    server.send(200, "text/html", "<script>history.back();</script>");
}

void karacsonyfaColorRed() {
    rKaracsonyfa = 255;
    gKaracsonyfa = 0;
    bKaracsonyfa = 0;
    setKaracsonyfaColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void karacsonyfaColorGreen() {
    rKaracsonyfa = 0;
    gKaracsonyfa = 255;
    bKaracsonyfa = 0;
    setKaracsonyfaColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void karacsonyfaColorBlue() {
    rKaracsonyfa = 0;
    gKaracsonyfa = 0;
    bKaracsonyfa = 255;
    setKaracsonyfaColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void karacsonyfaColorWhite() {
    rKaracsonyfa = 255;
    gKaracsonyfa = 255;
    bKaracsonyfa = 255;
    setKaracsonyfaColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void karacsonyfaColorRandom() {
    rKaracsonyfa = random(0,255);
    gKaracsonyfa = random(0,255);
    bKaracsonyfa = random(0,255);
    setKaracsonyfaColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}

void karacsonyfaAnimNo() {
    karacsonyfaAnim = 0;
    server.send(200, "text/html", "<script>history.back();</script>");
}
void karacsonyfaAnimVillogas() {
    karacsonyfaAnim = 1;
    server.send(200, "text/html", "<script>history.back();</script>");
}
void karacsonyfaAnimRandomVillogas() {
    karacsonyfaAnim = 2;
    server.send(200, "text/html", "<script>history.back();</script>");
}
void karacsonyfaAnimSzinesVillogas() {
    karacsonyfaAnim = 3;
    server.send(200, "text/html", "<script>history.back();</script>");
}
void karacsonyfaAnimUszas() {
    karacsonyfaAnim = 4;
    server.send(200, "text/html", "<script>history.back();</script>");
}
void karacsonyfaAnimRandomUszas() {
    karacsonyfaAnim = 5;
    server.send(200, "text/html", "<script>history.back();</script>");
}

void csillagColorRed() {
    rCsillag = 255;
    gCsillag = 0;
    bCsillag = 0;
    setCsillagColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void csillagColorGreen() {
    rCsillag = 0;
    gCsillag = 255;
    bCsillag = 0;
    setCsillagColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void csillagColorBlue() {
    rCsillag = 0;
    gCsillag = 0;
    bCsillag = 255;
    setCsillagColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void csillagColorWhite() {
    rCsillag = 255;
    gCsillag = 255;
    bCsillag = 255;
    setCsillagColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void csillagColorRandom() {
    rCsillag = random(0,255);
    gCsillag = random(0,255);
    bCsillag = random(0,255);
    setCsillagColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}

void renszarvasColorRed() {
    rRenszarvas = 255;
    gRenszarvas = 0;
    bRenszarvas = 0;
    setRenszarvasColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void renszarvasColorGreen() {
    rRenszarvas = 0;
    gRenszarvas = 255;
    bRenszarvas = 0;
    setRenszarvasColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void renszarvasColorBlue() {
    rRenszarvas = 0;
    gRenszarvas = 0;
    bRenszarvas = 255;
    setRenszarvasColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void renszarvasColorWhite() {
    rRenszarvas = 255;
    gRenszarvas = 255;
    bRenszarvas = 255;
    setRenszarvasColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void renszarvasColorRandom() {
    rRenszarvas = random(0,255);
    gRenszarvas = random(0,255);
    bRenszarvas = random(0,255);
    setRenszarvasColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}

void renszarvasAnimNo() {
    renszarvasAnim = 0;
    server.send(200, "text/html", "<script>history.back();</script>");
}
void renszarvasAnimVillogas() {
    renszarvasAnim = 1;
    server.send(200, "text/html", "<script>history.back();</script>");
}
void renszarvasAnimRandomVillogas() {
    renszarvasAnim = 2;
    server.send(200, "text/html", "<script>history.back();</script>");
}
void renszarvasAnimSzinesVillogas() {
    renszarvasAnim = 3;
    server.send(200, "text/html", "<script>history.back();</script>");
}
void renszarvasAnimUszas() {
    renszarvasAnim = 4;
    server.send(200, "text/html", "<script>history.back();</script>");
}
void renszarvasAnimRandomUszas() {
    renszarvasAnim = 5;
    server.send(200, "text/html", "<script>history.back();</script>");
}

void keretColorRed() {
    rKeret = 255;
    gKeret = 0;
    bKeret = 0;
    setKeretColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void keretColorGreen() {
    rKeret = 0;
    gKeret = 255;
    bKeret = 0;
    setKeretColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void keretColorBlue() {
    rKeret = 0;
    gKeret = 0;
    bKeret = 255;
    setKeretColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void keretColorWhite() {
    rKeret = 255;
    gKeret = 255;
    bKeret = 255;
    setKeretColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void keretColorRandom() {
    rKeret = random(0,255);
    gKeret = random(0,255);
    bKeret = random(0,255);
    setKeretColors();
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}

void keretAnimNo() {
    keretAnim = 0;
    server.send(200, "text/html", "<script>history.back();</script>");
}
void keretAnimVillogas() {
    keretAnim = 1;
    server.send(200, "text/html", "<script>history.back();</script>");
}
void keretAnimRandomVillogas() {
    keretAnim = 2;
    server.send(200, "text/html", "<script>history.back();</script>");
}
void keretAnimSzinesVillogas() {
    keretAnim = 3;
    server.send(200, "text/html", "<script>history.back();</script>");
}
void keretAnimUszas() {
    keretAnim = 4;
    server.send(200, "text/html", "<script>history.back();</script>");
}
void keretAnimRandomUszas() {
    keretAnim = 5;
    server.send(200, "text/html", "<script>history.back();</script>");
}

void fuzerBe() {
  digitalWrite(FUZER_PIN, LOW);
  server.send(200, "text/html", "<script>history.back();</script>");
}
void fuzerKi() {
  digitalWrite(FUZER_PIN, HIGH);
  server.send(200, "text/html", "<script>history.back();</script>");
}

void brightness25() {
    Serial.println("Brightness 25%");
    brightnessPulse = false;
    FastLED.setBrightness(40);
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void brightness50() {
    Serial.println("Brightness 50%");
    brightnessPulse = false;
    FastLED.setBrightness(100);
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void brightness75() {
    Serial.println("Brightness 75%");
    brightnessPulse = false;
    FastLED.setBrightness(160);
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void brightness100() {
    Serial.println("Brightness 100%");
    brightnessPulse = false;
    FastLED.setBrightness(255);
    FastLED.show();
    server.send(200, "text/html", "<script>history.back();</script>");
}
void pulse() {
    brightnessPulse = true;
    server.send(200, "text/html", "<script>history.back();</script>");
}





//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(115200);

  FastLED.addLeds<WS2812, LED_PIN_SZAM, GRB>(leds_szam, NUM_LEDS_SZAM);
  FastLED.addLeds<WS2812, LED_PIN_KERET, GRB>(leds_keret, NUM_LEDS_KERET);
  FastLED.addLeds<WS2812, LED_PIN_KARACSONYFA, GRB>(leds_karacsonyfa, NUM_LEDS_KARACSONYFA);
  FastLED.addLeds<WS2812, LED_PIN_RENSZARVAS, GRB>(leds_renszarvas, NUM_LEDS_RENSZARVAS);

  pinMode(FUZER_PIN, OUTPUT);
  //digitalWrite(FUZER_PIN, HIGH);

  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
 
  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/button=off", allOff);

  server.on("/button=szam-color01", szamColorRed);
  server.on("/button=szam-color02", szamColorGreen);
  server.on("/button=szam-color03", szamColorBlue);
  server.on("/button=szam-color04", szamColorWhite);
  server.on("/button=szam-color-random", szamColorRandom);

  server.on("/button=kilences-nincs", szamAnimNo);
  server.on("/button=kilences-villogas", szamAnimVillogas);
  server.on("/button=kilences-random-villogas", szamAnimRandomVillogas);
  server.on("/button=kilences-uszas", szamAnimUszas);
  server.on("/button=kilences-random-uszas", szamAnimRandomUszas);

  server.on("/button=karacsonyfa-color01", karacsonyfaColorRed);
  server.on("/button=karacsonyfa-color02", karacsonyfaColorGreen);
  server.on("/button=karacsonyfa-color03", karacsonyfaColorBlue);
  server.on("/button=karacsonyfa-color04", karacsonyfaColorWhite);
  server.on("/button=karacsonyfa-color-random", karacsonyfaColorRandom);

  server.on("/button=karacsonyfa-nincs", karacsonyfaAnimNo);
  server.on("/button=karacsonyfa-villogas", karacsonyfaAnimVillogas);
  server.on("/button=karacsonyfa-random-villogas", karacsonyfaAnimRandomVillogas);
  server.on("/button=karacsonyfa-szines-villogas", karacsonyfaAnimSzinesVillogas);
  server.on("/button=karacsonyfa-uszas", karacsonyfaAnimUszas);
  server.on("/button=karacsonyfa-random-uszas", karacsonyfaAnimRandomUszas);

  server.on("/button=csillag-color01", csillagColorRed);
  server.on("/button=csillag-color02", csillagColorGreen);
  server.on("/button=csillag-color03", csillagColorBlue);
  server.on("/button=csillag-color04", csillagColorWhite);
  server.on("/button=csillag-color-random", csillagColorRandom);

  server.on("/button=renszarvas-color01", renszarvasColorRed);
  server.on("/button=renszarvas-color02", renszarvasColorGreen);
  server.on("/button=renszarvas-color03", renszarvasColorBlue);
  server.on("/button=renszarvas-color04", renszarvasColorWhite);
  server.on("/button=renszarvas-color-random", renszarvasColorRandom);

  server.on("/button=renszarvas-nincs", renszarvasAnimNo);
  server.on("/button=renszarvas-villogas", renszarvasAnimVillogas);
  server.on("/button=renszarvas-random-villogas", renszarvasAnimRandomVillogas);
  server.on("/button=renszarvas-szines-villogas", renszarvasAnimSzinesVillogas);
  server.on("/button=renszarvas-uszas", renszarvasAnimUszas);
  server.on("/button=renszarvas-random-uszas", renszarvasAnimRandomUszas);

  server.on("/button=keret-color01", keretColorRed);
  server.on("/button=keret-color02", keretColorGreen);
  server.on("/button=keret-color03", keretColorBlue);
  server.on("/button=keret-color04", keretColorWhite);
  server.on("/button=keret-color-random", keretColorRandom);

  server.on("/button=keret-nincs", keretAnimNo);
  server.on("/button=keret-villogas", keretAnimVillogas);
  server.on("/button=keret-random-villogas", keretAnimRandomVillogas);
  server.on("/button=keret-szines-villogas", keretAnimSzinesVillogas);
  server.on("/button=keret-uszas", keretAnimUszas);
  server.on("/button=keret-random-uszas", keretAnimRandomUszas);

  server.on("/button=fuzer-be", fuzerBe);
  server.on("/button=fuzer-ki", fuzerKi);
  
  server.on("/button=25", brightness25);
  server.on("/button=50", brightness50);
  server.on("/button=75", brightness75);
  server.on("/button=100", brightness100);
  server.on("/button=pulse", pulse);

  server.begin();                  //Start server
  Serial.println("HTTP server started");
  delay(1000);
  allOffMethod();
  delay(200);
  allOffMethod();
  delay(200);
  allOffMethod();
  delay(200);
  allOffMethod();
  delay(200);
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();          //Handle client requests

  if(brightnessPulse){
    brightnessPulseMethod();
  }
  
  szamAnimMethod();
  karacsonyfaAnimMethod();
  renszarvasAnimMethod();
  keretAnimMethod();

  FastLED.show(); //BUG MEGOLDÁSÁRA
  delay(2);
  FastLED.show();
}



void szamAnimMethod(){
    switch (szamAnim) {
      case 1:
        szamVillogasAnimation(false);
        break;
      case 2:
        szamVillogasAnimation(true);
        break;
      case 3:
        szamUszasAnimation(false);
        break;
      case 4:
        szamUszasAnimation(true);
        break;
      default:
        break;
    }
}

void karacsonyfaAnimMethod(){
    switch (karacsonyfaAnim) {
      case 1:
        karacsonyfaVillogasAnimation(false);
        break;
      case 2:
        karacsonyfaVillogasAnimation(true);
        break;
      case 3:
        karacsonyfaSzinesVillogasAnimation();
        break;
      case 4:
        karacsonyfaUszasAnimation(false);
        break;
      case 5:
        karacsonyfaUszasAnimation(true);
        break;
      default:
        break;
    }
}

void renszarvasAnimMethod(){
    switch (renszarvasAnim) {
      case 1:
        renszarvasVillogasAnimation(false);
        break;
      case 2:
        renszarvasVillogasAnimation(true);
        break;
      case 3:
        renszarvasSzinesVillogasAnimation();
        break;
      case 4:
        renszarvasUszasAnimation(false);
        break;
      case 5:
        renszarvasUszasAnimation(true);  
        break;
      default:
        break;
    }
}

void keretAnimMethod(){
    switch (keretAnim) {
      case 1:
        keretVillogasAnimation(false);
        break;
      case 2:
        keretVillogasAnimation(true);
        break;
      case 3:
        keretSzinesVillogasAnimation();
        break;
      case 4:
        keretUszasAnimation(false);
        break;
      case 5:
        keretUszasAnimation(true);  
        break;
      default:
        break;
    }
}

unsigned long szamVillogasPM = 0;
bool szamVillogasState = false;
void szamVillogasAnimation(bool szines) {
    unsigned long szamVillogasCM = millis();
    if(szamVillogasCM - szamVillogasPM >= VILLOGAS_SPEED){
      szamVillogasPM = szamVillogasCM;
      Serial.println("animacio");
      if(szamVillogasState){
        setSzamColorsOff();
        FastLED.show();
        szamVillogasState = false;
      }else{
        if(szines){
          rSzam = random(0,255);
          gSzam = random(0,255);
          bSzam = random(0,255);
        }
        setSzamColors();
        FastLED.show();
        szamVillogasState = true;
      }
    }
}

//Úszás animáció
unsigned long szamUszasPM = 0;
byte szamLedIndex = 0;
bool szamUszasAnimState = true;
void szamUszasAnimation(bool szines){
  unsigned long szamUszasCM = millis();
  if(szamUszasCM - szamUszasPM >= 100){
    szamUszasPM = szamUszasCM;
    
    if(szamUszasAnimState){
      leds_szam[szamLedIndex] = CRGB(rSzam, gSzam, bSzam);
    }else{
      leds_szam[szamLedIndex] = CRGB(0, 0, 0);
    }
    szamLedIndex++;
    if(szamLedIndex > 9){
      szamUszasAnimState = !szamUszasAnimState;
      szamLedIndex = 0;
      if(szines){
        rSzam = random(0,255);
        gSzam = random(0,255);
        bSzam = random(0,255);
      }
    }
    
    FastLED.show();
  }
}


//Karácsonyfa villogás
unsigned long karacsonyfaVillogasPM = 0;
bool karacsonyfaVillogasState = false;
void karacsonyfaVillogasAnimation(bool szines) {
    unsigned long karacsonyfaVillogasCM = millis();
    if(karacsonyfaVillogasCM - karacsonyfaVillogasPM >= VILLOGAS_SPEED){
      karacsonyfaVillogasPM = karacsonyfaVillogasCM;
      if(karacsonyfaVillogasState){
        setKaracsonyfaColorsOff();
        FastLED.show();
        karacsonyfaVillogasState = false;
      }else{
        if(szines){
          rKaracsonyfa = random(0,255);
          gKaracsonyfa = random(0,255);
          bKaracsonyfa = random(0,255);
        }
        setKaracsonyfaColors();
        FastLED.show();
        karacsonyfaVillogasState = true;
      }
    }
}

//Karácsonyfaizzó szerú villogás
unsigned long karacsonyfaSzinesVillogasPM = 0;
void karacsonyfaSzinesVillogasAnimation(){
  unsigned long karacsonyfaSzinesVillogasCM = millis();
  if(karacsonyfaSzinesVillogasCM - karacsonyfaSzinesVillogasPM >= 400){
    karacsonyfaSzinesVillogasPM = karacsonyfaSzinesVillogasCM;
    for(int i=0;i<9;i++){
      if(i != 4){
        leds_karacsonyfa[i] = CRGB(random(0,255),random(0,255),random(0,255));
      }
    }
    FastLED.show();
  }
}

//Úszás animáció
unsigned long karacsonyfaUszasPM = 0;
byte karacsonyfaLedIndex0 = 0;
byte karacsonyfaLedIndex1 = 8;
bool karacsonyfaUszasAnimState = true;
void karacsonyfaUszasAnimation(bool szines){
  unsigned long karacsonyfaUszasCM = millis();
  if(karacsonyfaUszasCM - karacsonyfaUszasPM >= 100){
    karacsonyfaUszasPM = karacsonyfaUszasCM;
    
    if(karacsonyfaUszasAnimState){
      leds_karacsonyfa[karacsonyfaLedIndex0] = CRGB(rKaracsonyfa, gKaracsonyfa, bKaracsonyfa);
      leds_karacsonyfa[karacsonyfaLedIndex1] = CRGB(rKaracsonyfa, gKaracsonyfa, bKaracsonyfa);
    }else{
      leds_karacsonyfa[karacsonyfaLedIndex0] = CRGB(0, 0, 0);
      leds_karacsonyfa[karacsonyfaLedIndex1] = CRGB(0, 0, 0);
    }
    karacsonyfaLedIndex0++;
    karacsonyfaLedIndex1--;
    if(karacsonyfaLedIndex0 > 4){
      karacsonyfaUszasAnimState = !karacsonyfaUszasAnimState;
      karacsonyfaLedIndex0 = 0;
      karacsonyfaLedIndex1 = 8;
      if(szines){
        rKaracsonyfa = random(0,255);
        gKaracsonyfa = random(0,255);
        bKaracsonyfa = random(0,255);
      }
    }
    
    FastLED.show();
  }
}


unsigned long renszarvasVillogasPM = 0;
bool renszarvasVillogasState = false;
void renszarvasVillogasAnimation(bool szines) {
    unsigned long renszarvasVillogasCM = millis();
    if(renszarvasVillogasCM - renszarvasVillogasPM >= VILLOGAS_SPEED){
      renszarvasVillogasPM = renszarvasVillogasCM;
      if(renszarvasVillogasState){
        leds_renszarvas[0] = CRGB(rRenszarvas, gRenszarvas, bRenszarvas);
        leds_renszarvas[1] = CRGB(0, 0, 0);
        leds_renszarvas[2] = CRGB(rRenszarvas, gRenszarvas, bRenszarvas);
        leds_renszarvas[3] = CRGB(0, 0, 0);
        leds_renszarvas[4] = CRGB(rRenszarvas, gRenszarvas, bRenszarvas);
        leds_renszarvas[5] = CRGB(0, 0, 0);
        
        FastLED.show();
        renszarvasVillogasState = false;
      }else{
        if(szines){
          rRenszarvas = random(0,255);
          gRenszarvas = random(0,255);
          bRenszarvas = random(0,255);
        }

        leds_renszarvas[0] = CRGB(0, 0, 0);
        leds_renszarvas[1] = CRGB(rRenszarvas, gRenszarvas, bRenszarvas);
        leds_renszarvas[2] = CRGB(0, 0, 0);
        leds_renszarvas[3] = CRGB(rRenszarvas, gRenszarvas, bRenszarvas);
        leds_renszarvas[4] = CRGB(0, 0, 0);
        leds_renszarvas[5] = CRGB(rRenszarvas, gRenszarvas, bRenszarvas);
        
        FastLED.show();
        renszarvasVillogasState = true;
      }
    }
}

//Karácsonyfaizzó szerú villogás
unsigned long renszarvasSzinesVillogasPM = 0;
void renszarvasSzinesVillogasAnimation(){
  unsigned long renszarvasSzinesVillogasCM = millis();
  if(renszarvasSzinesVillogasCM - renszarvasSzinesVillogasPM >= 400){
    renszarvasSzinesVillogasPM = renszarvasSzinesVillogasCM;
    for(int i=0;i<6;i++){
      leds_renszarvas[i] = CRGB(random(0,255),random(0,255),random(0,255));
    }
    FastLED.show();
  }
}

//Úszás animáció
unsigned long renszarvasUszasPM = 0;
byte renszarvasLedIndex = 0;
bool renszarvasUszasAnimState = true;
void renszarvasUszasAnimation(bool szines){
  unsigned long renszarvasUszasCM = millis();
  if(renszarvasUszasCM - renszarvasUszasPM >= 100){
    renszarvasUszasPM = renszarvasUszasCM;
    
    if(renszarvasUszasAnimState){
      leds_renszarvas[renszarvasLedIndex] = CRGB(rRenszarvas, gRenszarvas, bRenszarvas);
    }else{
      leds_renszarvas[renszarvasLedIndex] = CRGB(0, 0, 0);
    }
    renszarvasLedIndex++;
    if(renszarvasLedIndex > 5){
      renszarvasUszasAnimState = !renszarvasUszasAnimState;
      renszarvasLedIndex = 0;
      if(szines){
        rRenszarvas = random(0,255);
        gRenszarvas = random(0,255);
        bRenszarvas = random(0,255);
      }
    }
    
    FastLED.show();
  }
}

unsigned long keretVillogasPM = 0;
bool keretVillogasState = false;
void keretVillogasAnimation(bool szines) {
    unsigned long keretVillogasCM = millis();
    if(keretVillogasCM - keretVillogasPM >= VILLOGAS_SPEED){
      keretVillogasPM = keretVillogasCM;
      Serial.println("animacio");
      if(keretVillogasState){
        setKeretColorsOff();
        FastLED.show();
        keretVillogasState = false;
      }else{
        if(szines){
          rKeret = random(0,255);
          gKeret = random(0,255);
          bKeret = random(0,255);
        }
        setKeretColors();
        FastLED.show();
        keretVillogasState = true;
      }
    }
}

//Karácsonyfaizzó szerú villogás
unsigned long keretSzinesVillogasPM = 0;
void keretSzinesVillogasAnimation(){
  unsigned long keretSzinesVillogasCM = millis();
  if(keretSzinesVillogasCM - keretSzinesVillogasPM >= 400){
    keretSzinesVillogasPM = keretSzinesVillogasCM;
    for(int i=0;i<144;i++){
      leds_keret[i] = CRGB(random(0,255),random(0,255),random(0,255));
    }
    FastLED.show();
  }
}

//Úszás animáció
unsigned long keretUszasPM = 0;
byte keretLedIndex0 = 0;
byte keretLedIndex1 = 143;
bool keretUszasAnimState = true;
void keretUszasAnimation(bool szines){
  unsigned long keretUszasCM = millis();
  if(keretUszasCM - keretUszasPM >= 15){
    keretUszasPM = keretUszasCM;
    
    if(keretUszasAnimState){
      leds_keret[keretLedIndex0] = CRGB(rKeret, gKeret, bKeret);
      leds_keret[keretLedIndex1] = CRGB(rKeret, gKeret, bKeret);
    }else{
      leds_keret[keretLedIndex0] = CRGB(0, 0, 0);
      leds_keret[keretLedIndex1] = CRGB(0, 0, 0);
    }
    keretLedIndex0++;
    keretLedIndex1--;
    if(keretLedIndex0 > 72){
      keretUszasAnimState = !keretUszasAnimState;
      keretLedIndex0 = 0;
      keretLedIndex1 = 143;
      if(szines){
        rKeret = random(0,255);
        gKeret = random(0,255);
        bKeret = random(0,255);
      }
    }
    
    FastLED.show();
  }
}

byte pulseBrightness = 0;
bool pulseState = false;
bool pulsePause = false;
unsigned long pulsePreviousMillis = 0;
unsigned long pulsePausePreviousMillis = 0;
void brightnessPulseMethod(){
  unsigned long pulseCurrentMillis = millis();
  unsigned long pulsePauseCurrentMillis = millis();
  if(pulseCurrentMillis - pulsePreviousMillis >= 10){
    pulsePreviousMillis = pulseCurrentMillis;
    if(pulseBrightness == 255){
      pulseState = true;
      pulsePause = true;
    }
    if(pulseBrightness == 0){
      pulseState = false;
    }
    if(pulseState){
      pulseBrightness--;
    }else{
      if(pulsePauseCurrentMillis - pulsePausePreviousMillis >= 1000){
        pulsePausePreviousMillis = pulsePauseCurrentMillis;
        pulsePause = false;
      }
      if(!pulsePause){
        pulseBrightness++;
      }
    }
    //Serial.println(pulseBrightness);
    FastLED.setBrightness(pulseBrightness);
    FastLED.show();
  }
}

void setSzamColors(){
    for(int i=0;i<10;i++){
      leds_szam[i] = CRGB(rSzam, gSzam, bSzam);
    }
    //leds_szam[0] = CRGB(rSzam, gSzam, bSzam);
    //leds_szam[1] = CRGB(rSzam, gSzam, bSzam);
    //leds_szam[2] = CRGB(rSzam, gSzam, bSzam);
    //leds_szam[3] = CRGB(rSzam, gSzam, bSzam);
    //leds_szam[4] = CRGB(rSzam, gSzam, bSzam);
    //leds_szam[5] = CRGB(rSzam, gSzam, bSzam);
    //leds_szam[6] = CRGB(rSzam, gSzam, bSzam);
    //leds_szam[7] = CRGB(rSzam, gSzam, bSzam);
    //leds_szam[8] = CRGB(rSzam, gSzam, bSzam);
    //leds_szam[9] = CRGB(rSzam, gSzam, bSzam);
    Serial.println("be");
}
void setSzamColorsOff(){
    for(int i=0;i<10;i++){
      leds_szam[i] = CRGB(0, 0, 0);
    }
    //leds_szam[0] = CRGB(0, 0, 0);
    //leds_szam[1] = CRGB(0, 0, 0);
    //leds_szam[2] = CRGB(0, 0, 0);
    //leds_szam[3] = CRGB(0, 0, 0);
    //leds_szam[4] = CRGB(0, 0, 0);
    //leds_szam[5] = CRGB(0, 0, 0);
    //leds_szam[6] = CRGB(0, 0, 0);
    //leds_szam[7] = CRGB(0, 0, 0);
    //leds_szam[8] = CRGB(0, 0, 0);
    //leds_szam[9] = CRGB(0, 0, 0);
    Serial.println("ki");
}
void setKeretColors(){
    for(int i=0;i<144;i++){
      leds_keret[i] = CRGB(rKeret, gKeret, bKeret);
    }
}
void setKeretColorsOff(){
    for(int i=0;i<144;i++){
      leds_keret[i] = CRGB(0, 0, 0);
    }
}
void setKaracsonyfaColors(){
    for(int i=0;i<9;i++){
      if(i!=4){
        leds_karacsonyfa[i] = CRGB(rKaracsonyfa, gKaracsonyfa, bKaracsonyfa);
      }
    }
    //leds_karacsonyfa[0] = CRGB(rKaracsonyfa, gKaracsonyfa, bKaracsonyfa);
    //leds_karacsonyfa[1] = CRGB(rKaracsonyfa, gKaracsonyfa, bKaracsonyfa);
    //leds_karacsonyfa[2] = CRGB(rKaracsonyfa, gKaracsonyfa, bKaracsonyfa);
    //leds_karacsonyfa[3] = CRGB(rKaracsonyfa, gKaracsonyfa, bKaracsonyfa);
    //leds_karacsonyfa[5] = CRGB(rKaracsonyfa, gKaracsonyfa, bKaracsonyfa);
    //leds_karacsonyfa[6] = CRGB(rKaracsonyfa, gKaracsonyfa, bKaracsonyfa);
    //leds_karacsonyfa[7] = CRGB(rKaracsonyfa, gKaracsonyfa, bKaracsonyfa);
    //leds_karacsonyfa[8] = CRGB(rKaracsonyfa, gKaracsonyfa, bKaracsonyfa);
}
void setKaracsonyfaColorsOff(){
    for(int i=0;i<9;i++){
      if(i!=4){
        leds_karacsonyfa[i] = CRGB(0, 0, 0);
      }
    }
}
void setCsillagColors(){
  leds_karacsonyfa[4] = CRGB(rCsillag, gCsillag, bCsillag);
}
void setCsillagOff(){
  leds_karacsonyfa[4] = CRGB(0, 0, 0);
}
void setRenszarvasColors(){
    for(int i=0;i<6;i++){
      leds_renszarvas[i] = CRGB(rRenszarvas, gRenszarvas, bRenszarvas);
    }
}
void setRenszarvasColorsOff(){
    for(int i=0;i<9;i++){
      leds_renszarvas[i] = CRGB(0, 0, 0);
    }
}
