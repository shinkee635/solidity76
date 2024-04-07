{
    let u256max := 0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
    let a := create2(0, u256max, 0, 0)
    let b := and(u256max, create2(0, u256max, 0, 0))
    mstore(0, eq(a, b))
}
// ====
// EVMVersion: >=constantinople
// ----
// Trace:
//   CREATE2(0, 0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff, 0, 0)
//   CREATE2(0, 0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff, 0, 0)
// Memory dump:
//      0: 0000000000000000000000000000000000000000000000000000000000000001
// Storage dump:
