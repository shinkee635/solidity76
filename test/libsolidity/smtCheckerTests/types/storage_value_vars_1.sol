pragma experimental SMTChecker;
contract C
{
    address a;
    bool b;
    uint c;
    function f(uint x) public {
        if (x == 0)
        {
            a = 0x0000000000000000000000000000000000000000000000000000000000000100;
            b = true;
        }
        else
        {
            a = 0x0000000000000000000000000000000000000000000000000000000000000200;
            b = false;
        }
        assert(a > 0x0000000000000000000000000000000000000000000000000000000000000000 && b);
    }
}
// ----
// Warning 6328: (410-493): CHC: Assertion violation happens here.\nCounterexample:\na = 512, b = false, c = 0\nx = 1\n\n\nTransaction trace:\nconstructor()\nState: a = 0, b = false, c = 0\nf(1)
