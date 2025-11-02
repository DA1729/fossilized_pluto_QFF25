#pragma once

#include <vector>
#include <cstdint>

struct fhe_key {
    int64_t secret_key;
    int64_t public_key;
    int64_t modulus;
};

struct ciphertext {
    std::vector<int64_t> data;
    double noise;
};

class fhe_engine {
private:
    fhe_key key;
    int64_t modulus;

    int64_t mod(int64_t a, int64_t m);

public:
    fhe_engine();

    void keygen();

    ciphertext encrypt(int64_t plaintext);

    int64_t decrypt(const ciphertext& ct);

    ciphertext add(const ciphertext& ct1, const ciphertext& ct2);

    ciphertext multiply(const ciphertext& ct1, const ciphertext& ct2);

    fhe_key get_key() const;
};
