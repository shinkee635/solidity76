pragma abicoder               v2;
contract C {
    struct S {
        uint16[] m;
    }
    function f(S calldata s) public pure returns (bool correct) {
        int8 x = int8(s.m[0]);
        uint r;
        assembly {
            r := x
        }
        correct = r == uint256(0x00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff80);
    }
}
// ====
// compileViaYul: true
// ----
// f((uint16[])): 0x20, 0x20, 0x01, 0x0180 -> true
