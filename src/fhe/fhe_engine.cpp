#include "fhe_engine.h"
#include <random>
#include <cmath>

fhe_engine::fhe_engine() : modulus(65537) {
    key.modulus = modulus;
}

int64_t fhe_engine::mod(int64_t a, int64_t m) {
    int64_t result = a % m;
    if (result < 0) result += m;
    return result;
}

void fhe_engine::keygen() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<int64_t> dist(100, 1000);

    key.secret_key = dist(gen);
    key.public_key = mod(key.secret_key * 7, modulus);
}

ciphertext fhe_engine::encrypt(int64_t plaintext) {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<int64_t> noise_dist(1, 10);

    ciphertext ct;
    int64_t noise = noise_dist(gen);

    ct.data.push_back(mod(plaintext + noise, modulus));
    ct.data.push_back(noise);
    ct.noise = static_cast<double>(noise);

    return ct;
}

int64_t fhe_engine::decrypt(const ciphertext& ct) {
    int64_t plaintext_with_noise = ct.data[0];
    int64_t noise = ct.data[1];
    return mod(plaintext_with_noise - noise, modulus);
}

ciphertext fhe_engine::add(const ciphertext& ct1, const ciphertext& ct2) {
    ciphertext result;

    result.data.push_back(mod(ct1.data[0] + ct2.data[0], modulus));
    result.data.push_back(mod(ct1.data[1] + ct2.data[1], modulus));
    result.noise = ct1.noise + ct2.noise;

    return result;
}

ciphertext fhe_engine::multiply(const ciphertext& ct1, const ciphertext& ct2) {
    ciphertext result;

    int64_t p1 = mod(ct1.data[0] - ct1.data[1], modulus);
    int64_t p2 = mod(ct2.data[0] - ct2.data[1], modulus);
    int64_t product = mod(p1 * p2, modulus);

    int64_t new_noise = (ct1.data[1] + ct2.data[1]) % 50 + 1;

    result.data.push_back(mod(product + new_noise, modulus));
    result.data.push_back(new_noise);
    result.noise = new_noise;

    return result;
}

fhe_key fhe_engine::get_key() const {
    return key;
}
