/*
  +----------------------------------------------------------------------+
  | PHP Version 7 Swiss Ephemeris extension                              |
  +----------------------------------------------------------------------+
  | Copyright (c) 2007-2021                                              |
  +----------------------------------------------------------------------+
  | Author: Joel Chen (cyjoelchen@gmail.com)                             |
  | Contributor: Alois Treindl
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_swephp.h"

#include "swephexp.h"

#define SWEPH_EXTENSION_VERSION "2.0 Rev: 29"

#if PHP_MAJOR_VERSION < 8
# define IS_PHP7	1
#else
# define IS_PHP7	0
#endif

/* If you declare any globals in php_swephp.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(swephp)
*/

/* True global resources - no need for thread safety here */
static int le_swephp;

/* {{{ swephp_functions[]
 *
 * Every user visible function must have an entry in swephp_functions[].
 */
zend_function_entry swephp_functions[] = {
	/**************************** 
	 * exports from sweph.c 
	 ****************************/
	PHP_FE(swe_calc, NULL)
	PHP_FE(swe_calc_ut, NULL)
	PHP_FE(swe_calc_pctr, NULL)
	PHP_FE(swe_fixstar, NULL)
	PHP_FE(swe_fixstar_ut, NULL)
	PHP_FE(swe_fixstar_mag, NULL)
	PHP_FE(swe_fixstar2, NULL)
	PHP_FE(swe_fixstar2_ut, NULL)
	PHP_FE(swe_fixstar2_mag, NULL)
	PHP_FE(swe_close, NULL)
	PHP_FE(swe_set_ephe_path, NULL)
	PHP_FE(swe_set_jpl_file, NULL)
	PHP_FE(swe_get_planet_name, NULL)
	PHP_FE(swe_set_topo, NULL)
	PHP_FE(swe_set_sid_mode, NULL)
	PHP_FE(swe_get_ayanamsa, NULL)
	PHP_FE(swe_get_ayanamsa_ut, NULL)
	PHP_FE(swe_get_ayanamsa_ex, NULL)
	PHP_FE(swe_get_ayanamsa_ex_ut, NULL)
	PHP_FE(swe_get_ayanamsa_name, NULL)
	PHP_FE(swe_version, NULL)
	PHP_FE(swe_get_library_path, NULL)
	PHP_FE(swe_get_current_file_data, NULL)

	/**************************** 
	 * exports from swedate.c 
	 ****************************/
	PHP_FE(swe_date_conversion, NULL)
	PHP_FE(swe_julday, NULL)
	PHP_FE(swe_revjul, NULL)
	PHP_FE(swe_jdet_to_utc, NULL)
	PHP_FE(swe_jdut1_to_utc, NULL)
	PHP_FE(swe_utc_to_jd, NULL)
	PHP_FE(swe_utc_time_zone, NULL)
	
	/**************************** 
	 * exports from swehouse.c 
	 ****************************/
	PHP_FE(swe_houses, NULL)
	PHP_FE(swe_houses_ex, NULL)
	PHP_FE(swe_houses_ex2, NULL)
	PHP_FE(swe_houses_armc, NULL)
	PHP_FE(swe_houses_armc_ex2, NULL)
	PHP_FE(swe_house_pos, NULL)
	PHP_FE(swe_gauquelin_sector, NULL)
	PHP_FE(swe_house_name, NULL)
	
	/**************************** 
	 * exports from swecl.c 
	 ****************************/
	PHP_FE(swe_sol_eclipse_where, NULL)
	PHP_FE(swe_lun_occult_where, NULL)
	PHP_FE(swe_sol_eclipse_how, NULL)
	PHP_FE(swe_sol_eclipse_when_loc, NULL)
	PHP_FE(swe_lun_occult_when_loc, NULL)
	PHP_FE(swe_sol_eclipse_when_glob, NULL)
	PHP_FE(swe_lun_occult_when_glob, NULL)
	PHP_FE(swe_lun_eclipse_how, NULL)
	PHP_FE(swe_lun_eclipse_when, NULL)	
	PHP_FE(swe_lun_eclipse_when_loc, NULL)
	PHP_FE(swe_pheno, NULL)
	PHP_FE(swe_pheno_ut, NULL)
	PHP_FE(swe_refrac, NULL)
	PHP_FE(swe_refrac_extended, NULL)
	PHP_FE(swe_set_lapse_rate, NULL)
	PHP_FE(swe_azalt, NULL)
	PHP_FE(swe_azalt_rev, NULL)
	PHP_FE(swe_rise_trans, NULL)
	PHP_FE(swe_rise_trans_true_hor, NULL)
	PHP_FE(swe_nod_aps, NULL)
	PHP_FE(swe_nod_aps_ut, NULL)
	PHP_FE(swe_get_orbital_elements, NULL)
	PHP_FE(swe_orbit_max_min_true_distance, NULL)
	PHP_FE(swe_heliacal_ut, NULL)
	PHP_FE(swe_heliacal_pheno_ut, NULL)
	PHP_FE(swe_vis_limit_mag, NULL)
		
	/**************************** 
	 * exports from swephlib.c 
	 ****************************/
	PHP_FE(swe_deltat, NULL)
	PHP_FE(swe_deltat_ex, NULL)
	PHP_FE(swe_time_equ, NULL)
	PHP_FE(swe_lmt_to_lat, NULL)
	PHP_FE(swe_lat_to_lmt, NULL)
	PHP_FE(swe_sidtime0, NULL)
	PHP_FE(swe_sidtime, NULL)
	PHP_FE(swe_cotrans, NULL)
	PHP_FE(swe_cotrans_sp, NULL)
	PHP_FE(swe_get_tid_acc, NULL)
	PHP_FE(swe_set_tid_acc, NULL)
	PHP_FE(swe_set_delta_t_userdef, NULL)
	PHP_FE(swe_degnorm, NULL)
	PHP_FE(swe_radnorm, NULL)
	PHP_FE(swe_rad_midp, NULL)
	PHP_FE(swe_deg_midp, NULL)
	PHP_FE(swe_split_deg, NULL)

	/******************************************************* 
	 * other functions from swephlib.c;
	 * they are not needed for Swiss Ephemeris,
	 * but may be useful to former Placalc users.
	 ********************************************************/
	PHP_FE(swe_csnorm, NULL)
	PHP_FE(swe_difcsn, NULL)
	PHP_FE(swe_difdegn, NULL)
	PHP_FE(swe_difcs2n, NULL)
	PHP_FE(swe_difdeg2n, NULL)
	PHP_FE(swe_difrad2n, NULL)
	PHP_FE(swe_csroundsec, NULL)
	PHP_FE(swe_d2l, NULL)
	PHP_FE(swe_day_of_week, NULL)
	PHP_FE(swe_cs2timestr, NULL)
	PHP_FE(swe_cs2lonlatstr, NULL)
	PHP_FE(swe_cs2degstr, NULL)

//	PHP_FE(confirm_sweph_compiled,	NULL)		/* For testing, remove later. */
	{NULL, NULL, NULL}	/* Must be the last line in swephp_functions[] */
};
/* }}} */

/* {{{ swephp_module_entry
 */
zend_module_entry swephp_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"swephp",
	swephp_functions,
	PHP_MINIT(swephp),
	NULL, //PHP_MSHUTDOWN(swephp),
	NULL, //PHP_RINIT(swephp),		/* Replace with NULL if there's nothing to do at request start */
	NULL, //PHP_RSHUTDOWN(swephp),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(swephp),
