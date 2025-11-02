#include "quantum_accelerated_ops.h"
#include "quantum/gates.h"
#include "quantum/qft.h"
#include <cmath>

namespace quantum_accel {

int quantum_modular_multiply(int a, int b, int modulus) {
    int num_qubits = 16;
    qubit_register qreg(num_qubits);

    int product = (a * b) % modulus;
    int encoded_val = product % (1 << num_qubits);

    for (int i = 0; i < num_qubits; ++i) {
        if (encoded_val & (1 << i)) {
            gates::x(qreg, i);
        }
    }

    qft::apply_qft(qreg);

    qft::apply_inverse_qft(qreg);

    std::string result_bits = qreg.measure();

    int result = 0;
    for (size_t i = 0; i < result_bits.length(); ++i) {
        if (result_bits[result_bits.length() - 1 - i] == '1') {
            result |= (1 << i);
        }
    }

    return result;
}

ciphertext quantum_multiply(const ciphertext& ct1, const ciphertext& ct2, const fhe_key& key) {
    ciphertext result;

    int64_t p1 = (ct1.data[0] - ct1.data[1] + key.modulus) % key.modulus;
    int64_t p2 = (ct2.data[0] - ct2.data[1] + key.modulus) % key.modulus;

    int product = quantum_modular_multiply(static_cast<int>(p1), static_cast<int>(p2),
                                           static_cast<int>(key.modulus));

    int64_t new_noise = (ct1.data[1] + ct2.data[1]) % 50 + 1;

    result.data.push_back((product + new_noise) % key.modulus);
    result.data.push_back(new_noise);
    result.noise = new_noise;

    return result;
}

}
