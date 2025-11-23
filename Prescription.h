#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include <string>

struct Prescription {
    int id;
    std::string doctorName;
    std::string patientName;
    std::string drugName;
    int quantity;
    std::string date;
    std::string status; // "pending", "dispensed"
    std::string diagnosis; // For AI epidemic detection
    std::string signature; // Digital signature for verification
};

#endif
