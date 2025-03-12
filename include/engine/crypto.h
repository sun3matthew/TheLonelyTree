#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>
#include <bitset>
#include <vector>
#include <sstream>
#include <iomanip>

static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

class Crypto{
    public:
        static std::string hash(std::string data);

        //! Incorrect
        // static std::string to_base64(unsigned long long value) {
        //     std::vector<unsigned char> bytes(8);
        //     for (int i = 7; i >= 0; --i) {
        //         bytes[i] = static_cast<unsigned char>(value & 0xFF);
        //         value >>= 8;
        //     }

        //     std::string base64_str;
        //     int padding = 0;
        //     for (size_t i = 0; i < 8; i += 3) {
        //         unsigned int block = (bytes[i] << 16) | (bytes[i + 1] << 8) | bytes[i + 2];
        //         for (int j = 0; j < 4; ++j) {
        //             unsigned int index = (block >> (18 - j * 6)) & 0x3F;
        //             base64_str.push_back(base64_chars[index]);
        //         }
        //     }

        //     while (base64_str.size() > 12) {
        //         base64_str.pop_back();
        //         padding++;
        //     }

        //     while (base64_str.size() < 12) {
        //         base64_str.push_back('=');
        //     }

        //     return base64_str;
        // }

        static unsigned long long fnv1a_hash(const std::string& str) {
            const unsigned long long FNV_offset_basis = 14695981039346656037ULL;
            const unsigned long long FNV_prime = 1099511628211ULL;
            
            unsigned long long hash = FNV_offset_basis;

            for (char c : str) {
                hash ^= static_cast<unsigned long long>(c);   // XOR byte into the hash
                hash *= FNV_prime;                             // Multiply by the prime
            }
            
            return hash;
        }

        static std::string toHex(unsigned long long value) {
            std::stringstream ss;
            ss << std::hex << std::setw(12) << std::setfill('0') << value;
            return ss.str();
        }
        
        static unsigned long long hexToLong(const std::string& hex_str) {
            unsigned long long value;
            std::stringstream ss(hex_str);
            ss >> std::hex >> value;
            return value;
        }

        static unsigned int hashToInt(std::string key){
            unsigned int hash = 0;
            for (char c : key){
                hash = hash * 31 + c;
            }
            return hash;
        }

        static std::string toHex(unsigned int i){
            std::stringstream stream;
            stream << std::setfill ('0') << std::setw(sizeof(unsigned int)*2) << std::hex << i;
            return stream.str();
        }
};

#endif // CRYPTO_H