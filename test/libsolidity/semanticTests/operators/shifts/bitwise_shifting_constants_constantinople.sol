contract C {
    function shl_1() public returns (bool) {
        uint256 c;
        assembly {
            c := shl(2, 1)
        }
        assert(c == 4);
        return true;
    }

    function shl_2() public returns (bool) {
        uint256 c;
        assembly {
            c := shl(
                1,
                uint256(0x00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff)
            )
        }
        assert(
            c ==
                uint256(0x00fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe)
        );
        return true;
    }

    function shl_3() public returns (bool) {
        uint256 c;
        assembly {
            c := shl(
                256,
                uint256(0x00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff)
            )
        }
        assert(c == 0);
        return true;
    }

    function shr_1() public returns (bool) {
        uint256 c;
        assembly {
            c := shr(1, 3)
        }
        assert(c == 1);
        return true;
    }

    function shr_2() public returns (bool) {
        uint256 c;
        assembly {
            c := shr(
                1,
                uint256(0x00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff)
            )
        }
        assert(
            c ==
                uint256(0x007fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff)
        );
        return true;
    }

    function shr_3() public returns (bool) {
        uint256 c;
        assembly {
            c := shr(
                256,
                uint256(0x00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff)
            )
        }
        assert(c == 0);
        return true;
    }
}
// ====
// EVMVersion: >=constantinople
// compileViaYul: also
// ----
// shl_1() -> 0x01
// shl_2() -> 0x01
// shl_3() -> 0x01
// shr_1() -> 0x01
// shr_2() -> 0x01
// shr_3() -> 0x01
