/**************************************************************************
    copyright            : (C) 2005-2007 by Lukáš Lalinský
    email                : lalinsky@gmail.com
 **************************************************************************/

/***************************************************************************
 *   This library is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License version   *
 *   2.1 as published by the Free Software Foundation.                     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful, but   *
 *   WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA         *
 *   02110-1301  USA                                                       *
 *                                                                         *
 *   Alternatively, this file is available under the Mozilla Public        *
 *   License Version 1.1.  You may obtain a copy of the License at         *
 *   http://www.mozilla.org/MPL/                                           *
 ***************************************************************************/

#include <taglib.h>
#include <libtag/toolkit/tdebug.h>
#include <libtag/toolkit/trefcounter.h>

#include "asfattribute.h"
#include <libtag/asf/asffile.h>
#include "asfutils.h"

using namespace TagLib;

class ASF::Attribute::AttributePrivate : public RefCounter
{
public:
  AttributePrivate()
    : pictureValue(ASF::Picture::fromInvalid()),
      stream(0),
      language(0) {}
  AttributeTypes type;
  String stringValue;
  ByteVector byteVectorValue;
  ASF::Picture pictureValue;
  union {
    unsigned int intValue;
    unsigned short shortValue;
    unsigned long long longLongValue;
    bool boolValue;
  };
  int stream;
  int language;
};

////////////////////////////////////////////////////////////////////////////////
// public members
////////////////////////////////////////////////////////////////////////////////

ASF::Attribute::Attribute() :
  d(new AttributePrivate())
{
  d->type = UnicodeType;
}

ASF::Attribute::Attribute(const ASF::Attribute &other) :
  d(other.d)
{
  d->ref();
}

ASF::Attribute::Attribute(const String &value) :
  d(new AttributePrivate())
{
  d->type = UnicodeType;
  d->stringValue = value;
}

ASF::Attribute::Attribute(const ByteVector &value) :
  d(new AttributePrivate())
{
  d->type = BytesType;
  d->byteVectorValue = value;
}

ASF::Attribute::Attribute(const ASF::Picture &value) :
  d(new AttributePrivate())
{
  d->type = BytesType;
  d->pictureValue = value;
}

ASF::Attribute::Attribute(unsigned int value) :
  d(new AttributePrivate())
{
  d->type = DWordType;
  d->intValue = value;
}

ASF::Attribute::Attribute(unsigned long long value) :
  d(new AttributePrivate())
{
  d->type = QWordType;
  d->longLongValue = value;
}

ASF::Attribute::Attribute(unsigned short value) :
  d(new AttributePrivate())
{
  d->type = WordType;
  d->shortValue = value;
}

ASF::Attribute::Attribute(bool value) :
  d(new AttributePrivate())
{
  d->type = BoolType;
  d->boolValue = value;
}

ASF::Attribute &ASF::Attribute::operator=(const ASF::Attribute &other)
{
  Attribute(other).swap(*this);
  return *this;
}

void ASF::Attribute::swap(Attribute &other)
{
  using std::swap;

  swap(d, other.d);
}

ASF::Attribute::~Attribute()
{
  if(d->deref())
    delete d;
}

ASF::Attribute::AttributeTypes ASF::Attribute::type() const
{
  return d->type;
}

String ASF::Attribute::toString() const
{
  return d->stringValue;
}

ByteVector ASF::Attribute::toByteVector() const
{
  if(d->pictureValue.isValid())
    return d->pictureValue.render();
  return d->byteVectorValue;
}

unsigned short ASF::Attribute::toBool() const
{
  return d->shortValue;
}

unsigned short ASF::Attribute::toUShort() const
{
  return d->shortValue;
}

unsigned int ASF::Attribute::toUInt() const
{
  return d->intValue;
}

unsigned long long ASF::Attribute::toULongLong() const
{
  return d->longLongValue;
}

ASF::Picture ASF::Attribute::toPicture() const
{
  return d->pictureValue;
}

