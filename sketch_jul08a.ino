#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// Cập nhật thông tin
// Thông tin về wifi
#define ssid "QUOC KHANH"
#define password "02092012"
// Thông tin về MQTT Broker
#define mqtt_server "m14.cloudmqtt.com" // Thay bằng thông tin của bạn
//#define mqtt_topic_pub "Pi/LEDStatus"   //Giữ nguyên nếu bạn tạo topic tên là demo
//#define mqtt_topic_sub "Pi/LEDStatus"
#define mqtt_user "mrxlkalf"    //Giữ nguyên nếu bạn tạo user là esp8266 và pass là 123456
#define mqtt_pwd "_r5_TObeJpaA"

const uint16_t mqtt_port = 15229; //Port của CloudMQTT
const byte ledPin = D2;
const byte ledPin2=D7;

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port); 
  client.setCallback(callback);
  pinMode(ledPin, OUTPUT);
}
// Hàm kết nối wifi
void setup_wifi() {
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
// Hàm call back để nhận dữ liệu
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print(". Message: ");
  //String receivedChar;
  if(strcmp(topic,"Pi/LEDStatus")==0)
  {
    for (int i = 0; i < length; i++) {
    
   
     char receivedChar = (char)payload[i];
     Serial.print(receivedChar);
    
    
    if (receivedChar == '1')
      // Kiểm tra nếu tin nhận được là 1 thì bật LED và ngược lại
      digitalWrite(ledPin, HIGH);
    if (receivedChar == '0')
      digitalWrite(ledPin, LOW);
    
      
      
  }
  }

  if(strcmp(topic,"room/lamp")==0)
  {
    for (int i = 0; i < length; i++) {
    
   
     char receivedChar = (char)payload[i];
     Serial.print(receivedChar);
    
    
    if (receivedChar == '1')
      // Kiểm tra nếu tin nhận được là 1 thì bật LED và ngược lại
      digitalWrite(ledPin2, HIGH);
    if (receivedChar == '0')
      digitalWrite(ledPin2, LOW);
    
      
      
  }
  }

  
   Serial.println();
}

  
  
  
 

// Hàm reconnect thực hiện kết nối lại khi mất kết nối với MQTT Broker
void reconnect() {
  // Chờ tới khi kết nối
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Thực hiện kết nối với mqtt user và pass
    if (client.connect("ESP8266Client",mqtt_user, mqtt_pwd)) {
      Serial.println("connected");
      // Khi kết nối sẽ publish thông báo
      //client.publish(mqtt_topic_pub, "ESP_reconnected");
      // ... và nhận lại thông tin này
      //client.subscribe(mqtt_topic_sub);
      client.subscribe("room/lamp");
      client.subscribe("Pi/LEDStatus");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Đợi 5s
      delay(5000);
    }
  }
}
void loop() {
  // Kiểm tra kết nối
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
