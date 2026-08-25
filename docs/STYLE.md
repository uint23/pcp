pcp coding style

## Struct definition alignment

Members align their names into a column. A blank line ends the group, and the
next group aligns independently.

```c
typedef struct {
	FILE*     file;
	some_type something;

	char something_else;
	int  another_thing;
} SomeStruct;
```

clang-format has no struct-only option, the setting that aligns declarations
aligns all of them, local variables included so it stays off and this is a
convention, not a formatter rule.
