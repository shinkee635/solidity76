contract C {
    function f(uint y) public pure {
        // fits FixedBytes with exactly 32-bytes
        y = uint(ffffffff00000000ffffffff00000000ffffffff00000000ffffffff00000000); // FixedBytes (32)

        // fits exactly into FixedBytes (32), ensures underscored literals won't hurt
        y = uint(ffffffff00000000ffffffff00000000ffffffff00000000ffffffff_00000000);
    }
}
// ----
