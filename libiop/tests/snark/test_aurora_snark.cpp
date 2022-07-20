#include <cstdint>

#include <gtest/gtest.h>

#include <libff/algebra/fields/binary/gf64.hpp>
#include <libff/algebra/curves/edwards/edwards_pp.hpp>
#include "libiop/snark/aurora_snark.hpp"
#include "libiop/relations/examples/r1cs_examples.hpp"

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

const mp_size_t sike_limbs = 7;
libff::bigint<sike_limbs> sike_modulus("24439423661345221551909145011457493619085780243761596511325807336205221239331976725970216671828618445898719026692884939342314733567");
typedef libff::Fp_model<sike_limbs, sike_modulus> Fp_sike;

TEST(AuroraSnarkLargeFieldTest, SimpleTest) {
    assert(Fp_sike::modulus_is_valid());
    Fp_sike::Rsquared = libff::bigint<sike_limbs>("1631483357165550719083731178910934187933895301703355372962834564740025113443660361043011370056616562298461884328920157440580164400");
    Fp_sike::Rcubed = libff::bigint<sike_limbs>("20837450692795383045350192246862524190972469314298443645963536460875107178404868644875693185558066690330093003204714510105284771771");
    Fp_sike::inv = 0x1;

    Fp_sike::num_bits = 434;
    Fp_sike::euler = libff::bigint<sike_limbs>("12219711830672610775954572505728746809542890121880798255662903668102610619665988362985108335914309222949359513346442469671157366783");
    Fp_sike::s = 1;
    Fp_sike::t = libff::bigint<sike_limbs>("12219711830672610775954572505728746809542890121880798255662903668102610619665988362985108335914309222949359513346442469671157366783");
    Fp_sike::t_minus_1_over_2 = libff::bigint<sike_limbs>("6109855915336305387977286252864373404771445060940399127831451834051305309832994181492554167957154611474679756673221234835578683391");
    Fp_sike::multiplicative_generator = Fp_sike("5");
    Fp_sike::root_of_unity = Fp_sike("24439423661345221551909145011457493619085780243761596511325807336205221239331976725970216671828618445898719026692884939342314733566");
    Fp_sike::nqr = Fp_sike("5");
    Fp_sike::nqr_to_t = Fp_sike("24439423661345221551909145011457493619085780243761596511325807336205221239331976725970216671828618445898719026692884939342314733566");

    /* Set up R1CS */
    typedef Fp_sike FieldT;
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

}
