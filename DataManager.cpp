#include "DataManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

//================================================================================
// Helper to split string by delimiter
//DONE
vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}//=================================================================================

// --- Users ---
vector<User> DataManager::loadUsers() {
    vector<User> users;
    ifstream file("users.txt");
    string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            auto parts = split(line, ',');
            if (parts.size() >= 3) {
                User u;
                u.username = parts[0];
                u.password = parts[1];
                u.role = static_cast<UserRole>(stoi(parts[2]));
                users.push_back(u);
            }
        }
        file.close();
    } else {
        // Default admin if no file
        users.push_back({"admin", "admin123", UserRole::ADMIN});
        users.push_back({"doc", "1234", UserRole::DOCTOR});
        users.push_back({"pharm", "1234", UserRole::PHARMACIST});
        users.push_back({"bill", "1234", UserRole::BILLING});
        saveUsers(users); // Create the file
    }
    return users;
}

void DataManager::saveUsers(const vector<User>& users) {
    ofstream file("users.txt");
    for (const auto& u : users) {
        file << u.username << "," << u.password << "," << static_cast<int>(u.role) << "\n";
    }
    file.close();
}

// --- Drugs ---
vector<Drug> DataManager::loadDrugs() {
    vector<Drug> drugs;
    ifstream file("drugs.txt");
    string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            auto parts = split(line, ',');
            if (parts.size() >= 5) {
                Drug d;
                d.name = parts[0];
                d.price = stof(parts[1]);
                d.quantity = stoi(parts[2]);
                d.expiryDate = parts[3];
                d.minThreshold = stoi(parts[4]);
                drugs.push_back(d);
            }
        }
        file.close();
    } else {
        // File doesn't exist, start with empty inventory
        saveDrugs(drugs);
    }
    return drugs;
} 

// Adds a new drug to the inventory if it does not already exist
bool DataManager::addDrug(const Drug& drug) {
    // Load current list of drugs
    auto drugs = loadDrugs();
    // Prevent duplicate drug names
    for (const auto& d : drugs) {
        if (d.name == drug.name) {
            return false;
        }
    }
    // Add new drug and persist
    drugs.push_back(drug);
    saveDrugs(drugs);
    return true;
}


void DataManager::saveDrugs(const vector<Drug>& drugs) {
    ofstream file("drugs.txt");
    for (const auto& d : drugs) {
        file << d.name << "," << d.price << "," << d.quantity << "," << d.expiryDate << "," << d.minThreshold << "\n";
    }
    file.close();
}

// --- Prescriptions ---
vector<Prescription> DataManager::loadPrescriptions() {
    vector<Prescription> prescriptions;
    ifstream file("prescriptions.txt");
    string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            auto parts = split(line, ',');
            if (parts.size() >= 9) {
                Prescription p;
                p.id = stoi(parts[0]);
                p.doctorName = parts[1];
                p.patientName = parts[2];
                p.drugName = parts[3];
                p.quantity = stoi(parts[4]);
                p.date = parts[5];
                p.status = parts[6];
                p.diagnosis = parts[7];
                p.signature = parts[8];
                prescriptions.push_back(p);
            }
        }
        file.close();
    }
    return prescriptions;
}

void DataManager::savePrescriptions(const vector<Prescription>& prescriptions) {
    ofstream file("prescriptions.txt");
    for (const auto& p : prescriptions) {
        file << p.id << "," << p.doctorName << "," << p.patientName << "," 
             << p.drugName << "," << p.quantity << "," << p.date << "," 
             << p.status << "," << p.diagnosis << "," << p.signature << "\n";
    }
    file.close();
}

// --- Transactions ---
vector<Transaction> DataManager::loadTransactions() {
    vector<Transaction> transactions;
    ifstream file("transactions.txt");
    string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            auto parts = split(line, ',');
            if (parts.size() >= 6) {
                Transaction t;
                t.id = stoi(parts[0]);
                t.prescriptionId = stoi(parts[1]);
                t.amount = stof(parts[2]);
                t.date = parts[3];
                t.paymentMethod = parts[4];
                t.signature = parts[5];
                transactions.push_back(t);
            }
        }
        file.close();
    }
    return transactions;
}

void DataManager::saveTransactions(const vector<Transaction>& transactions) {
    ofstream file("transactions.txt");
    for (const auto& t : transactions) {
        file << t.id << "," << t.prescriptionId << "," << t.amount << "," 
             << t.date << "," << t.paymentMethod << "," << t.signature << "\n";
    }
    file.close();
}
