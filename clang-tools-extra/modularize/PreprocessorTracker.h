//===- PreprocessorTracker.h - Tracks preprocessor activities -*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===--------------------------------------------------------------------===//
///
/// \file
/// \brief Macro expansions and preprocessor conditional consistency checker.
///
//===--------------------------------------------------------------------===//

#ifndef MODULARIZE_PREPROCESSOR_TRACKER_H
#define MODULARIZE_PREPROCESSOR_TRACKER_H

#include "clang/Lex/Preprocessor.h"

namespace Modularize {

/// \brief Preprocessor tracker for modularize.
///
/// The PreprocessorTracker class defines an API for
/// checking macro expansions and preprocessor conditional expressions
/// in a header file for consistency among one or more compilations of
/// the header in a #include scenario.  This is for helping a user
/// find which macro expansions or conditionals might be problematic with
/// respect to using the headers in the modules scenario, because they
/// evaluate to different values depending on how or where a header
/// is included.
///
/// The handlePreprocessorEntry function implementation will register
/// a PPCallbacks object in the given Preprocessor object.  The calls to
/// the callbacks will collect information about the macro expansions
/// and preprocessor conditionals encountered, for later analysis and
/// reporting of inconsistencies between runs performed by calls to
/// the reportInconsistentMacros and reportInconsistentConditionals
/// functions respectively.  The handlePreprocessorExit informs the
/// implementation that a preprocessing session is complete, allowing
/// it to do any needed compilation completion activities in the checker.
class PreprocessorTracker {
public:
  virtual ~PreprocessorTracker();

  // Handle entering a preprocessing session.
  // (Called after a Preprocessor object is created, but before preprocessing.)
  virtual void handlePreprocessorEntry(clang::Preprocessor &PP,
                                       llvm::StringRef RootHeaderFile) = 0;
  // Handle exiting a preprocessing session.
  // (Called after preprocessing is complete, but before the Preprocessor
  // object is destroyed.)
  virtual void handlePreprocessorExit() = 0;

  // Handle include directive.
  // This function is called every time an include directive is seen by the
  // preprocessor, for the purpose of later checking for 'extern "" {}' or
  // "namespace {}" blocks containing #include directives.
  virtual void handleIncludeDirective(llvm::StringRef DirectivePath,
                                      int DirectiveLine, int DirectiveColumn,
                                      llvm::StringRef TargetPath) = 0;

  // Check for include directives within the given source line range.
  // Report errors if any found.  Returns true if no include directives
  // found in block.
  virtual bool checkForIncludesInBlock(clang::Preprocessor &PP,
                                       clang::SourceRange BlockSourceRange,
                                       const char *BlockIdentifierMessage,
                                       llvm::raw_ostream &OS) = 0;

  // Report on inconsistent macro instances.
  // Returns true if any mismatches.
  virtual bool reportInconsistentMacros(llvm::raw_ostream &OS) = 0;

  // Report on inconsistent conditional directive instances.
  // Returns true if any mismatches.
  virtual bool reportInconsistentConditionals(llvm::raw_ostream &OS) = 0;

  // Create instance of PreprocessorTracker.
  static PreprocessorTracker *create(
    llvm::SmallVector<std::string, 32> &Headers,
    bool DoBlockCheckHeaderListOnly);
};

} // end namespace Modularize

#endif
