// Test for regression of https://github.com/ethereum/solidity/issues/8406

contract C {
  address constant e = 0x1212121212121212121212121000000000000000000000000000002134593163;

  function f() public returns (byte z) {
    assembly { z := e }
  }
}

// ====
// compileViaYul: also
// compileToEwasm: also
// ----
// f() -> 0x00
