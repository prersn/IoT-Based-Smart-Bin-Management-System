# IoT Based Smart Bin Management System

## Overview
This project aims to create a smart waste bin management system using IoT technology. It utilizes various sensors and microcontrollers to monitor the status of the waste bin and provide real-time feedback.

## Components Used
- ESP32 microcontroller
- Load cell sensor
- Laser sensor
- LEDs
- WiFi module

## Hardware 
-**Please refer to the circuit diagram from "cir_dia" file.**

## Code Files

## Library installations:
-**Please download all the libraries listed out in the "req_lib.txt" file before compiling the code**

### 1. Serial Monitor Output Code
- **File Name**: `serial_monitor_output.ino`
- **Description**: This code is used to monitor the system via the serial monitor. It provides real-time feedback on the weight of the waste bin, laser sensor status, and bin fullness.
- **How to Use**:
  1. Connect the ESP32 microcontroller to your computer via USB.
  2. Open the code file (`serial_monitor_output.ino`) in the Arduino IDE.
  3. Select the appropriate board and port from the Tools menu.
  4. Click on the "Upload" button to compile and upload the code to the microcontroller.
  5. Once uploaded, open the serial monitor to view the system output.

### 2. AWS Cloud Output Code
-**To set up aws policy and thing**: follow this video tutorial "https://youtu.be/idf-gGXvIu4?si=keX8djYWjrWSXsWa".
- **Folder Name**: `with_aws`
- **Folder contains**: `with_aws` and `header.h`
- **Description**: This code is used to connect the system to the AWS cloud for remote monitoring and data logging. It publishes weight data to AWS IoT for further analysis and management.
- **How to Use**:
  1. Open the code file (`aws_cloud_output.ino`) in the Arduino IDE.
  2. Ensure that the AWS IoT credentials and endpoints are correctly configured in the `header.h` file.
  3. Select the appropriate board and port from the Tools menu.
  4. Click on the "Upload" button to compile and upload the code to the microcontroller.

## Uploading Code to Microcontroller
To upload the code to the microcontroller:
1. Connect the ESP32 microcontroller to your computer via USB.
2. Open the code file (`*.ino`) in the Arduino IDE.
3. Select the appropriate board and port from the Tools menu.
4. Click on the "Upload" button to compile and upload the code to the microcontroller.
5. Once uploaded, open the serial monitor (if using serial monitor output code) to view the system feedback.

## Feedback
any feedback or suggestions for improving this project. Feel free to open an issue or pull request on GitHub.
