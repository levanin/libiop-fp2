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
    namespace p441_plus {
        const mp_size_t limbs = 7;
        libff::bigint<limbs> modulus("10851104105637278369047660385087127166874086428230148851028658457275118230263397666330776202291906589979031247851640913067987741704193");
        typedef libff::Fp_model<limbs, modulus> Fp;
        void init_params();
    }

    namespace p434 {
        const mp_size_t limbs = 7;
        libff::bigint<limbs> modulus("24439423661345221551909145011457493619085780243761596511325807336205221239331976725970216671828618445898719026692884939342314733567");
        typedef libff::Fp_model<limbs, modulus> Fp;
        typedef libff::Fp2_model<limbs, modulus> Fp2;
        void init_params();
    }

    namespace p503 {
        const mp_size_t limbs = 8;
        libff::bigint<limbs> modulus("13175843156907117380839252916199345042492186767578363998445663477035843932020761233518914911546024351608607150390087656982982306331019593961154237431807");
        typedef libff::Fp_model<limbs, modulus> Fp;
        typedef libff::Fp2_model<limbs, modulus> Fp2;
        void init_params();
    }

    namespace p610 {
        const mp_size_t limbs = 10;
        libff::bigint<limbs> modulus("2638940411073262671963620699288286770183560231187222316750407556465639836010558150163225530335162533481049256757217964651333810422125728537407397155806079217346919294449255613110157311");
        typedef libff::Fp_model<limbs, modulus> Fp;
        typedef libff::Fp2_model<limbs, modulus> Fp2;
        void init_params();
    }

    namespace p751 {
        const mp_size_t limbs = 12;
        libff::bigint<limbs> modulus("10354717741769305252977768237866805321427389645549071170116189679054678940682478846502882896561066713624553211618840202385203911976522554393044160468771151816976706840078913334358399730952774926980235086850991501872665651576831");
        typedef libff::Fp_model<limbs, modulus> Fp;
        typedef libff::Fp2_model<limbs, modulus> Fp2;
        void init_params();
    }

} // namespace libiop

#include "libiop/algebra/isogeny_field.tcc"

#endif // LIBIOP_ALGEBRA_LARGE_FIELD_HPP_
