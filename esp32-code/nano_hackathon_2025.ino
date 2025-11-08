#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>

// ====== WIFI & AWS CONFIG ======
const char WIFI_SSID[]     = "pocox6pro";
const char WIFI_PASSWORD[] = "********";
const char AWS_ENDPOINT[]  = "a10ndj6af7jzj8-ats.iot.us-east-1.amazonaws.com"; // e.g., a1b23c4d5e6f7g-ats.iot.ap-south-1.amazonaws.com
const int  AWS_PORT        = 8883;
const char MQTT_TOPIC[]    = "pharmachain/device1/data";

// ====== CERTIFICATES ======
const char CA_CERT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

const char DEVICE_CERT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUYy2iuM1gcwELPcwyRC3bYdbbEyMwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI1MTEwNTIwNTEy
NloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAJvUBUtdgxmJEoql2jxR
/YvqOElo5fvlTn3tPHZypYh6g4a5IS8S5GmAZret/d7pnLX65OdGV242mcUHAvVT
X1Nd2ghmKf6RI4I+4dcUImu44ifIIAmGty9UZazh5qEcMFWobvBirCedfV4RKTbH
g9zDay519yIWDGtQfEHkO+LxgYVsNRvltcmjfczAzYEQ8UXCWVngCCWgasojcu+M
LPpEd/+eIj7KlqBqL1ABJBqs6ShrUnVxbt5Fm6hdXooopWy/Lza8vgZ6Vid9LHdm
LiLOdzmzR9xD0B6JTIVNZkrNfQ8ZMjhHqkTL3SqlZuTWkXQX088bs2XiuWXpXNqS
xvECAwEAAaNgMF4wHwYDVR0jBBgwFoAUWvnwpQR6EAEPoPexmKOjCeO7RyIwHQYD
VR0OBBYEFDGwCjDof8mvyxIiFCYYErwDy352MAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQDGavGtbyHMe1ttAQ6LT8mywS+4
ehk0GNVIypbIcmMHbAcqzcrZU1NNKLa09VJQVQ8dNO6tyRMBN43L3ho/8/0BLXRJ
dGb8eQMoKzzG+yNgdEniEgD36bfdRuuHOY3/gPsO3BYr3/4PT7yMYBk1s8CmiOYC
I/ZtgL3YLPJfqOYakN0CGD1yHM2hyvimpZtbxqiVKv1nBQVCyEgL5FJe6Ag+7u3S
6bZ3eYxPT8dAwWzCLRh26HAu4qmbY8wMVPlrn3Tm+71dRYghTzovL/C3iGXPUAXj
2ToFnUfZa55Pww1NineS9b1e0016hI3xDXKTn0WDoIQM1zXekBvtWNnIJjvB
-----END CERTIFICATE-----
)KEY";

