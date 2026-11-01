// Keshav Yanamandra
// COMSC-210-5293, Fall 2026
// Lab 30

#include <iostream>
#include <map>
#include <array>
#include <list>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct Flight {
    string flightNumber;
    string destination;
    int scheduledTime;
    int delay;
    string status;
};

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
int timeToMinutes(string time);
bool showData(map<string, array<list<Flight>, 3> > &airportData);


int main() {

    // Create the map.
    // Each map key is an airport zone.
    // The array contains 3 lists:
    // 0 = scheduled flights
    // 1 = delayed flights
    // 2 = departed flights

    map<string, array<list<Flight>, 3> > airportData;

    // For the flight data:
    // ontime goes into list 0
    // delayed goes into list 1
    // cancelled goes into list 2
    //
    // Each list stores a string like:
    // AA326 to PHL 00:24

    // Open the flight data file.
    ifstream inputFile("flights.txt");

    if (!inputFile) {
        cout << "Error opening flights.txt" << endl;
        return 1;
    }

    string line;

    // Read each flight from the file.
    while (getline(inputFile, line)) {

        istringstream ss(line);

        string terminal;
        string flightNumber;
        string destination;
        string departureTime;
        string delay;
        string status;

        getline(ss, terminal, ',');
        getline(ss, flightNumber, ',');
        getline(ss, destination, ',');
        getline(ss, departureTime, ',');
        getline(ss, delay, ',');
        getline(ss, status, ',');

        Flight flight;

        flight.flightNumber = flightNumber;
        flight.destination = destination;
        flight.scheduledTime = timeToMinutes(departureTime);

        istringstream delayStream(delay);
        delayStream >> flight.delay;

        flight.status = status;

        // All flights start in the scheduled list.
        airportData[terminal][0].push_back(flight);
    }
    inputFile.close();

    showData(airportData);

    return 0;
}

bool showData(map<string, array<list<Flight>, 3> > &airportData) {

    if (airportData.empty()) {
        cout << "No airport data." << endl;
        return false;
    }

    for (map<string, array<list<Flight>, 3> >::iterator it = airportData.begin();
         it != airportData.end(); ++it) {

        cout << endl << it->first << endl;

        cout << "Scheduled flights:" << endl;
        for (list<Flight>::iterator flight = it->second[0].begin();
             flight != it->second[0].end(); ++flight) {

            cout << "  " << flight->flightNumber
                 << " to " << flight->destination << endl;
        }

        cout << "Delayed flights: "
             << it->second[1].size() << endl;

        cout << "Departed flights: "
             << it->second[2].size() << endl;
    }

    return true;
}

int timeToMinutes(string time) {
    istringstream ss(time);

    int hour;
    int minute;
    char colon;

    ss >> hour >> colon >> minute;

    return hour * 60 + minute;
}