#if ZEND_MODULE_API_NO >= 20010901
	SWEPH_EXTENSION_VERSION, 
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SWEPHP
ZEND_GET_MODULE(swephp)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("swephp.global_value",      "42", PHP_INI_ALL, OnUpdateInt, global_value, zend_swephp_globals, swephp_globals)
    STD_PHP_INI_ENTRY("swephp.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_swephp_globals, swephp_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_swephp_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_swephp_init_globals(zend_swephp_globals *swephp_globals)
{
	swephp_globals->global_value = 0;
	swephp_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(swephp)
{
	/* If you have INI entries, uncomment these lines 
	ZEND_INIT_MODULE_GLOBALS(swephp, php_swephp_init_globals, NULL);
	REGISTER_INI_ENTRIES();
	*/

	/* values for gregflag in swe_julday() and swe_revjul() */
	
	REGISTER_LONG_CONSTANT("SE_JUL_CAL", SE_JUL_CAL, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_GREG_CAL", SE_GREG_CAL, CONST_CS | CONST_PERSISTENT);
	
	/*
	 * planet numbers for the ipl parameter in swe_calc()
	 */
	REGISTER_LONG_CONSTANT("SE_ECL_NUT", SE_ECL_NUT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SUN", SE_SUN, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_MOON", SE_MOON, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_MERCURY", SE_MERCURY  , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_VENUS", SE_VENUS    , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_MARS", SE_MARS     , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_JUPITER", SE_JUPITER  , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SATURN", SE_SATURN   , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_URANUS", SE_URANUS   , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_NEPTUNE", SE_NEPTUNE  , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_PLUTO", SE_PLUTO    , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_MEAN_NODE", SE_MEAN_NODE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_TRUE_NODE", SE_TRUE_NODE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_MEAN_APOG", SE_MEAN_APOG, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_OSCU_APOG", SE_OSCU_APOG, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_EARTH", SE_EARTH    , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_CHIRON", SE_CHIRON   , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_PHOLUS", SE_PHOLUS   , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_CERES", SE_CERES    , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_PALLAS", SE_PALLAS   , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_JUNO", SE_JUNO     , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_VESTA", SE_VESTA    , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_INTP_APOG", SE_INTP_APOG, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_INTP_PERG", SE_INTP_PERG, CONST_CS | CONST_PERSISTENT);
	
	REGISTER_LONG_CONSTANT("SE_NPLANETS", SE_NPLANETS, CONST_CS | CONST_PERSISTENT);
	
	REGISTER_LONG_CONSTANT("SE_AST_OFFSET", SE_AST_OFFSET, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_VARUNA", SE_VARUNA, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_FICT_OFFSET", SE_FICT_OFFSET, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_FICT_OFFSET_1", SE_FICT_OFFSET_1, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_FICT_MAX", SE_FICT_MAX, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_NFICT_ELEM", SE_NFICT_ELEM, CONST_CS | CONST_PERSISTENT);

	REGISTER_LONG_CONSTANT("SE_COMET_OFFSET", SE_COMET_OFFSET, CONST_CS | CONST_PERSISTENT);

	REGISTER_LONG_CONSTANT("SE_NALL_NAT_POINTS", SE_NALL_NAT_POINTS, CONST_CS | CONST_PERSISTENT);

	/* Hamburger or Uranian"planets" */
	REGISTER_LONG_CONSTANT("SE_CUPIDO", SE_CUPIDO  , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_HADES", SE_HADES   , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_ZEUS", SE_ZEUS    , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_KRONOS", SE_KRONOS  , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_APOLLON", SE_APOLLON , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_ADMETOS", SE_ADMETOS , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_VULKANUS", SE_VULKANUS, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_POSEIDON", SE_POSEIDON, CONST_CS | CONST_PERSISTENT);

	/* other fictitious bodies */
	REGISTER_LONG_CONSTANT("SE_ISIS", SE_ISIS         	    , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_NIBIRU", SE_NIBIRU       	    , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_HARRINGTON", SE_HARRINGTON        , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_NEPTUNE_LEVERRIER", SE_NEPTUNE_LEVERRIER , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_NEPTUNE_ADAMS", SE_NEPTUNE_ADAMS     , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_PLUTO_LOWELL", SE_PLUTO_LOWELL      , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_PLUTO_PICKERING", SE_PLUTO_PICKERING   , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_VULCAN      		", SE_VULCAN      		, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_WHITE_MOON", SE_WHITE_MOON  		, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_PROSERPINA", SE_PROSERPINA  		, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_WALDEMATH", SE_WALDEMATH  		, CONST_CS | CONST_PERSISTENT);


	REGISTER_LONG_CONSTANT("SE_FIXSTAR", SE_FIXSTAR, CONST_CS | CONST_PERSISTENT);

	REGISTER_LONG_CONSTANT("SE_ASC", SE_ASC		, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_MC", SE_MC		, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_ARMC", SE_ARMC		, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_VERTEX", SE_VERTEX    , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_EQUASC", SE_EQUASC    , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_COASC1", SE_COASC1    , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_COASC2", SE_COASC2    , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_POLASC", SE_POLASC    , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_NASCMC", SE_NASCMC    , CONST_CS | CONST_PERSISTENT);

	/* flag bits for parameter iflag in function swe_calc() */

	REGISTER_LONG_CONSTANT("SEFLG_JPLEPH", SEFLG_JPLEPH, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SEFLG_SWIEPH", SEFLG_SWIEPH, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SEFLG_MOSEPH", SEFLG_MOSEPH, CONST_CS | CONST_PERSISTENT);

	REGISTER_LONG_CONSTANT("SEFLG_HELCTR", SEFLG_HELCTR	    , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SEFLG_TRUEPOS", SEFLG_TRUEPOS	, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SEFLG_J2000", SEFLG_J2000		, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SEFLG_NONUT", SEFLG_NONUT		, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SEFLG_SPEED3", SEFLG_SPEED3	    , CONST_CS | CONST_PERSISTENT);

	REGISTER_LONG_CONSTANT("SEFLG_SPEED", SEFLG_SPEED		, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SEFLG_NOGDEFL", SEFLG_NOGDEFL	, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SEFLG_NOABERR", SEFLG_NOABERR	, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SEFLG_EQUATORIAL", SEFLG_EQUATORIAL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SEFLG_XYZ", SEFLG_XYZ		, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SEFLG_RADIANS", SEFLG_RADIANS	, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SEFLG_BARYCTR", SEFLG_BARYCTR	, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SEFLG_TOPOCTR", SEFLG_TOPOCTR	, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SEFLG_SIDEREAL", SEFLG_SIDEREAL	, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SEFLG_ICRS", SEFLG_ICRS	, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SEFLG_DPSIDEPS_1980", SEFLG_DPSIDEPS_1980	, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SEFLG_JPLHOR", SEFLG_JPLHOR	, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SEFLG_JPLHOR_APPROX", SEFLG_JPLHOR_APPROX	, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SEFLG_CENTER_BODY", SEFLG_CENTER_BODY	, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SEFLG_TEST_PLMOON", SEFLG_TEST_PLMOON	, CONST_CS | CONST_PERSISTENT);

	REGISTER_LONG_CONSTANT("SE_SIDBITS", SE_SIDBITS, CONST_CS | CONST_PERSISTENT);
	/* for projection onto ecliptic of t0 */
	REGISTER_LONG_CONSTANT("SE_SIDBIT_ECL_T0", SE_SIDBIT_ECL_T0, CONST_CS | CONST_PERSISTENT);
	/* for projection onto solar system plane */
	REGISTER_LONG_CONSTANT("SE_SIDBIT_SSY_PLANE", SE_SIDBIT_SSY_PLANE, CONST_CS | CONST_PERSISTENT);
	
	/* sidereal modes (ayanamsas) */
	REGISTER_LONG_CONSTANT("SE_SIDM_FAGAN_BRADLEY", SE_SIDM_FAGAN_BRADLEY  , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_LAHIRI", SE_SIDM_LAHIRI         , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_DELUCE", SE_SIDM_DELUCE         , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_RAMAN", SE_SIDM_RAMAN          , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_USHASHASHI", SE_SIDM_USHASHASHI     , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_KRISHNAMURTI", SE_SIDM_KRISHNAMURTI   , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_DJWHAL_KHUL", SE_SIDM_DJWHAL_KHUL    , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_YUKTESHWAR", SE_SIDM_YUKTESHWAR     , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_JN_BHASIN", SE_SIDM_JN_BHASIN      , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_BABYL_KUGLER1", SE_SIDM_BABYL_KUGLER1  , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_BABYL_KUGLER2", SE_SIDM_BABYL_KUGLER2  , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_BABYL_KUGLER3", SE_SIDM_BABYL_KUGLER3  , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_BABYL_HUBER", SE_SIDM_BABYL_HUBER    , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_BABYL_ETPSC", SE_SIDM_BABYL_ETPSC    , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_ALDEBARAN_15TAU", SE_SIDM_ALDEBARAN_15TAU, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_HIPPARCHOS", SE_SIDM_HIPPARCHOS     , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_SASSANIAN", SE_SIDM_SASSANIAN      , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_GALCENT_0SAG", SE_SIDM_GALCENT_0SAG   , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_J2000", SE_SIDM_J2000          , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_J1900", SE_SIDM_J1900          , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_B1950", SE_SIDM_B1950          , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_SURYASIDDHANTA", SE_SIDM_SURYASIDDHANTA          , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_SURYASIDDHANTA_MSUN", SE_SIDM_SURYASIDDHANTA_MSUN          , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_ARYABHATA", SE_SIDM_ARYABHATA          , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_ARYABHATA_MSUN", SE_SIDM_ARYABHATA_MSUN          , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_SS_REVATI", SE_SIDM_SS_REVATI          , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_SS_CITRA", SE_SIDM_SS_CITRA          , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_TRUE_CITRA", SE_SIDM_TRUE_CITRA          , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_TRUE_REVATI", SE_SIDM_TRUE_REVATI          , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_TRUE_PUSHYA", SE_SIDM_TRUE_PUSHYA, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_GALCENT_RGILBRAND", SE_SIDM_GALCENT_RGILBRAND, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_GALEQU_IAU1958", SE_SIDM_GALEQU_IAU1958, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SE_SIDM_GALEQU_TRUE", SE_SIDM_GALEQU_TRUE, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SE_SIDM_GALEQU_MULA", SE_SIDM_GALEQU_MULA, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SE_SIDM_GALALIGN_MARDYKS", SE_SIDM_GALALIGN_MARDYKS, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SE_SIDM_TRUE_MULA", SE_SIDM_TRUE_MULA, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SE_SIDM_GALCENT_MULA_WILHELM", SE_SIDM_GALCENT_MULA_WILHELM, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SE_SIDM_ARYABHATA_522", SE_SIDM_ARYABHATA_522, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SE_SIDM_BABYL_BRITTON", SE_SIDM_BABYL_BRITTON, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SE_SIDM_TRUE_SHEORAN", SE_SIDM_TRUE_SHEORAN, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SE_SIDM_GALCENT_COCHRANE", SE_SIDM_GALCENT_COCHRANE, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SE_SIDM_GALEQU_FIORENZA", SE_SIDM_GALEQU_FIORENZA, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SE_SIDM_VALENS_MOON", SE_SIDM_VALENS_MOON, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SE_SIDM_LAHIRI_1940", SE_SIDM_LAHIRI_1940, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SE_SIDM_LAHIRI_VP285", SE_SIDM_LAHIRI_VP285, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SE_SIDM_KRISHNAMURTI_VP291", SE_SIDM_KRISHNAMURTI_VP291, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("SE_SIDM_LAHIRI_ICRC", SE_SIDM_LAHIRI_ICRC, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_SIDM_USER", SE_SIDM_USER           , CONST_CS | CONST_PERSISTENT);

    /* For projection onto ecliptic of t0. */
	REGISTER_LONG_CONSTANT("SE_SIDBIT_ECL_DATE", SE_SIDBIT_ECL_DATE, CONST_CS | CONST_PERSISTENT);

	REGISTER_LONG_CONSTANT("SE_NSIDM_PREDEF", SE_NSIDM_PREDEF, CONST_CS | CONST_PERSISTENT);

	/* used for swe_nod_aps(): */
	REGISTER_LONG_CONSTANT("SE_NODBIT_MEAN", SE_NODBIT_MEAN		, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_NODBIT_OSCU", SE_NODBIT_OSCU		, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_NODBIT_OSCU_BAR", SE_NODBIT_OSCU_BAR	, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_NODBIT_FOPOINT", SE_NODBIT_FOPOINT	, CONST_CS | CONST_PERSISTENT);

	/* default ephemeris used when no ephemeris flagbit is set */
	REGISTER_LONG_CONSTANT("SEFLG_DEFAULTEPH", SEFLG_DEFAULTEPH, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_MAX_STNAME", SE_MAX_STNAME, CONST_CS | CONST_PERSISTENT);

	/* defines for eclipse computations */

	REGISTER_LONG_CONSTANT("SE_ECL_CENTRAL", SE_ECL_CENTRAL		, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_ECL_NONCENTRAL", SE_ECL_NONCENTRAL	, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_ECL_TOTAL", SE_ECL_TOTAL		    , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_ECL_ANNULAR", SE_ECL_ANNULAR		, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_ECL_PARTIAL", SE_ECL_PARTIAL		, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_ECL_ANNULAR_TOTAL", SE_ECL_ANNULAR_TOTAL , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_ECL_PENUMBRAL", SE_ECL_PENUMBRAL	    , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_ECL_VISIBLE", SE_ECL_VISIBLE		, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_ECL_MAX_VISIBLE", SE_ECL_MAX_VISIBLE	, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_ECL_1ST_VISIBLE", SE_ECL_1ST_VISIBLE	, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_ECL_2ND_VISIBLE", SE_ECL_2ND_VISIBLE	, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_ECL_3RD_VISIBLE", SE_ECL_3RD_VISIBLE	, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_ECL_4TH_VISIBLE", SE_ECL_4TH_VISIBLE	, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_ECL_ONE_TRY", SE_ECL_ONE_TRY       , CONST_CS | CONST_PERSISTENT);

	/* for swe_rise_transit() */
	REGISTER_LONG_CONSTANT("SE_CALC_RISE", SE_CALC_RISE		    , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_CALC_SET", SE_CALC_SET			, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_CALC_MTRANSIT", SE_CALC_MTRANSIT	    , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_CALC_ITRANSIT", SE_CALC_ITRANSIT	    , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_BIT_DISC_CENTER", SE_BIT_DISC_CENTER   , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_BIT_DISC_BOTTOM", SE_BIT_DISC_BOTTOM   , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_BIT_NO_REFRACTION", SE_BIT_NO_REFRACTION, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_BIT_CIVIL_TWILIGHT", SE_BIT_CIVIL_TWILIGHT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_BIT_NAUTIC_TWILIGHT", SE_BIT_NAUTIC_TWILIGHT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_BIT_ASTRO_TWILIGHT", SE_BIT_ASTRO_TWILIGHT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_BIT_FIXED_DISC_SIZE", SE_BIT_FIXED_DISC_SIZE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_BIT_GEOCTR_NO_ECL_LAT", SE_BIT_GEOCTR_NO_ECL_LAT , CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_BIT_HINDU_RISING", SE_BIT_HINDU_RISING , CONST_CS | CONST_PERSISTENT);

	/* for swe_azalt() and swe_azalt_rev() */
	REGISTER_LONG_CONSTANT("SE_ECL2HOR", SE_ECL2HOR, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_EQU2HOR", SE_EQU2HOR, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_HOR2ECL", SE_HOR2ECL, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_HOR2EQU", SE_HOR2EQU, CONST_CS | CONST_PERSISTENT);
	
	/* for swe_refrac() */
	REGISTER_LONG_CONSTANT("SE_TRUE_TO_APP", SE_TRUE_TO_APP, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_APP_TO_TRUE", SE_APP_TO_TRUE, CONST_CS | CONST_PERSISTENT);

	/* for swe_set_tid_acc() */
	REGISTER_LONG_CONSTANT("SE_TIDAL_DE200", SE_TIDAL_DE200, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_TIDAL_DE403", SE_TIDAL_DE403, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_TIDAL_DE404", SE_TIDAL_DE404, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_TIDAL_DE405", SE_TIDAL_DE405, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_TIDAL_DE406", SE_TIDAL_DE406, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_TIDAL_DE421", SE_TIDAL_DE421, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_TIDAL_DE422", SE_TIDAL_DE422, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_TIDAL_DE430", SE_TIDAL_DE430, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_TIDAL_DE431", SE_TIDAL_DE431, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_TIDAL_DE441", SE_TIDAL_DE441, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_TIDAL_26", SE_TIDAL_26, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_TIDAL_STEPHENSON_2016", SE_TIDAL_STEPHENSON_2016, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_TIDAL_DEFAULT", SE_TIDAL_DEFAULT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_TIDAL_AUTOMATIC", SE_TIDAL_AUTOMATIC, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_TIDAL_MOSEPH", SE_TIDAL_MOSEPH, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_TIDAL_SWIEPH", SE_TIDAL_SWIEPH, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("SE_TIDAL_JPLEPH", SE_TIDAL_JPLEPH, CONST_CS | CONST_PERSISTENT);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(swephp)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(swephp)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(swephp)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(swephp)
{
	char version[255];

	swe_version(version);
	php_info_print_table_start();
	php_info_print_table_header(2, "swephp support", "enabled");
	php_info_print_table_row(2, "extension version", SWEPH_EXTENSION_VERSION);
	php_info_print_table_row(2, "library (libswe.a) version", version);
	php_info_print_table_row(2, "default ephemeris file path", SE_EPHE_PATH);
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* Joel - Real sweph port goes here */

/**************************** 
 * exports from sweph.c 
 ****************************/
/* {{{ about pod documentation comments
=pod

=head1 About pod documentation comments

Pod means Plain Old Documentation and comes from the world of Perl, a programming language similar to PHP.
To extract the documenation from this file, type

perldoc sweph.c

see also https://perldoc.perl.org/perlpod
=cut
 }}} */
/* {{{ pod
=pod

=head1 function swe_calc(tjd_et, ipl, iflag)

calculate position of planet ipl with time in Ephemeris Time (TDT)

=head3 Parameters

  double        tjd_et      Julian day in Ephemeris Time.
  int           ipl         Planet/body/object number or constant.
  int           iflag       Flag bits for computation requirements.

=head3 return array

  [0..5]        double	position and speed vector xx
  ['serr']      string	optional error message
  ['rc']        int		return flag, < 0 in case of error

=head3 C declaration

  int swe_calc ( double tjd_et, int ipl, int iflag, double* xx, char* serr);

=cut
 }}} */
PHP_FUNCTION(swe_calc)
{
	char *arg = NULL;
	int rc;
	long ipl, iflag;
	double tjd_et, xx[6];
	char serr[AS_MAXCH]; 
	int i;
	*serr = '\0';
	
	if(ZEND_NUM_ARGS() != 3) WRONG_PARAM_COUNT;
		
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dll",
			&tjd_et, &ipl, &iflag) == FAILURE) {
		return;
	}
	rc = swe_calc(tjd_et, (int)ipl, (int)iflag, xx, serr);

		/* create an array */
	array_init(return_value);
	for(i = 0; i < 6; i++)
		add_index_double(return_value, i, xx[i]);
	add_assoc_string(return_value, "serr", serr);
	add_assoc_long(return_value, "rc", rc);
}

/* {{{ pod
=pod

=head1 function swe_calc_ut(tjd_ut, ipl, iflag)

calculate position of planet ipl with time in Universal Time UT

=head3 Parameters

  double        tjd_ut      Julian day in Universal Time.
  int           ipl         Planet/body/object number or constant.
  int           iflag       Flag bits for computation requirements.

=head3 return array

  [0..5]		position and speed vector xx
  ['serr']		optional error message
  ['rc']		return flag, < 0 in case of error

=head3 C declaration

  int swe_calc_ut ( double tjd_ut, int ipl, int iflag, double* xx, char* serr);

=cut
 }}} */
PHP_FUNCTION(swe_calc_ut)
{
	char *arg = NULL;
	int rc;
	long ipl, iflag;
	double tjd_ut, xx[6];
	char serr[AS_MAXCH]; 
	int i;
	*serr = '\0';
	
	if(ZEND_NUM_ARGS() != 3) WRONG_PARAM_COUNT;
		
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dll",
			&tjd_ut, &ipl, &iflag) == FAILURE) {
		return;
	}
	rc = swe_calc_ut(tjd_ut, (int)ipl, (int)iflag, xx, serr);

	/* create an array */
	array_init(return_value);
	for(i = 0; i < 6; i++)
		add_index_double(return_value, i, xx[i]);
	add_assoc_string(return_value, "serr", serr);
	add_assoc_long(return_value, "rc", rc);
}

/* {{{ pod
=pod

=head1 function swe_calc_pctr(tjd_et, ipl, iplctr, iflag)

calculate position of planet ipl relative to a center object iplctr

=head3 Parameters

  double        tjd_et      Julian day in Ephemeris Time.
  int           ipl         Target planet/body/object number or constant.
  int           iplctr      Center planet/body/object number or constant.
  int           iflag       Flag bits for computation requirements.

=head3 return array

  [0..5]		double	position and speed vector xx
  ['serr']		string	optional error message
  ['rc']		int		return flag, < 0 in case of error

=head3 C declaration

  int swe_calc_pctr ( double tjd_et, int ipl, int iplctr, int iflag, double* xx, char* serr);

=cut
 }}} */
PHP_FUNCTION(swe_calc_pctr)
{
	char *arg = NULL;
	int rc;
	long ipl, iflag, iplctr;
	double tjd_et, xx[6];
	char serr[AS_MAXCH]; 
	int i;
	*serr = '\0';
	
	if(ZEND_NUM_ARGS() != 4) WRONG_PARAM_COUNT;
		
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dlll",
			&tjd_et, &ipl, &iplctr, &iflag) == FAILURE) {
		return;
	}
	rc = swe_calc_pctr(tjd_et, (int)ipl, (int)iplctr, (int)iflag, xx, serr);

		/* create an array */
	array_init(return_value);
	for(i = 0; i < 6; i++)
		add_index_double(return_value, i, xx[i]);
	add_assoc_string(return_value, "serr", serr);
	add_assoc_long(return_value, "rc", rc);
}

#define MAX_FIXSTAR_NAME (2 * SE_MAX_STNAME + 1)
/* {{{ pod
=pod

=head1 function swe_fixstar(star, tjd_et, iflag)

calculate position of a star with time in Ephemeris Time (TDT)

=head3 Parameters

  string        star        Name of fixed star to be searched, returned name of found star.
  double        tjd_et      Julian day in Ephemeris Time.
  int           iflag       Flag bits for computation requirements.

=head3 return array

  [0..5]		double	position and speed vector xx
  ['star']		string	returned star name, usually different from input
  ['serr']		string	optional error message
  ['rc']		int		return flag, < 0 in case of error

=head3 C declaration

  int swe_fixstar(char *star, double tjd_et, int32 iflag, double *xx, char *serr);

=cut
 }}} */
PHP_FUNCTION(swe_fixstar)
{
	char *arg = NULL;
	int rc;
	long iflag;
	double tjd_et, xx[6];
	char *star_ptr = NULL;
	int star_len;
	char star[MAX_FIXSTAR_NAME], serr[AS_MAXCH];
	int i;
	
	if(ZEND_NUM_ARGS() != 3) WRONG_PARAM_COUNT;
		
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sdl",
			&star_ptr, &star_len, &tjd_et, &iflag) == FAILURE) {
		return;
	}
	memset(star, 0, MAX_FIXSTAR_NAME);
	strncpy(star, star_ptr, star_len);
	// php_printf("%s", star);
	rc = swe_fixstar(star, tjd_et, (int)iflag, xx, serr);
	// php_printf("%s %s %d\n", star, serr, rc);

	array_init(return_value);
	for(i = 0; i < 6; i++)
		add_index_double(return_value, i, xx[i]);
	add_assoc_string(return_value, "star", star);
	add_assoc_string(return_value, "serr", serr);
	add_assoc_long(return_value, "rc", rc);
}

/* {{{ pod
=pod

=head1 function swe_fixstar2(star, tjd_et, iflag)

calculate position of a star with time in Ephemeris Time (TDT)

=head3 Parameters

  string        star        Name of fixed star, or string with line number in star file
  double        tjd_et      Julian day in Ephemeris Time.
  int           iflag       Flag bits for computation requirements.

=head3 return array

  [0..5]		double	position and speed vector xx
  ['star']		string	returned star name, usually different from input
  ['serr']		string	optional error message
  ['rc']		int		return flag, < 0 in case of error

=head3 C declaration

  int swe_fixstar2(char *star, double tjd_et, int32 iflag, double *xx, char *serr);

=cut
 }}} */
PHP_FUNCTION(swe_fixstar2)
{
	char *arg = NULL;
	int rc;
	long iflag;
	double tjd_et, xx[6];
	char *star_ptr = NULL;
	int star_len;
	char star[MAX_FIXSTAR_NAME], serr[AS_MAXCH];
	int i;
	
	if(ZEND_NUM_ARGS() != 3) WRONG_PARAM_COUNT;
		
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sdl",
			&star_ptr, &star_len, &tjd_et, &iflag) == FAILURE) {
		return;
	}
	memset(star, 0, MAX_FIXSTAR_NAME);
	strncpy(star, star_ptr, star_len);
	// php_printf("%s", star);
	rc = swe_fixstar2(star, tjd_et, (int)iflag, xx, serr);
	// php_printf("%s %s %d\n", star, serr, rc);

	array_init(return_value);
	for(i = 0; i < 6; i++)
		add_index_double(return_value, i, xx[i]);
	add_assoc_string(return_value, "star", star);
	add_assoc_string(return_value, "serr", serr);
	add_assoc_long(return_value, "rc", rc);
}

/* {{{ pod
=pod

=head1 function swe_fixstar_ut(star, tjd_ut, iflag)

calculate position of a star with time in Universal Time (UT)

=head3 Parameters

  string        star        Name of fixed star to be searched, returned name of found star.
  double        tjd_ut      Julian day in Universal Time.
  int           iflag       Flag bits for computation requirements.

=head3 return array

  [0..5]		double	position and speed vector xx
  ['star']		string	returned star name, usually different from input
  ['serr']		string	optional error message
  ['rc']		int		return flag, < 0 in case of error

=head3 C declaration

  int swe_fixstar_ut(char *star, double tjd_ut, int32 iflag, double *xx, char *serr);

=cut
 }}} */
PHP_FUNCTION(swe_fixstar_ut)
{
	char *arg = NULL;
	int rc;
	long iflag;
	double tjd_ut, xx[6];
	char *star_ptr = NULL;
	int star_len;
	char star[MAX_FIXSTAR_NAME], serr[AS_MAXCH];
	int i;
	*serr = '\0';
	
	if(ZEND_NUM_ARGS() != 3) WRONG_PARAM_COUNT;
		
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sdl",
			&star_ptr, &star_len, &tjd_ut, &iflag) == FAILURE) {
		return;
	}
	memset(star, 0, MAX_FIXSTAR_NAME);
	strncpy(star, star_ptr, star_len);
	rc = swe_fixstar_ut(star, tjd_ut, (int)iflag, xx, serr);

	array_init(return_value);
	for(i = 0; i < 6; i++)
		add_index_double(return_value, i, xx[i]);
	add_assoc_string(return_value, "star", star);
	add_assoc_string(return_value, "serr", serr);
	add_assoc_long(return_value, "rc", rc);
}

/* {{{ pod
=pod

=head1 function swe_fixstar2_ut(star, tjd_ut, iflag)

calculate position of a star with time in Universal Time (UT)

=head3 Parameters

  string        star        Name of fixed star, or string with line number in star file
  double        tjd_ut      Julian day in Universal Time.
  int           iflag       Flag bits for computation requirements.

=head3 return array

  [0..5]		double	position and speed vector xx
  ['star']		string	returned star name, usually different from input
  ['serr']		string	optional error message
  ['rc']		int		return flag, < 0 in case of error

=head3 C declaration

  int swe_fixstar2_ut(char *star, double tjd_ut, int32 iflag, double *xx, char *serr);

=cut
 }}} */
PHP_FUNCTION(swe_fixstar2_ut)
{
	char *arg = NULL;
	int rc;
	long iflag;
	double tjd_ut, xx[6];
	char *star_ptr = NULL;
	int star_len;
	char star[MAX_FIXSTAR_NAME], serr[AS_MAXCH];
	int i;
	*serr = '\0';
	
	if(ZEND_NUM_ARGS() != 3) WRONG_PARAM_COUNT;
		
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sdl",
			&star_ptr, &star_len, &tjd_ut, &iflag) == FAILURE) {
		return;
	}
	memset(star, 0, MAX_FIXSTAR_NAME);
	strncpy(star, star_ptr, star_len);
	rc = swe_fixstar2_ut(star, tjd_ut, (int)iflag, xx, serr);

	array_init(return_value);
	for(i = 0; i < 6; i++)
		add_index_double(return_value, i, xx[i]);
	add_assoc_string(return_value, "star", star);
	add_assoc_string(return_value, "serr", serr);
	add_assoc_long(return_value, "rc", rc);
}

