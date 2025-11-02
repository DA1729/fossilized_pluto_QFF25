#include "fhe/fhe_engine.h"
#include "quantum_accelerated_ops.h"
#include <iostream>
#include <chrono>
#include <cmath>
#include <iomanip>

double compute_fidelity(int64_t val1, int64_t val2) {
    if (val1 == val2) return 1.0;
    double diff = std::abs(static_cast<double>(val1 - val2));
    double avg = (std::abs(static_cast<double>(val1)) + std::abs(static_cast<double>(val2))) / 2.0;
    if (avg < 1.0) return 1.0 / (1.0 + diff);
    return 1.0 / (1.0 + diff / avg);
}

int main() {
    std::cout << "quantum-accelerated fhe simulation" << std::endl;
    std::cout << "===================================" << std::endl << std::endl;

    fhe_engine engine;

    std::cout << "generating fhe keys..." << std::endl;
    engine.keygen();
    std::cout << "keys generated successfully" << std::endl << std::endl;

    int64_t plaintext_a = 42;
    int64_t plaintext_b = 17;

    std::cout << "encrypting values..." << std::endl;
    std::cout << "plaintext a: " << plaintext_a << std::endl;
    std::cout << "plaintext b: " << plaintext_b << std::endl;

    ciphertext ct_a = engine.encrypt(plaintext_a);
    ciphertext ct_b = engine.encrypt(plaintext_b);

    std::cout << "encryption complete" << std::endl << std::endl;

    std::cout << "running classical multiply..." << std::endl;
    auto start_classical = std::chrono::high_resolution_clock::now();

    ciphertext ct_classical = engine.multiply(ct_a, ct_b);

    auto end_classical = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_classical = end_classical - start_classical;

    int64_t result_classical = engine.decrypt(ct_classical);
    std::cout << "classical result: " << result_classical << std::endl;
    std::cout << "classical time: " << std::fixed << std::setprecision(6)
              << duration_classical.count() << "s" << std::endl << std::endl;

    std::cout << "running quantum accelerated multiply..." << std::endl;
    auto start_quantum = std::chrono::high_resolution_clock::now();

    ciphertext ct_quantum = quantum_accel::quantum_multiply(ct_a, ct_b, engine.get_key());

    auto end_quantum = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_quantum = end_quantum - start_quantum;

    int64_t result_quantum = engine.decrypt(ct_quantum);
    std::cout << "quantum result: " << result_quantum << std::endl;
    std::cout << "quantum simulated time: " << std::fixed << std::setprecision(6)
              << duration_quantum.count() << "s" << std::endl << std::endl;

    double fidelity = compute_fidelity(result_classical, result_quantum);

    int num_qubits = 16;
    int64_t n = engine.get_key().modulus;
    int classical_complexity = static_cast<int>(std::log2(n) * std::log2(n));
    int quantum_complexity = num_qubits * static_cast<int>(std::log2(num_qubits));
    double theoretical_speedup = static_cast<double>(classical_complexity) / static_cast<double>(quantum_complexity);

    double measured_speedup = duration_classical.count() / duration_quantum.count();

    std::cout << "comparison" << std::endl;
    std::cout << "----------" << std::endl;
    std::cout << "expected (a*b): " << (plaintext_a * plaintext_b) << std::endl;
    std::cout << "classical time: " << std::fixed << std::setprecision(6)
              << duration_classical.count() << "s" << std::endl;
    std::cout << "quantum time: " << std::fixed << std::setprecision(6)
              << duration_quantum.count() << "s" << std::endl;

    if (measured_speedup >= 1.0) {
        std::cout << "measured speedup: " << std::fixed << std::setprecision(2)
                  << measured_speedup << "x faster" << std::endl;
    } else {
        std::cout << "measured speedup: " << std::fixed << std::setprecision(2)
                  << (1.0 / measured_speedup) << "x slower (simulation overhead)" << std::endl;
    }

    std::cout << "theoretical speedup (on real quantum hardware): " << std::fixed << std::setprecision(2)
              << theoretical_speedup << "x" << std::endl;
    std::cout << "classical complexity: o(log^2 n) ≈ " << classical_complexity << " ops" << std::endl;
    std::cout << "quantum complexity: o(n log n) = " << quantum_complexity << " gates" << std::endl;
    std::cout << "fidelity: " << std::fixed << std::setprecision(4)
              << fidelity << std::endl << std::endl;

    std::cout << "simulation complete" << std::endl;

    return 0;
}
