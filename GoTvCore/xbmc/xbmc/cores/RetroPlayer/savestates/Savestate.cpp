/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "Savestate.h"
#include "SavestateDefines.h"
#include "SavestateTranslator.h"
#include "utils/log.h"
#include "utils/Variant.h"
#include "utils/XMLUtils.h"

#include <libtinyxml/tinyxml.h>

using namespace KODI;
using namespace RETRO;

void CSavestate::Reset()
{
  m_path.clear();
  m_type = SAVE_TYPE::UNKNOWN;
  m_slot = -1;
  m_label.clear();
  m_size = 0;
  m_gameClient.clear();
  m_databaseId = -1;
  m_gamePath.clear();
  m_gameCRC.clear();
  m_playtimeFrames = 0;
  m_playtimeWallClock = 0.0;
  m_timestamp.Reset();
  m_thumbnail.clear();
}

void CSavestate::Serialize(CVariant& value) const
{
  value[SAVESTATE_FIELD_PATH] = m_path;
  value[SAVESTATE_FIELD_TYPE] = static_cast<unsigned int>(m_type);
  value[SAVESTATE_FIELD_SLOT] = m_slot;
  value[SAVESTATE_FIELD_LABEL] = m_label;
  value[SAVESTATE_FIELD_SIZE] = static_cast<unsigned int>(m_size);
  value[SAVESTATE_FIELD_GAMECLIENT] = m_gameClient;
  value[SAVESTATE_FIELD_DB_ID] = m_databaseId;
  value[SAVESTATE_FIELD_GAME_PATH] = m_gamePath;
  value[SAVESTATE_FIELD_GAME_CRC] = m_gameCRC;
  value[SAVESTATE_FIELD_FRAMES] = m_playtimeFrames;
  value[SAVESTATE_FIELD_WALLCLOCK] = m_playtimeWallClock;
  value[SAVESTATE_FIELD_TIMESTAMP] = m_timestamp.GetAsDBDateTime();
  value[SAVESTATE_FIELD_THUMBNAIL] = m_thumbnail;
}

void CSavestate::Deserialize(const CVariant& value)
{
  m_path = value[SAVESTATE_FIELD_PATH].asString();
  m_type = static_cast<SAVE_TYPE>(value[SAVESTATE_FIELD_TYPE].asInteger());
  m_slot = static_cast<int>(value[SAVESTATE_FIELD_SLOT].asInteger());
  m_label = value[SAVESTATE_FIELD_LABEL].asString();
  m_size = static_cast<size_t>(value[SAVESTATE_FIELD_SIZE].asUnsignedInteger());
  m_gameClient = value[SAVESTATE_FIELD_GAMECLIENT].asString();
  m_databaseId = static_cast<int>(value[SAVESTATE_FIELD_DB_ID].asInteger());
  m_gamePath = value[SAVESTATE_FIELD_GAME_PATH].asString();
  m_gameCRC = value[SAVESTATE_FIELD_GAME_CRC].asString();
  m_playtimeFrames = value[SAVESTATE_FIELD_FRAMES].asUnsignedInteger();
  m_playtimeWallClock = value[SAVESTATE_FIELD_WALLCLOCK].asDouble();
  m_timestamp.SetFromDBDateTime(value[SAVESTATE_FIELD_TIMESTAMP].asString());
  m_thumbnail = value[SAVESTATE_FIELD_THUMBNAIL].asString();
}

bool CSavestate::Serialize(const std::string& path) const
{
  if (m_type == SAVE_TYPE::UNKNOWN)
  {
    CLog::Log(LOGERROR, "Failed to serialize savestate (unknown type)");
    return false;
  }

  TiXmlDocument xmlFile;

  TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
  xmlFile.LinkEndChild(decl);

  TiXmlElement rootElement(SAVESTATE_XML_ROOT);
  TiXmlNode* root = xmlFile.InsertEndChild(rootElement);
  if (root == nullptr)
    return false;

  TiXmlElement* pElement = root->ToElement();
  if (!pElement)
    return false;

  XMLUtils::SetString(pElement, SAVESTATE_FIELD_PATH, m_path);
  XMLUtils::SetString(pElement, SAVESTATE_FIELD_TYPE, CSavestateTranslator::TranslateType(m_type));
  if (m_type == SAVE_TYPE::SLOT)
    XMLUtils::SetInt(pElement, SAVESTATE_FIELD_SLOT, m_slot);
  XMLUtils::SetString(pElement, SAVESTATE_FIELD_LABEL, m_label);
  XMLUtils::SetLong(pElement, SAVESTATE_FIELD_SIZE, static_cast<long>(m_size));
  XMLUtils::SetString(pElement, SAVESTATE_FIELD_GAMECLIENT, m_gameClient);
  XMLUtils::SetString(pElement, SAVESTATE_FIELD_GAME_PATH, m_gamePath);
  XMLUtils::SetString(pElement, SAVESTATE_FIELD_GAME_CRC, m_gameCRC);
  XMLUtils::SetLong(pElement, SAVESTATE_FIELD_FRAMES, static_cast<long>(m_playtimeFrames));
  XMLUtils::SetFloat(pElement, SAVESTATE_FIELD_WALLCLOCK, static_cast<float>(m_playtimeWallClock));
  XMLUtils::SetString(pElement, SAVESTATE_FIELD_TIMESTAMP, m_timestamp.GetAsDBDateTime());
  XMLUtils::SetString(pElement, SAVESTATE_FIELD_THUMBNAIL, m_thumbnail);

  if (!xmlFile.SaveFile(path))
  {
    CLog::Log(LOGERROR, "Failed to serialize savestate to %s: %s", path.c_str(), xmlFile.ErrorDesc());
    return false;
  }

  return true;
}

