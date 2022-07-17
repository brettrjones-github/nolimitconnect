/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://kodi.tv
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#ifdef __GNUC__
// under gcc, inline will only take place if optimizations are applied (-O). this will force inline even with optimizations.
#define XBMC_FORCE_INLINE __attribute__((always_inline))
#else
#define XBMC_FORCE_INLINE
#endif

#include <algorithm>
#include <stdexcept>
#include <vector>

template <typename T> class NlcPointGen
{
public:
  typedef NlcPointGen<T> this_type;

  NlcPointGen() noexcept = default;

  constexpr NlcPointGen(T a, T b)
  : x{a}, y{b}
  {}

  template<class U> explicit constexpr NlcPointGen(const NlcPointGen<U>& rhs)
  : x{static_cast<T> (rhs.x)}, y{static_cast<T> (rhs.y)}
  {}

  constexpr this_type operator+(const this_type &point) const
  {
    return {x + point.x, y + point.y};
  };

  this_type& operator+=(const this_type &point)
  {
    x += point.x;
    y += point.y;
    return *this;
  };

  constexpr this_type operator-(const this_type &point) const
  {
    return {x - point.x, y - point.y};
  };

  this_type& operator-=(const this_type &point)
  {
    x -= point.x;
    y -= point.y;
    return *this;
  };

  constexpr this_type operator*(T factor) const
  {
    return {x * factor, y * factor};
  }

  this_type& operator*=(T factor)
  {
    x *= factor;
    y *= factor;
    return *this;
  }

  constexpr this_type operator/(T factor) const
  {
    return {x / factor, y / factor};
  }

  this_type& operator/=(T factor)
  {
    x /= factor;
    y /= factor;
    return *this;
  }

  T x{}, y{};
};

template<typename T>
constexpr bool operator==(const NlcPointGen<T> &point1, const NlcPointGen<T> &point2) noexcept
{
  return (point1.x == point2.x && point1.y == point2.y);
}

template<typename T>
constexpr bool operator!=(const NlcPointGen<T> &point1, const NlcPointGen<T> &point2) noexcept
{
  return !(point1 == point2);
}

using NlcPoint = NlcPointGen<float>;
using NlcPointInt = NlcPointGen<int>;


/**
 * Generic two-dimensional size representation
 *
 * Class invariant: width and height are both non-negative
 * Throws std::out_of_range if invariant would be violated. The class
 * is exception-safe. If modification would violate the invariant, the size
 * is not changed.
 */
template <typename T> class NlcSizeGen
{
  T m_w{}, m_h{};

  void CheckSet(T width, T height)
  {
    if (width < 0)
    {
      throw std::out_of_range("Size may not have negative width");
    }
    if (height < 0)
    {
      throw std::out_of_range("Size may not have negative height");
    }
    m_w = width;
    m_h = height;
  }

public:
  typedef NlcSizeGen<T> this_type;

  NlcSizeGen() noexcept = default;

  NlcSizeGen(T width, T height)
  {
    CheckSet(width, height);
  }

  T Width() const
  {
    return m_w;
  }

  T Height() const
  {
    return m_h;
  }

  void SetWidth(T width)
  {
    CheckSet(width, m_h);
  }

  void SetHeight(T height)
  {
    CheckSet(m_w, height);
  }

  void Set(T width, T height)
  {
    CheckSet(width, height);
  }

  bool IsZero() const
  {
    return (m_w == static_cast<T> (0) && m_h == static_cast<T> (0));
  }

  T Area() const
  {
    return m_w * m_h;
  }

  NlcPointGen<T> ToPoint() const
  {
    return {m_w, m_h};
  }

  template<class U> explicit NlcSizeGen<T>(const NlcSizeGen<U>& rhs)
  {
    CheckSet(static_cast<T> (rhs.m_w), static_cast<T> (rhs.m_h));
  }

  this_type operator+(const this_type& size) const
  {
    return {m_w + size.m_w, m_h + size.m_h};
  };

  this_type& operator+=(const this_type& size)
  {
    CheckSet(m_w + size.m_w, m_h + size.m_h);
    return *this;
  };

  this_type operator-(const this_type& size) const
  {
    return {m_w - size.m_w, m_h - size.m_h};
  };

  this_type& operator-=(const this_type& size)
  {
    CheckSet(m_w - size.m_w, m_h - size.m_h);
    return *this;
  };

  this_type operator*(T factor) const
  {
    return {m_w * factor, m_h * factor};
  }

  this_type& operator*=(T factor)
  {
    CheckSet(m_w * factor, m_h * factor);
    return *this;
  }

  this_type operator/(T factor) const
  {
    return {m_w / factor, m_h / factor};
  }

  this_type& operator/=(T factor)
  {
    CheckSet(m_w / factor, m_h / factor);
    return *this;
  }
};

