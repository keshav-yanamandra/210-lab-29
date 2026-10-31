// Keshav Yanamandra
// COMSC-210-5293, Fall 2026
// Lab 29

#include <iostream>
#include <map>
#include <array>
#include <list>
#include <string>

using namespace std;

// Function will display some of the airport data
void showData(map<string, array<list<string>, 3> > &airportData);

int main() {

    // Create the map.
    // Each map key is an airport zone.
    // The array contains 3 lists:
    // 0 = arriving flights
    // 1 = delayed or waiting flights
    // 2 = departing flights
    map<string, array<list<string>, 3> > airportData;

    // Add some dummy data for the mockup.
    // The real project will later read this data from a file.
    airportData["Terminal A"][0].push_back("Flight 101");

    // Call a function to prove the map can be passed and used.
    showData(airportData);

    // Later the simulation will:
    // read at least 100 lines from a file
    // display the initial airport state
    // run through 30 time periods
    // move flights between the three lists
    // display the updated state after time passes

    return 0;
}

void showData(map<string, array<list<string>, 3> > &airportData) {

    // Display one piece of data from the nested structure.
    cout << "Airport zone: Terminal A" << endl;
    cout << "Arriving flight: "
         << airportData["Terminal A"][0].front() << endl;
}