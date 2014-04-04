/**
 * This file is part of meta.
 *
 * Author(s): Jens Finkhaeuser <jens@unwesen.co.uk>
 *
 * Copyright (c) 2009-2012 Jens Finkhaeuser.
 * Copyright (c) 2013 Unwesen Ltd.
 *
 * This software is licensed under the terms of the GNU GPLv3 for personal,
 * educational and non-profit use. For all other uses, alternative license
 * options are available. Please contact the copyright holder for additional
 * information, stating your intended usage.
 *
 * You can find the full text of the GPLv3 in the COPYING file in this code
 * distribution.
 *
 * This software is distributed on an "AS IS" BASIS, WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.
 **/
#include <iostream>
#include <stack>
#include <stdexcept>

#include <assert.h>
#include <stdint.h>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TextTestProgressListener.h>
#include <cppunit/Test.h>
#include <cppunit/TestFailure.h>

#include <meta/meta-config.h>
#include <meta/version.h>


/**
 * The VerboseOutput class implements CppUnit's TestListener interface,
 * and produces verbose output as to which tests are currently running, and
 * whether they succeeded or not. Output is produced on a std::ostream.
 *
 * Code taken with permission from fhtagn (http://fhtagn.unwesen.de/)
 **/
class VerboseOutput
  : public CppUnit::TestListener
{
public:
  /**
   * Constructor
   *
   * @param os Output stream on which to produce verbose test output.
   * @param indent_by [default: 2] Test suites may be nested, and output for
   *    each inner test suite is indented by this amount of spaces from the
   *    output of the enclosing test suite.
   * @param max_line [default: 79] Maximum number of characters per line; this
   *    value is a hint for laying out the output, but lines may still exceed
   *    this limit in extreme cases.
   **/
  VerboseOutput(std::ostream & os, size_t indent_by = 2, size_t max_line = 79);

private:
  /**
   * CppUnit distinguishes between tests that succeed, that fail (i.e. where
   * an assertion is not fulfilled), and errors that were not expected by the
   * test suite.
   **/
  enum status
  {
    ST_OK,
    ST_FAILURE,
    ST_ERROR
  };


  /**
   * Structure for counting successes/failures/errors of each test suite.
   **/
  struct results
  {
    uint32_t successes;
    uint32_t failures;
    uint32_t errors;
  };


  /** @see CppUnit::TestListener */
  virtual void startTest(CppUnit::Test * test);

  /** @see CppUnit::TestListener */
  virtual void addFailure(CppUnit::TestFailure const & failure);

  /** @see CppUnit::TestListener */
  virtual void endTest(CppUnit::Test * test);

  /** @see CppUnit::TestListener */
  virtual void startSuite(CppUnit::Test * suite);

  /** @see CppUnit::TestListener */
  virtual void endSuite(CppUnit::Test * suite);

  /** @see CppUnit::TestListener */
  virtual void startTestRun(CppUnit::Test * test,
          CppUnit::TestResult * eventManager);

  /** @see CppUnit::TestListener */
  virtual void endTestRun(CppUnit::Test * test,
          CppUnit::TestResult * eventManager);


  /** Used for counting results in each nested test suite. **/
  std::stack<results> m_results;

  /**
   * Result of individual test runs; must be preserved across a few function
   * calls.
   **/
  status              m_status;

  /** Stream to push output to. **/
  std::ostream &      m_os;

  /** Indentation depth **/
  size_t              m_indent_by;

  /** Maximum line size */
  size_t              m_max_line;
};


/*****************************************************************************
 * VerboseOutput implementation
 **/

namespace {

/** Helper struct to produce padding on an output stream. **/
struct pad
{
  pad(size_t pad_length, char pad_char = ' ')
    : m_pad_length(pad_length)
    , m_pad_char(pad_char)
  {
  }

  virtual ~pad()
  {
  }

  virtual void output(std::ostream & os) const
  {
    for (size_t i = 0 ; i < m_pad_length ; ++i) {
      os << m_pad_char;
    }
  }

  friend std::ostream & operator<<(std::ostream & os, pad const & p)
  {
    p.output(os);
    return os;
  }

  size_t          m_pad_length;
  char            m_pad_char;
};


/** Helper struct for indenting output on an output stream **/
struct indent
  : public pad
{
  indent(size_t indent_level, size_t pad_size)
    : pad(indent_level * pad_size)
  {
  }
};

/** Simplify use of the indent struct */
#define INDENT indent((m_results.size() - 1), m_indent_by)


/** Helper struct for drawing a line on an output stream **/
struct line
  : public indent
{
  line(size_t indent_level, size_t pad_size, char line_char,
      size_t max_line)
    : indent(indent_level, pad_size)
  {
    m_pad_length = max_line - m_pad_length;
    m_pad_char = line_char;
  }
};

/** Simplify use of the line struct */
#define LINE(line_char) line((m_results.size() - 1), m_indent_by, line_char, \
        m_max_line)

} // anonymous namespace


