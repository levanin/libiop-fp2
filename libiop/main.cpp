//
// Created by kcong on 21/12/22.
//

#include <chrono>
#include "cxxopts.hpp"
#include "libiop/snark/aurora_snark.hpp"
#include "libiop/snark/ligero_snark.hpp"
#include "libiop/relations/examples/r1cs_examples.hpp"
#include "libiop/algebra/isogeny_field.hpp"

// taken from https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
unsigned int next_pow2(unsigned int v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

namespace libiop {
    template<typename FieldT>
    void aurora_isogeny_pok(size_t m, size_t n, size_t secpar) {
        libff::inhibit_profiling_counters = true;
        typedef binary_hash_digest hash_type;

        const size_t num_constraints = next_pow2(m);
        const size_t num_variables = next_pow2(n)-1;
        // n is num_variables + num_inputs, so we just set num_inputs = 1
        const size_t num_inputs = 1;
        const size_t security_parameter = secpar;
        const size_t RS_extra_dimensions = 2;
        const size_t FRI_localization_parameter = 3;
        const LDT_reducer_soundness_type ldt_reducer_soundness_type = LDT_reducer_soundness_type::optimistic_heuristic;
        const FRI_soundness_type fri_soundness_type = FRI_soundness_type::heuristic;
        const field_subset_type domain_type = multiplicative_coset_type;

        r1cs_example<FieldT> r1cs_params = generate_r1cs_example<FieldT>(
                num_constraints, num_inputs, num_variables);
        if (!r1cs_params.constraint_system_.is_satisfied(
                r1cs_params.primary_input_, r1cs_params.auxiliary_input_)) {
            throw std::runtime_error("r1cs not satisfied");
        }

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
        auto prover_dur = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - prover_start);

        auto verifier_start = std::chrono::high_resolution_clock::now();
        const bool bit = aurora_snark_verifier<FieldT>(
                r1cs_params.constraint_system_,
                r1cs_params.primary_input_,
                argument,
                params);
        if (!bit) {
            throw std::runtime_error("failed");
        }

        printf("iop size in bytes %lu\n", argument.IOP_size_in_bytes());
        printf("bcs size in bytes %lu\n", argument.BCS_size_in_bytes());
        printf("argument size in bytes %lu\n", argument.size_in_bytes());

        auto verifier_dur = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - verifier_start);
        std::cout << "prover time (ms): " << prover_dur.count() << std::endl;
        std::cout << "verifier time (ms): " << verifier_dur.count() << std::endl;

        // we put a prefix in front so that it can be parsed later on to a csv
        std::cout << "====>" << prover_dur.count() << "," << verifier_dur.count() << "," << argument.size_in_bytes() << std::endl;
    }

    template<typename FieldT>
    void ligero_isogeny_pok(size_t m, size_t n, size_t secpar) {
        // n is num_variables + num_inputs, so we just set num_inputs = 1
        const size_t num_constraints = m;
        const size_t num_variables = n;
        const size_t num_inputs = 1;
        std::size_t constraint_dim = 302; // what is this?
        r1cs_example<FieldT> ex = generate_r1cs_example<FieldT>(num_constraints, num_inputs, num_variables);

        r1cs_constraint_system<FieldT> constraints = ex.constraint_system_;
        r1cs_primary_input<FieldT> primary_input = ex.primary_input_;
        r1cs_auxiliary_input<FieldT> auxiliary_input = ex.auxiliary_input_;

        if (!constraints.is_satisfied(primary_input, auxiliary_input)) {
            throw std::runtime_error("r1cs not satisfied");
        }

        /* Actual SNARK test */
        ligero_snark_parameters<FieldT, binary_hash_digest> parameters;
        parameters.security_level_ = secpar;
        parameters.height_width_ratio_ = 0.001;
        parameters.RS_extra_dimensions_ = 2;
        parameters.make_zk_ = true;
        parameters.domain_type_ = multiplicative_coset_type;
        parameters.LDT_reducer_soundness_type_ = LDT_reducer_soundness_type::proven;
        parameters.bcs_params_ = default_bcs_params<FieldT, binary_hash_digest>(
                blake2b_type, parameters.security_level_, constraint_dim);

        auto prover_start = std::chrono::high_resolution_clock::now();
        const ligero_snark_argument<FieldT, binary_hash_digest> argument =
                ligero_snark_prover<FieldT, binary_hash_digest>(constraints, primary_input, auxiliary_input, parameters);
        auto prover_dur = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - prover_start);

        auto verifier_start = std::chrono::high_resolution_clock::now();
        const bool bit = ligero_snark_verifier<FieldT, binary_hash_digest>(constraints, primary_input, argument, parameters);
        if (!bit) {
            throw std::runtime_error("failed");
        }

        printf("iop size in bytes %lu\n", argument.IOP_size_in_bytes());
        printf("bcs size in bytes %lu\n", argument.BCS_size_in_bytes());
        printf("argument size in bytes %lu\n", argument.size_in_bytes());

        auto verifier_dur = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - verifier_start);
        std::cout << "prover time (ms): " << prover_dur.count() << std::endl;
        std::cout << "verifier time (ms): " << verifier_dur.count() << std::endl;

        // we put a prefix in front so that it can be parsed later on to a csv
        std::cout << "====>" << prover_dur.count() << "," << verifier_dur.count() << "," << argument.size_in_bytes() << std::endl;
    }
}

