/**
 * @file Core/Processing/ParserProdLevel.h
 * Contains the header of class Core::Processing::ParserProdLevel.
 *
 * @copyright Copyright (C) 2014 Sarmad Khalid Abdullah
 *
 * @license This file is released under Alusus Public License, Version 1.0.
 * For details on usage and copying conditions read the full license in the
 * accompanying license file or at <http://alusus.net/alusus_license_1_0>.
 */
//==============================================================================

#ifndef PROCESSING_PARSERPRODLEVEL_H
#define PROCESSING_PARSERPRODLEVEL_H

namespace Core { namespace Processing
{

// TODO: DOC

class ParserProdLevel
{
  //============================================================================
  // Friend Classes

  friend class ParserState;


  //============================================================================
  // Member Variables

  private: Data::Module *module;

  private: Data::SymbolDefinition *prod;

  private: Int termStackIndex;


  //============================================================================
  // Constructors / Destructor

  public: ParserProdLevel() : module(0), prod(0), termStackIndex(-1)
  {
  }

  public: ParserProdLevel(const ParserProdLevel &level) : module(level.module),
    prod(level.prod),
    termStackIndex(level.termStackIndex)
  {
  }

  public: ~ParserProdLevel()
  {
  }


  //============================================================================
  // Member Functions

  protected: void setModule(Data::Module *m)
  {
    this->module = m;
  }

  public: Data::Module* getModule() const
  {
    return this->module;
  }

  protected: void setProd(Data::SymbolDefinition *p)
  {
    this->prod = p;
  }

  public: Data::SymbolDefinition* getProd() const
  {
    return this->prod;
  }

  protected: void setTermStackIndex(Int i)
  {
    this->termStackIndex = i;
  }

  public: Int getTermStackIndex() const
  {
    return this->termStackIndex;
  }

}; // class

} } // namespace

#endif