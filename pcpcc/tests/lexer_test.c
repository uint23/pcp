int variadic(int x, ...)
{
	return 1;
}

int main()
{
	int x67 = 67;
	int hex = 0x67;
	int oct = 067;
	int suf = 67L;

	variadic(hex, oct, suf);

	return x67;
}

