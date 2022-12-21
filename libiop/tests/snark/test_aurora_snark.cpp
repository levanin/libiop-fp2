#include <cstdint>
#include <chrono>

#include <gtest/gtest.h>

#include <libff/algebra/fields/binary/gf64.hpp>
#include <libff/algebra/curves/edwards/edwards_pp.hpp>
#include "libiop/snark/aurora_snark.hpp"
#include "libiop/relations/examples/r1cs_examples.hpp"
#include "libiop/algebra/isogeny_field.hpp"

namespace libiop {

TEST(AuroraSnarkTest, SimpleTest) {
    /* Set up R1CS */
    typedef libff::gf64 FieldT;
    typedef binary_hash_digest hash_type;

    const std::size_t num_constraints = 1 << 13;
    const std::size_t num_inputs = (1 << 5) - 1;
    const std::size_t num_variables = (1 << 13) - 1;
    const size_t security_parameter = 128;
    const size_t RS_extra_dimensions = 2;
    const size_t FRI_localization_parameter = 3;
    const LDT_reducer_soundness_type ldt_reducer_soundness_type = LDT_reducer_soundness_type::optimistic_heuristic;
    const FRI_soundness_type fri_soundness_type = FRI_soundness_type::heuristic;
    const field_subset_type domain_type = affine_subspace_type;

    r1cs_example<FieldT> r1cs_params = generate_r1cs_example<FieldT>(
        num_constraints, num_inputs, num_variables);
    EXPECT_TRUE(r1cs_params.constraint_system_.is_satisfied(
        r1cs_params.primary_input_, r1cs_params.auxiliary_input_));

    /* Actual SNARK test */
    for (std::size_t i = 0; i < 2; i++) {
        const bool make_zk = (i == 0) ? false : true;
        aurora_snark_parameters<FieldT, hash_type> params(
            security_parameter,
            ldt_reducer_soundness_type,
            fri_soundness_type,
            blake2b_type,
            FRI_localization_parameter,
            RS_extra_dimensions,
            make_zk,
            domain_type,
            num_constraints,
            num_variables);
        const aurora_snark_argument<FieldT, hash_type> argument = aurora_snark_prover<FieldT>(
            r1cs_params.constraint_system_,
            r1cs_params.primary_input_,
            r1cs_params.auxiliary_input_,
            params);

        printf("iop size in bytes %lu\n", argument.IOP_size_in_bytes());
        printf("bcs size in bytes %lu\n", argument.BCS_size_in_bytes());
        printf("argument size in bytes %lu\n", argument.size_in_bytes());

        const bool bit = aurora_snark_verifier<FieldT, hash_type>(
            r1cs_params.constraint_system_,
            r1cs_params.primary_input_,
            argument,
            params);

        EXPECT_TRUE(bit) << "failed on make_zk = " << i << " test";
    }
}

TEST(AuroraSnarkMultiplicativeTest, SimpleTest) {
    /* Set up R1CS */
    libff::edwards_pp::init_public_params();
    typedef libff::edwards_Fr FieldT;
    typedef binary_hash_digest hash_type;

    const size_t num_constraints = 1 << 13;
    const size_t num_inputs = (1 << 5) - 1;
    const size_t num_variables = (1 << 13) - 1;
    const size_t security_parameter = 128;
    const size_t RS_extra_dimensions = 2;
    const size_t FRI_localization_parameter = 3;
    const LDT_reducer_soundness_type ldt_reducer_soundness_type = LDT_reducer_soundness_type::optimistic_heuristic;
    const FRI_soundness_type fri_soundness_type = FRI_soundness_type::heuristic;
    const field_subset_type domain_type = multiplicative_coset_type;

    r1cs_example<FieldT> r1cs_params = generate_r1cs_example<FieldT>(
        num_constraints, num_inputs, num_variables);
    EXPECT_TRUE(r1cs_params.constraint_system_.is_satisfied(
        r1cs_params.primary_input_, r1cs_params.auxiliary_input_));

    /* Actual SNARK test */
    for (std::size_t i = 0; i < 2; i++) {
        const bool make_zk = (i == 0) ? false : true;
        aurora_snark_parameters<FieldT, hash_type> params(
            security_parameter,
            ldt_reducer_soundness_type,
            fri_soundness_type,
            blake2b_type,
            FRI_localization_parameter,
            RS_extra_dimensions,
            make_zk,
            domain_type,
            num_constraints,
            num_variables);
        const aurora_snark_argument<FieldT, hash_type> argument = aurora_snark_prover<FieldT>(
            r1cs_params.constraint_system_,
            r1cs_params.primary_input_,
            r1cs_params.auxiliary_input_,
            params);

        printf("iop size in bytes %lu\n", argument.IOP_size_in_bytes());
        printf("bcs size in bytes %lu\n", argument.BCS_size_in_bytes());
        printf("argument size in bytes %lu\n", argument.size_in_bytes());
        const bool bit = aurora_snark_verifier<FieldT>(
            r1cs_params.constraint_system_,
            r1cs_params.primary_input_,
            argument,
            params);

        EXPECT_TRUE(bit) << "failed on make_zk = " << i << " test";
    }
}


template<typename FieldT>
void test_isogeny_pok_fp(size_t m, size_t n) {
    libff::inhibit_profiling_counters = true;
    typedef binary_hash_digest hash_type;

    const size_t num_constraints = m;
    const size_t num_variables = n;
    const size_t num_inputs = num_constraints - num_variables;
    const size_t security_parameter = 128;
    const size_t RS_extra_dimensions = 2;
    const size_t FRI_localization_parameter = 3;
    const LDT_reducer_soundness_type ldt_reducer_soundness_type = LDT_reducer_soundness_type::optimistic_heuristic;
    const FRI_soundness_type fri_soundness_type = FRI_soundness_type::heuristic;
    const field_subset_type domain_type = multiplicative_coset_type;

    r1cs_example<FieldT> r1cs_params = generate_r1cs_example<FieldT>(
            num_constraints, num_inputs, num_variables);
    EXPECT_TRUE(r1cs_params.constraint_system_.is_satisfied(
            r1cs_params.primary_input_, r1cs_params.auxiliary_input_));

    /* Actual SNARK test */
    const bool make_zk = true;
    aurora_snark_parameters<FieldT, hash_type> params(
            security_parameter,
            ldt_reducer_soundness_type,
            fri_soundness_type,
            blake2b_type,
            FRI_localization_parameter,
            RS_extra_dimensions,
            make_zk,
            domain_type,
            num_constraints,
            num_variables);
    auto prover_start = std::chrono::high_resolution_clock::now();
    const aurora_snark_argument<FieldT, hash_type> argument = aurora_snark_prover<FieldT>(
            r1cs_params.constraint_system_,
            r1cs_params.primary_input_,
            r1cs_params.auxiliary_input_,
            params);
    auto prover_dur = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - prover_start);

