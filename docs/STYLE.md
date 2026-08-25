pcp coding style

Formatting is enforced by `.clang-format` at the repo root. Run it before
committing; CI diffs against it.

```sh
clang-format -i $(git ls-files '*.c' '*.h')
```

## Declaration alignment

Consecutive declarations align their names into a column. A blank line ends
the group, and the next group aligns independently.

```c
typedef struct {
	FILE*     file;
	some_type something;

	char something_else;
	int  another_thing;
} SomeStruct;
```

This is `AlignConsecutiveDeclarations: Consecutive`. clang-format applies it
to every run of declarations, not just struct members, so local variables
align too:

```c
static void open_sources(void)
{
	int           c;
	FILE*         f;
	unsigned long counter;
}
```

There is no setting that restricts alignment to records. Group declarations
with blank lines where a shared column reads badly — the blank line is the
only control you have.

Do not hand-align. clang-format recomputes the column on every run, so manual
spacing is either overwritten or silently wrong.

## Indentation

Tabs, 8 wide. Alignment padding is spaces (`UseTab: AlignWithSpaces`), so a
struct's columns line up at any tab width, not just 8.

Set your editor to match:

```
tabstop=8  shiftwidth=8  noexpandtab
```

## Braces

Functions open on their own line; everything else is K&R.

```c
static void open_sources(void)
{
	if (!src_file)
		die(ERR_SRC_FOPEN, "Failed to open file %s", src_file_path);
}
```

Short functions are not collapsed onto one line
(`AllowShortFunctionsOnASingleLine: None`).

## Files

Every file ends with a newline followed by one blank line
(`InsertNewlineAtEOF`, `KeepEmptyLines.AtEndOfFile`). No more than one
consecutive blank line anywhere.

## Linkage

Anything not used outside its translation unit is `static` — functions and
file-scope variables alike. Without it the symbol is exported, which invites
collisions as the compiler grows and suppresses `-Wunused-function`.

```c
static FILE* src_file;
static char* src_file_path;

static void parse_args(int argc, char* argv[]);
```
