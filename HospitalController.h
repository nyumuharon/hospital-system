#ifndef HOSPITALCONTROLLER_H
#define HOSPITALCONTROLLER_H

#include <vector>
#include <string>
#include "User.h"
#include "Drug.h"
#include "Prescription.h"
#include "Transaction.h"
#include "Transaction.h"
#include "DataManager.h"
#include "DigitalSignatureManager.h"

class HospitalController {
private:
    std::vector<User> users;
    std::vector<Drug> drugs;
    std::vector<Prescription> prescriptions;
    std::vector<Transaction> transactions;
    User* currentUser;
    DigitalSignatureManager signatureManager;

public:
    HospitalController();
    ~HospitalController();

    // User Management
    bool login(const std::string& username, const std::string& password);
    void logout();
    User* getCurrentUser();
    void addUser(const User& user);

    // Inventory
    void addDrug(const Drug& drug);
    std::vector<Drug>& getDrugs();
    Drug* findDrug(const std::string& name);
    void checkLowStock();
    void checkExpiry();

    // Prescription
    void createPrescription(const Prescription& p);
    std::vector<Prescription> getPendingPrescriptions();
    void dispensePrescription(int prescriptionId);

    // Billing
    void generateBill(int prescriptionId);
    void processPayment(int transactionId, const std::string& method);
    float getDailyRevenue();

    // AI & Reporting
    void checkEpidemicTrends();
    void generateReport();
};

#endif
