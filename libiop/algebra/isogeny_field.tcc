
#include "libiop/algebra/isogeny_field.hpp"
namespace libiop {
    namespace p434_smooth {
        void init_params() {
            assert(Fp::modulus_is_valid());
            Fp::Rsquared = libff::bigint<limbs>("11670842892826704282446807045015068798005654319470624232167485311695550996073777036812919341238917739503302545896714443026026871");
            Fp::Rcubed = libff::bigint<limbs>("22698988592753557634872139742080783794550501862711300293198750092029709806281700938341229976599779072917632061262364142479875440");
            Fp::inv = 0xffffffffffffffff;

            Fp::num_bits = 425;
            Fp::euler = libff::bigint<limbs>("35799937003923664382679411637877187918582685903947651139637413090144367049802700282182934577873952801609451699257155672864718848");
            Fp::s = 206;
            Fp::t = libff::bigint<limbs>("696198609130885597695136021593547814689632716312296141651066450089");
            Fp::t_minus_1_over_2 = libff::bigint<limbs>("348099304565442798847568010796773907344816358156148070825533225044");
            Fp::multiplicative_generator = Fp("5");
            Fp::root_of_unity = Fp("45088048282459687398485922424312487749745200896454603205929219968713742705532171884364168308847501716494182411630221149581149930");
            Fp::nqr = Fp("5");
            Fp::nqr_to_t = Fp("45088048282459687398485922424312487749745200896454603205929219968713742705532171884364168308847501716494182411630221149581149930");
        }
    }

    namespace p434 {
        void init_params() {
            assert(Fp::modulus_is_valid());
            Fp::Rsquared = libff::bigint<limbs>("1631483357165550719083731178910934187933895301703355372962834564740025113443660361043011370056616562298461884328920157440580164400");
            Fp::Rcubed = libff::bigint<limbs>("20837450692795383045350192246862524190972469314298443645963536460875107178404868644875693185558066690330093003204714510105284771771");
            Fp::inv = 1;

            Fp::num_bits = 434;
            Fp::euler = libff::bigint<limbs>("12219711830672610775954572505728746809542890121880798255662903668102610619665988362985108335914309222949359513346442469671157366783");
            Fp::s = 1;
            Fp::t = libff::bigint<limbs>("12219711830672610775954572505728746809542890121880798255662903668102610619665988362985108335914309222949359513346442469671157366783");
            Fp::t_minus_1_over_2 = libff::bigint<limbs>("6109855915336305387977286252864373404771445060940399127831451834051305309832994181492554167957154611474679756673221234835578683391");
            Fp::multiplicative_generator = Fp("5");
            Fp::root_of_unity = Fp("24439423661345221551909145011457493619085780243761596511325807336205221239331976725970216671828618445898719026692884939342314733566");
            Fp::nqr = Fp("5");
            Fp::nqr_to_t = Fp("24439423661345221551909145011457493619085780243761596511325807336205221239331976725970216671828618445898719026692884939342314733566");

            Fp2::euler = libff::bigint<2*limbs>("298642714449360337224575087275245664901894079921715928781788348409667672019529337582670447826847024596057210520043080363986618507866176274886156910695833198302828076578047080033097835019328286911395069210577513782278068741715790835485329586961735821523098271744");
            Fp2::s = 217;
            Fp2::t = libff::bigint<2*limbs>("2835782126831499814412631155210271063040201440463777749910388691755984945436839811256578195741029548769359524354538623156654328717538058679510949167866459989372424135823124274639652795496245331229");
            Fp2::t_minus_1_over_2 = libff::bigint<2*limbs>("1417891063415749907206315577605135531520100720231888874955194345877992472718419905628289097870514774384679762177269311578327164358769029339755474583933229994686212067911562137319826397748122665614");
            Fp2::non_residue = Fp("24439423661345221551909145011457493619085780243761596511325807336205221239331976725970216671828618445898719026692884939342314733566");
            Fp2::nqr = Fp2(Fp("2"),Fp("1"));
            Fp2::nqr_to_t = Fp2(Fp("1265641318332283385680345622106262255016986152776970677011496306402637098857925419143401023483759358905674114613032730660970188006"),Fp("24302508882629253666484860493871125283132764152316444477996794014562063591984863827165794089737987196964883650291668738548192491206"));
            Fp2::multiplicative_generator = Fp2(Fp("2"),Fp("1"));
            Fp2::root_of_unity = Fp2(Fp("1265641318332283385680345622106262255016986152776970677011496306402637098857925419143401023483759358905674114613032730660970188006"), Fp("24302508882629253666484860493871125283132764152316444477996794014562063591984863827165794089737987196964883650291668738548192491206"));
            Fp2::Frobenius_coeffs_c1[0] = Fp("1");
            Fp2::Frobenius_coeffs_c1[1] = Fp("24439423661345221551909145011457493619085780243761596511325807336205221239331976725970216671828618445898719026692884939342314733566");
            Fp2::simplify_mul = true;
        }
    }

