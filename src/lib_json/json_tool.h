// Copyright 2007-2010 Baptiste Lepilleur and The JsonCpp Authors
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#ifndef LIB_JSONCPP_JSON_TOOL_H_INCLUDED
#define LIB_JSONCPP_JSON_TOOL_H_INCLUDED


// Also support old flag NO_LOCALE_SUPPORT
#ifdef NO_LOCALE_SUPPORT
#define JSONCPP_NO_LOCALE_SUPPORT
#endif

#ifndef JSONCPP_NO_LOCALE_SUPPORT
#include <clocale>
#endif

/* This header provides common string manipulation support, such as UTF-8,
 * portable conversion from/to string...
 *
 * It is an internal header that must not be exposed.
 */

namespace Json {
static char getDecimalPoint() {
#ifdef JSONCPP_NO_LOCALE_SUPPORT
  return '\0';
#else
  struct lconv* lc = localeconv();
  return lc ? *(lc->decimal_point) : '\0';
#endif
}

// /// Converts a unicode code-point to UTF-8.
// static inline JSONCPP_STRING codePointToUTF8(unsigned int cp) {
//   JSONCPP_STRING result;

//   // based on description from http://en.wikipedia.org/wiki/UTF-8

//   if (cp <= 0x7f) {
//     result.resize(1);
//     result[0] = static_cast<char>(cp);
//   } else if (cp <= 0x7FF) {
//     result.resize(2);
//     result[1] = static_cast<char>(0x80 | (0x3f & cp));
//     result[0] = static_cast<char>(0xC0 | (0x1f & (cp >> 6)));
//   } else if (cp <= 0xFFFF) {
//     result.resize(3);
//     result[2] = static_cast<char>(0x80 | (0x3f & cp));
//     result[1] = static_cast<char>(0x80 | (0x3f & (cp >> 6)));
//     result[0] = static_cast<char>(0xE0 | (0xf & (cp >> 12)));
//   } else if (cp <= 0x10FFFF) {
//     result.resize(4);
//     result[3] = static_cast<char>(0x80 | (0x3f & cp));
//     result[2] = static_cast<char>(0x80 | (0x3f & (cp >> 6)));
//     result[1] = static_cast<char>(0x80 | (0x3f & (cp >> 12)));
//     result[0] = static_cast<char>(0xF0 | (0x7 & (cp >> 18)));
//   }

//   return result;
// }

/// Converts a unicode code-point to UTF-8.
static inline JSONCPP_STRING codePointToUTF8(unsigned int cp) {
  JSONCPP_STRING result;

  // based on description from http://en.wikipedia.org/wiki/UTF-8

  if (cp <= 0x7f) {
    result.resize(1);
    result[0] = static_cast<char>(cp);
  } else if (cp <= 0x7FF) {
    result.resize(2);
    result[1] = static_cast<char>(0x80 | (0x3f & cp));
    result[0] = static_cast<char>(0xC0 | (0x1f & (cp >> 6)));
  } else if (cp <= 0xFFFF) {
    if (cp >= 0x4E00 && cp <= 0x9FA5 || (cp >= 0xF900 && cp <= 0xFA2D))
    {
      wchar_t src[2] = { 0 };
      char dest[5] = { 0 };
      src[0] = static_cast<wchar_t>(cp);
      std::string curLocale = setlocale(LC_ALL, NULL);
      setlocale(LC_ALL, "chs");
      wcstombs_s(NULL, dest, 5, src, 2);
      result = dest;
      setlocale(LC_ALL, curLocale.c_str());
    } else {
      result.resize(3);
      result[2] = static_cast<char>(0x80 | (0x3f & cp));
      result[1] = static_cast<char>(0x80 | (0x3f & (cp >> 6)));
      result[0] = static_cast<char>(0xE0 | (0xf & (cp >> 12)));
    }
  } else if (cp <= 0x10FFFF) {
    result.resize(4);
    result[3] = static_cast<char>(0x80 | (0x3f & cp));
    result[2] = static_cast<char>(0x80 | (0x3f & (cp >> 6)));
    result[1] = static_cast<char>(0x80 | (0x3f & (cp >> 12)));
    result[0] = static_cast<char>(0xF0 | (0x7 & (cp >> 18)));
  }

  return result;
}
enum {
  /// Constant that specify the size of the buffer that must be passed to
  /// uintToString.
  uintToStringBufferSize = 3 * sizeof(LargestUInt) + 1
};

// Defines a char buffer for use with uintToString().
typedef char UIntToStringBuffer[uintToStringBufferSize];

/** Converts an unsigned integer to string.
 * @param value Unsigned integer to convert to string
 * @param current Input/Output string buffer.
 *        Must have at least uintToStringBufferSize chars free.
 */
static inline void uintToString(LargestUInt value, char*& current) {
  *--current = 0;
  do {
    *--current = static_cast<char>(value % 10U + static_cast<unsigned>('0'));
    value /= 10;
  } while (value != 0);
}

/** Change ',' to '.' everywhere in buffer.
 *
 * We had a sophisticated way, but it did not work in WinCE.
 * @see https://github.com/open-source-parsers/jsoncpp/pull/9
 */
static inline void fixNumericLocale(char* begin, char* end) {
  while (begin < end) {
    if (*begin == ',') {
      *begin = '.';
    }
    ++begin;
  }
}

static inline void fixNumericLocaleInput(char* begin, char* end) {
  char decimalPoint = getDecimalPoint();
  if (decimalPoint != '\0' && decimalPoint != '.') {
    while (begin < end) {
      if (*begin == '.') {
        *begin = decimalPoint;
      }
      ++begin;
    }
  }
}

/**
 * Delete zeros in the end of string, if it isn't last zero before '.' character.
 */
static inline void fixZerosInTheEnd(char* begin, char* end) {
  end--;
  while ((begin < end) && (*end == '0')) {
    // don't delete last zero before point.
    if (*(end - 1) != '.') {
      *end = '\0';
    }
    end--;
  }
}

} // namespace Json {

#endif // LIB_JSONCPP_JSON_TOOL_H_INCLUDED
