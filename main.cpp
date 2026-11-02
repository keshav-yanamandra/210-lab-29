// Keshav Yanamandra
// COMSC-210-5293, Fall 2026
// Lab 31

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

    // All flights start in list 0 as scheduled flights.
    // The flight status will determine what happens to each flight later.
    // Each list stores Flight records with the flight information.

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

    for (int period = 0; period < 30; period++) {

        int windowStart = 360 + (period * 30);
        int windowEnd = windowStart + 30;

        cout << endl;
        cout << "Period " << period + 1  << " (" << windowStart << " to " << windowEnd << ")" << endl;

        for (map<string, array<list<Flight>, 3> >::iterator terminal = airportData.begin();
            terminal != airportData.end(); ++terminal) {

            list<Flight> &scheduled = terminal->second[0];
            list<Flight> &delayed = terminal->second[1];
            list<Flight> &departed = terminal->second[2];

            // Check scheduled flights.
            for (list<Flight>::iterator it = scheduled.begin();
                it != scheduled.end();) {

                if (it->scheduledTime < windowEnd) {

                    if (it->status == "cancelled") {
                        it = scheduled.erase(it);
                    }
                    else if (it->delay < 15) {
                        departed.push_back(*it);
                        it = scheduled.erase(it);
                    }
                    else {
                        delayed.push_back(*it);
                        it = scheduled.erase(it);
                    }
                }
                else {
                    ++it;
                }
            }

            // Check flights that are currently delayed.
            for (list<Flight>::iterator it = delayed.begin();
                it != delayed.end();) {

                if (it->scheduledTime + it->delay < windowEnd) {
                    departed.push_back(*it);
                    it = delayed.erase(it);
                }
                else {
                    ++it;
                }
            }
        }

        int scheduledTotal = 0;
        int delayedTotal = 0;
        int departedTotal = 0;

        for (map<string, array<list<Flight>, 3> >::iterator terminal = airportData.begin();
             terminal != airportData.end(); ++terminal) {

            scheduledTotal += terminal->second[0].size();
            delayedTotal += terminal->second[1].size();
            departedTotal += terminal->second[2].size();
             }

        cout << "Scheduled: " << scheduledTotal << " Delayed: " << delayedTotal << " Departed: " << departedTotal << endl;
        


    }

    cout << endl;
    cout << "Final state:" << endl;
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

        cout << "Scheduled flights: " << it->second[0].size() << endl;

        for (list<Flight>::iterator flight = it->second[0].begin();
            flight != it->second[0].end(); ++flight) {

                cout << "  " << flight->flightNumber << " to " << flight->destination << endl;
        }

        cout << "Delayed flights: "
             << it->second[1].size() << endl;

        for (list<Flight>::iterator flight = it->second[1].begin();
            flight != it->second[1].end(); ++flight) {

                cout << "  " << flight->flightNumber << " to " << flight->destination << endl;
        }

        cout << "Departed flights: "
             << it->second[2].size() << endl;

        for (list<Flight>::iterator flight = it->second[2].begin();
            flight != it->second[2].end(); ++flight) {

            cout << "  " << flight->flightNumber
                << " to " << flight->destination << endl;
        }
    }

    return true;
}

int timeToMinutes(string time) {
    istringstream ss(time);

    int hour = 0;
    int minute = 0;
    char colon;

    if (!(ss >> hour >> colon >> minute)) {
        return 0;
    }

    return hour * 60 + minute;
}