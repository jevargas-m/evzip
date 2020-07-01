# evzip / evunzip

## Compression utility for ASCII files 
Implement Huffman codes to compress and uncompress text files ecoded with ASCII done character by character.

## Usage

### Binaries
To generate executables, run:
```
make all
```
Assumes `clang` compiler is installed.

### Compress a file:

```
./evzip "source file"
```
will output a compressed file with `.ez` extension.

Important to make sure file is ASCII encoded (not extended UTF), in order to convert a text file, you can use:
```
iconv -f utf-8 -t ascii//TRANSLIT "source file" > "destination_file"
```

#### Verbose output
Enabling the line `#define VERBOSE` in the source file `evzip.c` would make the program print a summary of all the prefix codes in the tree.

For example compressing a file containing the text `Mary has a little lamb` will output:
![mary](/examples/mary.png)

### Un-compress a file:
```
./evunzip "source ez file" "destination file"
```
Will output an identical file as the one originally compressed.

Both files equality can be checked with:
```
diff --report-identical-files file1 file2
```

### Example
Execute: `./my_diff_test` for everything in action.  Compressing the full Moby Dick novel in about 43% the original size.

![example](/examples/example.png)

## Key concepts
Use Huffman codes algorithm for recursively generating a full binary tree in which each leaf is a pre-fix code for a character, and each internal node has two children `zero` and `one` with a path leading to a pre-fix code.
See: https://en.wikipedia.org/wiki/Huffman_coding for further info on the algorithm rationale.

Implement a Full-Binary with prefix codes based on single character frequencies.  Implements a priority_queue as a min-heap with an underliying array to define the nodes with minimum frequency.  See `heap/` for implementation details.

For encoding `huffmantree.c` maintains a memoized array with the codes for every ASCII code.  To encode compressed output as a file it is done by manipulating individual bits for writing a defined binary file, that starts with the frequencies of each ASCII code in the original uncompressed file as follows: 
```
0xC0DE |Freq for ASCII code 0| |Freq for ASCII code 1| ..... |Freq for ASCII code 255|
0xDADA Encoded bits in a consecutive way
```

For decoding, read the compressed file, with the frequencies in the `0xC0DE` area generate the Huffman tree.  The function `decode_bit` is fed with consecutive bits read from the file and it returns a node in the tree, if the node is a leaf a character is written to the destination file.



