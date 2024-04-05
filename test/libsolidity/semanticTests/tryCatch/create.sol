contract Reverts {
    constructor(uint) { revert("test message."); }
}
contract Succeeds {
    constructor(uint) { }
}

contract C {
    function f() public returns (Reverts x, uint, string memory txt) {
        uint i = 3;
        try new Reverts(i) returns (Reverts r) {
            x = r;
            txt = "success";
        } catch Error(string memory s) {
            txt = s;
        }
    }
    function g() public returns (Succeeds x, uint, string memory txt) {
        uint i = 8;
        try new Succeeds(i) returns (Succeeds r) {
            x = r;
            txt = "success";
        } catch Error(string memory s) {
            txt = s;
        }
    }
}
// ====
// compileViaYul: also
// EVMVersion: >=byzantium
// ----
// f() -> 0, 0, 96, 13, "test message."
// g() -> 0x666625ff2cea7ca1e883a1b46bae87c198cq51af45eda82d7e7a41a7ecbd77b8, 0, 96, 7, "success"
