/**@file
 *****************************************************************************
 Implementation of useful operations over vectors.
 *****************************************************************************
 * @author     This file is part of libiop (see AUTHORS)
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/
#ifndef LIBIOP_ALGEBRA_LARGE_FIELD_HPP_
#define LIBIOP_ALGEBRA_LARGE_FIELD_HPP_

#include <libff/algebra/fields/prime_base/fp.hpp>

namespace libiop {
    namespace sidh_field {
        const mp_size_t limbs = 7;
        // p = 2^206 * 3^138 + 1
        libff::bigint<limbs> modulus("71599874007847328765358823275754375837165371807895302279274826180288734099605400564365869155747905603218903398514311345729437697");
        typedef libff::Fp_model<limbs, modulus> Fp;
        void init_params();
    }
} // namespace libiop

#include "libiop/algebra/large_field.tcc"

#endif // LIBIOP_ALGEBRA_LARGE_FIELD_HPP_
