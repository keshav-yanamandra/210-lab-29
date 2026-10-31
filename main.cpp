// Keshav Yanamandra
// COMSC-210-5293, Fall 2026
// Lab 30

#include <iostream>
#include <map>
#include <array>
#include <list>
#include <string>

using namespace std;

/*
I used AI for data preperartion. 
The data was downloaded directly from transtats.bts.gov 
the file is 235 MB from January 2024 
then filtered to SFO departures on 2024-01-15.

Data source:
flight data comes from the US DOT Bureau of Transportation Statistics
On-Time Performance data from transtats.bts.gov. The file contains flights
departing SFO on January 15, 2024.

Each line has six fields:
terminal, flight number, destination, scheduled departure time,
delay in minutes, and status.
Sample:
Terminal A,AA326,PHL,00:24,7,ontime
Terminal C,UA1003,DEN,05:00,0,cancelled
Terminal C,UA274,IAH,05:14,0,ontime
Terminal A,AA2614,ORD,05:45,860,delayed
...

terminal field is made-up by AI because the BTS data does not publish
terminal information. all other flight information comes from the dataset.

this data works for the project because the real scheduled departure
times can be used for the time periods, and the real delays and cancellations
can be used for events instead of using random numbers.
*/
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