    auto verifier_start = std::chrono::high_resolution_clock::now();
    const bool bit = aurora_snark_verifier<FieldT>(
            r1cs_params.constraint_system_,
            r1cs_params.primary_input_,
            argument,
            params);
    auto verifier_dur = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - verifier_start);

    printf("iop size in bytes %lu\n", argument.IOP_size_in_bytes());
    printf("bcs size in bytes %lu\n", argument.BCS_size_in_bytes());
    printf("argument size in bytes %lu\n", argument.size_in_bytes());

    std::cout << "prover time (ms): " << prover_dur.count() << std::endl;
    std::cout << "verifier time (ms): " << verifier_dur.count() << std::endl;

    EXPECT_TRUE(bit) << "test failed";
}

TEST(AuroraSnarkFpTest441, SimpleTest) {
    p441_plus::init_params();
    typedef p441_plus::Fp FieldT;
    test_isogeny_pok_fp<FieldT>(4096, 4095);
}

/**
 * Test Aurora on the proof of isogeny path circuit
 * @tparam FieldT
 * @param m number of constraints
 * @param n number of variables
 */
template<typename FieldT>
void test_isogeny_pok_fp2(size_t m, size_t n) {
    libff::inhibit_profiling_counters = true;
    typedef binary_hash_digest hash_type;

    const size_t num_constraints = m;
    const size_t num_variables = n;
    const size_t num_inputs = num_constraints - num_variables;
    const size_t security_parameter = 128;
    const size_t RS_extra_dimensions = 2;
    const size_t FRI_localization_parameter = 3;
    const LDT_reducer_soundness_type ldt_reducer_soundness_type = LDT_reducer_soundness_type::optimistic_heuristic;
    const FRI_soundness_type fri_soundness_type = FRI_soundness_type::heuristic;
    const field_subset_type domain_type = multiplicative_coset_type;

    r1cs_example<FieldT> r1cs_params = generate_r1cs_example<FieldT>(
            num_constraints, num_inputs, num_variables);
    EXPECT_TRUE(r1cs_params.constraint_system_.is_satisfied(
            r1cs_params.primary_input_, r1cs_params.auxiliary_input_));

    /* Actual SNARK test */
    const bool make_zk = true;
    aurora_snark_parameters<FieldT, hash_type> params(
            security_parameter,
            ldt_reducer_soundness_type,
            fri_soundness_type,
            blake2b_type,
            FRI_localization_parameter,
            RS_extra_dimensions,
            make_zk,
            domain_type,
            num_constraints,
            num_variables);
    auto prover_start = std::chrono::high_resolution_clock::now();
    const aurora_snark_argument<FieldT, hash_type> argument = aurora_snark_prover<FieldT>(
            r1cs_params.constraint_system_,
            r1cs_params.primary_input_,
            r1cs_params.auxiliary_input_,
            params);
    auto prover_dur = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - prover_start);

    auto verifier_start = std::chrono::high_resolution_clock::now();
    const bool bit = aurora_snark_verifier<FieldT>(
            r1cs_params.constraint_system_,
            r1cs_params.primary_input_,
            argument,
            params);
    auto verifier_dur = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - verifier_start);

    printf("iop size in bytes %lu\n", argument.IOP_size_in_bytes());
    printf("bcs size in bytes %lu\n", argument.BCS_size_in_bytes());
    printf("argument size in bytes %lu\n", argument.size_in_bytes());

    std::cout << "prover time (ms): " << prover_dur.count() << std::endl;
    std::cout << "verifier time (ms): " << verifier_dur.count() << std::endl;

    EXPECT_TRUE(bit) << "test failed";

}

TEST(AuroraSnarkFp2Test434, SimpleTest) {
    p434::init_params();
    typedef p434::Fp2 FieldT;
    test_isogeny_pok_fp2<FieldT>(1024, 1023);
}


TEST(AuroraSnarkFp2Test503, SimpleTest) {
    p503::init_params();
    typedef p503::Fp2 FieldT;
    test_isogeny_pok_fp2<FieldT>(1024, 1023);
}

TEST(AuroraSnarkFp2Test610, SimpleTest) {
    p610::init_params();
    typedef p610::Fp2 FieldT;
    test_isogeny_pok_fp2<FieldT>(2048, 2047);
}

TEST(AuroraSnarkFp2Test751, SimpleTest) {
    p751::init_params();
    typedef p751::Fp2 FieldT;
    test_isogeny_pok_fp2<FieldT>(2048, 2047);
}

}
