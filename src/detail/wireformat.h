#ifndef _LIBAMQPCPP_WIREFORMAT_H_INCLUDED_
#define _LIBAMQPCPP_WIREFORMAT_H_INCLUDED_

#include "detail/byteswap.h"
#include "export.h"
#include "table.h"

#include <boost/cstdint.hpp>

#include <istream>
#include <ostream>
#include <stdexcept> 
#include <limits> //added limits
#include <string>
#include <utility>

namespace amqpp
{
namespace detail
{

class AMQPP_EXPORT wireformat
{
	public:
		static inline void write_uint8(std::ostream& o, const uint8_t i)
		{
			o.write(reinterpret_cast<const char*>(&i), sizeof(i));
      if (!o.good())
      {
        throw std::runtime_error("Write failure");
      }
		}

		static inline uint8_t read_uint8(std::istream& o)
		{
      uint8_t i = 0;
			o.read(reinterpret_cast<char*>(&i), sizeof(i));
      if (!o.good())
      {
        throw std::runtime_error("Unable to read uint8");
      }
      return i;
		}

		static inline void write_uint16(std::ostream& o, const uint16_t i)
		{
			uint16_t bs = byteswap(i);
			o.write(reinterpret_cast<const char*>(&bs), sizeof(bs));
      if (!o.good())
      {
        throw std::runtime_error("Write failure");
      }
		}

		static inline uint16_t read_uint16(std::istream& o)
		{
      uint16_t i = 0;
			o.read(reinterpret_cast<char*>(&i), sizeof(i));
      if (!o.good())
      {
        throw std::runtime_error("Unable to read uint16");
      }
			return byteswap(i);
		}

		static inline void write_uint32(std::ostream& o, const uint32_t i)
		{
			uint32_t bs = byteswap(i);
			o.write(reinterpret_cast<const char*>(&bs), sizeof(bs));
      if (!o.good())
      {
        throw std::runtime_error("Write failure");
      }
		}

		static inline uint32_t read_uint32(std::istream& o)
		{
      uint32_t i = 0;
			o.read(reinterpret_cast<char*>(&i), sizeof(i));
      if (!o.good())
      {
        throw std::runtime_error("Unable to read uint32");
      }
			return byteswap(i);
		}

		static inline void write_uint64(std::ostream& o, const uint64_t i)
		{
			uint64_t bs = byteswap(i);
			o.write(reinterpret_cast<const char*>(&bs), sizeof(bs));
      if (!o.good())
      {
        throw std::runtime_error("Write failure");
      }
		}

		static inline uint64_t read_uint64(std::istream& o)
		{
      uint64_t i = 0;
			o.read(reinterpret_cast<char*>(&i), sizeof(i));
      if (!o.good())
      {
        throw std::runtime_error("Unable to read uint64");
      }
			return byteswap(i);
		}

		static void write_shortstring(std::ostream& o, const std::string& s);
		static std::string read_shortstring(std::istream& o);

		static void write_longstring(std::ostream& o, const std::string& s);
		static std::string read_longstring(std::istream& o);

    static void write_table(std::ostream& o, const amqpp::table& t);
    static amqpp::table read_table(std::istream& i);

    static void write_table_entry(std::ostream& o, const table_entry& e);
    static void write_table_value(std::ostream& o, const table_entry::field_value_t& d);

    static table_entry read_table_entry(std::istream& i);
    static table_entry::field_value_t read_field_value(std::istream& i);

};
} // namespace detail
} // namespace amqppp

#endif // _LIBAMQPCPP_WIREFORMAT_H_INCLUDED_
