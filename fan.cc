#include <thread>
#include <chrono>
#include <fstream>
#include <iostream>
#include <wiringPi.h>

#define FAN_THRESH 50 // Fan enable threshold in degrees C
#define FAN_MARGIN 4 // Switching margin
#define POLLING_INTERVAL_SECS 3 // Polling interval in seconds
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
    bool is_on = false;

	while(run) {
		// Get temp reading
		std::string result = get_temp();
		// Temp file logs in millidegrees
		uint32_t temp = strtol(result.c_str(), NULL, 10) / 1000;

        // Log current status
        std::cout 
            << "Temperature: "  << temp 
            << " (fan is "      << (is_on ? "on" : "off") 
            << ")"              << std::endl;

        bool should_be_on = temp > FAN_THRESH + FAN_MARGIN;
        bool should_be_off = temp < FAN_THRESH - FAN_MARGIN;

		if(!is_on && should_be_on) {
            std::cout << "Turning on fan." << std::endl;
			digitalWrite(OUT_PIN, FAN_ON);
            is_on = true;
		}

		else if(is_on && should_be_off){
            std::cout << "Turning off fan." << std::endl;
			digitalWrite(OUT_PIN, FAN_OFF);
            is_on = false;
		}
		
		std::this_thread::sleep_for(std::chrono::seconds(POLLING_INTERVAL_SECS));
	}
	return 0;
}
