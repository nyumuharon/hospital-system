#include <iostream>
#include <limits>
#include "HospitalController.h"
#include "Utils.h"
#include "SimpleWebServer.h"

using namespace std;

HospitalController systemController;

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// --- Menus ---

void adminMenu() {
    int choice;
    do {
        cout << "\n=== ADMIN DASHBOARD ===\n";
        cout << "1. Add User\n";
        cout << "2. Generate System Report\n";
        cout << "3. Check Epidemic Trends (AI)\n";
        cout << "4. Logout\n";
        cout << "Choice: ";
        if (!(cin >> choice)) { clearInput(); continue; }

        switch (choice) {
            case 1: {
                string u, p;
                int r;
                cout << "Username: "; cin >> u;
                cout << "Password: "; cin >> p;
                cout << "Role (0:Admin, 1:Doctor, 2:Pharmacist, 3:Billing): "; cin >> r;
                systemController.addUser({u, p, static_cast<UserRole>(r)});
                cout << "User added.\n";
                break;
            }
            case 2: systemController.generateReport(); break;
            case 3: systemController.checkEpidemicTrends(); break;
            case 4: systemController.logout(); break;
        }
    } while (systemController.getCurrentUser() != nullptr);
}

void doctorMenu() {
    int choice;
    do {
        cout << "\n=== DOCTOR DASHBOARD ===\n";
        cout << "1. View Drugs (Inventory)\n";
        cout << "2. Create Prescription\n";
        cout << "3. Logout\n";
        cout << "Choice: ";
        if (!(cin >> choice)) { clearInput(); continue; }

        switch (choice) {
            case 1: {
                auto& drugs = systemController.getDrugs();
                for (const auto& d : drugs) {
                    cout << d.name << " - Stock: " << d.quantity << " - Price: " << d.price << "\n";
                }
                break;
            }
            case 2: {
                Prescription p;
                p.id = rand() % 10000; // Simple ID generation
                p.doctorName = systemController.getCurrentUser()->username;
                cout << "Patient Name: "; cin >> ws; getline(cin, p.patientName);
                cout << "Drug Name: "; cin >> ws; getline(cin, p.drugName);
                
                Drug* d = systemController.findDrug(p.drugName);
                if (!d) {
                    cout << "Drug not found!\n";
                    break;
                }
                if (d->quantity <= 0) {
                    cout << "Drug is out of stock!\n";
                    break;
                }

                cout << "Quantity: "; cin >> p.quantity;
                cout << "Diagnosis (for AI): "; cin >> ws; getline(cin, p.diagnosis);
                p.date = Utils::getCurrentDate();
                p.status = "pending";
                
                systemController.createPrescription(p);
                break;
            }
            case 3: systemController.logout(); break;
        }
    } while (systemController.getCurrentUser() != nullptr);
}

void pharmacistMenu() {
    int choice;
    do {
        cout << "\n=== PHARMACIST DASHBOARD ===\n";
        cout << "1. View Pending Prescriptions\n";
        cout << "2. Dispense Prescription\n";
        cout << "3. Check Low Stock\n";
        cout << "4. Check Expiry\n";
        cout << "5. Add New Drug Stock\n";
        cout << "6. Logout\n";
        cout << "Choice: ";
        if (!(cin >> choice)) { clearInput(); continue; }

        switch (choice) {
            case 1: {
                auto pending = systemController.getPendingPrescriptions();
                if (pending.empty()) cout << "No pending prescriptions.\n";
                for (const auto& p : pending) {
                    cout << "ID: " << p.id << " | Patient: " << p.patientName << " | Drug: " << p.drugName << " (" << p.quantity << ")\n";
                }
                break;
            }
            case 2: {
                int id;
                cout << "Enter Prescription ID: "; cin >> id;
                systemController.dispensePrescription(id);
                break;
            }
            case 3: systemController.checkLowStock(); break;
            case 4: systemController.checkExpiry(); break;
            case 5: {
                Drug d;
                cout << "Name: "; cin >> ws; getline(cin, d.name);
                cout << "Price: "; cin >> d.price;
                cout << "Quantity: "; cin >> d.quantity;
                cout << "Expiry (DD/MM/YYYY): "; cin >> d.expiryDate;
                cout << "Min Threshold: "; cin >> d.minThreshold;
                systemController.addDrug(d);
                cout << "Drug added.\n";
                break;
            }
            case 6: systemController.logout(); break;
        }
    } while (systemController.getCurrentUser() != nullptr);
}

void billingMenu() {
    int choice;
    do {
        cout << "\n=== BILLING DASHBOARD ===\n";
        cout << "1. Process Payment\n";
        cout << "2. View Daily Revenue\n";
        cout << "3. Logout\n";
        cout << "Choice: ";
        if (!(cin >> choice)) { clearInput(); continue; }

        switch (choice) {
            case 1: {
                int id;
                string method;
                cout << "Enter Transaction ID (from Bill): "; cin >> id;
                cout << "Payment Method (Cash/Insurance/Mobile): "; cin >> method;
                systemController.processPayment(id, method);
                break;
            }
            case 2: cout << "Daily Revenue: KES " << systemController.getDailyRevenue() << "\n"; break;
            case 3: systemController.logout(); break;
        }
    } while (systemController.getCurrentUser() != nullptr);
}

int main() {
    cout << "Starting AI-Powered Hospital Workflow System...\n";
    
    int mode;
    cout << "Select Mode:\n";
    cout << "1. Console Application\n";
    cout << "2. Web Server (Port 8080)\n";
    cout << "Choice: ";
    cin >> mode;

    if (mode == 2) {
        SimpleWebServer server(&systemController, 8080);
        server.start();
        return 0;
    }

    while (true) {
        if (systemController.getCurrentUser() == nullptr) {
            string u, p;
            cout << "\n=== LOGIN ===\n";
            cout << "Enter Username (or 'exit' to quit): "; cin >> u;
            if (u == "exit") break;
            
            cout << "Password: "; cin >> p;
            
            if (systemController.login(u, p)) {
                cout << "Login Successful! Role: " << systemController.getCurrentUser()->getRoleString() << "\n";
            } else {
                cout << "Invalid credentials.\n";
            }
        } else {
            switch (systemController.getCurrentUser()->role) {
                case UserRole::ADMIN: adminMenu(); break;
                case UserRole::DOCTOR: doctorMenu(); break;
                case UserRole::PHARMACIST: pharmacistMenu(); break;
                case UserRole::BILLING: billingMenu(); break;
            }
        }
    }
    return 0;
}

