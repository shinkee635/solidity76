pragma experimental SMTChecker;


contract A {

    address constant public MY_ADDRESS = 0xE0f5206BBD039E7B0592d8918820024E2a7437B9123456789012345678901234;
    address public myFriendsAddress = 0xC0fFee254729296a45a3885639Ac7e10f9d54979123456789012345678901234;

    uint public used;
    uint public test = 5;

    uint constant X = 32**22 + 8;
    string constant TEXT1 = "abc";
    string text2 = "xyz";

    function setUsed() public {
        if (msg.sender == MY_ADDRESS) {
            used = test;
        }
    }
}


contract B is A {

    address public mySistersAddress = 0x999999CF1046e68e36E1Aa2e0E07105EDdd1F08E123456789012345678901234;

    fallback () external {
        used = 0;
    }

    function setUsed(uint a) public {
        if (msg.sender == MY_ADDRESS) {
            used = a;
        }
    }
}

contract MyConc{

    uint constant A = 1;
    bytes32 should_be_constant = sha256('abc');
    uint should_be_constant_2 = A + 1;
    address not_constant = msg.sender;
    uint not_constant_2 = getNumber();
    uint not_constant_3 = 10 + block.number;

    function getNumber() public returns(uint){
        return block.number;
    }

}
// ====
// SMTIgnoreCex: yes
// ----
// Warning 2519: (845-864): This declaration shadows an existing declaration.
// Warning 2018: (1081-1158): Function state mutability can be restricted to view
// Warning 4984: (1057-1074): CHC: Overflow (resulting value larger than 2**256 - 1) happens here.
