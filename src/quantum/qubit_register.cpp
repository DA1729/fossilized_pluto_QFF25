#include "qubit_register.h"
#include <cmath>
#include <algorithm>
#include <random>

qubit_register::qubit_register(int n) : num_qubits(n) {
    int dim = 1 << n;
    amplitudes.resize(dim, 0.0);
    amplitudes[0] = 1.0;
}

int qubit_register::size() const {
    return num_qubits;
}

std::complex<double> qubit_register::get_amplitude(int state) const {
    return amplitudes[state];
}

void qubit_register::set_amplitude(int state, std::complex<double> value) {
    amplitudes[state] = value;
}

void qubit_register::apply_unitary(const std::vector<std::vector<std::complex<double>>>& unitary,
                                   const std::vector<int>& target_qubits) {
    int dim = 1 << num_qubits;
    int u_dim = unitary.size();
    std::vector<std::complex<double>> new_amplitudes(dim, 0.0);

    for (int state = 0; state < dim; ++state) {
        int target_bits = 0;
        for (size_t i = 0; i < target_qubits.size(); ++i) {
            if (state & (1 << target_qubits[i])) {
                target_bits |= (1 << i);
            }
        }

        for (int u_row = 0; u_row < u_dim; ++u_row) {
            int new_target_bits = u_row;
            int new_state = state;

            for (size_t i = 0; i < target_qubits.size(); ++i) {
                if (new_target_bits & (1 << i)) {
                    new_state |= (1 << target_qubits[i]);
                } else {
                    new_state &= ~(1 << target_qubits[i]);
                }
            }

            new_amplitudes[new_state] += unitary[u_row][target_bits] * amplitudes[state];
        }
    }

    amplitudes = new_amplitudes;
}

std::string qubit_register::measure() {
    int dim = 1 << num_qubits;
    std::vector<double> probabilities(dim);

    for (int i = 0; i < dim; ++i) {
        probabilities[i] = std::norm(amplitudes[i]);
    }

    int max_idx = std::max_element(probabilities.begin(), probabilities.end()) - probabilities.begin();

    std::string result;
    for (int i = num_qubits - 1; i >= 0; --i) {
        result += ((max_idx >> i) & 1) ? '1' : '0';
    }

    return result;
}

void qubit_register::reset() {
    std::fill(amplitudes.begin(), amplitudes.end(), 0.0);
    amplitudes[0] = 1.0;
}
