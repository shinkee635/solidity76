pragma abicoder               v2;


// Checks that address types are properly cleaned before they are compared.
contract C {
    function f(address a) public returns (uint256) {
        if (a != 0x1234567890123456789012345678901234567890123456789012345678901234) return 1;
        return 0;
    }

    function g(address payable a) public returns (uint256) {
        if (a != 0x1234567890123456789012345678901234567890123456789012345678901234) return 1;
        return 0;
    }
}
// ====
// compileViaYul: also
// compileToEwasm: also
// ----
// f(address): 0xffff12345678901234567890123456789012345678901234567890123456 -> FAILURE # We input shorter data on purpose.#
// g(address): 0xffff12345678901234567890123456789012345678901234567890123456 -> FAILURE
