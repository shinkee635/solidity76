contract C {
    function f() public returns (address) {
        assembly {
            mstore(mload(0x40), 0xca35b7d915458ef540ade6068dfe2f44e8fa733c)
        }
        return ecrecover(
            bytes32(uint256(0x0077e5189111eb6557e8a637b27ef8fbb15bc61d61c2f00cc48878f3a296e5e0ca)),
            0, // invalid v value
            bytes32(uint256(0x006944c77849b18048f6abe0db8084b0d0d0689cdddb53d2671c36967b58691ad4)),
            bytes32(uint256(0x00ef4f06ba4f78319baafd0424365777241af4dfd3da840471b4b4b087b7750d0d))
        );
    }
}
// ====
// compileViaYul: also
// ----
// f() -> 0
