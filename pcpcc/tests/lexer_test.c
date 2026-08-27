int variadic(int x, ...)
{
	return x;
}

int main(void)
{
	/* hallo welt! */
	int i0 = 67;
	int i1 = 0x67;
	int i2 = 067;
	int i3 = 67L;

	float f0 = 10;
	float f1 = 10.1;
	float f2 = 10.1f;
	float f3 = 10.1e3;

	char c0 = 'a';
	char c1 = '\\';
	char c2 = '\'';
	char c3 = '\n';

	char* s0 = "diggy dogsticks\n";
	char* s1 = "zoooooom";
	char* s2 = "moo\"oo\"zz";

	variadic(i0, i1, i3);

	return 0;
}

