contract C {
    function f() pure public {
        address x = 0xFA0bFc97E48458494Ccd857e1A85DC91F7F0046E123456789012345678901234;
        x;
    }
}
// ----
// SyntaxError 9429: (64-130): This looks like an address but has an invalid checksum. Correct checksummed address: "0xFA0bfC97E48458494CCd857E1A85Dc91f7f0046e123456789012345678901234". If this is not used as an address, please prepend '00'. For more information please see https://docs.soliditylang.org/en/develop/types.html#address-literals
