# evzip / evunzip

## Compression utility
Implement Huffman codes to compress and uncompress files via 8-bit alphabet, specially useful on UTF-8/ASCII files.

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

The utility would compress any file, but will achieve best compression on text files encoded as UTF-8.  Due to the Huffman algorithm used on 8-bit alfabet (ASCII).

#### Verbose output
Using the command line option `-v` will make the program print a summary of all the prefix codes in the tree.

For example compressing a file containing the text `Mary has a little lamb`, using `./evzip -v examples/MaryLamb.txt` will output:
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

For encoding `huffmantree.c` maintains a memoized array with the codes for every ASCII code, this avoids the need to look for characters in the tree.  To encode compressed output as a file it is done by manipulating individual bits for writing a defined binary file, that starts with the frequencies of each ASCII code in the original uncompressed file as follows: 
```
0xC0DE |Freq for ASCII code 0| |Freq for ASCII code 1| ..... |Freq for ASCII code 255|
0xDADA Encoded bits in a consecutive way
```
The program will naively assume each character in the source file is from an alphabet of 8-bits (ASCII), and will try to redefine it using shorter ones for common characters.  If the source file is not made like this it will not achieve a good compression rate.

For decoding, read the compressed file, with the frequencies in the `0xC0DE` area generate the Huffman tree.  The function `decode_bit` is fed with consecutive bits read from the file and it returns a node in the tree, if the node is a leaf a character is written to the destination file.



