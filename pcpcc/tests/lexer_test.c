int variadic(int x, ...)
{
	return 1;
}

int main(void)
{
	/* hallo welt! */
	int x67 = 67;
	int hex = 0x67;
	int oct = 067;
	int suf = 67L;

	char char0 = 'a';
	char char1 = '\\';
	char char2 = '\'';
	char char3 = '\n';

	variadic(hex, oct, suf);

	return x67;
}