String ASF::Attribute::parse(ASF::File &f, int kind)
{
  unsigned int size, nameLength;
  String name;
  d->pictureValue = Picture::fromInvalid();
  // extended content descriptor
  if(kind == 0) {
    nameLength = readWORD(&f);
    name = readString(&f, nameLength);
    d->type = ASF::Attribute::AttributeTypes(readWORD(&f));
    size = readWORD(&f);
  }
  // metadata & metadata library
  else {
    int temp = readWORD(&f);
    // metadata library
    if(kind == 2) {
      d->language = temp;
    }
    d->stream = readWORD(&f);
    nameLength = readWORD(&f);
    d->type = ASF::Attribute::AttributeTypes(readWORD(&f));
    size = readDWORD(&f);
    name = readString(&f, nameLength);
  }

  if(kind != 2 && size > 65535) {
    debug("ASF::Attribute::parse() -- Value larger than 64kB");
  }

  switch(d->type) {
  case WordType:
    d->shortValue = readWORD(&f);
    break;

  case BoolType:
    if(kind == 0) {
      d->boolValue = (readDWORD(&f) == 1);
    }
    else {
      d->boolValue = (readWORD(&f) == 1);
    }
    break;

  case DWordType:
    d->intValue = readDWORD(&f);
    break;

  case QWordType:
    d->longLongValue = readQWORD(&f);
    break;

  case UnicodeType:
    d->stringValue = readString(&f, size);
    break;

  case BytesType:
  case GuidType:
    d->byteVectorValue = f.readBlock(size);
    break;
  }

  if(d->type == BytesType && name == "WM/Picture") {
    d->pictureValue.parse(d->byteVectorValue);
    if(d->pictureValue.isValid()) {
      d->byteVectorValue.clear();
    }
  }

  return name;
}

int ASF::Attribute::dataSize() const
{
  switch (d->type) {
  case WordType:
    return 2;
  case BoolType:
    return 4;
  case DWordType:
    return 4;
  case QWordType:
    return 5;
  case UnicodeType:
    return d->stringValue.size() * 2 + 2;
  case BytesType:
    if(d->pictureValue.isValid())
      return d->pictureValue.dataSize();
  case GuidType:
    return d->byteVectorValue.size();
  }
  return 0;
}

ByteVector ASF::Attribute::render(const String &name, int kind) const
{
  ByteVector data;

  switch (d->type) {
  case WordType:
    data.append(ByteVector::fromShort(d->shortValue, false));
    break;

  case BoolType:
    if(kind == 0) {
      data.append(ByteVector::fromUInt(d->boolValue ? 1 : 0, false));
    }
    else {
      data.append(ByteVector::fromShort(d->boolValue ? 1 : 0, false));
    }
    break;

  case DWordType:
    data.append(ByteVector::fromUInt(d->intValue, false));
    break;

  case QWordType:
    data.append(ByteVector::fromLongLong(d->longLongValue, false));
    break;

  case UnicodeType:
    data.append(renderString(d->stringValue));
    break;

  case BytesType:
    if(d->pictureValue.isValid()) {
      data.append(d->pictureValue.render());
      break;
    }
  case GuidType:
    data.append(d->byteVectorValue);
    break;
  }

  if(kind == 0) {
    data = renderString(name, true) +
           ByteVector::fromShort((int)d->type, false) +
           ByteVector::fromShort(data.size(), false) +
           data;
  }
  else {
    ByteVector nameData = renderString(name);
    data = ByteVector::fromShort(kind == 2 ? d->language : 0, false) +
           ByteVector::fromShort(d->stream, false) +
           ByteVector::fromShort(nameData.size(), false) +
           ByteVector::fromShort((int)d->type, false) +
           ByteVector::fromUInt(data.size(), false) +
           nameData +
           data;
  }

  return data;
}

int ASF::Attribute::language() const
{
  return d->language;
}

void ASF::Attribute::setLanguage(int value)
{
  d->language = value;
}

int ASF::Attribute::stream() const
{
  return d->stream;
}

void ASF::Attribute::setStream(int value)
{
  d->stream = value;
}
