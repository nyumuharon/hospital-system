#include "HospitalController.h"
#include "Utils.h"
#include <iostream>
#include <algorithm>
#include <map>

using namespace std;

HospitalController::HospitalController() : currentUser(nullptr) {
    if (!signatureManager.init()) {
        cerr << "Warning: Digital Signature Manager failed to initialize.\n";
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
bool HospitalController::login(const string& username, const string& password) {
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

vector<Drug>& HospitalController::getDrugs() {
    return drugs;
}

Drug* HospitalController::findDrug(const string& name) {
    for (auto& d : drugs) {
        if (Utils::toLowerCase(d.name) == Utils::toLowerCase(name)) {
            return &d;
        }
    }
    return nullptr;
}

void HospitalController::checkLowStock() {
    cout << "\n--- Low Stock Alert ---\n";
    bool found = false;
    for (const auto& d : drugs) {
        if (d.isLowStock()) {
            cout << "WARNING: " << d.name << " is low (" << d.quantity << " left)\n";
            found = true;
        }
    }
    if (!found) cout << "All stock levels are healthy.\n";
}

void HospitalController::checkExpiry() {
    // Simplified expiry check (comparing strings for now, ideally use time_t)
    cout << "\n--- Expiry Check ---\n";
    string today = Utils::getCurrentDate();
    // In a real app, parse date and compare. Here we just list them.
    for (const auto& d : drugs) {
        cout << "Drug: " << d.name << " Expires: " << d.expiryDate << "\n";
    }
}

// --- Prescription ---
void HospitalController::createPrescription(const Prescription& p) {
    Prescription newP = p;
    // Sign the critical data: patientName + drugName + quantity + date
    string dataToSign = newP.patientName + newP.drugName + to_string(newP.quantity) + newP.date;
    newP.signature = signatureManager.signData(dataToSign);
    
    prescriptions.push_back(newP);
    DataManager::savePrescriptions(prescriptions);
    
    // Reserve stock logic could go here, but for now we deduct on dispense
    cout << "Prescription created for " << p.patientName << "\n";
}

vector<Prescription> HospitalController::getPendingPrescriptions() {
    vector<Prescription> pending;
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
            string dataToVerify = p.patientName + p.drugName + to_string(p.quantity) + p.date;
            if (!signatureManager.verifySignature(dataToVerify, p.signature)) {
                cout << "SECURITY ALERT: Digital Signature Verification Failed! Prescription may be tampered.\n";
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
                cout << "Dispensed successfully. Bill generated.\n";
            } else {
                cout << "Error: Insufficient stock or drug not found.\n";
            }
            return;
        }
    }
    cout << "Prescription not found or already dispensed.\n";
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
                string dataToSign = to_string(t.id) + to_string(t.prescriptionId) + to_string(t.amount) + t.date;
                t.signature = signatureManager.signData(dataToSign);

                transactions.push_back(t);
                DataManager::saveTransactions(transactions);
                cout << "Bill generated: KES " << t.amount << "\n";
            }
        }
    }
}

void HospitalController::processPayment(int transactionId, const string& method) {
    for (auto& t : transactions) {
        if (t.id == transactionId) {
            t.paymentMethod = method;
            DataManager::saveTransactions(transactions);
            cout << "Payment processed via " << method << "\n";
            return;
        }
    }
    cout << "Transaction not found.\n";
}

float HospitalController::getDailyRevenue() {
    float total = 0;
    string today = Utils::getCurrentDate();
    for (const auto& t : transactions) {
        if (t.date == today && t.paymentMethod != "Pending") {
            total += t.amount;
        }
    }
    return total;
}

vector<Transaction>& HospitalController::getTransactions() {
    return transactions;
}

// --- AI & Reporting ---
void HospitalController::checkEpidemicTrends() {
    map<string, int> diagnosisCounts;
    for (const auto& p : prescriptions) {
        // Simple check: count diagnoses in the list
        // Ideally filter by date (last 24h)
        diagnosisCounts[p.diagnosis]++;
    }

    cout << "\n--- AI Epidemic Detection ---\n";
    bool alert = false;
    for (const auto& pair : diagnosisCounts) {
        if (pair.second > 5) { // Threshold of 5 cases
            cout << "ALERT: Potential outbreak of " << pair.first << " detected! (" << pair.second << " cases)\n";
            alert = true;
        }
    }
    if (!alert) cout << "No abnormal trends detected.\n";
}

void HospitalController::generateReport() {
    cout << "\n--- System Report ---\n";
    cout << "Total Users: " << users.size() << "\n";
    cout << "Total Drugs: " << drugs.size() << "\n";
    cout << "Total Prescriptions: " << prescriptions.size() << "\n";
    cout << "Total Transactions: " << transactions.size() << "\n";
    cout << "Daily Revenue: KES " << getDailyRevenue() << "\n";
}