/* {{{ pod
=pod

=head1 function swe_fixstar_mag(star)

deliver magnitude of star

=head3 Parameters

  string        star        Name of fixed star to be searched

=head3 return array

  ['mag']		double  star magnitude, in case of success
  ['star']		string	returned star name, usually different from input
  ['rc']		int		return flag, < 0 in case of error
  ['serr']		string	optional error message

=head3 C declaration

  int swe_fixstar_mag(char *star, double *mag, char *serr);

=cut
 }}} */
PHP_FUNCTION(swe_fixstar_mag)
{
	char *arg = NULL;
	int rc;
	double dmag;
	char *star_ptr = NULL;
	int star_len;
	char star[MAX_FIXSTAR_NAME], serr[AS_MAXCH];
	int i;
	*serr = '\0';
	
	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;
		
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&star_ptr, &star_len) == FAILURE) {
		return;
	}
	memset(star, 0, MAX_FIXSTAR_NAME);
	strncpy(star, star_ptr, star_len);
	rc = swe_fixstar_mag(star, &dmag, serr);

	array_init(return_value);
	if (rc >= 0) 
		add_assoc_double(return_value, "mag", dmag);
	add_assoc_string(return_value, "star", star);
	add_assoc_string(return_value, "serr", serr);
	add_assoc_long(return_value, "rc", rc);
}

/* {{{ pod
=pod

=head1 function swe_fixstar2_mag(star)

deliver magnitude of star

=head3 Parameters

  string        star        Name of fixed star to be searched

=head3 return array

  ['mag']		double  star magnitude, in case of success
  ['star']		string	returned star name, usually different from input
  ['rc']		int		return flag, < 0 in case of error
  ['serr']		string	optional error message

=head3 C declaration

  int swe_fixstar2_mag(char *star, double *mag, char *serr);

=cut
 }}} */
PHP_FUNCTION(swe_fixstar2_mag)
{
	char *arg = NULL;
	int rc;
	double dmag;
	char *star_ptr = NULL;
	int star_len;
	char star[MAX_FIXSTAR_NAME], serr[AS_MAXCH];
	int i;
	*serr = '\0';
	
	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;
		
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
			&star_ptr, &star_len) == FAILURE) {
		return;
	}
	memset(star, 0, MAX_FIXSTAR_NAME);
	strncpy(star, star_ptr, star_len);
	rc = swe_fixstar2_mag(star, &dmag, serr);

	array_init(return_value);
	if (rc >= 0) 
		add_assoc_double(return_value, "mag", dmag);
	add_assoc_string(return_value, "star", star);
	add_assoc_string(return_value, "serr", serr);
	add_assoc_long(return_value, "rc", rc);
}

/* {{{ pod
=pod

=head1 function swe_close()

close SE and release resources

=head3 Parameters (none)

=head3 return (none)

=head3 C declaration

void swe_close()

=cut
 }}} */
PHP_FUNCTION(swe_close)
{
	if(ZEND_NUM_ARGS() != 0) WRONG_PARAM_COUNT;

	swe_close();
	RETURN_NULL();
}

/* {{{ pod
=pod

=head1 function swe_set_ephe_path(path)

Define the path where SE ephemeris files reside

=head3 Parameters

    string      path        Single directory name or a list of directories, searched in sequence.

=head3 return (none)

=head3 C declaration

void swe_set_ephe_path(char *path);

=cut
 }}} */
PHP_FUNCTION(swe_set_ephe_path)
{
	char *arg = emalloc(100);
	size_t arg_len;
	int rc;

	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}
	
	swe_set_ephe_path(arg);
	RETURN_NULL();
}

/* {{{ pod
=pod

=head1 function swe_set_jpl_file(fname)

Define the file name for a JPL ephemeris file, used with flag SEFLG_JPLEPH

=head3 Parameters

    string      fname       File name of JPL ephemeris (must be sibling of ephemeris files).

=head3 return (none)

=head3 C declaration

void swe_set_jpl_file(char *fname);

=cut
 }}} */
PHP_FUNCTION(swe_set_jpl_file)
{
	char *arg = NULL;
	size_t arg_len;
	int rc;

	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	swe_set_jpl_file(arg);
	RETURN_NULL();
}

/* {{{ pod
=pod

=head1 function swe_get_planet_name(ipl)

get the name of a planet, asteroid or fictitious object

=head3 Parameters

    int         ipl         Planet/body/object number or constant.

=head3 return string

In case of error, an error message is returned instead of a planet name.

=head3 C declaration

(char *) swe_get_planet_name(int ipl, char *spname);

=cut
 }}} */
PHP_FUNCTION(swe_get_planet_name)
{
	long ipl;
	char name[AS_MAXCH];
	
	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &ipl) == FAILURE) {
		return;
	}

	swe_get_planet_name((int)ipl, name);

	RETURN_STRING(name);
}

/* {{{ pod
=pod

=head1 function swe_set_topo(geolon, geolat, geoalt)

Set topocentric reference places, used with flag SEFLG_TOPOCTR and some function

=head3 Parameters

    double       geolon      Longitude of location in degrees.
    double       geolat      Latitude of location in degrees.
    double       geoalt      Altitude of location in meters.

=head3 return (none)

=head3 C declaration

void swe_set_topo(double geolon, double geolat, double geoalt)

=cut
 }}} */
PHP_FUNCTION(swe_set_topo)
{
	int rc;
	double geo_lon, geo_lat, geo_alt;

	if(ZEND_NUM_ARGS() != 3) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddd",
		&geo_lon, &geo_lat, &geo_alt) == FAILURE) {
		return;
	}
	swe_set_topo(geo_lon, geo_lat, geo_alt);
	RETURN_NULL();
}

/* {{{ pod
=pod

=head1 function swe_set_sid_mode(sid_mode, &t0, &ayan_t0)

Set one of the numerous sidereal modes, used with flag SEFLG_SIDEREAL and some functions

=head3 Parameters

    int         sid_mode      Number of constant of ayanamsa to use.
    double      t0            Reference date if using SE_SIDM_USER flag, 0 otherwise.
    double      ayan_t0       Initial value of ayanamsa if using SE_SIDM_USER flag, 0 otherwise.

=head3 return (none)

=head3 C declaration

void swe_set_sid_mode(int32 sid_mode, double t0, double ayan_t0)

=cut
}}} */
PHP_FUNCTION(swe_set_sid_mode)
{
	long sid_mode;
	int rc;
	double t0, ayan_t0;
		
	if(ZEND_NUM_ARGS() != 3) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ldd",
		&sid_mode, &t0, &ayan_t0) == FAILURE) {
		return;
	}
	swe_set_sid_mode((int)sid_mode, t0, ayan_t0);
	RETURN_NULL();
}

/* {{{ pod
=pod

=head1 function swe_get_ayanamsa (tjd_et)

Compute the ayanamsa without nutation.

=head3 Parameters

    double         tjd_et      Julian day in Ephemeris Time.

=head3 return double

=head3 C declaration

double swe_get_ayanamsa(double tjd_et);

=cut
}}} */
PHP_FUNCTION(swe_get_ayanamsa)
{
	double tjd_et;
		
	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d",
		&tjd_et) == FAILURE) {
		return;
	}
	
	RETURN_DOUBLE(swe_get_ayanamsa(tjd_et));
}

/* {{{ pod
=pod

=head1 function swe_get_ayanamsa_ex(tjd_et, iflag)

Compute the ayanamsa with or without nutation, depending on flag.

=head3 Parameters

    double      tjd_et      Julian day in Ephemeris Time.
    int         iflag       Flag bits for computation requirements.

=head3 return array

    [
        'daya' => (double) Value of ayanamsa.
        'serr' => (string) Error message or empty string.
    ]

=head3 C declaration

int32 swe_get_ayanamsa_ex(double tjd_et, int32 iflag, double *daya, char *serr);

=cut
}}} */
PHP_FUNCTION(swe_get_ayanamsa_ex)
{
	double tjd_et, daya;
	long iflag;
	int rc;
	char serr[AS_MAXCH];
	*serr = '\0';

	if (ZEND_NUM_ARGS() != 2) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dl",
		&tjd_et, &iflag) == FAILURE) {
		return;
	}

	rc = swe_get_ayanamsa_ex(tjd_et, iflag, &daya, serr);

    array_init(return_value);

    add_assoc_double(return_value, "daya", daya);
    add_assoc_long(return_value, "rc", rc);
	if (rc < 0)
		add_assoc_string(return_value, "serr", serr);
}

/* {{{ pod
=pod

=head1 function swe_get_ayanamsa_ut(tjd_ut)

Compute the ayanamsa without nutation.

=head3 Parameters

    double         tjd_ut      Julian day in Universal Time.

=head3 return
	
	double

=head3 C declaration

double swe_get_ayanamsa_ut(double tjd_ut);

<<<<<<< HEAD

=cut
 }}} */
PHP_FUNCTION(swe_get_ayanamsa_ut)
{
	double tjd_ut;
		
	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d",
		&tjd_ut) == FAILURE) {
		return;
	}
	
	RETURN_DOUBLE(swe_get_ayanamsa_ut(tjd_ut));
}

/* {{{ pod
=pod

=head1 function swe_get_ayanamsa_ex_ut (tjd_ut, iflag)

Get ayanamsa value in current sidereal mode, time in UT

=head3 Parameters

    double      tjd_ut      Julian day in Universal Time.
    int         iflag       Flag bits for computation requirements.

=head3 return array

  ['daya']		double	ayanamsa value
  ['serr']		string	optional error message
  ['rc']		int		return flag, < 0 in case of error

=head3 C declaration

int32 swe_get_ayanamsa_ex_ut(double tjd_ut, int32 iflag, double *daya, char *serr);                 


=cut
 }}} */
PHP_FUNCTION(swe_get_ayanamsa_ex_ut)
{
	double tjd_ut, daya;
	long iflag;
	int rc;
	char serr[AS_MAXCH];
	*serr = '\0';

	if (ZEND_NUM_ARGS() != 2) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dl",
		&tjd_ut, &iflag) == FAILURE) {
		return;
	}

	rc = swe_get_ayanamsa_ex_ut(tjd_ut, iflag, &daya, serr);

    array_init(return_value);

    add_assoc_double(return_value, "daya", daya);
    add_assoc_long(return_value, "rc", rc);
	if (rc < 0)
		add_assoc_string(return_value, "serr", serr);
}

/* {{{ pod
=pod

=head1 function swe_get_ayanamsa_name (isidmode)

Get ayanamsa name for this  sidereal mode

=head3 Parameters

    int         isidmode      Number or constant of sidereal mode.

=head3 return string

=head3 C declaration

const char *swe_get_ayanamsa_name(int32 isidmode);


=cut
 }}} */
PHP_FUNCTION(swe_get_ayanamsa_name)
{
	long isidmode;
	
	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &isidmode) == FAILURE) {
		return;
	}

	RETURN_STRING(swe_get_ayanamsa_name((int)isidmode));
}

/* {{{ pod
=pod

=head1 function swe_version()

find out version number of your Swiss Ephemeris version

=head3 Parameters (none)

=head3 return string

Version information of the Swiss Ephemeris library,
different from the version of this PHP extension.

=head3 C declaration

char *swe_version(char* svers);

=cut
}}} */
PHP_FUNCTION(swe_version)
{
	char name[AS_MAXCH];
	
	if(ZEND_NUM_ARGS() != 0) WRONG_PARAM_COUNT;

	RETURN_STRING(swe_version(name));
}

