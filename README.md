# Hospital System AI

A comprehensive Hospital Management System built in C++ featuring a custom, lightweight HTTP server and a modern web interface.

## Features

*   **Core C++ Backend**: Efficient management of users, inventory, prescriptions, and transactions.
*   **Custom Web Server**: Built from scratch using Winsock, requiring no external web server dependencies (Apache/Nginx).
*   **Web Dashboard**: A responsive, dark-themed web interface for easy access to hospital data.
*   **Role-Based Access Control (RBAC)**:
    *   **Admin**: Full access to Inventory, Triage, and Reports.
    *   **Doctor**: Access to Triage Assessment and Inventory.
    *   **Pharmacist**: Access to Drug Inventory.
    *   **Billing**: Access to Financial Reports.
*   **Triage Assessment**: Dedicated module for recording vital signs and anthropometric measurements with automatic BMI calculation.
*   **Digital Signatures**: Secure signing of prescriptions and transactions.

## Tech Stack

*   **Backend**: C++ (Standard Library, Winsock2, Windows CryptoAPI)
*   **Frontend**: HTML5, CSS3 (Dark Mode), JavaScript (Vanilla)
*   **Data Persistence**: File-based storage (Text files)

## How to Run

1.  Compile the project:
    ```bash
    g++ main.cpp HospitalController.cpp DataManager.cpp DigitalSignatureManager.cpp SimpleWebServer.cpp -o HospitalSystem.exe -lws2_32 -lcrypt32
    ```
2.  Run the executable:
    ```bash
    .\HospitalSystem.exe
    ```
3.  Select **Mode 2** to start the Web Server.
4.  Open your browser and navigate to `http://localhost:8080`.
