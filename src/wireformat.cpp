#include "wireformat.h"
#include "exception.h"
#include "byteswap.h"

#include <boost/cstdint.hpp>

#include <cassert>
#include <iostream>


namespace amqpp
{
namespace detail
{

void wireformat::write_shortstring(std::ostream& o, const std::string& s)
{
	if (s.length() > 255)
	{
		throw std::logic_error("AMQPP: short strings can only be 255 characters long");
	}

	uint8_t len = static_cast<uint8_t>(s.length());
	write_uint8(o, len);
	o.write(s.data(), s.length());
}

void wireformat::read_shortstring(std::istream& i, std::string& s)
{
	uint8_t len = 0;
	read_uint8(i, len);
	if (!i.good())
	{
		// TODO: Throw a proper error
		throw std::runtime_error("Failure to read short string length");
	}
	char short_str[255];
	if (!(i.read(short_str, len).good()))
	{
		// TODO: Throw a proper error
		throw std::runtime_error("Failure to read short string data");
	}
	s.assign(short_str, len);
}

void wireformat::write_longstring(std::ostream& o, const std::string& s)
{
	uint32_t len = s.length();
	write_uint32(o, len);
	o.write(s.data(), len);
}

void wireformat::read_longstring(std::istream& i, std::string& s)
{
	uint32_t len = 0;
	read_uint32(i, len);
	if (!i.good())
	{
		// TODO: Throw a proper error
		throw std::runtime_error("Failure to read longstring length");
	}
	s.reserve(len);
	s.clear();

	const uint32_t BUFFER_LENGTH = 1024;
	char buffer[BUFFER_LENGTH];

	uint32_t read_bytes = 0;
	while (read_bytes < len)
	{
		uint32_t to_read = (len - read_bytes) > BUFFER_LENGTH ? BUFFER_LENGTH : (len - read_bytes);
		read_bytes += to_read;
		if (!(i.read(buffer, to_read).good()))
		{
			// TODO: Throw a proper error
			throw std::runtime_error("Failure to read longstring data");
		}
		s.append(buffer, to_read);
	}
}

} // namespace detail
} // namespace amqpp