/* {{{ pod
=pod

=head1 function swe_get_library_path()

Path to the SE PHP extension library file

=head3 Parameters (none)

=head3 return 

	string	path to currently used library

=head3 C declaration

char *swe_get_library_path(char *spath);

=cut
}}} */
PHP_FUNCTION(swe_get_library_path)
{
    char path[AS_MAXCH];

    if (ZEND_NUM_ARGS() != 0) WRONG_PARAM_COUNT;

    RETURN_STRING(swe_get_library_path(path));
}

/* {{{ pod
=pod

=head1 function swe_get_current_file_data (ifno)

This is a function mostly for debug purposes. It is also useful to find out the time range offered by
an asteroid file. The function should only be used directly AFTER a successful call to swe_calc() or
swe_fixstar.

It delivers information about the last used file, depending on parameter ifno:

  ifno = 0     planet file sepl_xxx, used for Sun .. Pluto, or jpl file
  ifno = 1     moon file semo_xxx
  ifno = 2     main asteroid file seas_xxx  if such an object was computed
  ifno = 3     other asteroid or planetary moon file, if such object was computed
  ifno = 4     star file

=head3 Parameters 
  
    int		ifno

=head3 return array

  ['path']		string
  ['tfstart']	double
  ['tfend']		double
  ['denum']		int

  In case of error, NULL is returned.

=head3 C declaration

 char * swe_get_current_file_data(int ifno, double *tfstart, double *tfend, int *denum);

=cut
}}} */
PHP_FUNCTION(swe_get_current_file_data)
{
	int ifno;
	int denum;
	double tfstart, tfend;
	char *a = NULL;

	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l",
			&ifno) == FAILURE) {
		return;
	}

	a = (char *)swe_get_current_file_data(ifno, &tfstart, &tfend, &denum);
	if (a == NULL) {
		RETURN_NULL();
	} else {
		array_init(return_value);
		add_assoc_string(return_value, "path", a);
		add_assoc_double(return_value, "tfstart", tfstart);
		add_assoc_double(return_value, "tfend", tfend);
		add_assoc_long(return_value, "denum", denum);
    }
}

/**************************** 
 * exports from swedate.c 
 ****************************/

/* {{{ pod
=pod

=head1 function swe_date_conversion (year, month, day, hour, cal_flag)

Converts a calendar date to julian day number tjd, with validity check for date.
Parameter cal_flag is a character, j or g, and not the same as the constants SE_GREG_CAL and
SE_JUL_CAL used in swe_juldate() and swe_revjul()

=head3 Parameters

  int		year
  int		month		(1..12)
  int		day			(1..31)
  double	hour		UT (0.0 .. 23.99999) clock time as double
  string	'j' or 'g' or string beginning with one of these letters

=head3 return value
  
  double|null    converted tjd, or in case of illegal input date, NULL.

=head3 C declaration

  int swe_date_conversion( int y , int m , int d , double utime, char c, double *tjd);


=cut
 }}} */
PHP_FUNCTION(swe_date_conversion)
{
	size_t arg_len;
	int rc;
	long year, month, day;
	double hour, tjd;
	char *gregflag = NULL;

	if(ZEND_NUM_ARGS() != 5) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lllds",
			&year, &month, &day, &hour, &gregflag, &arg_len) == FAILURE) {
		return;
	}
	if (arg_len < 1)
		RETURN_NULL();

	rc = swe_date_conversion((int)year, (int)month, (int)day, hour, gregflag[0], &tjd);
	if (rc == OK)
	{
		RETURN_DOUBLE(tjd);
	}
	else
		RETURN_NULL();
}


/* {{{ pod
=pod

=head1 function swe_julday (y, m, d, hour, gregflag)

Converts a calendar date to julian day number tjd, no validity check for date.

=head3 Parameters

  int		year		
  int		month		(1..12)
  int		day			(1..31)
  double	hour		UT (0.0 .. 23.99999) clock time as double
  int		gregflag	SE_GREG_CAL (==1) or SE_JUL_CAL (==0)

=head3 return value
  
  double    converted tjd

=head3 C declaration

  double swe_julday(int year, int month, int day, double hour, int gregflag); 


=cut
 }}} */
PHP_FUNCTION(swe_julday)
{
	int rc;
	long year, month, day, gregflag;
	double hour, julday;

	if(ZEND_NUM_ARGS() != 5) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llldl",
			&year, &month, &day, &hour, &gregflag) == FAILURE) {
		return;
	}

	RETURN_DOUBLE(swe_julday((int)year, (int)month, (int)day, hour, (int)gregflag));
}

/* {{{ pod
=pod

=head1 function swe_revjul (jd, gregflag)

Converts julian day number to calendar date

=head3 Parameters

  double	jd		julian day number
  int		gregflag	SE_GREG_CAL (==1) or SE_JUL_CAL (==0)

=head3 return array

  ['year']		int
  ['month']		int
  ['day']		int
  ['hour']		double

=head3 C declaration

  void swe_revjul ( double jd, int gregflag, int *jyear, int *jmon, int *jday, double *jut);


=cut
 }}} */
PHP_FUNCTION(swe_revjul)
{
	int year, month, day, gregflag;
	double hour, jd;
	
	if(ZEND_NUM_ARGS() != 2) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dl",
			&jd, &gregflag) == FAILURE) {
		return;
	}

	swe_revjul(jd, gregflag, &year, &month, &day, &hour);

	array_init(return_value);

	add_assoc_long(return_value, "year", year);
	add_assoc_long(return_value, "month", month);
	add_assoc_long(return_value, "day", day);
	add_assoc_double(return_value, "hour", hour);
}

/* {{{ pod
=pod

=head1 function swe_jdet_to_utc (tjd_et, gregflag)

Converts julian day number / time in Ephemeris time to date and time in UTC 

=head3 Parameters

  double	tjd_et		julian day number
  int		gregflag	SE_GREG_CAL (==1) or SE_JUL_CAL (==0)

=head3 return array

  ['year']	int
  ['month']	int
  ['day']	int
  ['hour']	int
  ['min']	int
  ['sec']	double

=head3 C declaration

  void swe_jdet_to_utc (double tjd_et, int32 gregflag, int32 *iyear, int32 *imonth, int32 *iday, int32 *ihour, int32 *imin, double *dsec);


=cut
 }}} */
PHP_FUNCTION(swe_jdet_to_utc)
{
	double tjd_et;
	long gregflag;
	int32 iyear, imonth, iday;
	int32 ihour, imin;
	double dsec;
	
	if(ZEND_NUM_ARGS() != 2) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dl",
			&tjd_et, &gregflag) == FAILURE) {
		return;
	}
	swe_jdet_to_utc(tjd_et, (int32)gregflag, &iyear, &imonth, &iday, &ihour, &imin, &dsec);
	
	array_init(return_value);
	add_assoc_long(return_value, "year", iyear);
	add_assoc_long(return_value, "month", imonth);
	add_assoc_long(return_value, "day", iday);
	add_assoc_long(return_value, "hour", ihour);
	add_assoc_long(return_value, "min", imin);
	add_assoc_double(return_value, "sec", dsec);
}

/* {{{ pod
=pod

=head1 function swe_jdut1_to_utc(tjd_ut, gregflag)

Converts julian day number / time in UT to date and time in UTC 

=head3 Parameters

  double    tjd_ut		julian day number and time in UT
  int       gregflag	SE_GREG_CAL (==1) or SE_JUL_CAL (==0)

=head3 return array

  ['year']  int
  ['month'] int
  ['day']   int
  ['hour']  int
  ['min']   int
  ['sec']	double

=head3 C declaration

  void swe_jdut1_to_utc (double tjd_ut, int32 gregflag, int32 *iyear, int32 *imonth, int32 *iday, int32 *ihour, int32 *imin, double *dsec);


=cut
 }}} */
PHP_FUNCTION(swe_jdut1_to_utc)
{
	double tjd_ut;
	long gregflag;
	int32 iyear, imonth, iday;
	int32 ihour, imin;
	double dsec;
	
	if(ZEND_NUM_ARGS() != 2) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dl",
			&tjd_ut, &gregflag) == FAILURE) {
		return;
	}
	swe_jdut1_to_utc(tjd_ut, (int32)gregflag, &iyear, &imonth, &iday, &ihour, &imin, &dsec);
	
	array_init(return_value);
	add_assoc_long(return_value, "year", iyear);
	add_assoc_long(return_value, "month", imonth);
	add_assoc_long(return_value, "day", iday);
	add_assoc_long(return_value, "hour", ihour);
	add_assoc_long(return_value, "min", imin);
	add_assoc_double(return_value, "sec", dsec);
}

/* {{{ pod
=pod

=head1 function swe_utc_to_jd(year, month, day, hour, min, dsec, gregflag)

Converts a calendar date to julian day number tjd, no validity check for date.

=head3 Parameters

  int		year
  int		month	(1..12)
  int		day		(1..31)
  int		hour	(0..23)
  int		min		(0..59)
  double	dsec	(0.0 .. 59.99999)
  int		gregflag	SE_GREG_CAL (==1) or SE_JUL_CAL (==0)

=head3 return array

  In case of success

  [0]		double	jd_et	jd in ET (TDT)
  [1]		double	jd_ut	jd in UT (UT1)
  ['rc']	int		0

  In case of error

  ['serr']	string
  ['rc']	int		-1


=head3 C declaration

  int swe_utc_to_jd( int32 iyear, int32 imonth, int32 iday, int32 ihour, int32 imin, double dsec, int32 gregflag, double *dret, char *serr);


=cut
 }}} */
PHP_FUNCTION(swe_utc_to_jd)
{
	long gregflag;
	long iyear, imonth, iday;
	long ihour, imin;
	double dsec, dret[2];
	char serr[AS_MAXCH];
	int32 rc;
	*serr = '\0';
	
	if(ZEND_NUM_ARGS() != 7) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llllldl",
			&iyear, &imonth, &iday, &ihour, &imin, &dsec, &gregflag) == FAILURE) {
		return;
	}
	rc = swe_utc_to_jd((int32)iyear, (int32)imonth, (int32)iday, (int32)ihour, (int32)imin,
			dsec, (int32)gregflag, dret, serr);
			
	array_init(return_value);
	add_assoc_long(return_value, "rc", rc);
	if (rc < 0) {
		add_assoc_string(return_value, "serr", serr);
	} else {
		add_index_double(return_value, 0, dret[0]);
		add_index_double(return_value, 1, dret[1]);
    }
}

/* {{{ pod
=pod

=head1 function swe_utc_time_zone(iyear, imonth, iday, ihour, imin, dsec, d_timezone)

Converts a calendar date in a zone with time offset d_timezone into a calendar date in UT (UT1).

=head3 Parameters

  int		year
  int		month	(1..12)
  int		day		(1..31)
  int		hour	(0..23)
  int		min		(0..59)
  double	dsec	(0.0 .. 59.99999)
  double	d_timezone	offset of time zone to UT, in hours as double

=head3 return array

  ['year']	int
  ['month']	int
  ['day']	int
  ['hour']	int
  ['min']	int
  ['sec']	double

=head3 C declaration

  void swe_utc_time_zone( int32 iyear, int32 imonth, int32 iday, int32 ihour, int32 imin, double dsec, double d_timezone, int32 *iyear_out, int32 *imonth_out, int32 *iday_out, int32 *ihour_out, int32 *imin_out, double *dsec_out); 


=cut
 }}} */
PHP_FUNCTION(swe_utc_time_zone)
{
	long iyear, imonth, iday;
	long ihour, imin;
	double dsec, d_timezone;
	int32 iyear_out, imonth_out, iday_out, ihour_out, imin_out;
	double dsec_out;
	
	int32 rc;
	
	if(ZEND_NUM_ARGS() != 7) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llllldd",
			&iyear, &imonth, &iday, &ihour, &imin, &dsec, &d_timezone) == FAILURE) {
		return;
	}
	swe_utc_time_zone((int32)iyear, (int32)imonth, (int32)iday, (int32)ihour, (int32)imin,
			dsec, d_timezone, &iyear_out, &imonth_out, &iday_out, &ihour_out,
			&imin_out, &dsec_out);
			
			
	array_init(return_value);
	add_assoc_long(return_value, "year", iyear_out);
	add_assoc_long(return_value, "month", imonth_out);
	add_assoc_long(return_value, "day", iday_out);
	add_assoc_long(return_value, "hour", ihour_out);
	add_assoc_long(return_value, "min", imin_out);
	add_assoc_double(return_value, "sec", dsec_out);
}


/**************************** 
 * exports from swehouse.c 
 ****************************/

/* {{{ pod
=pod

=head1 function swe_houses(tjd_ut, geolat, geolon, hsys)

calculated house cusps for given date/time, location and house system

=head3 Parameters

  double        tjd_ut      Julian day in Universal Time.
  double		geolat		latitude -90.00 .. 90.00 (north postive, south negative)
  double		geolon		longitude -180.00 .. 180.00 (east postive, sowest negative)
  string		hsys		first letter indicates house system, default 'P' for Placidus

=head3 return array

  ['cusps']	array of 13 or 37 doubles
  ['ascmc']	array of 10 doubles
  ['rc']	int	return flag, < 0 in case of error

=head3 C declaration

  int swe_houses( double tjd_ut, double geolat, double geolon, int hsys, double *cusps, double *ascmc);

=cut
 }}} */
PHP_FUNCTION(swe_houses)
{
	char *arg = NULL;
	size_t hsys_len;
	int rc;

	char *hsys = NULL;
	double tjd_ut, geolat, geolon;
	double cusps[37], ascmc[10]; 
	int i, houses;
	zval cusps_arr, ascmc_arr;
	
	if(ZEND_NUM_ARGS() != 4) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddds",
			&tjd_ut, &geolat, &geolon, &hsys, &hsys_len) == FAILURE) {
		return;
	}
	if (hsys_len < 1)
		return;

	rc = swe_houses(tjd_ut, geolat, geolon, hsys[0], cusps, ascmc);

	/* create 2 index array, and 1 assoc array */
	array_init(return_value);
	array_init(&cusps_arr);
	
	if (hsys[0] == 'G')
		houses = 37;
	else
		houses = 13;
		
	for(i = 0; i < houses; i++)
		add_index_double(&cusps_arr, i, cusps[i]);

	array_init(&ascmc_arr);
	for(i = 0; i < 10; i++)
		add_index_double(&ascmc_arr, i, ascmc[i]);
		
	add_assoc_zval(return_value, "cusps", &cusps_arr);
	add_assoc_zval(return_value, "ascmc", &ascmc_arr);
	add_assoc_long(return_value, "rc", rc);
}

/* {{{ pod
=pod

=head1 function swe_houses_ex(tjd_ut, iflag, geolat, geolon, hsys)

calculated house cusps for given date/time, location and house system

=head3 Parameters

  double        tjd_ut      Julian day in Universal Time.
  int			iflag
  double		geolat		latitude -90.00 .. 90.00 (north postive, south negative)
  double		geolon		longitude -180.00 .. 180.00 (east postive, sowest negative)
  string		hsys		first letter indicates house system, default 'P' for Placidus

=head3 return array

  ['cusps']	array of 13 or 37 doubles
  ['ascmc']	array of 10 doubles
  ['rc']	int	return flag, < 0 in case of error

=head3 C declaration

  int swe_houses_ex( double tjd_ut, int iflag, double geolat, double geolon, int hsys, double *cusps, double *ascmc);

=cut
 }}} */
