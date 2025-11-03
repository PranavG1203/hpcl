#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_TRANSACTIONS 1000000
#define INITIAL_BALANCE 1000

int main() {
    double balance = INITIAL_BALANCE;
    double balance_lock = INITIAL_BALANCE;
    double deposit_amount = 5.0;
    double withdraw_amount = 5.0;
    double start, end;
    int i;

    printf("Bank Transaction Simulation using OpenMP\n");
    printf("Initial Balance: %.2f\n\n", balance);

    // ------------------- Using Critical -------------------
    start = omp_get_wtime();

    #pragma omp parallel for
    for (i = 0; i < NUM_TRANSACTIONS; i++) {
        if (i % 2 == 0) {
            // Deposit
            #pragma omp critical
            balance += deposit_amount;
        } else {
            // Withdraw
            #pragma omp critical
            balance -= withdraw_amount;
        }
    }

    end = omp_get_wtime();
    printf("[Using critical] Final Balance: %.2f | Time: %f seconds\n", balance, end - start);

    // ------------------- Using Locks -------------------
    omp_lock_t lock;
    omp_init_lock(&lock);

    start = omp_get_wtime();

    #pragma omp parallel for
    for (i = 0; i < NUM_TRANSACTIONS; i++) {
        if (i % 2 == 0) {
            // Deposit
            omp_set_lock(&lock);
            balance_lock += deposit_amount;
            omp_unset_lock(&lock);
        } else {
            // Withdraw
            omp_set_lock(&lock);
            balance_lock -= withdraw_amount;
            omp_unset_lock(&lock);
        }
    }

    end = omp_get_wtime();
    printf("[Using locks]    Final Balance: %.2f | Time: %f seconds\n", balance_lock, end - start);

    omp_destroy_lock(&lock);

    // ------------------- Expected Final Balance -------------------
    double expected_balance = INITIAL_BALANCE; // Because deposits and withdrawals are equal
    printf("\nExpected Final Balance: %.2f\n", expected_balance);

    // ------------------- Verification -------------------
    if (balance == expected_balance && balance_lock == expected_balance)
        printf("\nVerification: PASSED ✅ (No race conditions)\n");
    else
        printf("\nVerification: FAILED ❌ (Inconsistent results)\n");

    return 0;
}
