contract C {
    function f() public pure returns (address r) {
        bytes32 x = 0x1122334455667788990011223344556677889900ff1234567890123456789012;
        bytes32 y;
        assembly {
            y := x
        }
        address z = address(y);
        assembly {
            r := z
        }
        require(z == 0x1122334455667788990011223344556677889900ff1234567890123456789012);
    }

    function g() public pure returns (address payable r) {
        bytes32 x = 0x1122334455667788990011223344556677889900ff1234567890123456789012;
        bytes32 y;
        assembly {
            y := x
        }
        address payable z = address(y);
        assembly {
            r := z
        }
        require(z == 0x1122334455667788990011223344556677889900ff1234567890123456789012);
    }
}

// ====
// compileViaYul: also
// compileToEwasm: also
// ----
// f() -> 0x1122334455667788990011223344556677889900ff1234567890123456789012
// g() -> 0x1122334455667788990011223344556677889900ff1234567890123456789012