PHP_FUNCTION(swe_houses_ex)
{
	char *arg = NULL;
	size_t hsys_len;
	int rc;
	char *hsys = NULL;
	double tjd_ut, geolat, geolon;
	double cusps[37], ascmc[10]; 
	int i, houses;
	long iflag;
	zval cusps_arr, ascmc_arr;
	
	if(ZEND_NUM_ARGS() != 5) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dldds",
			&tjd_ut, &iflag, &geolat, &geolon, &hsys, &hsys_len) == FAILURE) {
		return;
	}
	if (hsys_len < 1)
		return;

	rc = swe_houses_ex(tjd_ut, (int)iflag, geolat, geolon, hsys[0], cusps, ascmc);

	/* create 2 index array, and 1 assoc array */
	array_init(return_value);
	array_init(&cusps_arr);
	
	if (hsys[0] == 'G')
		houses = 37;
	else
		houses = 13;
		
	for(i = 0; i < houses; i++)
		add_index_double(&cusps_arr, i, cusps[i]);

	array_init(&ascmc_arr);
	for(i = 0; i < 10; i++)
		add_index_double(&ascmc_arr, i, ascmc[i]);
		
	add_assoc_zval(return_value, "cusps", &cusps_arr);
	add_assoc_zval(return_value, "ascmc", &ascmc_arr);
	add_assoc_long(return_value, "rc", rc);
}

/* {{{ pod
=pod

=head1 function swe_houses_ex2(tjd_ut, iflag, geolat, geolon, hsys)

calculated house cusps for given date/time, location and house system

=head3 Parameters

  double        tjd_ut      Julian day in Universal Time.
  int			iflag
  double		geolat		latitude -90.00 .. 90.00 (north postive, south negative)
  double		geolon		longitude -180.00 .. 180.00 (east postive, sowest negative)
  string		hsys		first letter indicates house system, default 'P' for Placidus

=head3 return array

  ['cusps']			array of 13 or 37 doubles
  ['ascmc']			array of 10 doubles
  ['cusp_speed']	array of 13 or 37 doubles
  ['ascmc_speed']	array of 10 doubles
  ['rc']			int	return flag, < 0 in case of error
  ['serr']			string

=head3 C declaration

  int swe_houses_ex2( double tjd_ut, int iflag, double geolat, double geolon, int hsys, double *cusps, double *ascmc, double *cusp_speed, double *ascmc_speed, char *serr);

=cut
 }}} */
PHP_FUNCTION(swe_houses_ex2)
{
	char *arg = NULL;
	size_t hsys_len;
	int rc;
	char *hsys = NULL;
	double tjd_ut, geolat, geolon;
	double cusps[37], ascmc[10], cusp_speed[37], ascmc_speed[10];
	int i, houses;
	long iflag;
	zval cusps_arr, ascmc_arr, cusp_speed_arr, ascmc_speed_arr;
	char serr[AS_MAXCH];
	*serr = '\0';

	if(ZEND_NUM_ARGS() != 5) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dldds",
			&tjd_ut, &iflag, &geolat, &geolon, &hsys, &hsys_len) == FAILURE) {
		return;
	}
	if (hsys_len < 1)
		return;

	rc = swe_houses_ex2(tjd_ut, (int)iflag, geolat, geolon, hsys[0], cusps, ascmc, cusp_speed, ascmc_speed, serr);

	array_init(return_value);

	array_init(&cusps_arr);
	array_init(&cusp_speed_arr);

	if (hsys[0] == 'G')
		houses = 37;
	else
		houses = 13;

	for (i = 0; i < houses; i++) {
	    add_index_double(&cusps_arr, i, cusps[i]);
	    add_index_double(&cusp_speed_arr, i, cusp_speed[i]);
	}

	array_init(&ascmc_arr);
	array_init(&ascmc_speed_arr);

	for(i = 0; i < 10; i++) {
	    add_index_double(&ascmc_arr, i, ascmc[i]);
	    add_index_double(&ascmc_speed_arr, i, ascmc_speed[i]);
	}

	add_assoc_zval(return_value, "cusps", &cusps_arr);
	add_assoc_zval(return_value, "ascmc", &ascmc_arr);
	add_assoc_zval(return_value, "cusp_speed", &cusp_speed_arr);
	add_assoc_zval(return_value, "ascmc_speed", &ascmc_speed_arr);
	add_assoc_long(return_value, "rc", rc);
	add_assoc_string(return_value, "serr", serr);
}

/* {{{ pod
=pod

=head1 function swe_houses_armc(armc, geolat, eps, hsys)

calculated house cusps for given armc, latitude, obliquity and house system

=head3 Parameters

  double        armc      	0.00 .. 360.00
  double		geolat		latitude -90.00 .. 90.00 (north postive, south negative)
  double		eps			obliquity of the ecliptic
  string		hsys		first letter indicates house system, default 'P' for Placidus

=head3 return array

  ['cusps']	array of 13 or 37 doubles
  ['ascmc']	array of 10 doubles
  ['rc']	int	return flag, < 0 in case of error

=head3 C declaration

  int swe_houses_armc( double armc, double geolat, double eps, int hsys, double *cusps, double *ascmc);

=cut
 }}} */
PHP_FUNCTION(swe_houses_armc)
{
	char *arg = NULL;
	size_t hsys_len;
	int rc;
	char *hsys = NULL;
	double armc, geolat, eps;
	double cusps[37], ascmc[10]; 
	int i, iflag, houses;
	zval cusps_arr, ascmc_arr;
	
	if(ZEND_NUM_ARGS() != 4) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddds",
			&armc, &geolat, &eps, &hsys, &hsys_len) == FAILURE) {
		return;
	}
	if (hsys_len < 1)
		return;

	rc = swe_houses_armc(armc, geolat, eps, hsys[0], cusps, ascmc);

	/* create 2 index array, and 1 assoc array */
	array_init(return_value);
	array_init(&cusps_arr);
	
	if (hsys[0] == 'G')
		houses = 37;
	else
		houses = 13;
		
	for(i = 0; i < houses; i++)
		add_index_double(&cusps_arr, i, cusps[i]);

	array_init(&ascmc_arr);
	for(i = 0; i < 10; i++)
		add_index_double(&ascmc_arr, i, ascmc[i]);
		
	add_assoc_zval(return_value, "cusps", &cusps_arr);
	add_assoc_zval(return_value, "ascmc", &ascmc_arr);
	add_assoc_long(return_value, "rc", rc);
}

/* {{{ pod
=pod

=head1 function swe_houses_armc_ex2(armc, geolat, eps, hsys)

calculated house cusps for given armc, latitude, obliquity and house system

=head3 Parameters

  double        armc      	0.00 .. 360.00
  double		geolat		latitude -90.00 .. 90.00 (north postive, south negative)
  double		eps			obliquity of the ecliptic
  string		hsys		first letter indicates house system, default 'P' for Placidus

=head3 return array

  ['cusps']			array of 13 or 37 doubles
  ['ascmc']			array of 10 doubles
  ['cusp_speed']	array of 13 or 37 doubles
  ['ascmc_speed']	array of 10 doubles
  ['rc']			int	return flag, < 0 in case of error
  ['serr']			string

=head3 C declaration

  int swe_houses_armc_ex2( double armc, double geolat, double eps, int hsys, double *cusps, double *ascmc, double *cusp_speed, double *ascmc_speed, char *serr);

=cut
 }}} */
PHP_FUNCTION(swe_houses_armc_ex2)
{
	char *arg = NULL;
	size_t hsys_len;
	int rc;
	char *hsys = NULL;
	double armc, geolat, eps;
	double cusps[37], ascmc[10], cusp_speed[37], ascmc_speed[10];
	int i, iflag, houses;
	zval cusps_arr, ascmc_arr, cusp_speed_arr, ascmc_speed_arr;
	char serr[AS_MAXCH];
	*serr = '\0';

	if(ZEND_NUM_ARGS() != 4) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddds",
			&armc, &geolat, &eps, &hsys, &hsys_len) == FAILURE) {
		return;
	}
	if (hsys_len < 1)
		return;

	rc = swe_houses_armc_ex2(armc, geolat, eps, hsys[0], cusps, ascmc, cusp_speed, ascmc_speed, serr);

	array_init(return_value);

	array_init(&cusps_arr);
	array_init(&cusp_speed_arr);

	if (hsys[0] == 'G')
		houses = 37;
	else
		houses = 13;

	for (i = 0; i < houses; i++) {
	    add_index_double(&cusps_arr, i, cusps[i]);
	    add_index_double(&cusp_speed_arr, i, cusp_speed[i]);
	}

	array_init(&ascmc_arr);
	array_init(&ascmc_speed_arr);

	for (i = 0; i < 10; i++) {
	    add_index_double(&ascmc_arr, i, ascmc[i]);
	    add_index_double(&ascmc_speed_arr, i, ascmc_speed[i]);
	}

	add_assoc_zval(return_value, "cusps", &cusps_arr);
	add_assoc_zval(return_value, "ascmc", &ascmc_arr);
	add_assoc_zval(return_value, "cusp_speed", &cusp_speed_arr);
	add_assoc_zval(return_value, "ascmc_speed", &ascmc_speed_arr);
	add_assoc_long(return_value, "rc", rc);
	add_assoc_string(return_value, "serr", serr);
}

/* {{{ pod
=pod

=head1 function swe_house_pos(armc, geolat, eps, hsys, xpin0, xpin1)

calculated house position of object for given armc, latitude, obliquity and house system

=head3 Parameters

  double        armc      	0.00 .. 360.00
  double		geolat		latitude -90.00 .. 90.00 (north postive, south negative)
  double		eps			obliquity of the ecliptic
  string		hsys		first letter indicates house system, default 'P' for Placidus
  double		xpin0		longitude of object
  double		xpin1		latitude of object

=head3 return value

	double		house position

	or, in case of error

	string		error message

=head3 C declaration

  double swe_house_pos(double armc, double geolat, double eps, int hsys, double *xpin, char *serr)

=cut
 }}} */
PHP_FUNCTION(swe_house_pos)
{
	char *arg = NULL;
	size_t hsys_len;
	char *hsys = NULL;
	double armc, geolat, eps, xpin[2], rc;
	char serr[AS_MAXCH]; 
	int i;
	*serr = '\0';
	
	if(ZEND_NUM_ARGS() != 6) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddsdd",
			&armc, &geolat, &eps, &hsys, &hsys_len, &xpin[0], &xpin[1]) == FAILURE) {
		return;
	}
	if (hsys_len < 1)
		return;

	rc = swe_house_pos(armc, geolat, eps, hsys[0], xpin, serr);

	array_init(return_value);

	if ((rc >= 1.0) && (rc < 13.0))
	{
		RETURN_DOUBLE(rc);
	}
	else
	{
		RETURN_STRING(serr);
	}	
}

/* {{{ pod
=pod

=head1 function swe_house_name(hsys)

Get the name of a house system

=head3 Parameters

  string		hsys		first letter indicates house system, default 'P' for Placidus

=head3 return value

	string		name of house system

	or fail if system unknown.

=head3 C declaration

  char *swe_house_name(int hsys)

=cut
 }}} */
PHP_FUNCTION(swe_house_name)
{
	size_t hsys_len;
	char *hsys = NULL;
	char *name;
	
	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &hsys, &hsys_len) == FAILURE) {
		return;
	}

	if (hsys_len < 1)
		return;
	name = swe_house_name(hsys[0]);

	RETURN_STRING(name);
}

/**************************** 
 * exports from sweecl.c 
 ****************************/

PHP_FUNCTION(swe_gauquelin_sector)
{
	char *arg = NULL;
	size_t arg_len;
	int ipl, iflag, imeth, rc;
	char *starname = NULL;
	double t_ut, geopos[3], atpress, attemp, dgsect;
	char serr[AS_MAXCH]; 
	int i;
	*serr = '\0';
	
	if(ZEND_NUM_ARGS() != 10) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dlsllddddd",
			&t_ut, &ipl, &starname, &arg_len, &iflag, &imeth, &geopos[0],
			&geopos[1], &geopos[2], &atpress, &attemp) == FAILURE) {
		return;
	}
	rc = swe_gauquelin_sector(t_ut, ipl, starname, iflag, imeth, geopos,
			atpress, attemp, &dgsect, serr);

	if (rc == ERR)
	{
		RETURN_STRING(serr);
	}
	else
	{
		RETURN_DOUBLE(dgsect);
	}	
}

PHP_FUNCTION(swe_sol_eclipse_where)
{
	char *arg = NULL;
	int arg_len, ifl, rc;
	double tjd_ut, geopos[2], attr[20];
	char serr[AS_MAXCH]; 
	int i;
	zval geopos_arr, attr_arr;
	*serr = '\0';

	if(ZEND_NUM_ARGS() != 2) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dl",
			&tjd_ut, &ifl, &arg_len) == FAILURE) {
		return;
	}
	rc = swe_sol_eclipse_where(tjd_ut, ifl, geopos,	attr, serr);

	array_init(return_value);
	add_assoc_long(return_value, "retflag", rc);

	if (rc == ERR)
	{
		add_assoc_string(return_value, "serr", serr);			
	}
	else
	{
		array_init(&geopos_arr);
		
		for(i = 0; i < 2; i++)
			add_index_double(&geopos_arr, i, geopos[i]);
	
		array_init(&attr_arr);
		for(i = 0; i < 20; i++)
			add_index_double(&attr_arr, i, attr[i]);
		
		add_assoc_zval(return_value, "geopos", &geopos_arr);
		add_assoc_zval(return_value, "attr", &attr_arr);
	}
}


/* {{{ pod
=pod

=head1 function swe_lun_occult_where($tjd_ut, $ipl, $star, $iflag);

Finds the place on earth where the occultation is maximal at a given
time. 

Get the name of a house system

=head3 Parameters

  tjd_ut	double   Julian day number, Universal Time
  ipl    	int		 Planet occulted
  star		string   Star name, if a star occultation is searched
  iflag   	int      (specify ephemeris to be used, cf. swe_calc( ))

=head3 return array


      -> retflag	int            ERR or eclipse type
	  -> serr       string         Error string, on error only
      -> star		string         Corrected star name
	  -> attr       array of 8 double:
					attr[0] fraction of object's diameter covered by moon (magnitude)
					attr[1] ratio of lunar diameter to object's diameter
					attr[2] fraction of object's disc covered by moon (obscuration)
					attr[3] diameter of core shadow in km
					attr[4] azimuth of object at tjd
					attr[5] true altitude of object above horizon at tjd
					attr[6] apparent altitude of object above horizon at tjd
					attr[7] angular distance of moon from object in degrees

	  -> geopos     array of 2 doubles, geogr. position where eclipse is maximal

=head3 C declaration

  int swe_lun_occult_where(double tjd, int32 ipl, char *starname, int32 ifl, double *geopos, double *attr, char *serr);

=cut
 }}} */
PHP_FUNCTION(swe_lun_occult_where)
{
	char *arg = NULL;
	int ipl, ifl, rc;
	size_t arg_len;
	double tjd_ut, geopos[2], attr[20];
	char serr[AS_MAXCH], *starname = NULL; 
	char star[AS_MAXCH];
	int i;
	zval geopos_arr, attr_arr;
	*serr = '\0';
	*star = '\0';

	if(ZEND_NUM_ARGS() != 4) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dlsd",
			&tjd_ut, &ipl, &starname, &arg_len, &ifl) == FAILURE) {
		return;
	}
    if (starname != NULL && arg_len > 0)
		strcpy(star, starname);
	rc = swe_lun_occult_where(tjd_ut, ipl, star, ifl, geopos, attr, serr);

	array_init(return_value);
	add_assoc_long(return_value, "retflag", rc);

	if (rc == ERR)
	{
		add_assoc_string(return_value, "serr", serr);			
	}
	else
	{
		array_init(&geopos_arr);
		for(i = 0; i < 2; i++)
			add_index_double(&geopos_arr, i, geopos[i]);
		array_init(&attr_arr);
		for(i = 0; i < 8; i++)
			add_index_double(&attr_arr, i, attr[i]);
		add_assoc_zval(return_value, "geopos", &geopos_arr);
		add_assoc_zval(return_value, "attr", &attr_arr);
		if (starname != NULL && arg_len > 0)
			add_assoc_string(return_value, "star", star);
	}
}

