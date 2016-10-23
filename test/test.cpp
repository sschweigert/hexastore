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
CxxTest::StaticSuiteDescription suiteDescription_AddingAndRemoving( "hexastore_test.h", 11, "AddingAndRemoving", suite_AddingAndRemoving, Tests_AddingAndRemoving );

static class TestDescription_suite_AddingAndRemoving_testCyclicalQueryChain : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_AddingAndRemoving_testCyclicalQueryChain() : CxxTest::RealTestDescription( Tests_AddingAndRemoving, suiteDescription_AddingAndRemoving, 23, "testCyclicalQueryChain" ) {}
 void runTest() { suite_AddingAndRemoving.testCyclicalQueryChain(); }
} testDescription_suite_AddingAndRemoving_testCyclicalQueryChain;

static class TestDescription_suite_AddingAndRemoving_testInsertionRemoval : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_AddingAndRemoving_testInsertionRemoval() : CxxTest::RealTestDescription( Tests_AddingAndRemoving, suiteDescription_AddingAndRemoving, 58, "testInsertionRemoval" ) {}
 void runTest() { suite_AddingAndRemoving.testInsertionRemoval(); }
} testDescription_suite_AddingAndRemoving_testInsertionRemoval;

static class TestDescription_suite_AddingAndRemoving_testDirectedTriangleDetection : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_AddingAndRemoving_testDirectedTriangleDetection() : CxxTest::RealTestDescription( Tests_AddingAndRemoving, suiteDescription_AddingAndRemoving, 81, "testDirectedTriangleDetection" ) {}
 void runTest() { suite_AddingAndRemoving.testDirectedTriangleDetection(); }
} testDescription_suite_AddingAndRemoving_testDirectedTriangleDetection;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
