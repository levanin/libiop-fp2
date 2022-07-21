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
// p = 2^206 * 3^138 + 1
libff::bigint<sike_limbs> sike_modulus("71599874007847328765358823275754375837165371807895302279274826180288734099605400564365869155747905603218903398514311345729437697");
typedef libff::Fp_model<sike_limbs, sike_modulus> Fp_sike;

TEST(AuroraSnarkLargeFieldTest, SimpleTest) {
    assert(Fp_sike::modulus_is_valid());
    Fp_sike::Rsquared = libff::bigint<sike_limbs>("11670842892826704282446807045015068798005654319470624232167485311695550996073777036812919341238917739503302545896714443026026871");
    Fp_sike::Rcubed = libff::bigint<sike_limbs>("22698988592753557634872139742080783794550501862711300293198750092029709806281700938341229976599779072917632061262364142479875440");
    Fp_sike::inv = 0xffffffffffffffff;

    Fp_sike::num_bits = 425;
    Fp_sike::euler = libff::bigint<sike_limbs>("35799937003923664382679411637877187918582685903947651139637413090144367049802700282182934577873952801609451699257155672864718848");
    Fp_sike::s = 206;
    Fp_sike::t = libff::bigint<sike_limbs>("696198609130885597695136021593547814689632716312296141651066450089");
    Fp_sike::t_minus_1_over_2 = libff::bigint<sike_limbs>("348099304565442798847568010796773907344816358156148070825533225044");
    Fp_sike::multiplicative_generator = Fp_sike("5");
    Fp_sike::root_of_unity = Fp_sike("45088048282459687398485922424312487749745200896454603205929219968713742705532171884364168308847501716494182411630221149581149930");
    Fp_sike::nqr = Fp_sike("5");
    Fp_sike::nqr_to_t = Fp_sike("45088048282459687398485922424312487749745200896454603205929219968713742705532171884364168308847501716494182411630221149581149930");

    /* Set up R1CS */
    typedef Fp_sike FieldT;
    typedef binary_hash_digest hash_type;

    const size_t num_constraints = 1 << 15;
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
