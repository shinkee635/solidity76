{
    let u256max := 0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
    let a := create(0, u256max, 0)
    let b := and(u256max, create(0, u256max, 0))
    mstore(0, eq(a, b))
}
// ----
// Trace:
//   CREATE(0, 0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff, 0)
//   CREATE(0, 0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff, 0)
// Memory dump:
//      0: 0000000000000000000000000000000000000000000000000000000000000001
// Storage dump:
