#ifndef DIGITALSIGNATUREMANAGER_H
#define DIGITALSIGNATUREMANAGER_H

#include <string>
#include <windows.h>
#include <wincrypt.h>
#include <vector>

#pragma comment(lib, "crypt32.lib")

class DigitalSignatureManager {
private:
    HCRYPTPROV hProv;
    HCRYPTKEY hKey;

public:
    DigitalSignatureManager();
    ~DigitalSignatureManager();

    bool init();
    std::string signData(const std::string& data);
    bool verifySignature(const std::string& data, const std::string& signatureBase64);
    
private:
    std::string base64Encode(const std::vector<BYTE>& data);
    std::vector<BYTE> base64Decode(const std::string& encoded);
};

#endif
