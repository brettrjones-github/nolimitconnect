/*
 *  Copyright (C) 2016-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "addons/AddonDatabase.h"
#include "filesystem/SpecialProtocol.h"
#include "settings/AdvancedSettings.h"

#include "gtest/gtest.h"
#include <set>

using namespace ADDON;


class AddonDatabaseTest : public ::testing::Test
{
protected:
  DatabaseSettings settings;
  CAddonDatabase database;

  void SetUp() override
  {
    settings.type = "sqlite3";
    settings.name = "test";
    settings.host = CSpecialProtocol::TranslatePath("special://temp/");

    database.Connect("test", settings, true);

    std::set<std::string> installed{"repository.a", "repository.b"};
    database.SyncInstalled(installed, installed, std::set<std::string>());

    VECADDONS addons;
    CreateAddon(addons, "foo.bar", "1.0.0");
    database.UpdateRepositoryContent("repository.a", AddonVersion("1.0.0"), "test", addons);

    addons.clear();
    CreateAddon(addons, "foo.baz", "1.1.0");
    database.UpdateRepositoryContent("repository.b", AddonVersion("1.0.0"), "test", addons);
  }

  void CreateAddon(VECADDONS& addons, std::string id, std::string version)
  {
    CAddonBuilder builder;
    builder.SetId(id);
    builder.SetVersion(AddonVersion(version));
    addons.push_back(builder.Build());
  }

  void TearDown() override
  {
    database.Close();
  }
};


TEST_F(AddonDatabaseTest, TestFindById)
{
  VECADDONS addons;
  EXPECT_TRUE(database.FindByAddonId("foo.baz", addons));
  EXPECT_EQ(1U, addons.size());
  EXPECT_EQ(addons.at(0)->ID(), "foo.baz");
  EXPECT_EQ(addons.at(0)->Version().asString(), "1.1.0");
  EXPECT_EQ(addons.at(0)->Origin(), "repository.b");
}

TEST_F(AddonDatabaseTest, TestFindByNonExistingId)
{
  VECADDONS addons;
  EXPECT_TRUE(database.FindByAddonId("does.not.exist", addons));
  EXPECT_EQ(0U, addons.size());
}
