CC=g++
CFLAGS=-std=c++17

INCLUDECADMIUM=-I ../../cadmium/include
INCLUDEDESTIMES=-I ../../DESTimes/include
INCLUDEJSON=-I ../../CadmiumModelJSONExporter/include

# CREATE BIN AND BUILD FOLDERS TO SAVE THE COMPILED FILES DURING RUNTIME
bin_folder := $(shell mkdir -p bin)
build_folder := $(shell mkdir -p build)
results_folder := $(shell mkdir -p simulation_results)

# TARGET TO COMPILE ALL THE TESTS TOGETHER (NOT SIMULATOR)

# === COMMON ===
message.o: data_structures/message.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) data_structures/message.cpp -o build/message.o

engagementUtils.o: usrlib/engagementUtils.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) usrlib/engagementUtils.cpp -o build/engagementUtils.o

# === TESTS ===
aircraft_test.o: test/aircraft_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/aircraft_test.cpp -o build/aircraft_test.o

radar_test.o: test/radar_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/radar_test.cpp -o build/radar_test.o

gunner_test.o: test/gunner_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/gunner_test.cpp -o build/gunner_test.o

missile_test.o: test/missile_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/missile_test.cpp -o build/missile_test.o

ads_test.o: test/ads_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/ads_test.cpp -o build/ads_test.o

tests: ads_test.o aircraft_test.o gunner_test.o radar_test.o missile_test.o message.o engagementUtils.o
		$(CC) -g -o bin/AIRCRAFT_TEST build/aircraft_test.o build/message.o build/engagementUtils.o
		$(CC) -g -o bin/RADAR_TEST build/radar_test.o build/message.o build/engagementUtils.o
		$(CC) -g -o bin/GUNNER_TEST build/gunner_test.o build/message.o build/engagementUtils.o
		$(CC) -g -o bin/MISSILE_TEST build/missile_test.o build/message.o build/engagementUtils.o
		$(CC) -g -o bin/ADS_TEST build/ads_test.o build/message.o build/engagementUtils.o

# === TOP MODEL ===
radar_MANPADS_engagement.o: model/radar_MANPADS_engagement.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) $(INCLUDEJSON) model/radar_MANPADS_engagement.cpp -o build/radar_MANPADS_engagement.o
	
# TARGET TO COMPILE ONLY SIMULATOR
simulator: radar_MANPADS_engagement.o message.o engagementUtils.o
	$(CC) -g -o bin/RADAR_MANPADS_ENGAGEMENT build/radar_MANPADS_engagement.o build/message.o build/engagementUtils.o
	
# TARGET TO COMPILE EVERYTHING (SIMULATOR + TESTS TOGETHER)
all: simulator tests

#CLEAN COMMANDS
clean: 
	rm -f bin/* build/*
