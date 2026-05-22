#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <fstream>
using namespace std;
const double  G_ACC = 9.81; // acceleration due to gravity
const double PI = 3.14159265358979323846;  // pi is taken to 20 digits to maintain accuracy for industry standards

struct Flight_kinematics {
    double totalFlightTime;
    double maxHorizontalRange; // distance on xy - plane of graph - ie - distance on ground
    double altitude;        //cruising altitude

};


// Function to calculate 3D flight metrics
Flight_kinematics ground(double v, double pitch_r, double yaw_r) {
    Flight_kinematics metrics;
    
    // Time of flight
    double v_z = v *sin(pitch_r);   //Vertical v
    metrics.totalFlightTime = (2.0 * v_z) /  G_ACC;
    
    // Max altitude
    metrics.altitude =   pow(v_z,2) / (2.0 * G_ACC);
    
    // Distance travelled
    double v_ground = v *   cos(pitch_r);
    metrics.maxHorizontalRange = v_ground * metrics.totalFlightTime;
    

    
    return metrics;
}

int main() {
    double v;   //final v
    double pitch_deg; // Pitch angle
    double yaw_deg;   // Heading angle
    int temp = 10;    //intervals for coordinate system

      cout << "=== 3D Flight Path Calculator ===" << endl;
    
      cout << "Takeoff speed (m/s): ";
    if (!(  cin >> v) || v <= 0) {
          cerr << "Invalid v.\n";
        return 1;
    }

      cout << "Kindly note: A pitch angle beyond critical AoA will result in a stall:"<<endl;
      cout << "Enter Pitch angle (0 to 90): ";
    if (!(  cin >> pitch_deg) || pitch_deg < 0 || pitch_deg > 90) {
          cerr << "Invalid pitch angle";
        return 1;
    }

      cout << "Enter Yaw angle (0 to 360): ";
    if (!(  cin >> yaw_deg) || yaw_deg < 0 || yaw_deg > 360) {
          cerr << "Invalid yaw angle";
        return 1;
    }

    //angles to radians
    double pitch_r = pitch_deg * (PI / 180.0);
    double yaw_r = yaw_deg * (PI / 180.0);

    // Calculate flight metrics
    Flight_kinematics metrics = ground(v, pitch_r, yaw_r);

      cout << "=== Flight Details ===\n";
      fixed(  cout);
      setprecision(2);
      cout << "Total Time of Flight   : " << metrics.totalFlightTime << "s"<<endl;
      cout << "Max Altitude (Z)       : " << metrics.altitude << "m"<<endl;
      cout << "Total Ground Distance  : " << metrics.maxHorizontalRange << "m"<<endl;

      cout << "--------------------------------------------------"<<endl;

    // Generate 3D Coordinate Table
      cout << "COORDINATE SYSTEM FOR FLIGHT - FOR GRAPHICAL ANALYSIS" <<endl;
      cout <<   setw(8) << "Time(s)" 
              <<   setw(14) << "X" 
              <<   setw(14) << "Y" 
              <<   setw(14) << "Z\n";
      cout << "---------------------------------------------------------\n"<<endl;

    double timeStep = metrics.totalFlightTime / temp;  // to define a coordinate system with respect to time

    // Calculate v vectors
    double vx = v *   cos(pitch_r) *   cos(yaw_r);
    double vy = v *   cos(pitch_r) *   sin(yaw_r);
    double vz = v *   sin(pitch_r);
      vector<double> arr_time;
      vector<double> arr_x;
      vector<double> arr_y;

    for (int i = 0; i <= temp; ++i) {
        double t = timeStep * i;
        if (t > metrics.totalFlightTime) t = metrics.totalFlightTime;

        // 3D Kinematic Equations
        double x = vx * t;
        double vy_iter = vy * t; // yaw shift speed
        double z = (vz * t) - (0.5 *  G_ACC *   pow(t, 2)); //weight against lift by nlm



        if (z < 0.0) z = 0.0;

          cout <<   setw(8) << t 
                  <<   setw(14) << x 
                  <<   setw(14) << vy_iter 
                  <<   setw(14) << z << "\n";
                  //write data to array for loading in dat file
                  arr_time.push_back(t);
                  arr_x.push_back(x);
                  arr_y.push_back(vy_iter);

  
                 
    }
    fstream fout;
    fout.open("time.bin", ios::out | ios::binary);
    if (fout){
        fout.write(reinterpret_cast<char*>(&arr_time), sizeof(double));
        //fout.write(reinterpret_cast<char*>(&arr_x), sizeof(arr_x));
        //fout.write(reinterpret_cast<char*>(&arr_y), sizeof(arr_y));

        fout.close();

     }
     fout.open("x.bin", ios::out| ios::binary );
     if (fout){
      fout.write(reinterpret_cast<char*>(&arr_x), sizeof(double));
     fout.close();
     }
     fout.open("y.bin", ios::in | ios::binary);
     if(fout){
      fout.write(reinterpret_cast<char*>(&arr_y), sizeof(double));
     fout.close();
     }
    return 0;
}
