# hyphenate
Command line hyphenation of words using libhyphen

## Example

    $ hyphenate example
    ex=am=ple

    $ hyphenate hyphenate
    hy=phen=ate

    $ hyphenate transubstantiation
    tran=sub=stan=ti=a=tion

## Example? Example!

This is almost purely the `example.c` that comes with the source for
libhyphen. I merely changed it to work more nicely from the command
line.

## Hyphenation dictionaries and libhyphen

If you don't have the dictionaries, you can install them on most
Debian derivatives like so:

	apt install hyphen-en-us libhyphen0

Note that if you have libreoffice installed on your machine, you
already have the necessary hyphenation dictionaries and libraries.

Alternate language dictionaries are available. 
You can see what dictionaries you have by checking `/usr/share/hyphen/`.

