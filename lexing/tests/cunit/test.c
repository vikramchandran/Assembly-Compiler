#include <stddef.h>
#include "../../string-helpers.h"
#include <CUnit/Basic.h>
#include <CUnit/TestDB.h>
#include "test.h"

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite1(void) {
  return 0;
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite1(void) {
  return 0;
}

/* The test initialization function.
 * Opens the temporary file used by the test.
 */
void init_test1(void) {
  return;
}

/* The test cleanup function.
 * Closes the temporary file used by the test in particular.
 */
void clean_test1(void) {
  return;
}

/* Simple test of is_digit().
 */
void testISDIGIT_Integers(void) {
  CU_ASSERT_FALSE(is_digit(0));
  CU_ASSERT_FALSE(is_digit(9));
  CU_ASSERT_FALSE(is_digit(1));
}

void testISDIGIT_Digits(void) {
  CU_ASSERT_TRUE(is_digit('1'));
  CU_ASSERT_TRUE(is_digit('0'));
  CU_ASSERT_TRUE(is_digit('9'));
}

void testISDIGIT_EscChars(void) {
  CU_ASSERT_FALSE(is_digit('\0'));
  CU_ASSERT_FALSE(is_digit('\"'));
  CU_ASSERT_FALSE(is_digit('\n'));
}

/* Simple test of is_alpha().
 */

void testisALPHA_Alpha(void) {
  CU_ASSERT_TRUE(is_alpha('e'));
  CU_ASSERT_TRUE(is_alpha('z'));
  CU_ASSERT_TRUE(is_alpha('o'));
  CU_ASSERT_TRUE(is_alpha('Q'));
  CU_ASSERT_TRUE(is_alpha('L'));
  CU_ASSERT_TRUE(is_alpha('O'));
}

void testisALPHA_Integers(void) {
  CU_ASSERT_FALSE(is_alpha('0'));
  CU_ASSERT_FALSE(is_alpha('9'));
  CU_ASSERT_FALSE(is_alpha('1'));
}

void testisALPHA_EscChars(void) {
  CU_ASSERT_FALSE(is_alpha('\0'));
  CU_ASSERT_FALSE(is_alpha('\"'));
  CU_ASSERT_FALSE(is_alpha('\n'));

}

/* Simple test of is_space().
 */

void testisSPACE_Space(void) {
  CU_ASSERT_TRUE(is_space(' '));
  CU_ASSERT_TRUE(is_space('\t'));
  CU_ASSERT_TRUE(is_space('\n'));
  CU_ASSERT_TRUE(is_space('\v'));
}

void testisSPACE_Integers(void) {
  CU_ASSERT_FALSE(is_space('1'));
  CU_ASSERT_FALSE(is_space('2'));
  CU_ASSERT_FALSE(is_space('2'));
  CU_ASSERT_FALSE(is_space('6'));
}

void testisSPACEE_ALPHA(void) {
  CU_ASSERT_FALSE(is_space('b'));
  CU_ASSERT_FALSE(is_space('q'));
  CU_ASSERT_FALSE(is_space('L'));
  CU_ASSERT_FALSE(is_space('D'));
}


/* Simple test of is_identifier_component().
 */

void testISIDENTIFIERCOMPONENT_ALPHA(void) {
  CU_ASSERT_TRUE(is_identifier_component('e'));
  CU_ASSERT_TRUE(is_identifier_component('z'));
  CU_ASSERT_TRUE(is_identifier_component('Q'));
  CU_ASSERT_TRUE(is_identifier_component('T'));
}

void testISIDENTIFIERCOMPONENT_MOREALLOWED(void) {
  CU_ASSERT_TRUE(is_identifier_component('1'));
  CU_ASSERT_TRUE(is_identifier_component('1'));
  CU_ASSERT_TRUE(is_identifier_component('4'));
  CU_ASSERT_TRUE(is_identifier_component('1'));
  CU_ASSERT_TRUE(is_identifier_component('_'));
}

void testISIDENTIFIERCOMPONENT_UNALLOWED(void) {
  CU_ASSERT_FALSE(is_identifier_component(3));
  CU_ASSERT_FALSE(is_identifier_component(2));
  CU_ASSERT_FALSE(is_identifier_component(1));
}

/* Simple test of is_valid_identifier().
 */

 void testISVALIDIDENTIFIER_ALPHA(void) {
  CU_ASSERT_TRUE(is_valid_identifier("eos"));
  CU_ASSERT_TRUE(is_valid_identifier("abqu"));
  CU_ASSERT_TRUE(is_valid_identifier("rst"));
  CU_ASSERT_TRUE(is_valid_identifier("zoiq"));
}

void testISVALIDIDENTIFIER_MOREALLOWED(void) {
  CU_ASSERT_TRUE(is_valid_identifier("as_193134"));
  CU_ASSERT_TRUE(is_valid_identifier("bz__19301aa_"));
  CU_ASSERT_TRUE(is_valid_identifier("qr_1938a"));
  CU_ASSERT_TRUE(is_valid_identifier("sso_90021_"));
}

void testISVALIDIDENTIFIER_UNALLOWED(void) {
  CU_ASSERT_FALSE(is_valid_identifier("390_a"));
  CU_ASSERT_FALSE(is_valid_identifier("450_b"));
  CU_ASSERT_FALSE(is_valid_identifier("9980ai"));
  CU_ASSERT_FALSE(is_valid_identifier("_ie"));
}







/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main() {
  CU_TestInfo isdigit_tests[] = {{"Test actual digits", testISDIGIT_Digits},
                                 {"Test esc chars", testISDIGIT_EscChars},
                                 {"Test numbers", testISDIGIT_Integers},
                                 CU_TEST_INFO_NULL};

  CU_TestInfo isalpha_tests[] = {{"Test actual alpha", testisALPHA_Alpha},
                                 {"Test numbers", testisALPHA_Integers},
                                 {"Test esc chars", testisALPHA_EscChars},
                                 CU_TEST_INFO_NULL};


  CU_TestInfo isspace_tests[] = {{"Test actual space", testisSPACE_Space},
                                 {"Test numbers", testisSPACE_Integers},
                                 {"Test alpha", testisSPACEE_ALPHA},
                                 CU_TEST_INFO_NULL};  

  CU_TestInfo isidentifiercomponent_tests[] = {{"Test allowed", testISIDENTIFIERCOMPONENT_MOREALLOWED},
                               {"Test unallowed", testISIDENTIFIERCOMPONENT_UNALLOWED},
                               {"Test allowed alpha", testISIDENTIFIERCOMPONENT_ALPHA},
                               CU_TEST_INFO_NULL}; 

  CU_TestInfo isvalididentifier[] = {{"Test alpha allowed", testISVALIDIDENTIFIER_ALPHA},
                               {"Test unallowed", testISVALIDIDENTIFIER_UNALLOWED},
                               {"Test more allowed", testISVALIDIDENTIFIER_MOREALLOWED},
                               CU_TEST_INFO_NULL};                                                     


  

  CU_SuiteInfo suites[] = {{"is_digit testing", init_suite1, clean_suite1, isdigit_tests},
                            {"is_alpha testing", init_suite1, clean_suite1, isalpha_tests}, 
                            {"is_space testing", init_suite1, clean_suite1, isspace_tests},
                            {"is_identifiercomponent testing", init_suite1, clean_suite1, isidentifiercomponent_tests},
                             {"is_valididentifier testing", init_suite1, clean_suite1, isvalididentifier},
                           CU_SUITE_INFO_NULL};

  CU_SuiteInfo suites1[] = {{"is_alpha testing", init_suite1, clean_suite1,
                         isalpha_tests},
                         CU_SUITE_INFO_NULL}; 

  CU_SuiteInfo suites2[] = {{"is_space testing", init_suite1, clean_suite1,
                         isspace_tests},
                         CU_SUITE_INFO_NULL}; 
   
  CU_SuiteInfo suites3[] = {{"is_identifiercomponent testing", init_suite1, clean_suite1,
                       isidentifiercomponent_tests},
                       CU_SUITE_INFO_NULL};

  CU_SuiteInfo suites4[] = {{"is_valididentifier testing", init_suite1, clean_suite1,
                       isvalididentifier},
                       CU_SUITE_INFO_NULL}; 
                         


  /* initialize the CUnit test registry */
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  if (CU_register_suites(suites)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
