#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <vector>
#include <string>
#include "User.h"
#include "Drug.h"
#include "Prescription.h"
#include "Transaction.h"

class DataManager {
public:
    static std::vector<User> loadUsers();
    static void saveUsers(const std::vector<User>& users);

    static std::vector<Drug> loadDrugs();
    static void saveDrugs(const std::vector<Drug>& drugs);

    static std::vector<Prescription> loadPrescriptions();
    static void savePrescriptions(const std::vector<Prescription>& prescriptions);

    static std::vector<Transaction> loadTransactions();
    static void saveTransactions(const std::vector<Transaction>& transactions);
};

#endif
