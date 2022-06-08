#include <Arduino.h>
#include <U8g2lib.h>		//IIC oled
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

int res = 0;
void setup(void)
{
	Serial.begin(115200); //设置串口波特率
	while (Serial == false)
	{
	}; //等待串行连接启动

	u8g2.begin();
	Wire.begin();	 //初始化Wire（IIC）库
	mySHTC3.begin(); //初始化SHTC
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
}
void loop(void)
{
	 
															//	连接WiFi

	while (!WiFi.isConnected())
	{
		delay(500);
		// Serial.print(".");
	}
	mySHTC3.update();			//更新温度传感器数据
	delay(200);					//延时后才能接收到的温度数据  ！！！！！！！！！！！！！
	udp.begin(localUdpPort); //启用UDP监听以接收数据
	udp.beginPacket(SendIP, 2000); 
//	delay(200);
	udp.print("03");						//udp 设备ID
	udp.print(mySHTC3.toDegC()); 			//udp 发送温度
	udp.print(mySHTC3.toPercent());			//udp 发送湿度
	udp.print("\r\n");
	udp.endPacket(); 
	//udp.stopAll();
	delay(200); 
	WiFi.forceSleepBegin();					//Wifi off
 	delay(60000);
 	WiFi.forceSleepWake(); 					//Wifi on
 	delay(100);
 	 
}
