#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

struct Transaction {
    int id;
    int prescriptionId;
    float amount;
    std::string date;
    std::string paymentMethod; // "Cash", "Insurance", "Mobile"
    std::string signature; // Digital signature for verification
};

#endif
