/**@file
 *****************************************************************************
 Large primes (> 400 bits) that have smooth multiplicative order.
 *****************************************************************************
 * @author     This file is part of libiop (see AUTHORS)
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/
#ifndef LIBIOP_ALGEBRA_LARGE_FIELD_HPP_
#define LIBIOP_ALGEBRA_LARGE_FIELD_HPP_

#include <libff/algebra/fields/prime_base/fp.hpp>
#include <libff/algebra/fields/prime_extension/fp2.hpp>

namespace libiop {
    namespace sidh_extra {
        const mp_size_t limbs = 7;
        // p = 2^206 * 3^138 + 1
        libff::bigint<limbs> modulus("71599874007847328765358823275754375837165371807895302279274826180288734099605400564365869155747905603218903398514311345729437697");
        typedef libff::Fp_model<limbs, modulus> Fp;
        void init_params();
    }

    namespace sidh {
        const mp_size_t limbs = 7;
        libff::bigint<limbs> modulus("24439423661345221551909145011457493619085780243761596511325807336205221239331976725970216671828618445898719026692884939342314733567");
        typedef libff::Fp_model<limbs, modulus> Fp;
        typedef libff::Fp2_model<limbs, modulus> Fp2;
        void init_params();
    }

} // namespace libiop

#include "libiop/algebra/large_field.tcc"

#endif // LIBIOP_ALGEBRA_LARGE_FIELD_HPP_