template<typename T>
inline bool operator==(const NlcSizeGen<T>& size1, const NlcSizeGen<T>& size2) noexcept
{
  return (size1.Width() == size2.Width() && size1.Height() == size2.Height());
}

template<typename T>
inline bool operator!=(const NlcSizeGen<T>& size1, const NlcSizeGen<T>& size2) noexcept
{
  return !(size1 == size2);
}

using NlcSize = NlcSizeGen<float>;
using NlcSizeInt = NlcSizeGen<int>;


template <typename T> class NlcRectGen
{
public:
  typedef NlcRectGen<T> this_type;
  typedef NlcPointGen<T> point_type;
  typedef NlcSizeGen<T> size_type;

  NlcRectGen() noexcept = default;

  constexpr NlcRectGen(T left, T top, T right, T bottom)
  : x1{left}, y1{top}, x2{right}, y2{bottom}
  {}

  constexpr NlcRectGen(const point_type &p1, const point_type &p2)
  : x1{p1.x}, y1{p1.y}, x2{p2.x}, y2{p2.y}
  {}

  constexpr NlcRectGen(const point_type &origin, const size_type &size)
  : x1{origin.x}, y1{origin.y}, x2{x1 + size.Width()}, y2{y1 + size.Height()}
  {}

  template<class U> explicit constexpr NlcRectGen(const NlcRectGen<U>& rhs)
  : x1{static_cast<T> (rhs.x1)}, y1{static_cast<T> (rhs.y1)}, x2{static_cast<T> (rhs.x2)}, y2{static_cast<T> (rhs.y2)}
  {}

  void SetRect(T left, T top, T right, T bottom)
  {
    x1 = left;
    y1 = top;
    x2 = right;
    y2 = bottom;
  }

  constexpr bool PtInRect(const point_type &point) const
  {
    return (x1 <= point.x && point.x <= x2 && y1 <= point.y && point.y <= y2);
  };

  this_type& operator-=(const point_type &point) XBMC_FORCE_INLINE
  {
    x1 -= point.x;
    y1 -= point.y;
    x2 -= point.x;
    y2 -= point.y;
    return *this;
  };

  constexpr this_type operator-(const point_type &point) const
  {
    return {x1 - point.x, y1 - point.y, x2 - point.x, y2 - point.y};
  }

  this_type& operator+=(const point_type &point) XBMC_FORCE_INLINE
  {
    x1 += point.x;
    y1 += point.y;
    x2 += point.x;
    y2 += point.y;
    return *this;
  };

  constexpr this_type operator+(const point_type &point) const
  {
    return {x1 + point.x, y1 + point.y, x2 + point.x, y2 + point.y};
  }

  this_type& operator-=(const size_type &size)
  {
    x2 -= size.Width();
    y2 -= size.Height();
    return *this;
  };

  constexpr this_type operator-(const size_type &size) const
  {
    return {x1, y1, x2 - size.Width(), y2 - size.Height()};
  }

  this_type& operator+=(const size_type &size)
  {
    x2 += size.Width();
    y2 += size.Height();
    return *this;
  };

  constexpr this_type operator+(const size_type &size) const
  {
    return {x1, y1, x2 + size.Width(), y2 + size.Height()};
  }

  this_type& Intersect(const this_type &rect)
  {
    x1 = clamp_range(x1, rect.x1, rect.x2);
    x2 = clamp_range(x2, rect.x1, rect.x2);
    y1 = clamp_range(y1, rect.y1, rect.y2);
    y2 = clamp_range(y2, rect.y1, rect.y2);
    return *this;
  };

  this_type& Union(const this_type &rect)
  {
    if (IsEmpty())
      *this = rect;
    else if (!rect.IsEmpty())
    {
      x1 = std::min(x1,rect.x1);
      y1 = std::min(y1,rect.y1);

      x2 = std::max(x2,rect.x2);
      y2 = std::max(y2,rect.y2);
    }

    return *this;
  };

  constexpr bool IsEmpty() const XBMC_FORCE_INLINE
  {
    return (x2 - x1) * (y2 - y1) == 0;
  };

  constexpr point_type P1() const XBMC_FORCE_INLINE
  {
    return {x1, y1};
  }

  constexpr point_type P2() const XBMC_FORCE_INLINE
  {
    return {x2, y2};
  }

  constexpr T Width() const XBMC_FORCE_INLINE
  {
    return x2 - x1;
  };

  constexpr T Height() const XBMC_FORCE_INLINE
  {
    return y2 - y1;
  };

  constexpr T Area() const XBMC_FORCE_INLINE
  {
    return Width() * Height();
  };

  size_type ToSize() const
  {
    return {Width(), Height()};
  };

  std::vector<this_type> SubtractRect(this_type splitterRect)
  {
    std::vector<this_type> newRectanglesList;
    this_type intersection = splitterRect.Intersect(*this);

    if (!intersection.IsEmpty())
    {
      this_type add;

      // add rect above intersection if not empty
      add = this_type(x1, y1, x2, intersection.y1);
      if (!add.IsEmpty())
        newRectanglesList.push_back(add);

      // add rect below intersection if not empty
      add = this_type(x1, intersection.y2, x2, y2);
      if (!add.IsEmpty())
        newRectanglesList.push_back(add);

      // add rect left intersection if not empty
      add = this_type(x1, intersection.y1, intersection.x1, intersection.y2);
      if (!add.IsEmpty())
        newRectanglesList.push_back(add);

      // add rect right intersection if not empty
      add = this_type(intersection.x2, intersection.y1, x2, intersection.y2);
      if (!add.IsEmpty())
        newRectanglesList.push_back(add);
    }
    else
    {
      newRectanglesList.push_back(*this);
    }

    return newRectanglesList;
  }

  std::vector<this_type> SubtractRects(std::vector<this_type> intersectionList)
  {
    std::vector<this_type> fragmentsList;
    fragmentsList.push_back(*this);

    for (typename std::vector<this_type>::iterator splitter = intersectionList.begin(); splitter != intersectionList.end(); ++splitter)
    {
      typename std::vector<this_type> toAddList;

      for (typename std::vector<this_type>::iterator fragment = fragmentsList.begin(); fragment != fragmentsList.end(); ++fragment)
      {
        std::vector<this_type> newFragmentsList = fragment->SubtractRect(*splitter);
        toAddList.insert(toAddList.end(), newFragmentsList.begin(), newFragmentsList.end());
      }

      fragmentsList.clear();
      fragmentsList.insert(fragmentsList.end(), toAddList.begin(), toAddList.end());
    }

    return fragmentsList;
  }

  T x1{}, y1{}, x2{}, y2{};
private:
  static constexpr T clamp_range(T x, T l, T h)
  {
    return (x > h) ? h : ((x < l) ? l : x);
  }
};

template<typename T>
constexpr bool operator==(const NlcRectGen<T> &rect1, const NlcRectGen<T> &rect2) noexcept
{
  return (rect1.x1 == rect2.x1 && rect1.y1 == rect2.y1 && rect1.x2 == rect2.x2 && rect1.y2 == rect2.y2);
}

template<typename T>
constexpr bool operator!=(const NlcRectGen<T> &rect1, const NlcRectGen<T> &rect2) noexcept
{
  return !(rect1 == rect2);
}

using NlcRect = NlcRectGen<float>;
using NlcRectInt = NlcRectGen<int>;
