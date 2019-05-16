/**
 * @file Core/Basic/Box.h
 * Contains the header of class Core::Basic::Box.
 *
 * @copyright Copyright (C) 2017 Sarmad Khalid Abdullah
 *
 * @license This file is released under Alusus Public License, Version 1.0.
 * For details on usage and copying conditions read the full license in the
 * accompanying license file or at <https://alusus.org/alusus_license_1_0>.
 */
//==============================================================================

#ifndef CORE_BASIC_BOX_H
#define CORE_BASIC_BOX_H

namespace Core { namespace Basic
{

template<class CTYPE, class PTYPE=TiObject> class Box : public PTYPE
{
  //============================================================================
  // Type Info

  TEMPLATE_TYPE_INFO(Box, PTYPE, "Core.Basic", "Core", "alusus.org", (CTYPE, PTYPE));


  //============================================================================
  // Member Variables

  private: CTYPE obj;


  //============================================================================
  // Constructors & Destructor

  public: Box() : obj(0) {}

  public: Box(CTYPE const &o) : obj(o)
  {
  }

  public: virtual ~Box()
  {
  }

  public: static SharedPtr<Box<CTYPE, PTYPE>> create(CTYPE const &o)
  {
    return std::make_shared<Box<CTYPE, PTYPE>>(o);
  }


  //============================================================================
  // Member Functions

  public: void set(CTYPE const &o)
  {
    this->obj = o;
  }

  public: CTYPE const& get() const
  {
    return this->obj;
  }

}; // class


//==============================================================================
// Default Types

typedef Box<SharedPtr<TiObject>> TioSharedBox;
typedef Box<WeakPtr<TiObject>> TioWeakBox;

} } // namespace

#endif