VerboseOutput::VerboseOutput(std::ostream & os, size_t indent_by /* = 2 */,
    size_t max_line /* = 79 */)
  : m_results()
  , m_status(ST_OK)
  , m_os(os)
  , m_indent_by(indent_by)
  , m_max_line(max_line)
{
  results tmp = { 0, 0, 0 };
  m_results.push(tmp);
}



// Called when just before a TestCase is run.
void
VerboseOutput::startTest(CppUnit::Test * test)
{
  std::string label = "Running `" + test->getName() + "'";
  size_t pad_size = m_max_line - label.size() - INDENT.m_pad_length - 12;
  if (pad_size > m_max_line) {
    pad_size = 1;
  }
  m_os << INDENT << label << "..." << pad(pad_size);
}


// Called when a failure occurs while running a test.
void
VerboseOutput::addFailure(CppUnit::TestFailure const & failure)
{
  m_status = (failure.isError() ? ST_ERROR : ST_FAILURE);
}


// Called just after a TestCase was run (even if a failure occured).
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void
VerboseOutput::endTest(CppUnit::Test * test)
{
  switch (m_status) {
    case ST_OK:
      m_os << "OK" << std::endl;
      ++(m_results.top().successes);
      break;
    case ST_FAILURE:
      m_os << "FAILURE" << std::endl;
      ++(m_results.top().failures);
      break;
    case ST_ERROR:
      m_os << "ERROR" << std::endl;
      ++(m_results.top().errors);
      break;
    default:
      assert(0);
  }
  m_status = ST_OK;
}
#pragma GCC diagnostic pop


// Called by a TestComposite just before running its child tests.
void
VerboseOutput::startSuite(CppUnit::Test * suite)
{
  m_os << INDENT << LINE('=') << std::endl;
  m_os << indent(m_results.size(), m_indent_by) << "Starting test suite `"
    << suite->getName() << "' with " << suite->getChildTestCount()
    << " children" << std::endl;
  m_os << INDENT << LINE('-') << std::endl;

  results tmp = { 0, 0, 0 };
  m_results.push(tmp);
}

// Called by a TestComposite after running its child tests.
void
VerboseOutput::endSuite(CppUnit::Test * suite)
{
  results suite_results = m_results.top();
  m_results.pop();

  m_os << INDENT << LINE('-') << std::endl;
  m_os << indent(m_results.size(), m_indent_by) << "Suite `" << suite->getName()
    << "' - Total: " << (suite_results.successes + suite_results.failures
        + suite_results.errors)
    << "   Success: " << suite_results.successes
    << "   Failure: " << suite_results.failures
    << "   Error: " << suite_results.errors << std::endl << std::endl;

  m_results.top().successes += suite_results.successes;
  m_results.top().failures += suite_results.failures;
  m_results.top().errors += suite_results.errors;
}


// Called by a TestRunner before running the test.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void
VerboseOutput::startTestRun(CppUnit::Test * test,
        CppUnit::TestResult * eventManager)
{
  m_os << std::endl;
}
#pragma GCC diagnostic pop


// Called by a TestRunner after running the test.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void
VerboseOutput::endTestRun(CppUnit::Test * test,
        CppUnit::TestResult * eventManager)
{
  m_os << LINE('=') << std::endl;
  if (m_results.top().errors || m_results.top().failures) {
    m_os << INDENT << "Details of errors and failures follow:"
      << std::endl << std::endl;
  }
}
#pragma GCC diagnostic pop



/*****************************************************************************
 * Main
 **/

int main(int argc, char **argv)
{
  std::cout << meta::copyright_string() << std::endl;

  std::string testPath = (argc > 1) ? std::string(argv[1]) : "";

  // Create the event manager and test controller
  CppUnit::TestResult controller;

  // Listener printing verbose or HTML test information
  VerboseOutput progress(std::cout);
  controller.addListener(&progress);

  // Add a listener that colllects test result
  CppUnit::TestResultCollector result;
  controller.addListener(&result);


  // Add the top suite to the test runner
  CppUnit::TestRunner runner;
  runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
  try {
    if (testPath.empty()) {
      runner.run(controller);
    } else {
      runner.run(controller, testPath);
    }

    // Print test in a compiler compatible format.
    CppUnit::CompilerOutputter outputter(&result,std::cerr);
    outputter.write();
  } catch (std::invalid_argument const & e) {
    // Test path not resolved
    std::cerr << std::endl <<  "ERROR: " << e.what() << std::endl;
    return 2;
  }

  return result.wasSuccessful() ? 0 : 1;
}
