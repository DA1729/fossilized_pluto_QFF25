#include "gates.h"
#include <cmath>

namespace gates {

const double PI = 3.14159265358979323846;

void x(qubit_register& qreg, int target) {
    std::vector<std::vector<std::complex<double>>> x_gate = {
        {0.0, 1.0},
        {1.0, 0.0}
    };
    qreg.apply_unitary(x_gate, {target});
}

void h(qubit_register& qreg, int target) {
    double inv_sqrt2 = 1.0 / std::sqrt(2.0);
    std::vector<std::vector<std::complex<double>>> h_gate = {
        {inv_sqrt2, inv_sqrt2},
        {inv_sqrt2, -inv_sqrt2}
    };
    qreg.apply_unitary(h_gate, {target});
}

void z(qubit_register& qreg, int target) {
    std::vector<std::vector<std::complex<double>>> z_gate = {
        {1.0, 0.0},
        {0.0, -1.0}
    };
    qreg.apply_unitary(z_gate, {target});
}

void cnot(qubit_register& qreg, int control, int target) {
    std::vector<std::vector<std::complex<double>>> cnot_gate = {
        {1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 1.0},
        {0.0, 0.0, 1.0, 0.0}
    };
    qreg.apply_unitary(cnot_gate, {control, target});
}

void swap(qubit_register& qreg, int qubit1, int qubit2) {
    std::vector<std::vector<std::complex<double>>> swap_gate = {
        {1.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}
    };
    qreg.apply_unitary(swap_gate, {qubit1, qubit2});
}

void phase_rk(qubit_register& qreg, int target, int k) {
    double angle = 2.0 * PI / std::pow(2.0, k);
    std::complex<double> phase(std::cos(angle), std::sin(angle));
    std::vector<std::vector<std::complex<double>>> rk_gate = {
        {1.0, 0.0},
        {0.0, phase}
    };
    qreg.apply_unitary(rk_gate, {target});
}

}
