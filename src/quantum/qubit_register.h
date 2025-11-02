#pragma once

#include <vector>
#include <complex>
#include <string>

class qubit_register {
private:
    int num_qubits;
    std::vector<std::complex<double>> amplitudes;

public:
    qubit_register(int n);

    int size() const;
    std::complex<double> get_amplitude(int state) const;
    void set_amplitude(int state, std::complex<double> value);

    void apply_unitary(const std::vector<std::vector<std::complex<double>>>& unitary,
                       const std::vector<int>& target_qubits);

    std::string measure();

    void reset();
};
