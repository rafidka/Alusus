/**
 * @file Core/Basic/GlobalStorage.h
 * Contains the header of class Core::Basic::GlobalStorage.
 *
 * @copyright Copyright (C) 2014 Sarmad Khalid Abdullah
 *
 * @license This file is released under Alusus Public License, Version 1.0.
 * For details on usage and copying conditions read the full license in the
 * accompanying license file or at <https://alusus.org/alusus_license_1_0>.
 */
//==============================================================================

#ifndef CORE_BASIC_GLOBAL_STORAGE_H
#define CORE_BASIC_GLOBAL_STORAGE_H

namespace Core { namespace Basic
{

// TODO: DOC

class GlobalStorage
{
  //============================================================================
  // Data Types

  private: struct Desc
  {
    Str str;
    void *ptr;
  };

  private: typedef DirectSortedIndex<Desc, Str, &Desc::str> Index;


  //============================================================================
  // Member Variables

  private: std::vector<Desc> objects;

  private: Index index;


  //============================================================================
  // Constructor

  /// Prevent the singleton class from being inistantiated.
  private: GlobalStorage() : index(&objects)
  {
  }


  //============================================================================
  // Member Functions

  public: void* getObject(Char const *desc);

  public: void setObject(Char const *desc, void *object);

  /// Get the singleton object.
  public: static GlobalStorage* getSingleton();

}; // class

} } // namespace

/**
 * @brief A shortcut to access the ID generator singleton.
 * @ingroup basic_utils
 */
#define GLOBAL_STORAGE Core::Basic::GlobalStorage::getSingleton()

#endif
