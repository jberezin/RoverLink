//Transceiver 1 Sketch:
//--------------------------------------------
//ESP32 Tranceiver 1 via ESP-NOW Protocol
// 
// Connect via USB to pc running Mission Planner from Ardupilot--Ardurover
//--------------------------------------------


#include <esp_now.h>
#include <WiFi.h>


//#define DEBUGGING 1

// Comment out the following line if this is for the mission planner rather than the rover
//#define ROVER 1
 
 
#ifdef ROVER
#define SerialPort Serial2   
#else
#define SerialPort Serial
#endif

 

//------------------------------------------------------------
#ifdef ROVER
//uint8_t RxMACaddress[] = {0x3C, 0x61, 0x05, 0x0D, 0x73, 0x48};  // Spain robot
uint8_t RxMACaddress[] = {0xC4, 0x4F, 0x33, 0x7F, 0xF6, 0x19};
#else
//uint8_t RxMACaddress[] = {0x3C, 0x61, 0x05, 0x0C, 0xC4, 0x14};  // Spain robot
uint8_t RxMACaddress[] = {0xAC, 0x67, 0xB2, 0x2A, 0xD7, 0x5C};
#endif
//------------------------------------------------------------
typedef struct TxStruct
{
  char buffer[255];
} TxStruct;
TxStruct sentData;
//------------------------------------------------------------
typedef struct RxStruct
{
  char buffer[255];
} RxStruct;
RxStruct receivedData;
//------------------------------------------------------------
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  int i;
  i=1;
#ifdef DEBUGGING  
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");  
#endif  
  
}
//------------------------------------------------------------
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len)
{ 
  int byteCount = len;
  memcpy(&receivedData, incomingData, byteCount);
  
  SerialPort.write(incomingData, byteCount);  // rover does not use USB
 
}
//======================================================================================
void setup()
{
#ifdef ROVER
  SerialPort.begin(115200, SERIAL_8N1, 26, 25);   // for ttgo tdisplay 2nd serial port because rover is using serial rather than usb to plug into pixhawk
#else  
  SerialPort.begin(115200);
#endif  

  //----------------------------------------------------------
  WiFi.mode(WIFI_STA);

#ifdef DEBUGGING
  Serial.print("ESP32 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
#endif
  
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  //----------------------------------------------------------
  esp_now_register_send_cb(OnDataSent);
  //----------------------------------------------------------
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, RxMACaddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  //----------------------------------------------------------
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
  //----------------------------------------------------------
  esp_now_register_recv_cb(OnDataRecv);
}
//======================================================================================
void loop()
{
  int input_count = 0;
  int bytes_read = 0;


  if (SerialPort.available()) {      // If anything comes in Serial (USB),

    input_count = SerialPort.available();
    if (input_count > 249)
    {
      input_count = 249;
    }

    bytes_read = SerialPort.readBytes(sentData.buffer, input_count);   // this is a problem because of the carriage returnb

    sentData.buffer[bytes_read] = 0; // safety for printing
    

    esp_err_t result = esp_now_send(RxMACaddress, (uint8_t *) &sentData.buffer, bytes_read);
#ifdef DEBUGGING
    SerialPort.write( (const uint8_t *)&sentData.buffer,bytes_read);
    if (result == ESP_OK)
    {
      Serial.println("Sent with success");
    }
    else
    {
      Serial.println("Error sending the data");
    }
#endif
  }
}
