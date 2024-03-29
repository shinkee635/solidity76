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
 * @author Alex Beregszaszi
 * @date 2018
 * Tests for the assembler.
 */

#include <libsolutil/JSON.h>
#include <libevmasm/Assembly.h>

#include <boost/test/unit_test.hpp>

#include <string>
#include <tuple>
#include <memory>
#include <libyul/Exceptions.h>

using namespace std;
using namespace solidity::langutil;
using namespace solidity::evmasm;

namespace solidity::frontend::test
{

namespace
{
	void checkCompilation(evmasm::Assembly const& _assembly)
	{
		LinkerObject output = _assembly.assemble();
		BOOST_CHECK(output.bytecode.size() > 0);
		BOOST_CHECK(output.toHex().length() > 0);
	}
}

BOOST_AUTO_TEST_SUITE(Assembler)

BOOST_AUTO_TEST_CASE(all_assembly_items)
{
	map<string, unsigned> indices = {
		{ "root.asm", 0 },
		{ "sub.asm", 1 }
	};
	Assembly _assembly;
	auto root_asm = make_shared<CharStream>("lorem ipsum", "root.asm");
	_assembly.setSourceLocation({1, 3, root_asm});

	Assembly _subAsm;
	auto sub_asm = make_shared<CharStream>("lorem ipsum", "sub.asm");
	_subAsm.setSourceLocation({6, 8, sub_asm});
	// PushImmutable
	_subAsm.appendImmutable("someImmutable");
	_subAsm.append(Instruction::INVALID);
	shared_ptr<Assembly> _subAsmPtr = make_shared<Assembly>(_subAsm);

	// Tag
	auto tag = _assembly.newTag();
	_assembly.append(tag);
	// Operation
	_assembly.append(u256(1));
	_assembly.append(u256(2));
	// Push
	_assembly.append(Instruction::KECCAK256);
	// PushProgramSize
	_assembly.appendProgramSize();
	// PushLibraryAddress
	_assembly.appendLibraryAddress("someLibrary");
	// PushTag + Operation
	_assembly.appendJump(tag);
	// PushData
	_assembly.append(bytes{0x1, 0x2, 0x3, 0x4});
	// PushSubSize
	auto sub = _assembly.appendSubroutine(_subAsmPtr);
	// PushSub
	_assembly.pushSubroutineOffset(static_cast<size_t>(sub.data()));
	// PushDeployTimeAddress
	_assembly.append(PushDeployTimeAddress);
	// AssignImmutable.
	// Note that since there is no reference to "someOtherImmutable", this will compile to a simple POP in the hex output.
	_assembly.appendImmutableAssignment("someOtherImmutable");
	_assembly.append(u256(2));
	_assembly.appendImmutableAssignment("someImmutable");
	// Operation
	_assembly.append(Instruction::STOP);
	_assembly.appendAuxiliaryDataToEnd(bytes{0x42, 0x66});
	_assembly.appendAuxiliaryDataToEnd(bytes{0xee, 0xaa});

	checkCompilation(_assembly);

	BOOST_CHECK_EQUAL(
		_assembly.assemble().toHex(),
		"5b600160022060877f__$bf005014d9d0f534b8fcb268bd84c491a2380f4acd260d1ccfe9cd8201$__"
		"600056607f6022605d7f000000000000000000000000000000000000000000000000000000000000000050"
		"60028060015250"
		"00fe"
		"7f0000000000000000000000000000000000000000000000000000000000000000"
		"fe010203044266eeaa"
	);
	BOOST_CHECK_EQUAL(
		_assembly.assemblyString(),
		"    /* \"root.asm\":1:3   */\n"
		"tag_1:\n"
		"  keccak256(0x02, 0x01)\n"
		"  bytecodeSize\n"
		"  linkerSymbol(\"bf005014d9d0f534b8fcb268bd84c491a2380f4acd260d1ccfe9cd8201f7e994\")\n"
		"  jump(tag_1)\n"
		"  data_a6885b3731702da62e8e4a8f584ac46a7f6822f4e2ba50fba902f67b1588d23b\n"
		"  dataSize(sub_0)\n"
		"  dataOffset(sub_0)\n"
		"  deployTimeAddress()\n"
		"  assignImmutable(\"0xc3978657661c4d8e32e3d5f42597c009f0d3859e9f9d0d94325268f9799e2bfb\")\n"
		"  0x02\n"
		"  assignImmutable(\"0x26f2c0195e9d408feff3abd77d83f2971f3c9a18d1e8a9437c7835ae4211fc9f\")\n"
		"  stop\n"
		"stop\n"
		"data_a6885b3731702da62e8e4a8f584ac46a7f6822f4e2ba50fba902f67b1588d23b 01020304\n"
		"\n"
		"sub_0: assembly {\n"
		"        /* \"sub.asm\":6:8   */\n"
		"      immutable(\"0x26f2c0195e9d408feff3abd77d83f2971f3c9a18d1e8a9437c7835ae4211fc9f\")\n"
		"      invalid\n"
		"}\n"
		"\n"
		"auxdata: 0x4266eeaa\n"
	);
	BOOST_CHECK_EQUAL(
		util::jsonCompactPrint(_assembly.assemblyJSON(indices)),
		"{\".auxdata\":\"4266eeaa\",\".code\":["
		"{\"begin\":1,\"end\":3,\"name\":\"tag\",\"source\":0,\"value\":\"1\"},"
		"{\"begin\":1,\"end\":3,\"name\":\"JUMPDEST\",\"source\":0},"
		"{\"begin\":1,\"end\":3,\"name\":\"PUSH\",\"source\":0,\"value\":\"1\"},"
		"{\"begin\":1,\"end\":3,\"name\":\"PUSH\",\"source\":0,\"value\":\"2\"},"
		"{\"begin\":1,\"end\":3,\"name\":\"KECCAK256\",\"source\":0},"
		"{\"begin\":1,\"end\":3,\"name\":\"PUSHSIZE\",\"source\":0},"
		"{\"begin\":1,\"end\":3,\"name\":\"PUSHLIB\",\"source\":0,\"value\":\"someLibrary\"},"
		"{\"begin\":1,\"end\":3,\"name\":\"PUSH [tag]\",\"source\":0,\"value\":\"1\"},"
		"{\"begin\":1,\"end\":3,\"name\":\"JUMP\",\"source\":0},"
		"{\"begin\":1,\"end\":3,\"name\":\"PUSH data\",\"source\":0,\"value\":\"A6885B3731702DA62E8E4A8F584AC46A7F6822F4E2BA50FBA902F67B1588D23B\"},"
		"{\"begin\":1,\"end\":3,\"name\":\"PUSH #[$]\",\"source\":0,\"value\":\"0000000000000000000000000000000000000000000000000000000000000000\"},"
		"{\"begin\":1,\"end\":3,\"name\":\"PUSH [$]\",\"source\":0,\"value\":\"0000000000000000000000000000000000000000000000000000000000000000\"},"
		"{\"begin\":1,\"end\":3,\"name\":\"PUSHDEPLOYADDRESS\",\"source\":0},"
		"{\"begin\":1,\"end\":3,\"name\":\"ASSIGNIMMUTABLE\",\"source\":0,\"value\":\"someOtherImmutable\"},"
		"{\"begin\":1,\"end\":3,\"name\":\"PUSH\",\"source\":0,\"value\":\"2\"},"
		"{\"begin\":1,\"end\":3,\"name\":\"ASSIGNIMMUTABLE\",\"source\":0,\"value\":\"someImmutable\"},"
		"{\"begin\":1,\"end\":3,\"name\":\"STOP\",\"source\":0}"
		"],\".data\":{\"0\":{\".code\":["
		"{\"begin\":6,\"end\":8,\"name\":\"PUSHIMMUTABLE\",\"source\":1,\"value\":\"someImmutable\"},"
		"{\"begin\":6,\"end\":8,\"name\":\"INVALID\",\"source\":1}"
		"]},\"A6885B3731702DA62E8E4A8F584AC46A7F6822F4E2BA50FBA902F67B1588D23B\":\"01020304\"}}"
	);
}

BOOST_AUTO_TEST_CASE(immutable)
{
	map<string, unsigned> indices = {
		{ "root.asm", 0 },
		{ "sub.asm", 1 }
	};
	Assembly _assembly;
	auto root_asm = make_shared<CharStream>("lorem ipsum", "root.asm");
	_assembly.setSourceLocation({1, 3, root_asm});

	Assembly _subAsm;
	auto sub_asm = make_shared<CharStream>("lorem ipsum", "sub.asm");
	_subAsm.setSourceLocation({6, 8, sub_asm});
	_subAsm.appendImmutable("someImmutable");
	_subAsm.appendImmutable("someOtherImmutable");
	_subAsm.appendImmutable("someImmutable");
	shared_ptr<Assembly> _subAsmPtr = make_shared<Assembly>(_subAsm);

	_assembly.append(u256(42));
	_assembly.appendImmutableAssignment("someImmutable");
	_assembly.append(u256(23));
	_assembly.appendImmutableAssignment("someOtherImmutable");

	auto sub = _assembly.appendSubroutine(_subAsmPtr);
	_assembly.pushSubroutineOffset(static_cast<size_t>(sub.data()));

	checkCompilation(_assembly);

	BOOST_CHECK_EQUAL(
		_assembly.assemble().toHex(),
		// root.asm
		// assign "someImmutable"
		"602a" // PUSH1 42 - value for someImmutable
		"80" // DUP1
		"6001" // PUSH1 1 - offset of first someImmutable in sub_0
		"52" // MSTORE
		"80" // DUP1
		"6043" // PUSH1 67 - offset of second someImmutable in sub_0
		"52" // MSTORE
		"50" // POP
		// assign "someOtherImmutable"
		"6017" // PUSH1 23 - value for someOtherImmutable
		"80" // DUP1
		"6022" // PUSH1 34 - offset of someOtherImmutable in sub_0
		"52" // MSTORE
		"50" // POP
		"6063" // PUSH1 0x63 - dataSize(sub_0)
		"6017" // PUSH1 0x17 - dataOffset(sub_0)
		"fe" // INVALID
		// end of root.asm
		// sub.asm
		"7f0000000000000000000000000000000000000000000000000000000000000000" // PUSHIMMUTABLE someImmutable - data at offset 1
		"7f0000000000000000000000000000000000000000000000000000000000000000" // PUSHIMMUTABLE someOtherImmutable - data at offset 34
		"7f0000000000000000000000000000000000000000000000000000000000000000" // PUSHIMMUTABLE someImmutable - data at offset 67
	);
	BOOST_CHECK_EQUAL(
		_assembly.assemblyString(),
		"    /* \"root.asm\":1:3   */\n"
		"  0x2a\n"
		"  assignImmutable(\"0x26f2c0195e9d408feff3abd77d83f2971f3c9a18d1e8a9437c7835ae4211fc9f\")\n"
		"  0x17\n"
		"  assignImmutable(\"0xc3978657661c4d8e32e3d5f42597c009f0d3859e9f9d0d94325268f9799e2bfb\")\n"
		"  dataSize(sub_0)\n"
		"  dataOffset(sub_0)\n"
		"stop\n"
		"\n"
		"sub_0: assembly {\n"
		"        /* \"sub.asm\":6:8   */\n"
		"      immutable(\"0x26f2c0195e9d408feff3abd77d83f2971f3c9a18d1e8a9437c7835ae4211fc9f\")\n"
		"      immutable(\"0xc3978657661c4d8e32e3d5f42597c009f0d3859e9f9d0d94325268f9799e2bfb\")\n"
		"      immutable(\"0x26f2c0195e9d408feff3abd77d83f2971f3c9a18d1e8a9437c7835ae4211fc9f\")\n"
		"}\n"
	);
	BOOST_CHECK_EQUAL(
		util::jsonCompactPrint(_assembly.assemblyJSON(indices)),
		"{\".code\":["
		"{\"begin\":1,\"end\":3,\"name\":\"PUSH\",\"source\":0,\"value\":\"2A\"},"
		"{\"begin\":1,\"end\":3,\"name\":\"ASSIGNIMMUTABLE\",\"source\":0,\"value\":\"someImmutable\"},"
		"{\"begin\":1,\"end\":3,\"name\":\"PUSH\",\"source\":0,\"value\":\"17\"},"
		"{\"begin\":1,\"end\":3,\"name\":\"ASSIGNIMMUTABLE\",\"source\":0,\"value\":\"someOtherImmutable\"},"
		"{\"begin\":1,\"end\":3,\"name\":\"PUSH #[$]\",\"source\":0,\"value\":\"0000000000000000000000000000000000000000000000000000000000000000\"},"
		"{\"begin\":1,\"end\":3,\"name\":\"PUSH [$]\",\"source\":0,\"value\":\"0000000000000000000000000000000000000000000000000000000000000000\"}"
		"],\".data\":{\"0\":{\".code\":["
		"{\"begin\":6,\"end\":8,\"name\":\"PUSHIMMUTABLE\",\"source\":1,\"value\":\"someImmutable\"},"
		"{\"begin\":6,\"end\":8,\"name\":\"PUSHIMMUTABLE\",\"source\":1,\"value\":\"someOtherImmutable\"},"
		"{\"begin\":6,\"end\":8,\"name\":\"PUSHIMMUTABLE\",\"source\":1,\"value\":\"someImmutable\"}"
		"]}}}"
	);
}

BOOST_AUTO_TEST_CASE(subobject_encode_decode)
{
	Assembly assembly;

	shared_ptr<Assembly> subAsmPtr = make_shared<Assembly>();
	shared_ptr<Assembly> subSubAsmPtr = make_shared<Assembly>();

	assembly.appendSubroutine(subAsmPtr);
	subAsmPtr->appendSubroutine(subSubAsmPtr);

	BOOST_CHECK(assembly.encodeSubPath({0}) == 0);
	BOOST_REQUIRE_THROW(assembly.encodeSubPath({1}), solidity::evmasm::AssemblyException);
	BOOST_REQUIRE_THROW(assembly.decodeSubPath(1), solidity::evmasm::AssemblyException);

	vector<size_t> subPath{0, 0};
	BOOST_CHECK(assembly.decodeSubPath(assembly.encodeSubPath(subPath)) == subPath);
}

BOOST_AUTO_TEST_SUITE_END()

} // end namespaces
