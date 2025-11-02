#pragma once

#include "qubit_register.h"

namespace qft {

void apply_qft(qubit_register& qreg);

void apply_inverse_qft(qubit_register& qreg);

}