const char PRIVATE_KEY[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAm9QFS12DGYkSiqXaPFH9i+o4SWjl++VOfe08dnKliHqDhrkh
LxLkaYBmt6393umctfrk50ZXbjaZxQcC9VNfU13aCGYp/pEjgj7h1xQia7jiJ8gg
CYa3L1RlrOHmoRwwVahu8GKsJ519XhEpNseD3MNrLnX3IhYMa1B8QeQ74vGBhWw1
G+W1yaN9zMDNgRDxRcJZWeAIJaBqyiNy74ws+kR3/54iPsqWoGovUAEkGqzpKGtS
dXFu3kWbqF1eiiilbL8vNry+BnpWJ30sd2YuIs53ObNH3EPQHolMhU1mSs19Dxky
OEeqRMvdKqVm5NaRdBfTzxuzZeK5Zelc2pLG8QIDAQABAoIBACzNohQQN/Bwm9cn
Zn7vaAIBftuEgJOTU6UbP8YionuCFiz3L5ZUTT4UP/RR32tnKbqc/5GLhPnYUw/2
OyCELyVOF7FQhoR+AIKjR9XKITdtugCMJsH2bNb74hv5+5CZBUCro/ITvMHLI0+X
iY0Jgl1IBpoeKscvJbT5ODDNyXYvXQ/67sNFFMq6H8lT+Gi07Q6ZYj4xySYbQ7oN
sed/cJWScSqnEr3ma2jYzrSoaH93BAHPxRmelmU6eUbVPtwgmnr4dLn/BUo2Rjqj
scwPtrx9F+bWGQwLCoSLYEC4ngRd/chokI7pPzponGXa2/Q0+lnH9KCQ1BGXSS5Y
xJGEUz0CgYEAziWTXE7OBHJ6lATfHxqkd6YSbThiVrmRW0jY7328+XQgyuESgwaq
dnMN1Za/Hq9HiVUbkexCPU6K6wjM1+DYHhw7GBF1ZhMZv0GXGj20PcucztKsfCl/
piNAF4G9X6/xbtlcLxpkdkYVTIC07PwdsiqqVc9l/UrYWZ17sm0yB1MCgYEAwYNA
ic9IMZgrcmGfmk+Uya26pNLsTh+2uhzwhU2gFYarOg8DblnX923nldlHRt1gB1Q0
VjvwW7+RMQwOCYQjGj0duXNmvJ6Q75Me3jw6jFveruJHPEUBE2TPOLwyh+cQJFl9
5+lFk7nqhTfXUVA4iCLhVYFzDNGr7YC/5xIwxCsCgYEAo4dDQnJxEy4M9uZ2aKIl
3ZQsAeA8gjdqi1NaY4EHK4tniAjLOo1LI/NZAho3OgwpX1PYLwWP9jocER5GAOoo
Bpq2df1Dd9mnDwTk7LgOBwdCOP7+PPBehlhoOz18EC/uAdUDP4pcL0ahFrLy0xuc
Iv2dabzAVxHMViqLOZtNmGkCgYBIbiov6sfo2D4a1/N3PKZCPqaHnvMX4sLwMnjM
TFvI8tkQKCJuoW42jIqMCgGlwqJ428mjJVg33yInK43tfentnkaL7Wfc3bT+FyEM
qZp4gsJ8zvE+280lRyxQ3hBD0fa2xFBeB3AldQcUyevr4YznV7bj3EoF2Ezz3qb1
hFeAdQKBgAcKE+Be+FgFkFU93lb1Cnyb67IK09AMMVXYDGtezmmWKX+B5X18DgDa
fTXcLHygUI3eTZpj7gqswfhrLcdPtGfdQf6VXM56Nb2qwarUay0uEGgWBkw7ydCW
ZiVl/+mbs6pSjgP/b8Ucd9oUiuIrDww4TaneQFkIsW1/k30rUv6E
-----END RSA PRIVATE KEY-----
)KEY";

// ====== OBJECTS ====== 
WiFiClientSecure net;
MQTTClient client(256);

// ====== GPS ROUTE (KLU → SRIVILLIPUTHUR) ======
const float startLat = 9.5747;
const float startLon = 77.6798;
const float endLat   = 9.5636;
const float endLon   = 77.6822;
float currentLat = startLat;
float currentLon = startLon;
float stepLat, stepLon;
int direction = 1; // 1 = forward, -1 = return

// ====== SPIKE CONTROL ======
bool spikeActive = false;
unsigned long spikeStart = 0;

// ====== SIMULATION BASES ======
float baseTemp = 25.0;   // baseline temp in °C
float baseHum  = 60.0;   // baseline humidity %
int   publishCounter = 0;

// ====== UTILS ======
void seedRandom() {
  // Use ESP32 hardware RNG to seed pseudo-random number generator
  unsigned long r = esp_random();
  randomSeed((uint32_t)r);
}

// ====== CONNECTION HELPERS ======
void connectToWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected!");
  Serial.print("IP: "); Serial.println(WiFi.localIP());
}

void connectToAWS() {
  Serial.print("Connecting to AWS IoT Core...");
  while (!client.connect("PharmaNodeESP32")) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\n✅ Connected to AWS IoT Core!");
}

