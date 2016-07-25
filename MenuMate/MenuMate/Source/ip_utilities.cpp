#include <cassert>
#include <sstream>
#include <vector>

#include <windows.h>
#include <iphlpapi.h>

#include "ip_utilities.h"

namespace ip_utilities {

ip_address_t::ip_address_t()
    : ip_address_(0x0ul)
{
}

ip_address_t::ip_address_t(
    const uint8_t octet_a,
    const uint8_t octet_b,
    const uint8_t octet_c,
    const uint8_t octet_d)
    : ip_address_(octet_a
                  | octet_b << 8
                  | octet_c << 16
                  | octet_d << 24)
{
} 

ip_address_t::ip_address_t(
    const ip_address_t &initializer)
    : ip_address_(initializer.ip_address_)
{
}

ip_address_t::~ip_address_t()
{
}

ip_address_t &
ip_address_t::operator=(const ip_address_t &rhs)
{
        ip_address_ = rhs.ip_address_;
        return *this;
}

ip_address_t::operator std::wstring()
const
{
        std::wostringstream stringbuilder;

        stringbuilder << static_cast<unsigned int>(octet_a())
                      << L"."
                      << static_cast<unsigned int>(octet_b())
                      << L"."
                      << static_cast<unsigned int>(octet_c())
                      << L"."
                      << static_cast<unsigned int>(octet_d());

        return stringbuilder.str();
}

const uint8_t
ip_address_t::octet_a()
const
{
        return ip_address_ & 0xfful;
}

const uint8_t
ip_address_t::octet_b()
const
{
        return (ip_address_ >> 8) & 0xff;
}

const uint8_t
ip_address_t::octet_c()
const
{
        return (ip_address_ >> 16) & 0xff;
}

const uint8_t
ip_address_t::octet_d()
const
{
        return (ip_address_ >> 24) & 0xff;
}

subnet_mask_t::subnet_mask_t()
    : subnet_mask_()
{
}

subnet_mask_t::subnet_mask_t(
    const uint8_t octet_a,
    const uint8_t octet_b,
    const uint8_t octet_c,
    const uint8_t octet_d)
    : subnet_mask_(ip_address_t(octet_a, octet_b, octet_c, octet_d))
{
} 

subnet_mask_t::subnet_mask_t(
    const subnet_mask_t &initializer)
    : subnet_mask_(initializer.subnet_mask_)
{
}

subnet_mask_t::~subnet_mask_t()
{
}

subnet_mask_t &
subnet_mask_t::operator=(const subnet_mask_t &rhs)
{
        subnet_mask_ = rhs.subnet_mask_;
        return *this;
}

subnet_mask_t::operator std::wstring()
const
{
        return subnet_mask_;
}

const uint8_t
subnet_mask_t::octet_a()
const
{
        return subnet_mask_.octet_a();
}

const uint8_t
subnet_mask_t::octet_b()
const
{
        return subnet_mask_.octet_b();
}

const uint8_t
subnet_mask_t::octet_c()
const
{
        return subnet_mask_.octet_c();
}

const uint8_t
subnet_mask_t::octet_d()
const
{
        return subnet_mask_.octet_d();
}

adapter_address_info_t::adapter_address_info_t()
    : ip_address_(),
      subnet_mask_()
{
}

adapter_address_info_t::adapter_address_info_t(
    const ip_address_t &ip_address,
    const subnet_mask_t &subnet_mask)
    : ip_address_(ip_address),
      subnet_mask_(subnet_mask)
{
}

adapter_address_info_t::adapter_address_info_t(
    const adapter_address_info_t &initializer)
    : ip_address_(initializer.ip_address_),
      subnet_mask_(initializer.subnet_mask_)
{
}

adapter_address_info_t::~adapter_address_info_t()
{
}

adapter_address_info_t &
adapter_address_info_t::operator=(const adapter_address_info_t &rhs)
{
        if (this == &rhs)
                return *this;

        ip_address_ = rhs.ip_address_;
        subnet_mask_ = rhs.subnet_mask_;

        return *this;
}

adapter_address_info_t::operator std::wstring()
const
{
        return static_cast<std::wstring>(ip_address_)
               + L"/"
               + static_cast<std::wstring>(subnet_mask_);
}

const ip_address_t &
adapter_address_info_t::ip_address()
const
{
        return ip_address_;
}

const subnet_mask_t &
adapter_address_info_t::subnet_mask()
const
{
        return subnet_mask_;
}

std::vector<adapter_address_info_t>
get_adapter_addresses()
{
        std::vector<adapter_address_info_t> addresses;
        std::vector<uint8_t> buffer;
        MIB_IPADDRTABLE *ip_addr_table = 0x0;
        ULONG buffer_length = 0;

        if (GetIpAddrTable(0x0, &buffer_length, false)
            != ERROR_INSUFFICIENT_BUFFER)
                assert(0
                       && "The system isn't requesting any buffer space!");

        buffer.resize(buffer_length);
        ip_addr_table = reinterpret_cast<MIB_IPADDRTABLE *>(&buffer[0]);
        if (GetIpAddrTable(ip_addr_table, &buffer_length, false)
            != NO_ERROR)
                assert(0
                       && "We've hit a mysterious issue...");

        for (DWORD i = 0, j = ip_addr_table->dwNumEntries; i < j; i++) {
                const MIB_IPADDRROW &addr_row =
                    ip_addr_table->table[i];
                addresses.push_back(
                    adapter_address_info_t(
                        ip_address_t(
                            addr_row.dwAddr & 0xff,
                            (addr_row.dwAddr >> 8) & 0xff,
                            (addr_row.dwAddr >> 16) & 0xff,
                            (addr_row.dwAddr >> 24) & 0xff),
                        subnet_mask_t(
                            addr_row.dwMask & 0xff,
                            (addr_row.dwMask >> 8) & 0xff,
                            (addr_row.dwMask >> 16) & 0xff,
                            (addr_row.dwMask >> 24) & 0xff)));
        }

        return addresses;
}

} /* ip_utilities */

