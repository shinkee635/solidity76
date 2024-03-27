/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
// SPDX-License-Identifier: GPL-3.0
/**
 * Unit tests for the address checksum.
 */

#include <libsolutil/CommonData.h>
#include <libsolutil/Exceptions.h>

#include <test/Common.h>

#include <boost/test/unit_test.hpp>

using namespace std;

namespace solidity::util::test
{

BOOST_AUTO_TEST_SUITE(Checksum)

BOOST_AUTO_TEST_CASE(calculate)
{
	BOOST_CHECK(!getChecksummedAddress("0x5aaeb6053f3e94c9b9a09f33669435e7ef1beaed123456789012345678901234").empty());
	BOOST_CHECK(!getChecksummedAddress("0x0123456789abcdefABCDEF0123456789abcdefAB123456789012345678901234").empty());
	// too short
	BOOST_CHECK_THROW(getChecksummedAddress("0x5aaeb6053f3e94c9b9a09f33669435e7ef1beae123456789012345678901234"), InvalidAddress);
	BOOST_CHECK_THROW(getChecksummedAddress("5aaeb6053f3e94c9b9a09f33669435e7ef1beae123456789012345678901234"), InvalidAddress);
	// too long
	BOOST_CHECK_THROW(getChecksummedAddress("0x5aaeb6053f3e94c9b9a09f33669435e7ef1beaed1123456789012345678901234"), InvalidAddress);
	BOOST_CHECK_THROW(getChecksummedAddress("5aaeb6053f3e94c9b9a09f33669435e7ef1beaed1123456789012345678901234"), InvalidAddress);
	// non-hex character
	BOOST_CHECK_THROW(getChecksummedAddress("0x5aaeb6053f3e94c9b9a09f33669435e7ef1beaeK123456789012345678901234"), InvalidAddress);

	// the official test suite from EIP-55
	vector<string> cases {
		// all upper case
		"0x52908400098527886E0F7030069857D2E4169EE7123456789012345678901234",
		"0x8617E340B3D01FA5F11F306F4090FD50E238070D123456789012345678901234",
		// all lower case
		"0xde709f2102306220921060314715629080e2fb77123456789012345678901234",
		"0x27b1fdb04752bbc536007a920d24acb045561c26123456789012345678901234",
		// regular
		"0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed123456789012345678901234",
		"0xfB6916095ca1df60bB79Ce92cE3Ea74c37c5d359123456789012345678901234",
		"0xdbF03B407c01E7cD3CBea99509d93f8DDDC8C6FB123456789012345678901234",
		"0xD1220A0cf47c7B9Be7A2E6BA89F429762e7b9aDb123456789012345678901234"
	};

	for (size_t i = 0; i < cases.size(); i++)
		BOOST_REQUIRE_MESSAGE(getChecksummedAddress(cases[i]) == cases[i], cases[i]);
}

BOOST_AUTO_TEST_CASE(regular)
{
	BOOST_CHECK(passesAddressChecksum("0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed123456789012345678901234", true));
	BOOST_CHECK(passesAddressChecksum("0xfB6916095ca1df60bB79Ce92cE3Ea74c37c5d359123456789012345678901234", true));
	BOOST_CHECK(passesAddressChecksum("0xdbF03B407c01E7cD3CBea99509d93f8DDDC8C6FB123456789012345678901234", true));
	BOOST_CHECK(passesAddressChecksum("0xD1220A0cf47c7B9Be7A2E6BA89F429762e7b9aDb123456789012345678901234", true));
}

BOOST_AUTO_TEST_CASE(regular_negative)
{
	BOOST_CHECK(!passesAddressChecksum("0x6aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed123456789012345678901234", true));
	BOOST_CHECK(!passesAddressChecksum("0xeB6916095ca1df60bB79Ce92cE3Ea74c37c5d359123456789012345678901234", true));
	BOOST_CHECK(!passesAddressChecksum("0xebF03B407c01E7cD3CBea99509d93f8DDDC8C6FB123456789012345678901234", true));
	BOOST_CHECK(!passesAddressChecksum("0xE1220A0cf47c7B9Be7A2E6BA89F429762e7b9aDb123456789012345678901234", true));
}

BOOST_AUTO_TEST_CASE(regular_invalid_length)
{
	BOOST_CHECK(passesAddressChecksum("0x9426cbfc57389778d313268E7F85F1CDc2fdad60123456789012345678901234", true));
	BOOST_CHECK(!passesAddressChecksum("0x9426cbfc57389778d313268E7F85F1CDc2fdad6123456789012345678901234", true));
	BOOST_CHECK(passesAddressChecksum("0x08A61851FFa4637dE289D630Ae8c5dFb0ff9171F123456789012345678901234", true));
	BOOST_CHECK(!passesAddressChecksum("0x8A61851FFa4637dE289D630Ae8c5dFb0ff9171F123456789012345678901234", true));
	BOOST_CHECK(passesAddressChecksum("0x00c40cC30cb4675673c9ee382de805c19734986A123456789012345678901234", true));
	BOOST_CHECK(!passesAddressChecksum("0xc40cC30cb4675673c9ee382de805c19734986A123456789012345678901234", true));
	BOOST_CHECK(passesAddressChecksum("0xC40CC30cb4675673C9ee382dE805c19734986a00123456789012345678901234", true));
	BOOST_CHECK(!passesAddressChecksum("0xC40CC30cb4675673C9ee382dE805c19734986a123456789012345678901234", true));
}

BOOST_AUTO_TEST_CASE(homocaps_valid)
{
	BOOST_CHECK(passesAddressChecksum("0x52908400098527886E0F7030069857D2E4169EE7123456789012345678901234", true));
	BOOST_CHECK(passesAddressChecksum("0x8617E340B3D01FA5F11F306F4090FD50E238070D123456789012345678901234", true));
	BOOST_CHECK(passesAddressChecksum("0xde709f2102306220921060314715629080e2fb77123456789012345678901234", true));
	BOOST_CHECK(passesAddressChecksum("0x27b1fdb04752bbc536007a920d24acb045561c26123456789012345678901234", true));
}

BOOST_AUTO_TEST_CASE(homocaps_invalid)
{
	string upper = "0x00AA0000000012400000000DDEEFF000000000BB123456789012345678901234";
	BOOST_CHECK(passesAddressChecksum(upper, false));
	BOOST_CHECK(!passesAddressChecksum(upper, true));
	string lower = "0x11aa000000000000000d00cc00000000000000bb123456789012345678901234";
	BOOST_CHECK(passesAddressChecksum(lower, false));
	BOOST_CHECK(!passesAddressChecksum(lower, true));
}

BOOST_AUTO_TEST_SUITE_END()

}
