# assert

Automated tests for command-line programs.

```sh
$ assert - <<EOF
tests:
- name: Test printf
  exec: printf "Hello, %s!" "world"
  expect: Hello, world!
- name: Test cat with unexisting file
  exec: cat this_file_does_not_exist
  expect: "cat: this_file_does_not_exist: No such file or directory"
EOF
```

Outputs the following:

```sh
✔ Test printf
✔ Test cat with unexisting file
```

## Usage

**assert** receives as input the "test data", which is a YAML-formatted set of
tests cases. The data can be passed either as a file (`assert my_tests.yml`) or
piped (`assert -`).

### Options

```
-f, --filter
Run only tests that you want, based on the test name. Regex is supported.
```

## Installation from source

The following will clone a few dependencies and install them:

```sh
$ make get_dependencies
```

And then we can compile the program with:

```sh
$ make build
```

If executed successfully, an `assert` executable should be available inside
the `dist` folder:

```sh
$ dist/assert my_test_data.yml
```

## LICENSE

The MIT License (MIT)

Copyright (c) 2024 Dhuan Oliveira

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the “Software”), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