int main(int argc, const char* argv[]) {
    cxxopts::Options options("main", "Isogeny zkSNARK simulator");
    options.add_options()
            ("proof_system", "the underlying proof system pick from {aurora,ligero}", cxxopts::value<std::string>()->default_value("aurora"))
            ("base_field", "the base field of the circuit in the format p{num_bits}{x,+,+x,}", cxxopts::value<std::string>()->default_value("p434x"))
            ("m", "number of constraints", cxxopts::value<size_t>()->default_value("1024"))
            ("n", "number of variables", cxxopts::value<size_t>()->default_value("1023"))
            ("h,help", "print usage")
            ;

    auto result = options.parse(argc, argv);
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }
    auto proof_system = result["proof_system"].as<std::string>();
    auto base_field = result["base_field"].as<std::string>();
    auto m = result["m"].as<size_t>();
    auto n = result["n"].as<size_t>();

    if ("aurora" == proof_system) {
        if ("p434x" == base_field) {
            libiop::p434x::init_params();
            typedef libiop::p434x::Fp2 FieldT;
            libiop::aurora_isogeny_pok<FieldT>(m, n, 128);
        } else if ("p441+" == base_field) {
            libiop::p441plus::init_params();
            typedef libiop::p441plus::Fp FieldT;
            libiop::aurora_isogeny_pok<FieldT>(m, n, 128);
        } else if ("p503x" == base_field) {
            libiop::p503x::init_params();
            typedef libiop::p503x::Fp2 FieldT;
            libiop::aurora_isogeny_pok<FieldT>(m, n, 128);
        } else if ("p509+" == base_field) {
            libiop::p509_plus::init_params();
            typedef libiop::p509_plus::Fp FieldT;
            libiop::aurora_isogeny_pok<FieldT>(m, n, 128);
        } else if ("p610x" == base_field) {
            libiop::p610x::init_params();
            typedef libiop::p610x::Fp2 FieldT;
            libiop::aurora_isogeny_pok<FieldT>(m, n, 192);
        } else if ("p619+" == base_field) {
            libiop::p619plus::init_params();
            typedef libiop::p619plus::Fp FieldT;
            libiop::aurora_isogeny_pok<FieldT>(m, n, 192);
        } else if ("p751x" == base_field) {
            libiop::p751x::init_params();
            typedef libiop::p751x::Fp2 FieldT;
            libiop::aurora_isogeny_pok<FieldT>(m, n, 256);
        } else if ("p761+" == base_field) {
            libiop::p761plus::init_params();
            typedef libiop::p761plus::Fp FieldT;
            libiop::aurora_isogeny_pok<FieldT>(m, n, 256);
        } else {
            std::cerr << "invalid base_field: " << base_field << std::endl;
            std::cerr << options.help() << std::endl;
            return 1;
        }

    } else if ("ligero" == proof_system) {
        if ("p434x" == base_field) {
            libiop::p434x::init_params();
            typedef libiop::p434x::Fp2 FieldT;
            libiop::ligero_isogeny_pok<FieldT>(m, n, 128);
        } else if ("p441+" == base_field) {
            libiop::p441plus::init_params();
            typedef libiop::p441plus::Fp FieldT;
            libiop::ligero_isogeny_pok<FieldT>(m, n, 128);
        } else if ("p503x" == base_field) {
            libiop::p503x::init_params();
            typedef libiop::p503x::Fp2 FieldT;
            libiop::ligero_isogeny_pok<FieldT>(m, n, 128);
        } else if ("p509+" == base_field) {
            libiop::p509_plus::init_params();
            typedef libiop::p509_plus::Fp FieldT;
            libiop::ligero_isogeny_pok<FieldT>(m, n, 128);
        } else if ("p610x" == base_field) {
            libiop::p610x::init_params();
            typedef libiop::p610x::Fp2 FieldT;
            libiop::ligero_isogeny_pok<FieldT>(m, n, 192);
        } else if ("p619+" == base_field) {
            libiop::p619plus::init_params();
            typedef libiop::p619plus::Fp FieldT;
            libiop::ligero_isogeny_pok<FieldT>(m, n, 192);
        } else if ("p751x" == base_field) {
            libiop::p751x::init_params();
            typedef libiop::p751x::Fp2 FieldT;
            libiop::ligero_isogeny_pok<FieldT>(m, n, 256);
        } else if ("p761+" == base_field) {
            libiop::p761plus::init_params();
            typedef libiop::p761plus::Fp FieldT;
            libiop::ligero_isogeny_pok<FieldT>(m, n, 256);
        } else {
            std::cerr << "invalid base_field: " << base_field << std::endl;
            std::cerr << options.help() << std::endl;
            return 1;
        }
    } else {
        std::cerr << "invalid proof_system: " << proof_system << std::endl;
        std::cerr << options.help() << std::endl;
        return 1;
    }
    return 0;
}