    namespace p503 {
        void init_params() {
            assert(Fp::modulus_is_valid());
            Fp::Rsquared = libff::bigint<limbs>("12976075140998535365735219559849701164099730123828701692985583627477611067833607704803387901886029477717925760778691439352068370394284762170373938413855");
            Fp::Rcubed = libff::bigint<limbs>("231399425989808434660747075406980822724639292886781794882634872409040054653867377273252997862206343775066219860879710302204984764909315619815206723892");
            Fp::inv = 1;

            Fp::num_bits = 503;
            Fp::euler = libff::bigint<limbs>("6587921578453558690419626458099672521246093383789181999222831738517921966010380616759457455773012175804303575195043828491491153165509796980577118715903");
            Fp::s = 1;
            Fp::t = libff::bigint<limbs>("6587921578453558690419626458099672521246093383789181999222831738517921966010380616759457455773012175804303575195043828491491153165509796980577118715903");
            Fp::t_minus_1_over_2 = libff::bigint<limbs>("3293960789226779345209813229049836260623046691894590999611415869258960983005190308379728727886506087902151787597521914245745576582754898490288559357951");
            Fp::multiplicative_generator = Fp("5");
            Fp::root_of_unity = Fp("13175843156907117380839252916199345042492186767578363998445663477035843932020761233518914911546024351608607150390087656982982306331019593961154237431806");
            Fp::nqr = Fp("5");
            Fp::nqr_to_t = Fp("13175843156907117380839252916199345042492186767578363998445663477035843932020761233518914911546024351608607150390087656982982306331019593961154237431806");

            Fp2::euler = libff::bigint<2*limbs>("86801421447708056502430947043368515016655490366407203549895025131628103882231140719581972897823599178342074026504186696955540219238880180908202547973391062648254313611197165505740626283012973189342234581186687553300149058544370294937983602903115864356011012197104999884872053752852461110212209487642624");
            Fp2::s = 251;
            Fp2::t = libff::bigint<2*limbs>("47976429212428596615557699922605110835642327878653818435563311722969848476269480040370024752668016348704709804439083422206556915900569519458053665923190650630097698805103025358021652016961912989244552791646104869196050867820501");
            Fp2::t_minus_1_over_2 = libff::bigint<2*limbs>("23988214606214298307778849961302555417821163939326909217781655861484924238134740020185012376334008174352354902219541711103278457950284759729026832961595325315048849402551512679010826008480956494622276395823052434598025433910250");
            Fp2::non_residue = Fp("13175843156907117380839252916199345042492186767578363998445663477035843932020761233518914911546024351608607150390087656982982306331019593961154237431806");
            Fp2::nqr = Fp2(Fp("2"),Fp("1"));
            Fp2::nqr_to_t = Fp2(Fp("2435735179550318590245463363562321141454105318405650435028859364318113233385266633876699051357677340558201240537300922875178445638724899040440618013061"),Fp("11339065558968619902944260837868108008534688818314107442835444690309754367161465830461046798750633364806133241737639049543328658704310330170580729108176"));
            Fp2::multiplicative_generator = Fp2(Fp("2"),Fp("1"));
            Fp2::root_of_unity = Fp2(Fp("2435735179550318590245463363562321141454105318405650435028859364318113233385266633876699051357677340558201240537300922875178445638724899040440618013061"), Fp("11339065558968619902944260837868108008534688818314107442835444690309754367161465830461046798750633364806133241737639049543328658704310330170580729108176"));
            Fp2::Frobenius_coeffs_c1[0] = Fp("1");
            Fp2::Frobenius_coeffs_c1[1] = Fp("13175843156907117380839252916199345042492186767578363998445663477035843932020761233518914911546024351608607150390087656982982306331019593961154237431806");
            Fp2::simplify_mul = true;
        }
    }
}