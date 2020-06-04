all:
	g++ -Wall -Wextra -O3 -o fanctrl fan.cc -pthread -lwiringPi -lrt -lcrypt

clean:
	rm -rf fanctrl
