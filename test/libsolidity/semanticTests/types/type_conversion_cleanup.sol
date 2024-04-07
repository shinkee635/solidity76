contract Test {
    function test() public returns (uint ret) { return uint(address(Test(address(0x3344556677889900112233445566778899001122334455667788990011223344)))); }
}
// ====
// compileViaYul: also
// compileToEwasm: also
// ----
// test() -> 0x3344556677889900112233445566778899001122334455667788990011223344
