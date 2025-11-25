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
        // Load defaults if empty - Restoring original 99 drugs
        drugs.push_back({"Paracetamol", 50.0, 100, "01/01/2026", 20});
        drugs.push_back({"Amoxicillin", 120.0, 100, "01/06/2025", 15});
        drugs.push_back({"Ibuprofen", 80.0, 2000, "15/03/2026", 50});
        drugs.push_back({"Insulin", 200.0, 200, "10/12/2025", 10});
        drugs.push_back({"Metformin", 150.0, 1005, "20/08/2026", 30});
        drugs.push_back({"Cough Syrup", 90.0, 120, "05/05/2025", 10});
        drugs.push_back({"Aspirin", 60.0, 800, "11/11/2026", 40});
        drugs.push_back({"Antacid", 45.0, 250, "30/09/2025", 20});
        drugs.push_back({"Vitamin C", 30.0, 400, "14/02/2026", 25});
        drugs.push_back({"Pain Relief Gel", 75.0, 100, "01/07/2025", 10});
        drugs.push_back({"Penicillin", 130.0, 900, "12/12/2025", 30});
        drugs.push_back({"Cetirizine", 55.0, 180, "18/04/2026", 15});
        drugs.push_back({"Chloroquine", 110.0, 104, "22/01/2026", 10});
        drugs.push_back({"Erythromycin", 145.0, 700, "19/06/2025", 25});
        drugs.push_back({"Hydrocortisone", 175.0, 600, "03/03/2026", 20});
        drugs.push_back({"Prednisolone", 160.0, 1100, "25/08/2025", 40});
        drugs.push_back({"Zinc Tablets", 40.0, 330, "09/09/2026", 15});
        drugs.push_back({"Multivitamin", 70.0, 250, "14/10/2025", 20});
        drugs.push_back({"Loratadine", 65.0, 200, "01/01/2026", 15});
        drugs.push_back({"Omeprazole", 85.0, 190, "15/05/2025", 10});
        drugs.push_back({"Diclofenac", 90.0, 170, "20/11/2026", 15});
        drugs.push_back({"Folic Acid", 35.0, 220, "08/08/2025", 20});
        drugs.push_back({"Doxycycline", 140.0, 600, "12/02/2026", 30});
        drugs.push_back({"Enalapril", 95.0, 120, "30/04/2025", 10});
        drugs.push_back({"Warfarin", 120.0, 90, "17/07/2026", 10});
        drugs.push_back({"Heparin", 250.0, 30, "05/12/2025", 5});
        drugs.push_back({"Metronidazole", 100.0, 180, "28/02/2026", 15});
        drugs.push_back({"Ciprofloxacin", 135.0, 70, "10/10/2025", 10});
        drugs.push_back({"Levofloxacin", 145.0, 80, "15/01/2026", 10});
        drugs.push_back({"Moxifloxacin", 190.0, 50, "20/03/2025", 5});
        drugs.push_back({"Ranitidine", 75.0, 20, "01/06/2026", 5});
        drugs.push_back({"Pantoprazole", 95.0, 150, "14/09/2025", 15});
        drugs.push_back({"Esomeprazole", 105.0, 90, "25/12/2026", 10});
        drugs.push_back({"Lactulose", 85.0, 110, "04/04/2025", 10});
        drugs.push_back({"Oral Rehydration Salts", 25.0, 500, "18/08/2026", 30});
        drugs.push_back({"Hydrocodone", 210.0, 40, "01/02/2025", 5});
        drugs.push_back({"Morphine", 300.0, 200, "10/11/2026", 10});
        drugs.push_back({"Codeine", 180.0, 700, "22/05/2025", 25});
        drugs.push_back({"Acetaminophen", 50.0, 3500, "30/01/2026", 100});
        drugs.push_back({"Clotrimazole Cream", 95.0, 1000, "14/07/2025", 50});
        drugs.push_back({"Nystatin", 100.0, 800, "09/03/2026", 40});
        drugs.push_back({"Ketoconazole", 130.0, 600, "12/12/2025", 30});
        drugs.push_back({"Benzylpenicillin", 150.0, 900, "01/10/2026", 45});
        drugs.push_back({"Gentamicin", 140.0, 100, "15/04/2025", 10});
        drugs.push_back({"Neomycin", 115.0, 140, "20/08/2026", 15});
        drugs.push_back({"Tetracycline", 125.0, 100, "05/05/2025", 10});
        drugs.push_back({"Rifampicin", 190.0, 600, "11/01/2026", 30});
        drugs.push_back({"Isoniazid", 130.0, 800, "28/09/2025", 40});
        drugs.push_back({"Pyrazinamide", 160.0, 70, "14/02/2026", 10});
        drugs.push_back({"Ethambutol", 170.0, 90, "01/06/2025", 10});
        drugs.push_back({"Streptomycin", 180.0, 500, "19/11/2026", 25});
        drugs.push_back({"Chloramphenicol", 135.0, 600, "03/03/2025", 30});
        drugs.push_back({"Diazepam", 95.0, 1200, "25/07/2026", 50});
        drugs.push_back({"Lorazepam", 100.0, 110, "09/12/2025", 10});
        drugs.push_back({"Phenobarbital", 110.0, 900, "14/04/2026", 45});
        drugs.push_back({"Phenytoin", 120.0, 1000, "01/09/2025", 50});
        drugs.push_back({"Carbamazepine", 125.0, 800, "15/01/2026", 40});
        drugs.push_back({"Sodium Valproate", 135.0, 700, "20/05/2025", 35});
        drugs.push_back({"Lamotrigine", 150.0, 500, "08/10/2026", 25});
        drugs.push_back({"Haloperidol", 140.0, 900, "12/02/2025", 45});
        drugs.push_back({"Olanzapine", 160.0, 600, "30/06/2026", 30});
        drugs.push_back({"Risperidone", 155.0, 700, "17/11/2025", 35});
        drugs.push_back({"Sertraline", 145.0, 800, "05/03/2026", 40});
        drugs.push_back({"Fluoxetine", 130.0, 1000, "28/07/2025", 50});
        drugs.push_back({"Citalopram", 135.0, 900, "10/12/2026", 45});
        drugs.push_back({"Escitalopram", 140.0, 600, "15/04/2025", 30});
        drugs.push_back({"Amitriptyline", 120.0, 800, "20/09/2026", 40});
        drugs.push_back({"Imipramine", 115.0, 900, "01/01/2025", 45});
        drugs.push_back({"Clomipramine", 125.0, 700, "14/05/2026", 35});
        drugs.push_back({"Propranolol", 95.0, 1500, "25/10/2025", 75});
        drugs.push_back({"Atenolol", 90.0, 1600, "04/02/2026", 80});
        drugs.push_back({"Bisoprolol", 100.0, 1100, "18/06/2025", 55});
        drugs.push_back({"Nifedipine", 105.0, 1400, "01/11/2026", 70});
        drugs.push_back({"Diltiazem", 110.0, 1000, "10/03/2025", 50});
        drugs.push_back({"Verapamil", 120.0, 90, "22/07/2026", 10});
        drugs.push_back({"Spironolactone", 130.0, 60, "30/12/2025", 5});
        drugs.push_back({"Digoxin", 140.0, 500, "14/04/2026", 25});
        drugs.push_back({"Nitroglycerin", 150.0, 400, "09/09/2025", 20});
        drugs.push_back({"Isosorbide Dinitrate", 160.0, 500, "12/01/2026", 25});
        drugs.push_back({"Clopidogrel", 170.0, 70, "01/05/2025", 10});
        drugs.push_back({"Aspirin Low Dose", 60.0, 3000, "15/10/2026", 150});
        drugs.push_back({"Warfarin Sodium", 180.0, 400, "20/02/2025", 20});
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
