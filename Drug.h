#ifndef DRUG_H
#define DRUG_H

#include <string>

struct Drug {
    std::string name;
    float price;
    int quantity;
    std::string expiryDate; // Format: DD/MM/YYYY
    int minThreshold;  // For low stock alert

    bool isLowStock() const {
        return quantity <= minThreshold;
    }
};

#endif
