#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Config.h>

// Initialize WiFi Config
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
WiFiClient espClient;

// Initialize Ultrasonic Sensor Config
const int trigPin = TRIG_PIN;   
const int echoPin = ECHO_PIN;   

// Initialize MQTT Config
const char* mqttServer = MQTT_SERVER;
const char* mqttUser = MQTT_USER;
const char* mqttPass = MQTT_PASSWORD;
const char* subTopic = SUB_TOPIC;
const char* pubTopic = PUB_TOPIC;
String clientId = CLIENTID_PREFIX;
PubSubClient client(espClient);

// init variables
int iteration = 0;
long lastMsgTs = 0;

void setup_wifi() {
    delay(10);

    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    // While trying to connect to WiFi
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    randomSeed(micros());
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
    // somewhere to put the message
    char messageBuffer[length+1];
    snprintf(messageBuffer, length+1, "%s", payload);
    Serial.print("Message arrived: [ ");
    Serial.print(messageBuffer);
    Serial.print(" ]\n");

    // Switch on the LED if payload==on
    // and Switch off if payload==off
    // do nothing if else

    // NOT: that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01
    if (strcmp( messageBuffer, "on" ) == 0) {
        digitalWrite(LED_BUILTIN, LOW);
    } else if (strcmp( messageBuffer, "off" ) == 0) {
        digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
    }
}

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...\n");
        // Create a random client ID
        clientId += String(random(0xffff), HEX);
        char firstMsg[100];
        snprintf(firstMsg, 100, "%s - connected ", clientId.c_str());
        // Attempt to connect
        if (client.connect(clientId.c_str(), mqttUser, mqttPass)) {
            Serial.print(firstMsg);
            Serial.print("to server: ");
            Serial.println(mqttServer);
            // Once connected, publish an announcement...
            client.publish(pubTopic, firstMsg);
            // ... and resubscribe
            client.subscribe(subTopic);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void setup() {
    // set baud rate
    Serial.begin(BAUD_RATE);

    // initialize pins & builtin LED
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(trigPin, OUTPUT); 
    pinMode(echoPin, INPUT); 

    // connect to wifi
    setup_wifi();

    // connect to mqtt server
    client.setServer(mqttServer, 1883);
    // setup if a message is received, do something
    client.setCallback(callback);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    
    // loop reading & publishing to a topic to mqtt server
    long now = millis();
    if (now - lastMsgTs > 100) {
        // set the trigpin to LOW
        digitalWrite(trigPin, LOW);  
        delayMicroseconds(2);  
        // Sets the trigPin on HIGH state for 10 micro seconds  
        digitalWrite(trigPin, HIGH);  
        delayMicroseconds(10);  
        digitalWrite(trigPin, LOW);  

        // Reads the echoPin, returns the sound wave travel time in microseconds  
        long duration = pulseIn(echoPin, HIGH);  
        // Calculate the distance  
        long distance = duration*0.034/2;  

        // build message
        ++iteration;
        lastMsgTs = now;
        char pubMsg[100];
        snprintf(pubMsg, 100, "distance: %ld - interation: %d", distance, iteration);
        Serial.print("Publish message:");
        Serial.println(pubMsg);
        
        // publish message
        client.publish(pubTopic, pubMsg);
    }
}
