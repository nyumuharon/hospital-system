#include "DigitalSignatureManager.h"
#include <iostream>
#include <sstream>

// Base64 encoding table
static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

DigitalSignatureManager::DigitalSignatureManager() : hProv(0), hKey(0) {}

DigitalSignatureManager::~DigitalSignatureManager() {
    if (hKey) CryptDestroyKey(hKey);
    if (hProv) CryptReleaseContext(hProv, 0);
}

bool DigitalSignatureManager::init() {
    const char* containerName = "HospitalSystemKeyContainer";
    
    // Try to acquire context for existing container
    if (!CryptAcquireContext(&hProv, containerName, NULL, PROV_RSA_AES, 0)) {
        // If doesn't exist, create it
        if (GetLastError() == NTE_BAD_KEYSET) {
            if (!CryptAcquireContext(&hProv, containerName, NULL, PROV_RSA_AES, CRYPT_NEWKEYSET)) {
                std::cerr << "Error creating key container: " << GetLastError() << "\n";
                return false;
            }
        } else {
            std::cerr << "Error acquiring context: " << GetLastError() << "\n";
            return false;
        }
    }

    // Try to get the signature key
    if (!CryptGetUserKey(hProv, AT_SIGNATURE, &hKey)) {
        if (GetLastError() == NTE_NO_KEY) {
            // Generate if not found
            if (!CryptGenKey(hProv, AT_SIGNATURE, CRYPT_EXPORTABLE, &hKey)) {
                std::cerr << "Error generating key: " << GetLastError() << "\n";
                return false;
            }
        } else {
            std::cerr << "Error getting user key: " << GetLastError() << "\n";
            return false;
        }
    }

    return true;
}

std::string DigitalSignatureManager::signData(const std::string& data) {
    HCRYPTHASH hHash = 0;
    std::string signature = "";

    if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
        std::cerr << "Error during CryptCreateHash." << std::endl;
        return "";
    }

    if (!CryptHashData(hHash, (BYTE*)data.c_str(), data.length(), 0)) {
        std::cerr << "Error during CryptHashData." << std::endl;
        CryptDestroyHash(hHash);
        return "";
    }

    DWORD dwSigLen = 0;
    if (!CryptSignHash(hHash, AT_SIGNATURE, NULL, 0, NULL, &dwSigLen)) {
        std::cerr << "Error during CryptSignHash (length)." << std::endl;
        CryptDestroyHash(hHash);
        return "";
    }

    std::vector<BYTE> pbSignature(dwSigLen);
    if (!CryptSignHash(hHash, AT_SIGNATURE, NULL, 0, pbSignature.data(), &dwSigLen)) {
        std::cerr << "Error during CryptSignHash (data)." << std::endl;
        CryptDestroyHash(hHash);
        return "";
    }

    signature = base64Encode(pbSignature);
    CryptDestroyHash(hHash);
    return signature;
}

bool DigitalSignatureManager::verifySignature(const std::string& data, const std::string& signatureBase64) {
    HCRYPTHASH hHash = 0;
    HCRYPTKEY hPubKey = 0; // We would normally load the public key here. 
                           // For this demo, since we are in the same session/object, we use the key pair we generated.
                           // In a real scenario, you'd export the public key and import it for verification.
                           // However, since we are verifying within the same running instance for this demo (or assuming shared key access),
                           // we will use the existing hKey which contains the public key part.
                           
    // Note: In a real distributed system, the verifier needs the signer's PUBLIC key.
    // Here we assume the controller holds the key pair for the "Hospital System Authority".

    if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
        return false;
    }

    if (!CryptHashData(hHash, (BYTE*)data.c_str(), data.length(), 0)) {
        CryptDestroyHash(hHash);
        return false;
    }

    std::vector<BYTE> pbSignature = base64Decode(signatureBase64);

    if (!CryptVerifySignature(hHash, pbSignature.data(), pbSignature.size(), hKey, NULL, 0)) {
        // std::cerr << "Signature verification failed." << std::endl; // Optional logging
        CryptDestroyHash(hHash);
        return false;
    }

    CryptDestroyHash(hHash);
    return true;
}

std::string DigitalSignatureManager::base64Encode(const std::vector<BYTE>& data) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    size_t dataLen = data.size();
    size_t idx = 0;

    while (dataLen--) {
        char_array_3[i++] = data[idx++];
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while((i++ < 3))
            ret += '=';
    }

    return ret;
}

std::vector<BYTE> DigitalSignatureManager::base64Decode(const std::string& encoded_string) {
    size_t in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::vector<BYTE> ret;

    while (in_len-- && ( encoded_string[in_] != '=') && (isalnum(encoded_string[in_]) || (encoded_string[in_] == '+') || (encoded_string[in_] == '/'))) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret.push_back(char_array_3[i]);
            i = 0;
        }
    }

    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;

        for (j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
    }

    return ret;
}
