/**
 * @file Spp/RootManagerExtension.cpp
 * Contains the implementation of class Spp::RootManagerExtension.
 *
 * @copyright Copyright (C) 2020 Sarmad Khalid Abdullah
 *
 * @license This file is released under Alusus Public License, Version 1.0.
 * For details on usage and copying conditions read the full license in the
 * accompanying license file or at <https://alusus.org/alusus_license_1_0>.
 */
//==============================================================================

#include "spp.h"

namespace Spp
{

//==============================================================================
// Initialization Functions

RootManagerExtension::Overrides* RootManagerExtension::extend(
  Core::Main::RootManager *rootManager,
  SharedPtr<BuildManager> const &jitBuildManager,
  SharedPtr<Rt::AstMgr> const &astM,
  SharedPtr<Rt::BuildMgr> const &buildM
) {
  auto extension = std::make_shared<RootManagerExtension>(rootManager);
  rootManager->addDynamicInterface(extension);

  auto overrides = new Overrides();
  extension->jitBuildManager = jitBuildManager;
  extension->rtAstMgr = astM;
  extension->rtBuildMgr = buildM;

  overrides->importFileRef = extension->importFile.set(&RootManagerExtension::_importFile).get();

  return overrides;
}


void RootManagerExtension::unextend(Core::Main::RootManager *rootManager, Overrides *overrides)
{
  auto extension = ti_cast<RootManagerExtension>(rootManager);
  extension->importFile.reset(overrides->importFileRef);
  extension->jitBuildManager.remove();
  extension->rtAstMgr.remove();
  extension->rtBuildMgr.remove();
  rootManager->removeDynamicInterface<RootManagerExtension>();
  delete overrides;
}


//==============================================================================
// Main Functions

void RootManagerExtension::_importFile(TiObject *self, Char const *filename)
{
  PREPARE_SELF(rootManager, Core::Main::RootManager);
  Str error;
  if (!rootManager->tryImportFile(filename, error)) {
    throw EXCEPTION(FileException, filename, C('r'), error.c_str());
  }
}

} // namespace