PHP_FUNCTION(swe_sol_eclipse_how)
{
	char *arg = NULL;
	int arg_len, ifl, rc;
	double tjd_ut, geopos[3], attr[20];
	char serr[AS_MAXCH]; 
	int i;
	zval geopos_arr, attr_arr;
	*serr = '\0';

	if(ZEND_NUM_ARGS() != 5) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dlddd",
			&tjd_ut, &ifl, &geopos[0], &geopos[1], &geopos[2], &arg_len) == FAILURE) {
		return;
	}
	rc = swe_sol_eclipse_how(tjd_ut, ifl, geopos, attr, serr);

	array_init(return_value);
	add_assoc_long(return_value, "retflag", rc);

	if (rc == ERR)
	{
		add_assoc_string(return_value, "serr", serr);			
	}
	else
	{
		array_init(&attr_arr);
		for(i = 0; i < 20; i++)
			add_index_double(&attr_arr, i, attr[i]);
		
		add_assoc_zval(return_value, "attr", &attr_arr);
	}
}

PHP_FUNCTION(swe_sol_eclipse_when_loc)
{
	char *arg = NULL;
	int arg_len, ifl, rc;
	double tjd_start,  geopos[3], tret[10], attr[20];
	char serr[AS_MAXCH]; 
	int i;
	int backward;
	zval tret_arr, attr_arr;
	*serr = '\0';

	if(ZEND_NUM_ARGS() != 6) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dldddl",
			&tjd_start, &ifl, &geopos[0], &geopos[1], &geopos[2], &backward, &arg_len) == FAILURE) {
		return;
	}
	rc = swe_sol_eclipse_when_loc(tjd_start, ifl, geopos, tret, attr, backward, serr);

	array_init(return_value);
	add_assoc_long(return_value, "retflag", rc);

	if (rc == ERR)
	{
		add_assoc_string(return_value, "serr", serr);			
	}
	else
	{
		array_init(&tret_arr);
		for(i = 0; i < 10; i++)
			add_index_double(&tret_arr, i, attr[i]);
		add_assoc_zval(return_value, "tret", &tret_arr);

		array_init(&attr_arr);
		for(i = 0; i < 20; i++)
			add_index_double(&attr_arr, i, attr[i]);
		
		add_assoc_zval(return_value, "attr", &attr_arr);
	}
}

PHP_FUNCTION(swe_lun_occult_when_loc)
{
	char *arg = NULL;
	int ipl, ifl, rc;
	size_t arg_len;
	double tjd_start, geopos[3], tret[10], attr[20];
	char serr[AS_MAXCH], *starname = NULL; 
	int i;
	int backward;
	zval tret_arr, attr_arr;
	*serr = '\0';

	if(ZEND_NUM_ARGS() != 6) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dlsldddl",
			&tjd_start, &ipl, &starname, &arg_len, &ifl, &geopos[0], &geopos[1],
			&geopos[2], &backward) == FAILURE) {
		return;
	}
	rc = swe_lun_occult_when_loc(tjd_start, ipl, starname, ifl, geopos,
			tret, attr, backward, serr);

	array_init(return_value);
	add_assoc_long(return_value, "retflag", rc);

	if (rc == ERR)
	{
		add_assoc_string(return_value, "serr", serr);			
	}
	else
	{
		array_init(&tret_arr);
		for(i = 0; i < 10; i++)
			add_index_double(&tret_arr, i, attr[i]);
		add_assoc_zval(return_value, "tret", &tret_arr);

		array_init(&attr_arr);
		for(i = 0; i < 20; i++)
			add_index_double(&attr_arr, i, attr[i]);
		
		add_assoc_zval(return_value, "attr", &attr_arr);
	}
}

PHP_FUNCTION(swe_sol_eclipse_when_glob)
{
	char *arg = NULL;
	int arg_len, rc, ifl, ifltype;
	double tjd_start, tret[10];
	char serr[AS_MAXCH]; 
	int i, backward;
	zval tret_arr;
	*serr = '\0';

	if(ZEND_NUM_ARGS() != 4) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dlll",
			&tjd_start, &ifl, &ifltype, &backward, &arg_len) == FAILURE) {
		return;
	}
	rc = swe_sol_eclipse_when_glob(tjd_start, ifl, ifltype,	tret, backward, serr);

	array_init(return_value);
	add_assoc_long(return_value, "retflag", rc);

	if (rc == ERR)
	{
		add_assoc_string(return_value, "serr", serr);			
	}
	else
	{
		array_init(&tret_arr);
		
		for(i = 0; i < 10; i++)
			add_index_double(&tret_arr, i, tret[i]);
			
		add_assoc_zval(return_value, "tret", &tret_arr);
	}
}

PHP_FUNCTION(swe_lun_occult_when_glob)
{
	char *arg = NULL;
	int rc, ipl, ifl, ifltype;
	size_t arg_len, s_len = 0;
	double tjd_start, tret[10];
	char serr[AS_MAXCH], *starname = NULL; 
	char star[AS_MAXCH];
	int i, backward;
	zval tret_arr;
	*serr = '\0';
	*star = '\0';

	if(ZEND_NUM_ARGS() != 6) WRONG_PARAM_COUNT;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dlslll",
			&tjd_start, &ipl, &starname, s_len, &ifl, &ifltype, 
			&backward, &arg_len) == FAILURE) {
		return;
	}
return;
    if (starname != NULL && s_len > 0)
		strcpy(star, starname);
	// rc = swe_lun_occult_when_glob(tjd_start, ipl, star, ifl, ifltype, tret, backward, serr);
    rc = ERR;
	array_init(return_value);
	add_assoc_long(return_value, "retflag", rc);

	if (rc < 0) {
		add_assoc_string(return_value, "serr", serr);			
	} else {
		array_init(&tret_arr);
		for(i = 0; i < 10; i++)
			add_index_double(&tret_arr, i, tret[i]);
		add_assoc_zval(return_value, "tret", &tret_arr);
		if (starname != NULL && s_len > 0)
			add_assoc_string(return_value, "star", star);
	}
}

PHP_FUNCTION(swe_lun_eclipse_how)
{
	char *arg = NULL;
	int arg_len, rc, ifl;
	double tjd_ut, geopos[3], attr[10];
	char serr[AS_MAXCH], *starname = NULL; 
	int i, backward;
	zval attr_arr;
	*serr = '\0';

	if(ZEND_NUM_ARGS() != 5) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dlddd",
			&tjd_ut, &ifl, &geopos[0], &geopos[1], &geopos[2], &arg_len) == FAILURE) {
		return;
	}
	rc = swe_lun_eclipse_how(tjd_ut, ifl, geopos, attr, serr);

	array_init(return_value);
	add_assoc_long(return_value, "retflag", rc);

	if (rc == ERR)
	{
		add_assoc_string(return_value, "serr", serr);			
	}
	else
	{
		array_init(&attr_arr);
		
		for(i = 0; i < 10; i++)
			add_index_double(&attr_arr, i, attr[i]);
			
		add_assoc_zval(return_value, "attr", &attr_arr);
	}
}

PHP_FUNCTION(swe_lun_eclipse_when)
{
	char *arg = NULL;
	int arg_len, rc, ifl, ifltype;
	double tjd_start, tret[10];
	char serr[AS_MAXCH]; 
	int i, backward;
	zval tret_arr;
	*serr = '\0';

	if(ZEND_NUM_ARGS() != 4) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dlll",
			&tjd_start, &ifl, &ifltype, &backward, &arg_len) == FAILURE) {
		return;
	}
	rc = swe_lun_eclipse_when(tjd_start, ifl, ifltype, tret, backward, serr);

	array_init(return_value);
	add_assoc_long(return_value, "retflag", rc);

	if (rc == ERR)
	{
		add_assoc_string(return_value, "serr", serr);			
	}
	else
	{
		array_init(&tret_arr);
		
		for(i = 0; i < 10; i++)
			add_index_double(&tret_arr, i, tret[i]);
			
		add_assoc_zval(return_value, "tret", &tret_arr);
	}
}

PHP_FUNCTION(swe_lun_eclipse_when_loc)
{
	char *arg = NULL;
	int arg_len, rc, ifl;
	double tjd_ut, geopos[3], tret[10], attr[20];
	char serr[AS_MAXCH], *starname = NULL;
	int i, backward;
	zval tret_arr, attr_arr;
	*serr = '\0';

	if(ZEND_NUM_ARGS() != 6) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dldddl",
			&tjd_ut, &ifl, &geopos[0], &geopos[1], &geopos[2], &backward, &arg_len) == FAILURE) {
		return;
	}
	rc = swe_lun_eclipse_when_loc(tjd_ut, ifl, geopos, tret, attr, backward, serr);

	array_init(return_value);
	add_assoc_long(return_value, "retflag", rc);

	if (rc == ERR)
	{
		add_assoc_string(return_value, "serr", serr);			
	}
	else
	{
		array_init(&tret_arr);

		for(i = 0; i < 10; i++)
			add_index_double(&tret_arr, i, tret[i]);
		add_assoc_zval(return_value, "tret", &tret_arr);	

		array_init(&attr_arr);

		for(i = 0; i < 20; i++)
			add_index_double(&attr_arr, i, attr[i]);			
		add_assoc_zval(return_value, "attr", &attr_arr);
	}
}

PHP_FUNCTION(swe_pheno)
{
	char *arg = NULL;
	int arg_len, rc, ipl, iflag;
	double tjd, attr[20];
	char serr[AS_MAXCH]; 
	int i;
	zval attr_arr;
	*serr = '\0';

	if(ZEND_NUM_ARGS() != 3) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dll",
			&tjd, &ipl, &iflag, &arg_len) == FAILURE) {
		return;
	}
	rc = swe_pheno(tjd, ipl, iflag, attr, serr);

	array_init(return_value);
	add_assoc_long(return_value, "retflag", rc);

	if (rc == ERR)
	{
		add_assoc_string(return_value, "serr", serr);			
	}
	else
	{
		array_init(&attr_arr);
		
		for(i = 0; i < 20; i++)
			add_index_double(&attr_arr, i, attr[i]);
			
		add_assoc_zval(return_value, "attr", &attr_arr);
	}
}

PHP_FUNCTION(swe_pheno_ut)
{
	char *arg = NULL;
	int arg_len, rc, ipl, iflag;
	double tjd_ut, attr[20];
	char serr[AS_MAXCH]; 
	int i;
	zval attr_arr;
	*serr = '\0';

	if(ZEND_NUM_ARGS() != 3) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dll",
			&tjd_ut, &ipl, &iflag, &arg_len) == FAILURE) {
		return;
	}
	rc = swe_pheno_ut(tjd_ut, ipl, iflag, attr, serr);

	array_init(return_value);
	add_assoc_long(return_value, "retflag", rc);

	if (rc == ERR)
	{
		add_assoc_string(return_value, "serr", serr);			
	}
	else
	{
		array_init(&attr_arr);
		
		for(i = 0; i < 20; i++)
			add_index_double(&attr_arr, i, attr[i]);
			
		add_assoc_zval(return_value, "attr", &attr_arr);
	}
}

PHP_FUNCTION(swe_refrac)
{
	char *arg = NULL;
	int arg_len, rc, calc_flag;
	double inalt, atpress, attemp;

	if(ZEND_NUM_ARGS() != 4) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddl",
			&inalt, &atpress, &attemp, &calc_flag, &arg_len) == FAILURE) {
		return;
	}
	rc = swe_refrac(inalt, atpress, attemp, calc_flag);

	RETURN_DOUBLE(rc);
}

PHP_FUNCTION(swe_refrac_extended)
{
	char *arg = NULL;
	int arg_len, rc, calc_flag, i;
	double inalt, geoalt, atpress, lapse_rate, attemp;
	double dret[4];

	if(ZEND_NUM_ARGS() != 6) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddddl",
			&inalt, &geoalt, &atpress, &lapse_rate, &attemp, &calc_flag, &arg_len) == FAILURE) {
		return;
	}
	rc = swe_refrac_extended(inalt, geoalt, atpress, lapse_rate, attemp, calc_flag, dret);

	RETURN_DOUBLE(rc);
	
	array_init(return_value);
	for(i = 0; i < 3; i++)
		add_index_double(return_value, i, dret[i]);
	add_assoc_double(return_value, "retflag", rc);
}

PHP_FUNCTION(swe_set_lapse_rate)
{
	char *arg = NULL;
	int arg_len;
	double lapse_rate;

	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d",
			&lapse_rate, &arg_len) == FAILURE) {
		return;
	}
	swe_set_lapse_rate(lapse_rate);
}

PHP_FUNCTION(swe_heliacal_ut)
{
	char *arg = NULL;
	int arg_len, rc, ipl, iflag;
	double tjdstart, dgeo[3], datm[4], dobs[6], dret[3];
	char serr[AS_MAXCH], *objectname = NULL; 
	int i, olen, event_type, helflag;
	*serr = '\0';
	if(ZEND_NUM_ARGS() != 17) WRONG_PARAM_COUNT;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddddddddddddddsll",
			&tjdstart, &dgeo[0], &dgeo[1], &dgeo[2], &datm[0], &datm[1], &datm[2], &datm[3],  &dobs[0], &dobs[1], &dobs[2], &dobs[3], &dobs[4], &dobs[5],  &objectname, &olen, &event_type, &helflag,  &arg_len) == FAILURE) {
		return;
	}
	rc = swe_heliacal_ut(tjdstart, dgeo, datm, dobs, objectname, event_type, helflag, dret, serr);
	array_init(return_value);
	add_assoc_long(return_value, "rc", rc);
	if (rc == ERR) {
		add_assoc_string(return_value, "serr", serr);			
	} else {
		for(i = 0; i < 3 ; i++)
			add_index_double(return_value, i, dret[i]);
	}
}

PHP_FUNCTION(swe_heliacal_pheno_ut)
{
	char *arg = NULL;
	int arg_len, rc, ipl, iflag;
	double tjdstart, dgeo[3], datm[4], dobs[6], darr[50];
	char serr[AS_MAXCH], *objectname = NULL; 
	int i, olen, event_type, helflag;
	*serr = '\0';
	if(ZEND_NUM_ARGS() != 17) WRONG_PARAM_COUNT;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddddddddddddddsll",
			&tjdstart, &dgeo[0], &dgeo[1], &dgeo[2], &datm[0], &datm[1], &datm[2], &datm[3],  &dobs[0], &dobs[1], &dobs[2], &dobs[3], &dobs[4], &dobs[5],  &objectname, &olen, &event_type, &helflag,  &arg_len) == FAILURE) {
		return;
	}
	rc = swe_heliacal_pheno_ut(tjdstart, dgeo, datm, dobs, objectname, event_type, helflag, darr, serr);
	array_init(return_value);
	add_assoc_long(return_value, "rc", rc);
	if (rc == ERR) {
		add_assoc_string(return_value, "serr", serr);			
	} else {
		for(i = 0; i <= 29 ; i++)
			add_index_double(return_value, i, darr[i]);
	}
}

PHP_FUNCTION(swe_vis_limit_mag)
{
	char *arg = NULL;
	int arg_len, rc, ipl, iflag;
	double tjdstart, dgeo[3], datm[4], dobs[6], darr[8];
	char serr[AS_MAXCH], *objectname = NULL; 
	int i, olen, helflag;
	*serr = '\0';
	if(ZEND_NUM_ARGS() != 17) WRONG_PARAM_COUNT;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddddddddddddddsl",
			&tjdstart, &dgeo[0], &dgeo[1], &dgeo[2], &datm[0], &datm[1], &datm[2], &datm[3],  &dobs[0], &dobs[1], &dobs[2], &dobs[3], &dobs[4], &dobs[5],  &objectname, &olen, &helflag,  &arg_len) == FAILURE) {
		return;
	}
	rc = swe_vis_limit_mag(tjdstart, dgeo, datm, dobs, objectname, helflag, darr, serr);
	array_init(return_value);
	add_assoc_long(return_value, "rc", rc);
	if (rc == ERR) {
		add_assoc_string(return_value, "serr", serr);			
	} else {
		for(i = 0; i < 8 ; i++)
			add_index_double(return_value, i, darr[i]);
	}
}

