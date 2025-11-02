#include "qft.h"
#include "gates.h"

namespace qft {

void apply_qft(qubit_register& qreg) {
    int n = qreg.size();

    for (int i = 0; i < n; ++i) {
        gates::h(qreg, i);
        for (int j = i + 1; j < n; ++j) {
            int k = j - i + 1;
            gates::phase_rk(qreg, j, k);
        }
    }

    for (int i = 0; i < n / 2; ++i) {
        gates::swap(qreg, i, n - 1 - i);
    }
}

void apply_inverse_qft(qubit_register& qreg) {
    int n = qreg.size();

    for (int i = 0; i < n / 2; ++i) {
        gates::swap(qreg, i, n - 1 - i);
    }

    for (int i = n - 1; i >= 0; --i) {
        for (int j = n - 1; j > i; --j) {
            int k = j - i + 1;
            gates::phase_rk(qreg, j, -k);
        }
        gates::h(qreg, i);
    }
}

}
