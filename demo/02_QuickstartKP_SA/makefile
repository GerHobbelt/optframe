all: fcore-kp

fcore-kp:
	echo "WILL JOIN FILES INTO COMPLETE EXAMPLE"
	./join.sh
	echo "WILL COMPILE"
	g++ -g -O3 --std=c++20 -I../../include  mainKP-fcore-ex.cpp -Wfatal-errors -o app_KP-fcore
	echo "WILL RUN"
	./app_KP-fcore

run:
	@echo "Running FCore (c++17)"
	./app_KP-fcore 
	
