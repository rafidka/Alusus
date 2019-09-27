/**
 * @file Core/Processing/InteractiveCharInStream.h
 * Contains the header of class Core::Processing::InteractiveCharInStream.
 *
 * @copyright Copyright (C) 2019 Sarmad Khalid Abdullah
 *
 * @license This file is released under Alusus Public License, Version 1.0.
 * For details on usage and copying conditions read the full license in the
 * accompanying license file or at <https://alusus.org/alusus_license_1_0>.
 */
//==============================================================================

#ifndef CORE_PROCESSING_INTERACTIVECHARINSTREAM_H
#define CORE_PROCESSING_INTERACTIVECHARINSTREAM_H

#if defined(__MINGW32__) || defined(__MINGW64__)
#include "Win32Helpers.h"
#endif

namespace Core::Processing
{

class InteractiveCharInStream : public TiObject, public CharInStreaming
{
  //============================================================================
  // Type Info

  TYPE_INFO(InteractiveCharInStream, TiObject, "Core.Data.Ast", "Core", "alusus.org", (
    INHERITANCE_INTERFACES(CharInStreaming)
  ));


  //============================================================================
  // Member Variables

  public: Int lineNumber;


  //============================================================================
  // Constructors & Destructor

  public: InteractiveCharInStream() : lineNumber(1)
  {
    outStream << this->lineNumber << S("> ");
  }

  public: virtual ~InteractiveCharInStream()
  {
  }


  //============================================================================
  // Member Functions

#if defined(__MINGW32__) || defined(__MINGW64__)
  public: virtual WChar get()
  {
    WChar c = readWCharFromConsole();
    if (c == L'\n') {
#else
  public: virtual Char get()
  {
    Char c;
    inStream.get(c);
    if (c == C('\n')) {
#endif
      outStream << ++this->lineNumber << S("> ");
    }
    return c;
  }

  public: virtual Bool isEof()
  {
    return inStream.eof();
  }

}; // class

} // namespace

#endif
