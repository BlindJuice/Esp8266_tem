#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <SparkFun_SHTC3.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
 
// #include <AsyncUDP.h> //引用以使用异步UDP
SHTC3 mySHTC3;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

const char *ssid = "EVINF";
const char *password = "Yueqian0205";
WiFiUDP udp;					  //创建UDP对象
unsigned int localUdpPort = 2333; //本地端口号
IPAddress SendIP(172,16,255,255);

void setup(void)
{
	Serial.begin(115200); //设置串口波特率
	while (Serial == false)
	{
	}; //等待串行连接启动

	u8g2.begin();
	Wire.begin();	 //初始化Wire（IIC）库
	mySHTC3.begin(); //开

	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	while (!WiFi.isConnected())
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("Connected");
	Serial.print("IP Address:");
	Serial.println(WiFi.localIP());
	udp.begin(localUdpPort); //启用UDP监听以接收数据
}

void loop(void)
{
	char tt[7]; 
	char rhh[7]; 
	mySHTC3.update();

	sprintf(tt, " %2.1f", mySHTC3.toDegC());
	sprintf(rhh, " H1%2.1f", mySHTC3.toPercent());	 	// H1 湿度
	delay(2000);
	udp.beginPacket(SendIP, 2000); 
	udp.print("03");						//udp 设备ID
	udp.print(mySHTC3.toDegC()); 			//udp 发送温度
	udp.print(mySHTC3.toPercent());			//udp 发送湿度
	udp.endPacket(); 

	Serial.print( "00" ); //向串口打印温度数据
	Serial.print(mySHTC3.toDegC()); 
	Serial.print(mySHTC3.toPercent());  
	
	 
}
