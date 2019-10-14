# dbj--nanoplay
### [nano](https://en.wikipedia.org/wiki/Nano) play for the [nano lib](https://github.com/dbj-systems/dbj--nanolib)

In this context, "nano" can be transalated as "tiny"

## The Purpose

Of this project is twofold

A. to act as a testing ground for the dbj++ [Nanolib](https://github.com/dbj-systems/dbj--nanolib)
B. to be an entry point into the concepts of dbj++ [Nanolib](https://github.com/dbj-systems/dbj--nanolib)

## Usage

This is a Visual Studio / Windows project. It is also C++17, windows console project. Probably the easiest way to start, is to press the big green button above titled "Clone or Download" and then select "Open in Desktop".

At this point, if you are a git connoisseur please jump to "The code guide"

This is currently (2019Q4). "Open in Desktop" is a necessity, since Visual Studio 2019 GIT integration simply "does not know" about git submodules.

In this project, dbj++ Nano is an [git submodule](https://gist.github.com/gitaarik/8735255). The text behind this link is also and actually, fully revealing why would anybody bother with git submodules.

GitHub Windows Desktop App is properly handling the git submodules, to me that means "transparently". If not already on you machine you will be asked to install it. Installation is quick and the app is pretty sleek and small.

After you have assigned a folder  somewhere on your machine, and github desktop app has done its quiet job, the sub folder of the project `dbj--nanolib` will contain the latest dbj++ Nanolib as a submodule.

It is a normal folder in which you can see the files and even open them in the VS IDE. Study them and put break codes in there.

## The code guide

**Assumption** (and the authors idea) is you will be following the code here through the excellent Visual Studio debugger. Good, but where to put the break points? What to look for?

dbj++ Nanolib is primarily for server side scenarios.  `dbj++nanoplay.cpp` is where the main() is. It is actually pretty empty as such.

In here "dbj++ TU" is of course used. TU stands for "Test Unit". `dbj::tu` namespace resides in the Nanolib and contains a tiny (aka 'nano') but complete testing framework. It is in the `dbj++tu.h` file, inside the `dbj--nanolib` folder. Basically wherever you see
```cpp
TU_REGISTER(
    [] {
        /* ... some code here ... */
    }
);
```
you can place a break point to follow that TU (aka Test Unit), through a debugger. The number of TU's will grow in future release. 

One interesting TU place is inside the Nanolib in the folder `dbj--nanolib` in the file `dbj++valstat.h` where by the end of file we test the `valstat` default traits. That is why we have done `#define TESTING_DBJ_RETVALS` before including it into this project.

Idea and guiding principle is that standard C++ can be simple and useful. Thus the code you find here is not overly complex, neither the files are 10K lines monsters. I will be adding TU's in future releases to test the quality of dbj++ Nanolib implementation and to show the ways of using it.

And yes, code in here tends to be well commented.

## Contact

In case you hotly dislike or hotly like my work, or anything in between, for what you might find in here, please do write me:

[dbj@dbj.org](mailto:dbj@dbj.org)

## License

### CC BY-SA 4.0 -- https://creativecommons.org/licenses/by-sa/4.0/ 

---
[![dbjdbj github avatar](https://github.com/dbjdbj.png)
 &copy; 2019](https://dbj.netlify.com)
