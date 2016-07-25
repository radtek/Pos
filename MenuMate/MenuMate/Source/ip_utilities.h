#ifndef h_ip_utilities
#define h_ip_utilities

#include <stdint.h>
#include <string>
#include <vector>

namespace ip_utilities {

class ip_address_t {
public:
	ip_address_t();
	ip_address_t(const uint8_t octet_a,
		     const uint8_t octet_b,
		     const uint8_t octet_c,
		     const uint8_t octet_d);
	ip_address_t(const ip_address_t &initializer);
	~ip_address_t();

	ip_address_t &operator=(const ip_address_t &rhs);

	operator std::wstring() const;

	const uint8_t octet_a() const;
	const uint8_t octet_b() const;
	const uint8_t octet_c() const;
	const uint8_t octet_d() const;

private:
	uint32_t ip_address_;
}; /* ip_address_t */

class subnet_mask_t {
public:
	subnet_mask_t();
	subnet_mask_t(const uint8_t a,
		      const uint8_t b,
		      const uint8_t c,
		      const uint8_t d);
	subnet_mask_t(const subnet_mask_t &initializer);
	~subnet_mask_t();

	subnet_mask_t &operator=(const subnet_mask_t &rhs);
	
	operator std::wstring() const;
	operator const uint32_t() const;

	const uint8_t octet_a() const;
	const uint8_t octet_b() const;
	const uint8_t octet_c() const;
	const uint8_t octet_d() const;

private:
	ip_address_t subnet_mask_;
}; /* subnet_mask_t */

class adapter_address_info_t {
public:
	adapter_address_info_t();
	adapter_address_info_t(
	    const ip_address_t &ip_address,
	    const subnet_mask_t &subnet_mask);
	adapter_address_info_t(
	    const adapter_address_info_t &initializer);
	~adapter_address_info_t();

	adapter_address_info_t &operator=(
	    const adapter_address_info_t &rhs);

	operator std::wstring() const;

	const ip_address_t &ip_address() const;
	const subnet_mask_t &subnet_mask() const;

private:
	ip_address_t ip_address_;
	subnet_mask_t subnet_mask_;
}; /* adapter_address_info_t */

std::vector<adapter_address_info_t> get_adapter_addresses();

} /* ip_utilities */

#endif /* ! h_ip_utilities */

