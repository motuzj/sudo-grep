# sudo-grep

## Options

The following options are available for `sudo-grep`:

- `-i` / `--ignore-case`: Ignore case in pattern and input data.
- `-v` / `--invert-match`: Invert the sense of matching, to select non-matching lines.

## Building

To build project, you can use either `make` or you can build directly with `gcc`.

```shell
make
```

## Testing

For testing, you must have `sudo-grep` in the root directory of the project.

```shell
make test
```

### Test file format

The `.in` and `.out` files have specific formats for defining test cases.

#### `.in` format:

```
test name
arguments
```

- `test description`: A description for the test case.
- `arguments`: An agruments to run `sudo-grep` with. (options + pattern)

#### `.out` format:

```
expected output
```

- `expected output`: The expected output when running the test case.
