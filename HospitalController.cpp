#include "HospitalController.h"
#include "Utils.h"
#include <iostream>
#include <algorithm>
#include <map>

HospitalController::HospitalController() : currentUser(nullptr) {
    if (!signatureManager.init()) {
        std::cerr << "Warning: Digital Signature Manager failed to initialize.\n";
    }
    users = DataManager::loadUsers();
    drugs = DataManager::loadDrugs();
    prescriptions = DataManager::loadPrescriptions();
    transactions = DataManager::loadTransactions();
}

HospitalController::~HospitalController() {
    DataManager::saveUsers(users);
    DataManager::saveDrugs(drugs);
    DataManager::savePrescriptions(prescriptions);
    DataManager::saveTransactions(transactions);
}

// --- User Management ---
bool HospitalController::login(const std::string& username, const std::string& password) {
    for (auto& u : users) {
        if (u.username == username && u.password == password) {
            currentUser = &u;
            return true;
        }
    }
    return false;
}

void HospitalController::logout() {
    currentUser = nullptr;
}

User* HospitalController::getCurrentUser() {
    return currentUser;
}

void HospitalController::addUser(const User& user) {
    users.push_back(user);
    DataManager::saveUsers(users);
}

// --- Inventory ---
void HospitalController::addDrug(const Drug& drug) {
    drugs.push_back(drug);
    DataManager::saveDrugs(drugs);
}

std::vector<Drug>& HospitalController::getDrugs() {
    return drugs;
}

Drug* HospitalController::findDrug(const std::string& name) {
    for (auto& d : drugs) {
        if (Utils::toLowerCase(d.name) == Utils::toLowerCase(name)) {
            return &d;
        }
    }
    return nullptr;
}

void HospitalController::checkLowStock() {
    std::cout << "\n--- Low Stock Alert ---\n";
    bool found = false;
    for (const auto& d : drugs) {
        if (d.isLowStock()) {
            std::cout << "WARNING: " << d.name << " is low (" << d.quantity << " left)\n";
            found = true;
        }
    }
    if (!found) std::cout << "All stock levels are healthy.\n";
}

void HospitalController::checkExpiry() {
    // Simplified expiry check (comparing strings for now, ideally use time_t)
    std::cout << "\n--- Expiry Check ---\n";
    std::string today = Utils::getCurrentDate();
    // In a real app, parse date and compare. Here we just list them.
    for (const auto& d : drugs) {
        std::cout << "Drug: " << d.name << " Expires: " << d.expiryDate << "\n";
    }
}

// --- Prescription ---
void HospitalController::createPrescription(const Prescription& p) {
    Prescription newP = p;
    // Sign the critical data: patientName + drugName + quantity + date
    std::string dataToSign = newP.patientName + newP.drugName + std::to_string(newP.quantity) + newP.date;
    newP.signature = signatureManager.signData(dataToSign);
    
    prescriptions.push_back(newP);
    DataManager::savePrescriptions(prescriptions);
    
    // Reserve stock logic could go here, but for now we deduct on dispense
    std::cout << "Prescription created for " << p.patientName << "\n";
}

std::vector<Prescription> HospitalController::getPendingPrescriptions() {
    std::vector<Prescription> pending;
    for (const auto& p : prescriptions) {
        if (p.status == "pending") {
            pending.push_back(p);
        }
    }
    return pending;
}

void HospitalController::dispensePrescription(int prescriptionId) {
    for (auto& p : prescriptions) {
        if (p.id == prescriptionId && p.status == "pending") {
            // Verify Signature
            std::string dataToVerify = p.patientName + p.drugName + std::to_string(p.quantity) + p.date;
            if (!signatureManager.verifySignature(dataToVerify, p.signature)) {
                std::cout << "SECURITY ALERT: Digital Signature Verification Failed! Prescription may be tampered.\n";
                return;
            }

            Drug* d = findDrug(p.drugName);
            if (d && d->quantity >= p.quantity) {
                d->quantity -= p.quantity;
                p.status = "dispensed";
                
                // Auto-generate bill
                generateBill(p.id);
                
                DataManager::saveDrugs(drugs);
                DataManager::savePrescriptions(prescriptions);
                std::cout << "Dispensed successfully. Bill generated.\n";
            } else {
                std::cout << "Error: Insufficient stock or drug not found.\n";
            }
            return;
        }
    }
    std::cout << "Prescription not found or already dispensed.\n";
}

// --- Billing ---
void HospitalController::generateBill(int prescriptionId) {
    for (const auto& p : prescriptions) {
        if (p.id == prescriptionId) {
            Drug* d = findDrug(p.drugName);
            if (d) {
                Transaction t;
                t.id = transactions.size() + 1;
                t.prescriptionId = prescriptionId;
                t.amount = p.quantity * d->price;
                t.date = Utils::getCurrentDate();
                t.paymentMethod = "Pending";
                
                // Sign transaction
                std::string dataToSign = std::to_string(t.id) + std::to_string(t.prescriptionId) + std::to_string(t.amount) + t.date;
                t.signature = signatureManager.signData(dataToSign);

                transactions.push_back(t);
                DataManager::saveTransactions(transactions);
                std::cout << "Bill generated: KES " << t.amount << "\n";
            }
        }
    }
}

void HospitalController::processPayment(int transactionId, const std::string& method) {
    for (auto& t : transactions) {
        if (t.id == transactionId) {
            t.paymentMethod = method;
            DataManager::saveTransactions(transactions);
            std::cout << "Payment processed via " << method << "\n";
            return;
        }
    }
    std::cout << "Transaction not found.\n";
}

float HospitalController::getDailyRevenue() {
    float total = 0;
    std::string today = Utils::getCurrentDate();
    for (const auto& t : transactions) {
        if (t.date == today && t.paymentMethod != "Pending") {
            total += t.amount;
        }
    }
    return total;
}

std::vector<Transaction>& HospitalController::getTransactions() {
    return transactions;
}

// --- AI & Reporting ---
void HospitalController::checkEpidemicTrends() {
    std::map<std::string, int> diagnosisCounts;
    for (const auto& p : prescriptions) {
        // Simple check: count diagnoses in the list
        // Ideally filter by date (last 24h)
        diagnosisCounts[p.diagnosis]++;
    }

    std::cout << "\n--- AI Epidemic Detection ---\n";
    bool alert = false;
    for (const auto& pair : diagnosisCounts) {
        if (pair.second > 5) { // Threshold of 5 cases
            std::cout << "ALERT: Potential outbreak of " << pair.first << " detected! (" << pair.second << " cases)\n";
            alert = true;
        }
    }
    if (!alert) std::cout << "No abnormal trends detected.\n";
}

void HospitalController::generateReport() {
    std::cout << "\n--- System Report ---\n";
    std::cout << "Total Users: " << users.size() << "\n";
    std::cout << "Total Drugs: " << drugs.size() << "\n";
    std::cout << "Total Prescriptions: " << prescriptions.size() << "\n";
    std::cout << "Total Transactions: " << transactions.size() << "\n";
    std::cout << "Daily Revenue: KES " << getDailyRevenue() << "\n";
}
