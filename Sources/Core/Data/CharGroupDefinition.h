/**
 * @file Core/Data/CharGroupDefinition.h
 * Contains the header of class Core::Data::CharGroupDefinition.
 *
 * @copyright Copyright (C) 2014 Sarmad Khalid Abdullah
 *
 * @license This file is released under Alusus Public License, Version 1.0.
 * For details on usage and copying conditions read the full license in the
 * accompanying license file or at <http://alusus.net/alusus_license_1_0>.
 */
//==============================================================================

#ifndef DATA_CHARGROUPDEFINITION_H
#define DATA_CHARGROUPDEFINITION_H

namespace Core { namespace Data
{

/**
 * @brief Represents a single char group definition.
 * @ingroup data_char_group_units
 *
 * This class contains the definition of a single character group. The
 * definition includes the CharGroupUnit tree and the identifier of the char
 * group.
 */
class CharGroupDefinition : public IdentifiableObject, public virtual IdHolder
{
  //============================================================================
  // Type Info

  TYPE_INFO(CharGroupDefinition, IdentifiableObject, "Core.Data", "Core", "alusus.net");
  IMPLEMENT_INTERFACES_1(IdentifiableObject, IdHolder);


  //============================================================================
  // Member Variables

  /**
   * @brief The char group object.
   *
   * This object is the root of the tree that defines the character group.
   */
  private: SharedPtr<CharGroupUnit> charGroupUnit;


  //============================================================================
  // Constructors / Destructor

  public: CharGroupDefinition()
  {
  }

  public: CharGroupDefinition(SharedPtr<CharGroupUnit> const &u) : charGroupUnit(u)
  {
    if (u == 0) {
      throw InvalidArgumentException(STR("u"), STR("Core::Data::CharGroupDefinition::CharGroupDefinition"));
    }
  }

  public: virtual ~CharGroupDefinition()
  {
  }

  public: static SharedPtr<CharGroupDefinition> create(SharedPtr<CharGroupUnit> const &u)
  {
    return std::make_shared<CharGroupDefinition>(u);
  }


  //============================================================================
  // Member Functions

  /**
   * @brief Set the formula head object.
   *
   * This variable points to the unit at the root of the formula tree.
   * Once set, modifying this value is not allowed. This is to prevent
   * accidentally modifying the term while being in use by some state.
   */
  public: void setCharGroupUnit(SharedPtr<CharGroupUnit> const &u)
  {
    if (u == 0) {
      throw InvalidArgumentException(STR("h"), STR("Core::Data::CharGroupDefinition::setCharGroupUnit"));
    }
    if (this->charGroupUnit != 0) {
      throw GeneralException(STR("Modifying an already set char group unit is not allowed."),
                             STR("Core::Data::CharGroupDefinition::setCharGroupUnit"));
    }
    this->charGroupUnit = u;
  }

  /// Get the formula head object.
  public: SharedPtr<CharGroupUnit> const& getCharGroupUnit() const
  {
    return this->charGroupUnit;
  }

};

} } // namespace

#endif