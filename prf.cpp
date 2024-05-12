#include <iostream>
#include <iomanip>
#include <sstream>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <openssl/rand.h> // For random key generation
#include <chrono>


std::string hmac_sha256(const std::string& key, const std::string& data) {
    unsigned char* digest;
    unsigned int digest_len;
    digest = HMAC(EVP_sha256(), key.c_str(), key.length(),
                  reinterpret_cast<const unsigned char*>(data.c_str()), data.length(), nullptr, &digest_len);

    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < digest_len; ++i) {
        ss << std::setw(2) << (unsigned int)digest[i];
    }
    return ss.str();
}

int main() {
    std::string message = "Hello, this is a test message.";
    int num_iterations = 1000;
    long total_execution_time = 0;

    for (int i = 0; i < num_iterations; ++i) {
        // Generate a random key for each iteration
        unsigned char key[16];  // 16 bytes key
        RAND_bytes(key, sizeof(key));

        // Convert key to hexadecimal string
        std::stringstream key_stream;
        key_stream << std::hex << std::setfill('0');
        for (int j = 0; j < sizeof(key); ++j) {
            key_stream << std::setw(2) << (unsigned int)key[j];
        }
        std::string key_str = key_stream.str();

        auto start_time = std::chrono::high_resolution_clock::now();  // Start time

        // Compute HMAC-SHA256
        std::string hmac_output = hmac_sha256(key_str, message);

        auto end_time = std::chrono::high_resolution_clock::now();  // End time

        // Calculate elapsed time
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        long elapsed_time = duration.count();  // Elapsed time in microseconds

        // Accumulate total execution time
        total_execution_time += elapsed_time;

        // Output HMAC and elapsed time
        std::cout << "HMAC: " << hmac_output << std::endl;
        std::cout << "Elapsed Time: " << elapsed_time << " microseconds" << std::endl;
    }

    // Calculate average execution time
    long average_execution_time = total_execution_time / num_iterations;

    // Output total execution time and average execution time
    std::cout << "Total Execution Time: " << total_execution_time << " microseconds" << std::endl;
    std::cout << "Average Execution Time: " << average_execution_time << " microseconds" << std::endl;

    return 0;
}
