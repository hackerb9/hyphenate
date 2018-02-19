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

This is almost purely the example.c that comes with the source for
libhyphen. I merely changed it to work more nicely from the command
line.

## Hyphenation dictionaries

Install with:

	apt install hyphen-en-us

Note that if you have libreoffice already installed on your machine,
you already have the necessary hyphenation dictionaries installed in
`/usr/share/hyphen/`.

Alternate language dictionaries are available. 