// ====== GPS MOVEMENT ======
void updateGPS() {
  // Precompute steps to divide route into ~40 segments
  stepLat = (endLat - startLat) / 40.0;
  stepLon = (endLon - startLon) / 40.0;

  currentLat += stepLat * direction;
  currentLon += stepLon * direction;

  // If we've reached beyond endpoints, reverse direction
  // (use lat as driver; route roughly monotonic)
  if (direction == 1) {
    if ((stepLat > 0 && currentLat >= endLat) || (stepLat < 0 && currentLat <= endLat)) {
      direction = -1;
    }
  } else {
    if ((stepLat > 0 && currentLat <= startLat) || (stepLat < 0 && currentLat >= startLat)) {
      direction = 1;
    }
  }
}

// ====== SIMULATED SENSOR GENERATION ======
float simulateTemperature(bool duringSpike) {
  // small natural jitter around baseTemp
  float jitter = (random(-30, 31)) / 10.0; // -3.0 .. +3.0
  float t = baseTemp + jitter;

  if (duringSpike) {
    t += 7.0; // spike magnitude
  }

  // Occasionally produce a tiny trend (simulate day warming/cooling)
  if (publishCounter % 60 == 0) {
    baseTemp += (random(-10, 11) / 100.0); // slow drift
  }
  return t;
}

float simulateHumidity() {
  float jitter = (random(-40, 41)) / 10.0; // -4.0 .. +4.0
  float h = baseHum + jitter;
  // keep in 10..95 bounds
  if (h < 10.0) h = 10.0;
  if (h > 95.0) h = 95.0;
  return h;
}

// ====== PUBLISH ======
void publishData() {
  publishCounter++;

  // Possibly trigger a spike randomly (roughly once per ~30-50 publishes)
  if (!spikeActive && random(0, 40) == 0) {
    spikeActive = true;
    spikeStart = millis();
    Serial.println("🌡️ Spike started!");
  }

  // Determine if spike is currently active (persist for 5 seconds)
  bool duringSpike = false;
  if (spikeActive) {
    if (millis() - spikeStart <= 5000UL) {
      duringSpike = true;
    } else {
      spikeActive = false; // end spike
      Serial.println("🌡️ Spike ended.");
    }
  }

  // Simulate sensors
  float temp = simulateTemperature(duringSpike);
  float hum  = simulateHumidity();

  // Simulate seal status (rare BROKEN)
  String sealStatus = (random(0, 80) == 0) ? "BROKEN" : "INTACT"; // ~1.25% chance each publish

  // Move GPS a step
  updateGPS();

  // Build JSON
  StaticJsonDocument<256> doc;
  doc["batch_id"]   = "BATCH_KLU01";
  doc["stage"]      = "Distributor";
  doc["temperature"]= round(temp * 100.0) / 100.0; // 2 decimals
  doc["humidity"]   = round(hum  * 100.0) / 100.0;
  doc["latitude"]   = currentLat;
  doc["longitude"]  = currentLon;
  doc["seal_status"]= sealStatus;
  doc["timestamp"]  = millis();

  char jsonBuffer[256];
  serializeJson(doc, jsonBuffer);

  // Publish
  if (client.publish(MQTT_TOPIC, jsonBuffer)) {
    Serial.print("✅ Published: ");
    Serial.println(jsonBuffer);
  } else {
    Serial.println("❌ Publish failed!");
  }

  // Adaptive delay: normal 10s, faster (3s) on alert/spike
  int delayTime = 10000;
  if (duringSpike || sealStatus == "BROKEN" || temp > 32.0 || hum > 80.0) {
    delayTime = 3000;
    Serial.println("⚠️ ALERT: sending faster updates...");
  }

  Serial.printf("⏳ Next update in %d s\n\n", delayTime / 1000);
  delay(delayTime);
}

// ====== SETUP & LOOP ======
void setup() {
  Serial.begin(115200);
  delay(1000);
  seedRandom();

  // initialize GPS position
  currentLat = startLat;
  currentLon = startLon;

  connectToWiFi();

  // set certs for TLS
  net.setCACert(CA_CERT);
  net.setCertificate(DEVICE_CERT);
  net.setPrivateKey(PRIVATE_KEY);

  client.begin(AWS_ENDPOINT, AWS_PORT, net);
  connectToAWS();
}

void loop() {
  client.loop();
  if (!client.connected()) connectToAWS();
  publishData();
}