bool CSavestate::Deserialize(const std::string& path)
{
  Reset();

  TiXmlDocument xmlFile;
  if (!xmlFile.LoadFile(path))
  {
    CLog::Log(LOGERROR, "Failed to open %s: %s", path.c_str(), xmlFile.ErrorDesc());
    return false;
  }

  TiXmlElement* pElement = xmlFile.RootElement();
  if (!pElement || pElement->NoChildren() || pElement->ValueStr() != SAVESTATE_XML_ROOT)
  {
    CLog::Log(LOGERROR, "Can't find root <%s> tag", SAVESTATE_XML_ROOT);
    return false;
  }

  // Path
  if (!XMLUtils::GetString(pElement, SAVESTATE_FIELD_PATH, m_path))
  {
    CLog::Log(LOGERROR, "Savestate has no <%s> element", SAVESTATE_FIELD_PATH);
    return false;
  }

  // Type
  std::string type;
  if (!XMLUtils::GetString(pElement, SAVESTATE_FIELD_TYPE, type))
  {
    CLog::Log(LOGERROR, "Savestate has no <%s> element", SAVESTATE_FIELD_TYPE);
    return false;
  }
  m_type = CSavestateTranslator::TranslateType(type);
  if (m_type == SAVE_TYPE::UNKNOWN)
  {
    CLog::Log(LOGERROR, "Invalid savestate type: %s", type.c_str());
    return false;
  }

  // Slot
  if (m_type == SAVE_TYPE::SLOT)
  {
    if (!XMLUtils::GetInt(pElement, SAVESTATE_FIELD_SLOT, m_slot))
    {
      CLog::Log(LOGERROR, "Savestate has type \"%s\" but no <%s> element!", type.c_str(), SAVESTATE_FIELD_TYPE);
      return false;
    }
    if (m_slot < 0)
    {
      CLog::Log(LOGERROR, "Invalid savestate slot: %d", m_slot);
      return false;
    }
  }

  // Label (optional)
  XMLUtils::GetString(pElement, SAVESTATE_FIELD_LABEL, m_label);

  // Size
  long size;
  if (!XMLUtils::GetLong(pElement, SAVESTATE_FIELD_SIZE, size))
  {
    CLog::Log(LOGERROR, "Savestate has no <%s> element", SAVESTATE_FIELD_SIZE);
    return false;
  }
  if (size < 0)
  {
    CLog::Log(LOGERROR, "Invalid savestate size: %ld", size);
    return false;
  }
  m_size = size;

  // Game client
  if (!XMLUtils::GetString(pElement, SAVESTATE_FIELD_GAMECLIENT, m_gameClient))
  {
    CLog::Log(LOGERROR, "Savestate has no <%s> element", SAVESTATE_FIELD_GAMECLIENT);
    return false;
  }

  // Game path
  if (!XMLUtils::GetString(pElement, SAVESTATE_FIELD_GAME_PATH, m_gamePath))
  {
    CLog::Log(LOGERROR, "Savestate has no <%s> element", SAVESTATE_FIELD_GAME_PATH);
    return false;
  }

  // Game CRC
  if (!XMLUtils::GetString(pElement, SAVESTATE_FIELD_GAME_CRC, m_gameCRC))
  {
    CLog::Log(LOGERROR, "Savestate has no <%s> element", SAVESTATE_FIELD_GAME_CRC);
    return false;
  }

  // Playtime (frames)
  long playtimeFrames;
  if (!XMLUtils::GetLong(pElement, SAVESTATE_FIELD_FRAMES, playtimeFrames))
  {
    CLog::Log(LOGERROR, "Savestate has no <%s> element", SAVESTATE_FIELD_FRAMES);
    return false;
  }
  if (playtimeFrames < 0)
  {
    CLog::Log(LOGERROR, "Invalid savestate frame count: %ld", playtimeFrames);
    return false;
  }
  m_size = playtimeFrames;

  // Playtime (wall clock)
  float playtimeWallClock;
  if (!XMLUtils::GetFloat(pElement, SAVESTATE_FIELD_WALLCLOCK, playtimeWallClock))
  {
    CLog::Log(LOGERROR, "Savestate has no <%s> element", SAVESTATE_FIELD_WALLCLOCK);
    return false;
  }
  m_playtimeWallClock = playtimeWallClock;

  // Timestamp
  std::string timestamp;
  if (!XMLUtils::GetString(pElement, SAVESTATE_FIELD_TIMESTAMP, timestamp))
  {
    CLog::Log(LOGERROR, "Savestate has no <%s> element", SAVESTATE_FIELD_TIMESTAMP);
    return false;
  }
  if (!m_timestamp.SetFromDBDateTime(timestamp))
  {
    CLog::Log(LOGERROR, "Invalid savestate timestamp: %s", timestamp.c_str());
    return false;
  }

  // Thumbnail
  if (!XMLUtils::GetString(pElement, SAVESTATE_FIELD_THUMBNAIL, m_thumbnail))
  {
    CLog::Log(LOGERROR, "Savestate has no <%s> element", SAVESTATE_FIELD_THUMBNAIL);
    return false;
  }

  return true;
}
