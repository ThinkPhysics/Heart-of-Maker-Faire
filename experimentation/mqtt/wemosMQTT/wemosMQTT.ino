// WeMos test code for MQTT parsing.
// Based on Robot_Orchestra_Wemos


#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "nustem";
const char* password = "nustem123";
const char* mqtt_server = "192.168.1.1";

String subsTopicString;
char subsTopicArray[100];

WiFiClient espClient;
PubSubClient client(espClient);

// Each device has a unique name, generated from the hardware MAC address.
// These variables will store those names.
String huzzahMACAddress;
String skutterNameString;
char skutterNameArray[60];

void setup() {
    Serial.begin(115200);
    setup_wifi();

    // Get this Huzzah's MAC address and use it to register with the MQTT server
    huzzahMACAddress = WiFi.macAddress();
    skutterNameString = "skutter_" + huzzahMACAddress;
    Serial.println(skutterNameString);
    skutterNameString.toCharArray(skutterNameArray, 60);
    
    // For testing purposes, subscribe to everything in this namespace
    subsTopicString = "heart/#";
    subsTopicString.toCharArray(subsTopicArray, 60);
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);

    // The actual channel subscription is handled in reconnect(), based on
    // the character arrays calculated above.
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}

// Handle MQTT message receipt
void callback(char* topic, byte* payload, unsigned int length) {

    // Convert topic and messgae to C++ String types, for ease of handling
    String payloadString;
    for (int i = 0; i < length; i++) {
        payloadString += String((char)payload[i]);
    }
    String topicString;
    for (int i = 0; i < strlen(topic); i++) {
        topicString += String((char)topic[i]);
    }
    // Debug: print the (processed) received message to serial
    Serial.print("Message arrived on [");
    Serial.print(topicString);
    Serial.print("] : ");
    Serial.println(payloadString);

    // Let's start throwing Strings around
    // Find the index position of the first /, if any
    int pieceEnd = topicString.indexOf('/');

    // Now loop through the String and chunk it up
    while (pieceEnd != -1) {
        // Select the string from start to first '/', and output
        String subString = topicString.substring(0, pieceEnd);
        Serial.println(subString);
        // Now chop that part off. We'll do this destructively for now.
        topicString = topicString.substring(pieceEnd+1);
        // Find the next slash, if any
        pieceEnd = topicString.indexOf('/');
        // ...and loop
    }
    // ...and don't forget to print the last bit
    // (this will repeat if we have a topic with a closing slash.
    // But that will never happen. Right?)
    Serial.println(topicString);
    
}

