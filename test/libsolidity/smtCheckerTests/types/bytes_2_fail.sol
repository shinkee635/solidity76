pragma experimental SMTChecker;

contract C
{
	function f(bytes memory b1, bytes memory b2) public pure {
		b1 = b2;
		assert(b1[1] == b2[2]);
	}
}
// ----
// Warning 6328: (119-141): CHC: Assertion violation happens here.\nCounterexample:\n\nb1 = [7719, 7718, 7719, 7719, 7719]\nb2 = [7719, 7718, 7719, 7719, 7719]\n\n\nTransaction trace:\nconstructor()\nf([], [7719, 7718, 7719, 7719, 7719])
