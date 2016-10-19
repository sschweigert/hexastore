/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_AddingAndRemoving_init = false;
#include "/media/sebastian/d26d5d81-5c65-43dc-ba08-1a1a9e4b0609/Documents/Non-work Programming/hexastore/test/hexastore_test.h"

static AddingAndRemoving suite_AddingAndRemoving;

static CxxTest::List Tests_AddingAndRemoving = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_AddingAndRemoving( "hexastore_test.h", 7, "AddingAndRemoving", suite_AddingAndRemoving, Tests_AddingAndRemoving );

static class TestDescription_suite_AddingAndRemoving_testInsertion : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_AddingAndRemoving_testInsertion() : CxxTest::RealTestDescription( Tests_AddingAndRemoving, suiteDescription_AddingAndRemoving, 12, "testInsertion" ) {}
 void runTest() { suite_AddingAndRemoving.testInsertion(); }
} testDescription_suite_AddingAndRemoving_testInsertion;

static class TestDescription_suite_AddingAndRemoving_testAddition2 : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_AddingAndRemoving_testAddition2() : CxxTest::RealTestDescription( Tests_AddingAndRemoving, suiteDescription_AddingAndRemoving, 25, "testAddition2" ) {}
 void runTest() { suite_AddingAndRemoving.testAddition2(); }
} testDescription_suite_AddingAndRemoving_testAddition2;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