/* {{{ pod
=pod

=head1 function swe_azalt(tjd_ut, calc_flag, geolon, geolat, geoalt, atpress, attemp, xin0, xin1)

Computes azimut and height, from either ecliptic or equatorial coordinates

=head3 Parameters

  double        tjd_ut      
  int           calc_flag
  double		geolon		longitude
  double		geolat		latitude
  double		geoalt		altitude above sea
  double		atpress		atmospheric pressure
  double		attemp		atmospheric temperature
  double		xin0		longitude of object
  double		xin1		latitude of object

=head3 return array

  [0..1]			array of 2 doubles: 
                     xaz[0] = azimuth
					 xaz[1] = true altitude
					 xaz[2] = apparent altitude


=head3 C declaration

  void  swe_azalt( double tjd_ut, int32  calc_flag, double *geopos, double atpress, double attemp, double *xin, double *xaz)

=cut
 }}} */
PHP_FUNCTION(swe_azalt)
{
	char *arg = NULL;
	int arg_len, rc, calc_flag;
	double tjd_ut, geopos[3], atpress, attemp, xin[2], xaz[3];
	int i;

	if(ZEND_NUM_ARGS() != 9) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dldddddddd",
			&tjd_ut, &calc_flag, 
			&geopos[0], &geopos[1], &geopos[2], 
			&atpress, &attemp,
			&xin[0], &xin[1], 
			&arg_len) == FAILURE) {
		return;
	}
	swe_azalt(tjd_ut, calc_flag, geopos, atpress, attemp, xin, xaz);

	array_init(return_value);
	for(i = 0; i < 3; i++)
		add_index_double(return_value, i, xaz[i]);
}

/* {{{ pod
=pod

=head1 function swe_azalt_rev(tjd_ut, iflag, xin0, xin1)

computes either ecliptical or equatorial coordinates from azimuth and true altitude in degrees.

=head3 Parameters

  double        tjd_ut      
  int           iflag		either SE_HOR2ECL or SE_HOR2EQU
  double		xin0		azimut, in degrees
  double		xin1		true altitude, in degrees

=head3 return array

  [0..2]			array of 3 doubles: 
                     xout[0] = longitude
                     xout[1] = latitude


=head3 C declaration

  void  swe_azalt_rev( double tjd_ut, int32  calc_flag, double *geopos, double *xin, double *xout) 

=cut
 }}} */
PHP_FUNCTION(swe_azalt_rev)
{
	char *arg = NULL;
	int arg_len, calc_flag;
	double tjd_ut, geopos[3], xin[2], xout[3];
	int i;

	if(ZEND_NUM_ARGS() != 7) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dlddddd",
			&tjd_ut, &calc_flag, 
			&geopos[0], &geopos[1], &geopos[2], 
			&xin[0], &xin[1], 
			&arg_len) == FAILURE) {
		return;
	}
	swe_azalt_rev(tjd_ut, calc_flag, geopos, xin, xout);

	array_init(return_value);
	for(i = 0; i < 2; i++)
		add_index_double(return_value, i, xout[i]);
}

/* {{{ pod
=pod

=head1 function swe_rise_trans(tjd_ut, ipl, starname, epheflag, rsmi, geolon, geolat, geoalt, atpress, attemp)

rise, set, and meridian transits of sun, moon, planets, and stars

=head3 Parameters

  double        tjd_ut      
  int           ipl	        planet number
  string        starname	(used instead of planet if not null or empty)
  int           epheflag
  int           rsmi        flag combination, defines what is computed
  double		geolon		longitude
  double		geolat		latitude
  double		geoalt		altitude above sea
  double		atpress		atmospheric pressure
  double		attemp		atmospheric temperature

=head3 return array

  [0..9]			array of 10 doubles
  ['star']	        string, present only if starname was used in call parameters
  ['rc']			int	return flag, < 0 in case of error
  
  in case of error
  ['rc']			int	< 0 in case of error
  ['serr']			string

=head3 C declaration

  int  swe_rise_trans( double tjd_ut, int32 ipl, char *starname, int32 epheflag, int32 rsmi, double *geopos, double atpress, double attemp, double *tret, char *serr)

=cut
 }}} */
PHP_FUNCTION(swe_rise_trans)
{
	char *arg = NULL;
	int arg_len, rc, s_len;
	long ipl, epheflag, rsmi;
	double tjd_ut, geopos[3], tret[10], atpress, attemp;
	char serr[AS_MAXCH], *starname = NULL; 
	char star[AS_MAXCH];
	int i;
	zval tret_arr;
	*serr = '\0';
	*star = '\0';

	if(ZEND_NUM_ARGS() != 10) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dlsllddddd",
			&tjd_ut, &ipl, &starname, &s_len, &epheflag, &rsmi,
			&(geopos[0]), &(geopos[1]), &(geopos[2]),
			&atpress, &attemp, 
			&arg_len) == FAILURE) {
		return;
	}

    if (starname != NULL && s_len > 0)
		strcpy(star, starname);
	rc = swe_rise_trans(tjd_ut, ipl, star, epheflag, rsmi,
			&(geopos[0]), atpress, attemp, tret, serr);

	array_init(return_value);
	add_assoc_long(return_value, "rc", rc);

	if (rc == ERR)
	{
		add_assoc_string(return_value, "serr", serr);			
	}
	else
	{
		array_init(&tret_arr);
		for(i = 0; i < 10; i++)
			add_index_double(&tret_arr, i, tret[i]);
		add_assoc_zval(return_value, "tret", &tret_arr);
		if (starname != NULL && s_len > 0)
			add_assoc_string(return_value, "star", star);
	}
}

/* {{{ pod
=pod

=head1 function swe_rise_trans_true_hor(tjd_ut, ipl, starname, epheflag, rsmi, geolon, geolat, geoalt, atpress, attemp, horhgt)

rise, set, and meridian transits of sun, moon, planets, and stars

=head3 Parameters

  double        tjd_ut      
  int           ipl	        planet number
  string        starname	(used instead of planet if not null or empty)
  int           epheflag
  int           rsmi        flag combination, defines what is computed
  double		geolon		longitude
  double		geolat		latitude
  double		geoalt		altitude above sea
  double		atpress		atmospheric pressure
  double		attemp		atmospheric temperature
  double		horhgt		height of horizon

=head3 return array

  [0..9]			array of 10 doubles
  ['star']	        string, present only if starname was used in call parameters
  ['rc']			int	return flag, < 0 in case of error
  
  in case of error
  ['rc']			int	< 0 in case of error
  ['serr']			string

=head3 C declaration

  int  swe_rise_trans_true_hor( double tjd_ut, int32 ipl, char *starname, int32 epheflag, int32 rsmi, double *geopos, double atpress, double attemp, double horhgt, double *tret, char *serr)

=cut
 }}} */
PHP_FUNCTION(swe_rise_trans_true_hor)
{
	char *arg = NULL;
	int arg_len, rc, s_len;
	long ipl, epheflag, rsmi;
	double tjd_ut, geopos[3], tret[10], atpress, attemp, horhgt;
	char serr[AS_MAXCH], *starname = NULL; 
	char star[AS_MAXCH];
	int i;
	zval tret_arr;
	*serr = '\0';
	*star = '\0';

	if(ZEND_NUM_ARGS() != 11) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dlslldddddd",
			&tjd_ut, &ipl, &starname, &s_len, &epheflag, &rsmi,
			&geopos[0], &geopos[1], &geopos[2],
			&atpress, &attemp, &horhgt,
			&arg_len) == FAILURE) {
		return;
	}
    if (starname != NULL && s_len > 0)
		strcpy(star, starname);
	rc = swe_rise_trans_true_hor(tjd_ut, ipl, star, epheflag, rsmi,
			geopos, atpress, attemp, horhgt, tret, serr);

	array_init(return_value);
	add_assoc_long(return_value, "rc", rc);

	if (rc == ERR)
	{
		add_assoc_string(return_value, "serr", serr);			
	}
	else
	{
		array_init(&tret_arr);
		for(i = 0; i < 10; i++)
			add_index_double(&tret_arr, i, tret[i]);
		add_assoc_zval(return_value, "tret", &tret_arr);
		if (starname != NULL && s_len > 0)
			add_assoc_string(return_value, "star", star);
	}
}

PHP_FUNCTION(swe_nod_aps)
{
	char *arg = NULL;
	int arg_len, rc;
    long ipl, iflag, method;
	double tjd_et, xnasc[6], xndsc[6], xperi[6], xaphe[6];
	char serr[AS_MAXCH]; 
	int i;
	zval xnasc_arr, xndsc_arr, xperi_arr, xaphe_arr;
	*serr = '\0';

	if(ZEND_NUM_ARGS() != 4) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dlll",
			&tjd_et, &ipl, &iflag, &method,
			&arg_len) == FAILURE) {
		return;
	}
	rc = swe_nod_aps(tjd_et, ipl, iflag, method, xnasc, xndsc, xperi, xaphe, serr);

	array_init(return_value);
	add_assoc_long(return_value, "retflag", rc);

	if (rc == ERR)
	{
		add_assoc_string(return_value, "serr", serr);			
	}
	else
	{
		array_init(&xnasc_arr);
		for(i = 0; i < 6; i++)
			add_index_double(&xnasc_arr, i, xnasc[i]);

		array_init(&xndsc_arr);
		for(i = 0; i < 6; i++)
			add_index_double(&xndsc_arr, i, xndsc[i]);

		array_init(&xperi_arr);
		for(i = 0; i < 6; i++)
			add_index_double(&xperi_arr, i, xperi[i]);

		array_init(&xaphe_arr);
		for(i = 0; i < 6; i++)
			add_index_double(&xaphe_arr, i, xaphe[i]);
			
		add_assoc_zval(return_value, "xnasc", &xnasc_arr);
		add_assoc_zval(return_value, "xndsc", &xndsc_arr);
		add_assoc_zval(return_value, "xnperi", &xperi_arr);
		add_assoc_zval(return_value, "xnaphe", &xaphe_arr);
	}
}

PHP_FUNCTION(swe_nod_aps_ut)
{
	char *arg = NULL;
	int arg_len, rc;
    long ipl, iflag, method;
	double tjd_ut, xnasc[6], xndsc[6], xperi[6], xaphe[6];
	char serr[AS_MAXCH]; 
	int i;
	zval xnasc_arr, xndsc_arr, xperi_arr, xaphe_arr;
	*serr = '\0';

	if(ZEND_NUM_ARGS() != 4) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dlll",
			&tjd_ut, &ipl, &iflag, &method,
			&arg_len) == FAILURE) {
		return;
	}
	rc = swe_nod_aps_ut(tjd_ut, ipl, iflag, method, xnasc, xndsc, xperi, xaphe, serr);

	array_init(return_value);
	add_assoc_long(return_value, "retflag", rc);

	if (rc == ERR)
	{
		add_assoc_string(return_value, "serr", serr);			
	}
	else
	{
		array_init(&xnasc_arr);
		for(i = 0; i < 6; i++)
			add_index_double(&xnasc_arr, i, xnasc[i]);

		array_init(&xndsc_arr);
		for(i = 0; i < 6; i++)
			add_index_double(&xndsc_arr, i, xndsc[i]);

		array_init(&xperi_arr);
		for(i = 0; i < 6; i++)
			add_index_double(&xperi_arr, i, xperi[i]);

		array_init(&xaphe_arr);
		for(i = 0; i < 6; i++)
			add_index_double(&xaphe_arr, i, xaphe[i]);
			
		add_assoc_zval(return_value, "xnasc", &xnasc_arr);
		add_assoc_zval(return_value, "xndsc", &xndsc_arr);
		add_assoc_zval(return_value, "xnperi", &xperi_arr);
		add_assoc_zval(return_value, "xnaphe", &xaphe_arr);
	}
}

/* {{{ pod
=pod

=head1 function swe_get_orbital_elements(tjd_1t, ipl, iflag)

Calculates osculating orbital elements (Kepler elements) of a planet 
or asteroid or the Earth-Moon barycentre. 
The function returns error if called for the Sun, the lunar nodes, or the apsides.

=head3 Parameters

  double        tjd_et      Julian day in Ephemeris Time.
  int           ipl         Planet/body/object number or constant.
  int           iflag       Flag bits for computation requirements.

=head3 return array

  [0..16]       array double, with elements
  ['serr']      string	optional error message
  ['rc']        int		return flag, < 0 in case of error

=head3 C declaration

  int swe_get_orbital_elements ( double tjd_et, int ipl, int iflag, double* xx, char* serr);

=cut
 }}} */
PHP_FUNCTION(swe_get_orbital_elements)
{
	char *arg = NULL;
	int rc;
	long ipl, iflag;
	double tjd_et, dret[50];
	char serr[AS_MAXCH]; 
	int i;
	*serr = '\0';
	
	if(ZEND_NUM_ARGS() != 3) WRONG_PARAM_COUNT;
		
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dll",
			&tjd_et, &ipl, &iflag) == FAILURE) {
		return;
	}
	rc = swe_get_orbital_elements(tjd_et, (int)ipl, (int)iflag, dret, serr);

		/* create an array */
	array_init(return_value);
	for(i = 0; i < 17; i++)
		add_index_double(return_value, i, dret[i]);
	add_assoc_string(return_value, "serr", serr);
	add_assoc_long(return_value, "rc", rc);
}

/* {{{ pod
=pod

=head1 function swe_orbit_max_min_true_distance(tjd_et, ipl, iflag)

This function calculates calculates the maximum possible distance, the
minimum possible distance, and the current true distance of planet, the EMB,
or an asteroid. The calculation can be done either heliocentrically or
geocentrically.

=head3 Parameters

  double        tjd_et      Julian day in Ephemeris Time.
  int           ipl         Planet/body/object number or constant.
  int           iflag       Flag bits for computation requirements.

=head3 return array

  ['dmax']      double
  ['dmin']      double
  ['dtrue']     double
  ['serr']      string	optional error message
  ['rc']        int		return flag, < 0 in case of error

=head3 C declaration

  int swe_orbit_max_min_true_distance ( double tjd_et, int ipl, int iflag, double* xx, char* serr);

=cut
 }}} */
PHP_FUNCTION(swe_orbit_max_min_true_distance)
{
	char *arg = NULL;
	int rc;
	long ipl, iflag;
	double tjd_et, dmax, dmin, dtrue;
	char serr[AS_MAXCH]; 
	int i;
	*serr = '\0';
	
	if(ZEND_NUM_ARGS() != 3) WRONG_PARAM_COUNT;
		
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dll",
			&tjd_et, &ipl, &iflag) == FAILURE) {
		return;
	}
	rc = swe_orbit_max_min_true_distance(tjd_et, (int)ipl, (int)iflag, &dmax, &dmin, &dtrue, serr);

		/* create an array */
	array_init(return_value);
	if (rc >= 0) {
		add_assoc_double(return_value, "dmax", dmax);
		add_assoc_double(return_value, "dmin", dmin);
		add_assoc_double(return_value, "dtrue", dtrue);
	}
	add_assoc_string(return_value, "serr", serr);
	add_assoc_long(return_value, "rc", rc);
}

/**************************** 
 * exports from swephlib.c 
 ****************************/

/* {{{ pod
=pod

=head1 function swe_deltat(tjd)

Calculate delta t (difference between UT & ET) from Julian day number.

=head3 Parameters

  double        tjd         Julian day in Universal Time.

=head3 return value

  double        Delta T.

=head3 C declaration

  double swe_deltat(double tjd);

=cut
 }}} */
PHP_FUNCTION(swe_deltat)
{
	double tjd_ut;
	
	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d",
			&tjd_ut) == FAILURE) {
		return;
	}

	RETURN_DOUBLE(swe_deltat(tjd_ut));
}

/* {{{ pod
=pod

=head1 function swe_deltat_ex(tjd, ephe_flag)

Calculate delta t (difference between UT & ET) from Julian day number.

If the function is called with SEFLG_SWIEPH before calling swe_set_ephe_path(),
or with or SEFLG_JPLEPH before calling swe_set_jpl_file(),
then the function returns a warning.

=head3 Parameters

  double        tjd         Julian day in Universal Time.
  int           ephe_flag   Ephemeris flag (one of SEFLG_SWIEPH, SEFLG_JPLEPH, SEFLG_MOSEPH).

=head3 return array

    [
        'tjd_et' => (double) Delta T.
        'serr' => (string) Error string.
    ]

=head3 C declaration

  double swe_deltat_ex(double tjd, int32 ephe_flag, char *serr);

=cut
 }}} */
