#pragma once

#include "qubit_register.h"
#include <vector>
#include <complex>

namespace gates {

void x(qubit_register& qreg, int target);

void h(qubit_register& qreg, int target);

void z(qubit_register& qreg, int target);

void cnot(qubit_register& qreg, int control, int target);

void swap(qubit_register& qreg, int qubit1, int qubit2);

void phase_rk(qubit_register& qreg, int target, int k);

}
