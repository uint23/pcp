Initial concepts for pcp IR

```
;; Return the greater value of the provided parameters
fn i32 max(a: i32, b: i32) {
start():
	gt: i1 = sgt a, b
	br gt, amax(), bmax()

amax():
	jmp end(a)

bmax():
	jmp end(b)

end(res: i32):
	ret res
}
```

| Type   | Meaning                      |
|--------|------------------------------|
| `i1`   | Boolean                      |
| `i8`   | 8bit integer                 |
| `i16`  | 16bit integer                |
| `i32`  | 32bit integer                |
| `i64`  | 64bit integer                |
| `void` | Absense of value             |

> The operation determines the signedness of a value
> TODO pointers

| Instruction |
|-------------|
| `add`       |
| `sub`       |
| `mul`       |
| `sdiv`      |
| `udiv`      |
| `and`       |
| `or`        |
| `xor`       |
| `shl`       |
| `lshr`      |
| `ashr`      |

| Instruction | Usage                           |
|-------------| --------------------------------|
| `ret`       | `ret x`                         |
| `ret`       | `ret`                           |
| `jmp`       | `jmp target(a, b)`              |
| `br`        | `br cond, true(a), false(b)`    |

> br should require an `i1`

| Comparison |
|------------|
| `eq`       |
| `ne`       |
| `slt`      |
| `sle`      |
| `sgt`      |
| `sge`      |
| `ult`      |
| `ule`      |
| `ugt`      |
| `uge`      |