PHP_FUNCTION(swe_deltat_ex)
{
	double tjd_ut, tjd_et;
	long ephe_flag;
	char serr[AS_MAXCH];
	*serr = '\0';

	if (ZEND_NUM_ARGS() != 2) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dl",
			&tjd_ut, &ephe_flag) == FAILURE) {
		return;
	}

	tjd_et = swe_deltat_ex(tjd_ut, ephe_flag, serr);

	array_init(return_value);

	add_assoc_double(return_value, "tjd_et", tjd_et);
	add_assoc_string(return_value, "serr", serr);
}

/* {{{ pod
=pod

=head1 function swe_time_equ(tjd_et)

Get the difference between local apparent and local mean time.

=head3 Parameters

  double        tjd_et      Julian day in Ephemeris Time.

=head3 return array

    [
        'rd' => (int) Return code.
        'td' => (double) Local Apparent Time - Local Mean Time.
        'serr' => (string) Error string.
    ]

=head3 C declaration

  int swe_time_equ(double tjd_et, double *e, char *serr);

=cut
 }}} */
PHP_FUNCTION(swe_time_equ)
{
	char *arg = NULL;
	double tjd, te;
	int rc;
	char serr[AS_MAXCH]; 
	*serr = '\0';
	
	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d",
			&tjd) == FAILURE) {
		return;
	}
	rc = swe_time_equ(tjd, &te, serr);
	array_init(return_value);
	
	add_assoc_long(return_value, "rc", rc);
	add_assoc_double(return_value, "te", te);
	add_assoc_string(return_value, "serr", serr);
}

/* {{{ pod
=pod

=head1 function swe_lmt_to_lat(tjd_lmt, geolon)

Converts Local Mean Time (LMT) to Local Apparent Time (LAT).

=head3 Parameters

  double        tjd_lmt     Julian day in Local Mean Time.
  double        geolon      Longitude of geographic location.

=head3 return array

    [
        'rd' => (int) Return code.
        'tjd_lat' => (double) Local Apparent Time.
        'serr' => (string) Error string.
    ]

=head3 C declaration

  int32 swe_lmt_to_lat(double tjd_lmt, double geolon, double *tjd_lat, char *serr);

=cut
 }}} */
PHP_FUNCTION(swe_lmt_to_lat)
{
	char *arg = NULL;
	double tjd_lmt, geolon;
	double tjd_lat;
	int rc;
	char serr[AS_MAXCH]; 
	*serr = '\0';
	
	if(ZEND_NUM_ARGS() != 2) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd",
			&tjd_lmt, &geolon) == FAILURE) {
		return;
	}
	rc = swe_lmt_to_lat(tjd_lmt, geolon, &tjd_lat, serr);
	array_init(return_value);
	
	add_assoc_long(return_value, "rc", rc);
	add_assoc_double(return_value, "tjd_lat", tjd_lat);
	add_assoc_string(return_value, "serr", serr);
}

/* {{{ pod
=pod

=head1 function swe_lat_to_lmt(tjd_lat, geolon)

Converts Local Apparent Time (LAT) to Local Mean Time (LMT).

=head3 Parameters

  double        tjd_lat     Julian day in Local Apparent Time.
  double        geolon      Longitude of geographic location.

=head3 return array

    [
        'rd' => (int) Return code.
        'tjd_lmt' => (double) Local Mean Time.
        'serr' => (string) Error string.
    ]

=head3 C declaration

  int32 swe_lat_to_lmt(double tjd_lat, double geolon, double *tjd_lmt, char *serr);

=cut
 }}} */
PHP_FUNCTION(swe_lat_to_lmt)
{
	char *arg = NULL;
	double tjd_lmt, tjd_lat, geolon;
	int rc;
	char serr[AS_MAXCH]; 
	*serr = '\0';
	
	if(ZEND_NUM_ARGS() != 2) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd",
			&tjd_lat, &geolon) == FAILURE) {
		return;
	}
	rc = swe_lmt_to_lat(tjd_lat, geolon, &tjd_lmt, serr);
	array_init(return_value);
	
	add_assoc_long(return_value, "rc", rc);
	add_assoc_double(return_value, "tjd_lmt", tjd_lmt);
	add_assoc_string(return_value, "serr", serr);
}

/* {{{ pod
=pod

=head1 function swe_sidtime0(tjd_ut, eps, nut)

Get sidereal time with user-specified ecliptic obliquity and nutation.

=head3 Parameters

  double        tjd_ut      Julian day in Universal Time.
  double        eps         Obliquity of ecliptic, in degrees.
  double        nut         Nutation in longitude, in degrees.

=head3 return value

    double      Sidereal time in degrees.

=head3 C declaration

  double swe_sidtime0(double tjd_ut, double eps, double nut);

=cut
 }}} */
PHP_FUNCTION(swe_sidtime0)
{
	double tjd_ut, eps, nut, rc;
	
	if(ZEND_NUM_ARGS() != 3) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddd",
			&tjd_ut, &eps, &nut) == FAILURE) {
		return;
	}
	RETURN_DOUBLE(swe_sidtime0(tjd_ut, eps, nut));
}

/* {{{ pod
=pod

=head1 function swe_sidtime(tjd_ut)

Get sidereal time (ecliptic obliquity and nutation calculated internally).

=head3 Parameters

  double        tjd_ut      Julian day in Universal Time.

=head3 return value

    double      Sidereal time in degrees.

=head3 C declaration

  double swe_sidtime(double tjd_ut);

=cut
 }}} */
PHP_FUNCTION(swe_sidtime)
{
	double tjd_ut, rc;
	
	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d",
			&tjd_ut) == FAILURE) {
		return;
	}

	RETURN_DOUBLE(swe_sidtime(tjd_ut));
}

/* {{{ pod
=pod

=head1 function swe_cotrans(lng, lat, dist, eps)

Transform coordinates from ecliptic to equatorial, or vice-versa.
Convert equatorial to ecliptic if eps is positive.
Convert ecliptic to equatorial if eps is negative.

=head3 Parameters

  double        lng     Longitude/right ascension position.
  double        lat     Latitude/declination position.
  double        dist    Distance (ignored).

=head3 return array

    [
        0 => (double) Converted longitude/right ascension position.
        1 => (double) Converted latitude/declination position.
        2 => (double) distance (unchanged).
    ]

=head3 C declaration

  void swe_cotrans(double *xpo, double *xpn, double eps);

=cut
 }}} */
PHP_FUNCTION(swe_cotrans)
{
	int i;
	double xpo[3], xpn[3], eps;
	
	if(ZEND_NUM_ARGS() != 4) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddd",
			&xpo[0], &xpo[1], &xpo[2], &eps) == FAILURE) {
		return;
	}
	
	swe_cotrans(xpo, xpn, eps);

	/* create an array */
	array_init(return_value);
	for(i = 0; i < 3; i++)
		add_index_double(return_value, i, xpn[i]);
}

/* {{{ pod
=pod

=head1 function swe_cotrans_sp(lng, lat, dist, lngs, lats, dists, eps)

Transform position and speed coordinates from ecliptic to equatorial, or vice-versa.
Convert equatorial to ecliptic if eps is positive.
Convert ecliptic to equatorial if eps is negative.

=head3 Parameters

  double        lng     Longitude/right ascension.
  double        lat     Latitude/declination.
  double        dist    Distance (ignored).
  double        lngs    Longitude/right ascension velocity.
  double        lat     Latitude/declination velocity.
  double        dists   Distance velocity (ignored).

=head3 return array

    [
        0 => (double) Converted longitude/right ascension value.
        1 => (double) Converted latitude/declination value.
        2 => (double) distance (unchanged).
        3 => (double) Converted longitude/right ascension velocity.
        4 => (double) Converted latitude/declination velocity.
        5 => (double) distance velocity (unchanged).
    ]

=head3 C declaration

  void swe_cotrans_sp(double *xpo, double *xpn, double eps);

=cut
 }}} */
PHP_FUNCTION(swe_cotrans_sp)
{
	int i;
	double xpo[6], xpn[6], eps;
	
	if(ZEND_NUM_ARGS() != 7) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ddddddd",
			&xpo[0], &xpo[1], &xpo[2], &xpo[3], &xpo[4], &xpo[5], &eps) == FAILURE) {
		return;
	}
	
	swe_cotrans_sp(xpo, xpn, eps);

	/* create an array */
	array_init(return_value);
	for(i = 0; i < 6; i++)
		add_index_double(return_value, i, xpn[i]);
}

/* {{{ pod
=pod

=head1 function swe_get_tid_acc()

Get tidal acceleration used in swe_deltat().

=head3 Parameters (none)

=head3 return value

    double      Tidal acceleration value.

=head3 C declaration

  double swe_get_tid_acc(void);

=cut
 }}} */
PHP_FUNCTION(swe_get_tid_acc)
{
	if(ZEND_NUM_ARGS() != 0) WRONG_PARAM_COUNT;

	RETURN_DOUBLE(swe_get_tid_acc());
}

/* {{{ pod
=pod

=head1 function swe_set_tid_acc(t_acc)

Set tidal acceleration used in swe_deltat().

=head3 Parameters

    double      Tidal acceleration.

=head3 return (none)

=head3 C declaration

  void swe_set_tid_acc(double t_acc);

=cut
 }}} */
PHP_FUNCTION(swe_set_tid_acc)
{
	double t_acc;
	
	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d",
			&t_acc) == FAILURE) {
		return;
	}
	swe_set_tid_acc(t_acc);
	
	RETURN_NULL();
}

/* {{{ pod
=pod

=head1 function swe_set_delta_t_userdef(t_acc)

Set fixed Delta T value to be returned by swe_deltat().

=head3 Parameters

    double      Delta T.

=head3 return (none)

=head3 C declaration

  void swe_set_delta_t_userdef(double t_acc);

=cut
 }}} */
PHP_FUNCTION(swe_set_delta_t_userdef)
{
	double dt;
	
	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d",
			&dt) == FAILURE) {
		return;
	}
	swe_set_delta_t_userdef(dt);
	
	RETURN_NULL();
}

/* {{{ pod
=pod

=head1 function swe_degnorm(deg)

Normalize degrees to the range >= 0 & < 360.

=head3 Parameters

    double      Degree.

=head3 return value

    double      Normalized degree.

=head3 C declaration

  double swe_degnorm(double x);

=cut
 }}} */
PHP_FUNCTION(swe_degnorm)
{
	double x, rc;
	
	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d", &x) == FAILURE) {
		return;
	}

	RETURN_DOUBLE(swe_degnorm(x));
}

/* {{{ pod
=pod

=head1 function swe_radnorm(rad)

Normalize radians to the range >= 0 & < 2 PI.

=head3 Parameters

    double      Radians.

=head3 return value

    double      Normalized radians.

=head3 C declaration

  double swe_radnorm(double x);

=cut
 }}} */
PHP_FUNCTION(swe_radnorm)
{
	double x;
	
	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d",
			&x) == FAILURE) {
		return;
	}

	RETURN_DOUBLE(swe_radnorm(x));
}

PHP_FUNCTION(swe_rad_midp)
{
	double x1, x0;
	
	if(ZEND_NUM_ARGS() != 2) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd",
			&x1, &x0) == FAILURE) {
		return;
	}

	RETURN_DOUBLE(swe_rad_midp(x1, x0));
}

PHP_FUNCTION(swe_deg_midp)
{
	double x1, x0;
	
	if(ZEND_NUM_ARGS() != 2) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd",
			&x1, &x0) == FAILURE) {
		return;
	}

	RETURN_DOUBLE(swe_deg_midp(x1, x0));
}

PHP_FUNCTION(swe_split_deg)
{
	long roundflag;
	double ddeg, dsecfr;
	int ideg, imin, isec, isgn;
	
	if(ZEND_NUM_ARGS() != 2) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dl",
			&ddeg, &roundflag) == FAILURE) {
		return;
	}
	swe_split_deg(ddeg, (int)roundflag, &ideg, &imin, &isec, &dsecfr, &isgn);
	array_init(return_value);
	add_assoc_long(return_value, "deg", ideg);
	add_assoc_long(return_value, "min", imin);
	add_assoc_long(return_value, "sec", isec);
	add_assoc_double(return_value, "secfr", dsecfr);
	add_assoc_long(return_value, "sgn", isgn);
}

PHP_FUNCTION(swe_csnorm)
{
	long p;
	
	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l",
			&p) == FAILURE) {
		return;
	}
	RETURN_LONG(swe_csnorm(p));
}

PHP_FUNCTION(swe_difcsn)
{
	long p1, p2;
	
	if(ZEND_NUM_ARGS() != 2) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll",
			&p1, &p2) == FAILURE) {
		return;
	}
	RETURN_LONG(swe_difcsn((int)p1, (int)p2));
}

PHP_FUNCTION(swe_difdegn)
{
	double p1, p2;
	
	if(ZEND_NUM_ARGS() != 2) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd",
			&p1, &p2) == FAILURE) {
		return;
	}
	RETURN_DOUBLE(swe_difdegn(p1, p2));
}

PHP_FUNCTION(swe_difcs2n)
{
	long p1, p2;
	
	if(ZEND_NUM_ARGS() != 2) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll",
			&p1, &p2) == FAILURE) {
		return;
	}
	RETURN_LONG(swe_difcs2n((int)p1, (int)p2));
}

PHP_FUNCTION(swe_difdeg2n)
{
	double p1, p2;
	
	if(ZEND_NUM_ARGS() != 2) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd",
			&p1, &p2) == FAILURE) {
		return;
	}
	RETURN_DOUBLE(swe_difdeg2n(p1, p2));
}

PHP_FUNCTION(swe_difrad2n)
{
	double p1, p2;
	
	if(ZEND_NUM_ARGS() != 2) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd",
			&p1, &p2) == FAILURE) {
		return;
	}
	RETURN_DOUBLE(swe_difrad2n(p1, p2));
}

PHP_FUNCTION(swe_csroundsec)
{
	long x;
	
	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l",
			&x) == FAILURE) {
		return;
	}
	RETURN_LONG(swe_csroundsec((int)x));
}

PHP_FUNCTION(swe_d2l)
{
	double x;
	
	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d",
			&x) == FAILURE) {
		return;
	}
	RETURN_LONG(swe_d2l(x));
}

PHP_FUNCTION(swe_day_of_week)
{
	double jd;
	
	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "d",
			&jd) == FAILURE) {
		return;
	}
	RETURN_LONG(swe_day_of_week(jd));
}

PHP_FUNCTION(swe_cs2timestr)
{
	long t, sep, suppressZero;
	char a[AS_MAXCH];
	
	if(ZEND_NUM_ARGS() != 3) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lll",
			&t, &sep, &suppressZero) == FAILURE) {
		return;
	}

	RETURN_STRING(swe_cs2timestr((int)t, (int)sep, (int)suppressZero, a));
}

PHP_FUNCTION(swe_cs2lonlatstr)
{
	long t;
	size_t pchar_len, mchar_len;
	char s[AS_MAXCH], *pchar = NULL, *mchar = NULL;
	
	if(ZEND_NUM_ARGS() != 3) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lss",
			&t, &pchar, &pchar_len, &mchar, &mchar_len) == FAILURE) {
		return;
	}
	if ((pchar_len < 1) || (mchar_len < 1))
		return;

	RETURN_STRING(swe_cs2lonlatstr((int)t, pchar[0], mchar[0], s));
}

PHP_FUNCTION(swe_cs2degstr)
{
	long t;
	char a[AS_MAXCH];
	
	if(ZEND_NUM_ARGS() != 1) WRONG_PARAM_COUNT;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l",
			&t) == FAILURE) {
		return;
	}

	RETURN_STRING(swe_cs2degstr((int)t, a));
}


#if 0
/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_swephp_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_swephp_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char string[256];

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = sprintf(string, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "swephp", arg);
	RETURN_STRINGL(string, len);
}
#endif

/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
