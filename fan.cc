#include <thread>
#include <chrono>
#include <fstream>
#include <iostream>
#include <wiringPi.h>

#define FAN_THRESH 50 // Fan enable threshold in degrees C
#define FAN_MARGIN 4 // Switching margin
#define TEMPS_FILE "/sys/class/thermal/thermal_zone0/temp"
#define OUT_PIN 3 // GPIO22
#define FAN_ON 1
#define FAN_OFF 0

// Reads the contents of temp file
std::string get_temp() {
	std::ifstream f(TEMPS_FILE);

	std::string s;
	std::getline(f, s);
	f.close();
	
	return s;
}


int main() {
	wiringPiSetup();
	pinMode(OUT_PIN, OUTPUT);
	bool run = true;

	while(run) {
		// Get temp reading
		std::string result = get_temp();
		// Temp file logs in millidegrees
		uint32_t temp = strtol(result.c_str(), NULL, 10) / 1000;

		if(temp > FAN_THRESH + FAN_MARGIN) {
			digitalWrite(OUT_PIN, FAN_ON);
		}
		else if(temp < FAN_THRESH - FAN_MARGIN){
			digitalWrite(OUT_PIN, FAN_OFF);
		}
		
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
	return 0;
}
