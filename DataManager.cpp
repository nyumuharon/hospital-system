#include "DataManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Helper to split string by delimiter
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// --- Users ---
std::vector<User> DataManager::loadUsers() {
    std::vector<User> users;
    std::ifstream file("users.txt");
    std::string line;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            auto parts = split(line, ',');
            if (parts.size() >= 3) {
                User u;
                u.username = parts[0];
                u.password = parts[1];
                u.role = static_cast<UserRole>(std::stoi(parts[2]));
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

void DataManager::saveUsers(const std::vector<User>& users) {
    std::ofstream file("users.txt");
    for (const auto& u : users) {
        file << u.username << "," << u.password << "," << static_cast<int>(u.role) << "\n";
    }
    file.close();
}

// --- Drugs ---
std::vector<Drug> DataManager::loadDrugs() {
    std::vector<Drug> drugs;
    std::ifstream file("drugs.txt");
    std::string line;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            auto parts = split(line, ',');
            if (parts.size() >= 5) {
                Drug d;
                d.name = parts[0];
                d.price = std::stof(parts[1]);
                d.quantity = std::stoi(parts[2]);
                d.expiryDate = parts[3];
                d.minThreshold = std::stoi(parts[4]);
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


void DataManager::saveDrugs(const std::vector<Drug>& drugs) {
    std::ofstream file("drugs.txt");
    for (const auto& d : drugs) {
        file << d.name << "," << d.price << "," << d.quantity << "," << d.expiryDate << "," << d.minThreshold << "\n";
    }
    file.close();
}

// --- Prescriptions ---
std::vector<Prescription> DataManager::loadPrescriptions() {
    std::vector<Prescription> prescriptions;
    std::ifstream file("prescriptions.txt");
    std::string line;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            auto parts = split(line, ',');
            if (parts.size() >= 9) {
                Prescription p;
                p.id = std::stoi(parts[0]);
                p.doctorName = parts[1];
                p.patientName = parts[2];
                p.drugName = parts[3];
                p.quantity = std::stoi(parts[4]);
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

void DataManager::savePrescriptions(const std::vector<Prescription>& prescriptions) {
    std::ofstream file("prescriptions.txt");
    for (const auto& p : prescriptions) {
        file << p.id << "," << p.doctorName << "," << p.patientName << "," 
             << p.drugName << "," << p.quantity << "," << p.date << "," 
             << p.status << "," << p.diagnosis << "," << p.signature << "\n";
    }
    file.close();
}

// --- Transactions ---
std::vector<Transaction> DataManager::loadTransactions() {
    std::vector<Transaction> transactions;
    std::ifstream file("transactions.txt");
    std::string line;
    if (file.is_open()) {
        while (std::getline(file, line)) {
            auto parts = split(line, ',');
            if (parts.size() >= 6) {
                Transaction t;
                t.id = std::stoi(parts[0]);
                t.prescriptionId = std::stoi(parts[1]);
                t.amount = std::stof(parts[2]);
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

void DataManager::saveTransactions(const std::vector<Transaction>& transactions) {
    std::ofstream file("transactions.txt");
    for (const auto& t : transactions) {
        file << t.id << "," << t.prescriptionId << "," << t.amount << "," 
             << t.date << "," << t.paymentMethod << "," << t.signature << "\n";
    }
    file.close();
}
