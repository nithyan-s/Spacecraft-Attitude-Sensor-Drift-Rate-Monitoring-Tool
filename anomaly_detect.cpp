// Program to analyze anomalies in the drift rate compensation of spacecraft telemetry

// Include the necessary header files
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm> 

using namespace std;

// Function to analyze telemetry drift rates and detect anomalies
void check_anamoly(string drift_file) {
    // Open the drift file for reading data and report files for writing anomalies
    ifstream in(drift_file);
    ofstream pitch("pitch_report.txt", ios::app);  // Appending to keep a continuous log
    ofstream roll("roll_report.txt", ios::app);
    ofstream yaw("yaw_report.txt", ios::app);

    // Check if files are successfully opened, otherwise exit
    if (!in) {
        cerr << "Unable to open drift file." << endl;
        exit(0);
    }
    if (!pitch) {
        cerr << "Unable to create pitch report file." << endl;
        exit(0);
    }
    if (!roll) {
        cerr << "Unable to create roll report file." << endl;
        exit(0);
    }
    if (!yaw) {
        cerr << "Unable to create yaw report file." << endl;
        exit(0);
    }

    // Containers to store the telemetry data for pitch, roll, and yaw
    vector<double> p_data;  // For storing pitch telemetry data
    vector<double> r_data;  // For storing roll telemetry data
    vector<double> y_data;  // For storing yaw telemetry data
    vector<int> time(3);    // For storing time in HH:MM:SS format

    // Skip the first line in the file (assuming it contains headers)
    string line;
    getline(in, line);

    // Process each line of telemetry data from the file
    while (getline(in, line)) {
        stringstream ss(line); // Convert each line to a string stream for easy extraction
        double p, r, y;        // Variables to store current pitch, roll, yaw values
        string fid;            // Additional field in data (file ID or identifier)

        // Extract time, telemetry values for pitch (p), roll (r), and yaw (y)
        ss >> time[0] >> time[1] >> time[2];  // Time is captured as HH MM SS
        ss >> fid >> p >> r >> y;

        // Flags to indicate if any anomalies are detected in the current data
        bool pDetected = false;
        bool rDetected = false;
        bool yDetected = false;    

        // For the first data point, initialize vectors and set status as "Normal"
        if (p_data.empty()) {
            p_data.push_back(p);
            r_data.push_back(r);
            y_data.push_back(y);

            // Log initial "Status Normal" for the first data entry
            pitch << time[0] << ":" << time[1] << ":" << time[2] << " <> Status Normal..." << endl;
            roll << time[0] << ":" << time[1] << ":" << time[2] << " <> Status Normal..." << endl;
            yaw << time[0] << ":" << time[1] << ":" << time[2] << " <> Status Normal..." << endl;
        } 
        else {
            // Push the new telemetry values into the respective vectors for further analysis
            p_data.push_back(p);
            r_data.push_back(r);
            y_data.push_back(y);

            // Anomaly 1: Constant Value (Data Frozen)
            // Detects if the current value is the same as the previous value
            if (p_data[p_data.size() - 2] == p) {
                pitch << "[ALERT] " << time[0] << ":" << time[1] << ":" << time[2] << " <> Data Frozen..." << endl;
                pDetected = true;
            }
            if (r_data[r_data.size() - 2] == r) {
                roll << "[ALERT] " << time[0] << ":" << time[1] << ":" << time[2] << " <> Data Frozen..." << endl;
                rDetected = true;
            }
            if (y_data[y_data.size() - 2] == y) {
                yaw << "[ALERT] " << time[0] << ":" << time[1] << ":" << time[2] << " <> Data Frozen..." << endl;
                yDetected = true;
            }

            // Anomaly 2: Sudden Drop (Glitch)
            // Detects a sudden drop where value becomes 0, and the previous value was significantly higher
            if (!pDetected && p == 0 && abs(p_data[p_data.size() - 2]) > 0.5) {  
                pitch << "[ALERT] " << time[0] << ":" << time[1] << ":" << time[2] << " <> Sudden Drop Detected..." << endl;
                pDetected = true;
            }
            if (!rDetected && r == 0 && abs(r_data[r_data.size() - 2]) > 0.5) {
                roll << "[ALERT] " << time[0] << ":" << time[1] << ":" << time[2] << " <> Sudden Drop Detected..." << endl;
                rDetected = true;
            }
            if (!yDetected && y == 0 && abs(y_data[y_data.size() - 2]) > 0.5) {
                yaw << "[ALERT] " << time[0] << ":" << time[1] << ":" << time[2] << " <> Sudden Drop Detected..." << endl;
                yDetected = true;
            }

            // Anomaly 3: Spikes (Deviation Beyond Threshold)
            // Detects if there is a large spike in the data, outside of allowed deviation (e.g., 0.5 threshold)
            if (!pDetected && abs(p - p_data[0]) > 0.5) {
                pitch << "[ALERT] " << time[0] << ":" << time[1] << ":" << time[2] << " <> Spike in Data Detected..." << endl;
            }
            if (!rDetected && abs(r - r_data[0]) > 0.5) {
                roll << "[ALERT] " << time[0] << ":" << time[1] << ":" << time[2] << " <> Spike in Data Detected..." << endl;
            }
            if (!yDetected && abs(y - y_data[0]) > 0.5) {
                yaw << "[ALERT] " << time[0] << ":" << time[1] << ":" << time[2] << " <> Spike in Data Detected..." << endl;
            }

            // If no anomaly was detected, log the status as normal
            if (!pDetected && !rDetected && !yDetected) {
                pitch << time[0] << ":" << time[1] << ":" << time[2] << " <> Status Normal..." << endl;
                roll << time[0] << ":" << time[1] << ":" << time[2] << " <> Status Normal..." << endl;
                yaw << time[0] << ":" << time[1] << ":" << time[2] << " <> Status Normal..." << endl;
            }
        }
    }

    // Close all open files after processing
    in.close();
    pitch.close();
    roll.close();
    yaw.close();
}

// Driver function to initiate the anomaly analysis
int main() {

    // User input for initialization date (used to locate corresponding drift file)
    string input_date = "";
    cout << "Enter the initialization date (DD:MMM:YY): ";
    getline(cin, input_date);

    // Formulate the drift file name based on the provided date
    string drift_file = input_date + "_drift.txt";
    ifstream file(drift_file);

    // Error handling in case the drift file cannot be opened
    if (!file.is_open()) {
        cerr << "Unable to open file: " << drift_file << endl;
        return 1;
    }

    // Call the function to analyze the drift data and generate anomaly reports
    check_anamoly(drift_file);

    // Close the file and confirm report generation
    file.close();
    cout << "....Respective Reports generated...\n....Terminating Program...." << endl;

    return 0;
}
