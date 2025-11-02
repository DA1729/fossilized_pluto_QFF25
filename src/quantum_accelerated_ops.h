#pragma once

#include "fhe/fhe_engine.h"
#include "quantum/qubit_register.h"

namespace quantum_accel {

ciphertext quantum_multiply(const ciphertext& ct1, const ciphertext& ct2, const fhe_key& key);

int quantum_modular_multiply(int a, int b, int modulus);

}
