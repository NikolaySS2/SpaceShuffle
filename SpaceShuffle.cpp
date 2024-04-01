#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<vector<string>> weatherConditions;

const int numberOfPorts = 5;

struct SpacePort {
    int perfectDay = 0;
    int lengthEquator = 0;
    int lowestTemps = 1;
    int maxTemps = 32;
    int maxWindSpeed = 11;
    int maxHumidity = 55;
    int maxPrecipitation = 0;
    string lightning = "No";
    string cloudsFirst = "Cumulus";
    string cloudsSecond = "Nimbus";
    string csvFileName;
    string name;

};

void CreateFile(vector<SpacePort> ports) {
    ofstream outputFile("LaunchAnalysisReport.csv");
    if (!outputFile) {
        cout << "Error creating file" << endl;
        return;
    }
    outputFile << "Port name,Date" << endl;

    for (size_t i = 0; i < numberOfPorts; i++) {
        outputFile << ports[i].name << ",";
        if (ports[i].perfectDay == 0) {
            outputFile << "No viable shuffle day" << endl;
        }
        else {
            outputFile << ports[i].perfectDay << endl;
        }
    }
    outputFile.close();
}

void GetWeatherConditions(const string fileName) {
    ifstream File(fileName);
    if (!File.is_open()) {
        cout << "Unable to open file" << endl;
        return;
    }
    string line;
    string word;
    while (getline(File, line)) {
        stringstream ss(line);
        vector<string> row;
        getline(ss, word, ',');
        while (getline(ss, word, ',')) {
            row.push_back(word);
        }
        weatherConditions.push_back(row);
    }
    File.close();
}

int GetAppropriateDays(SpacePort port) {
    const size_t numberOfDays = weatherConditions[0].size();

    for (size_t i = 0; i < numberOfDays; i++) {
        if (stoi(weatherConditions[1][i]) > port.lowestTemps &&
            stoi(weatherConditions[1][i]) < port.maxTemps &&
            stoi(weatherConditions[2][i]) < port.maxWindSpeed &&
            stoi(weatherConditions[3][i]) < port.maxHumidity &&
            stoi(weatherConditions[4][i]) == port.maxPrecipitation &&
            weatherConditions[5][i] == port.lightning &&
            weatherConditions[6][i] != port.cloudsFirst &&
            weatherConditions[6][i] != port.cloudsSecond) {
            return i + 1;
        }
    }
    return 0;
}

void GetBestDay(vector<SpacePort>& ports) {
    int day = 0;
    for (size_t i = 0; i < numberOfPorts;i++) {
        GetWeatherConditions(ports[i].csvFileName);
        day = GetAppropriateDays(ports[i]);
        ports[i].perfectDay = day;
        weatherConditions.clear();
    }
}

void GetPerfectDayAndLocation(vector<SpacePort> ports) {
    int port = -1;
    int currentMaxLength = 999999;
    for (size_t i = 0; i < numberOfPorts; i++) {
        if (ports[i].perfectDay != 0 && ports[i].lengthEquator <= currentMaxLength) {
            port = i;
            currentMaxLength = ports[i].lengthEquator;
        }
    }
    if (port != -1) {
        cout << "Perfect location and day:" << endl;
        cout << ports[port].name << "  day: " << ports[port].perfectDay;
    }
    else {
        cout << "There are no appropriate days";
    }
}



int main() {
    string portNames[numberOfPorts] = { "Kourou", "Tenegashima", "Cape Canaveral", "Mahia", "Kodiak" };
    int arr[numberOfPorts] = { 333, 1934, 1807, 2500, 4112 };
    vector<SpacePort> spacePorts(numberOfPorts);

    cout << "Enter file path" << endl;
    string fileName;
    getline(cin, fileName);

    string choice;
    cout << "Do you want to use default weather criteria? (Yes/No): ";
    cin >> choice;

    SpacePort port;
    for (size_t i = 0; i < numberOfPorts; i++) {
        port.name = portNames[i];
        port.csvFileName = fileName + "\\" + portNames[i] + ".csv";
        port.lengthEquator = arr[i];
        spacePorts[i] = port;
    }

    if (choice == "No") {
        int numberOfPortsToChange;
        while (true) {
            cout << "How many port's criteria do you want to change?" << endl;
            cin >> numberOfPortsToChange;

            if (numberOfPortsToChange > numberOfPorts && numberOfPortsToChange<0 && !numberOfPortsToChange) {
                cout << "Invalid input" << endl;
            }
            else {
                break;
            }
        }
       
            string portName;
            bool isValid = false;
            for (size_t j = 0; j < numberOfPortsToChange; j++) {
                cout << "Enter the name of the port to change criteria: ";
                cin >> portName;

                for (int i = 0; i < numberOfPorts; i++) {
                    if (portName == portNames[i]) {
                        cout << "Enter lowest temperature: ";
                        cin >> spacePorts[i].lowestTemps;

                        cout << "Enter maximum temperature: ";
                        cin >> spacePorts[i].maxTemps;

                        cout << "Enter maximum wind speed: ";
                        cin >> spacePorts[i].maxWindSpeed;

                        cout << "Enter maximum humidity: ";
                        cin >> spacePorts[i].maxHumidity;

                        cout << "Enter maximum precipitation: ";
                        cin >> spacePorts[i].maxPrecipitation;

                        cout << "Enter lightning (Yes/No): ";
                        cin >> spacePorts[i].lightning;

                        cout << "Enter first cloud condition: ";
                        cin >> spacePorts[i].cloudsFirst;

                        cout << "Enter second cloud condition: ";
                        cin >> spacePorts[i].cloudsSecond;

                        isValid = true;
                        break;
                    }
                }
                if (!isValid) {
                    cout << "Invalid port name" << endl;
                    return 0;
                }
            }      
    }

    GetBestDay(spacePorts);
    CreateFile(spacePorts);
    GetPerfectDayAndLocation(spacePorts);

    return 0;
}
