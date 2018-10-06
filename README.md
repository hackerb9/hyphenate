# hyphenate
Command line hyphenation of words using libhyphen

## Example

    $ hyphenate example
    ex=am=ple

    $ hyphenate hyphenate
    hy=phen=ate

    $ hyphenate transubstantiation
    tran=sub=stan=ti=a=tion

## Example? Example.c!

This is almost purely the `example.c` that comes with the source for
libhyphen. I just cleaned it up and changed it to work nicely from the
command line. I also fixed the UTF-8 multibyte character support
(isn't that the "spiﬃ=est"!).

## Installation

    git clone http://github.com/hackerb9/hyphenation
    cd hyphenation
    sudo apt install libhyphen-dev libhyphen0 hyphen-en-us
    make
    make install

## About hyphenation dictionaries and libhyphen

If you don't have the dictionaries, you can install them on most
Debian derivatives like so:

	apt install hyphen-en-us

You can see what dictionaries you have installed on your computer by
checking `/usr/share/hyphen/`. If you have LibreOffice installed on
your machine, you already have the necessary hyphenation dictionaries
and libraries.

### Alternate language dictionaries are available.

You can install the hyphenation patterns for whatever language you
prefer, from Afrikaans to Zulu. For example, for German:

    apt install hyphen-de

If you have your computer set up for a different locale (e.g.,
`LANG="de_DE"`), this program should automatically look for the correct
hyphenation dictionary. However, this has not been tested by a real
user. If it doesn't work, please file a bug report.

## Note of surprise

How can there not have been a standard Unix utility to do this before?
Originally, I wasn't going to write this, because I presumed that, at
the worst, I'd just use `groff`. Unfortunately, while it hyphenates
great, there's no way to get the raw hyphenation before it gets
converted into a PDF or whatever. (You can use the "-a" option to
peek, but then you lose UTF-8).

