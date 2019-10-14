# dbj--nanoplay
### [nano](https://en.wikipedia.org/wiki/Nano) play for the [nano lib](https://github.com/dbj-systems/dbj--nanolib)

## The Purpose

Of this project is twofold

A. to act as a testing ground for the dbj++ [Nanolib](https://github.com/dbj-systems/dbj--nanolib)
B. to be an entry point into the concepts of dbj++ [Nanolib](https://github.com/dbj-systems/dbj--nanolib)

## Usage

Currently this is a Visual Studio / Windows project. By far the easiest wat to start, is to press the big green button above titled "Clone or Download" and then select "Open in Desktop".

If you are a git connoisseur please jump to "The code guide"

This is currently (2019Q4) a necessity, since Visual Studio 2019 GIT integration simply "does not know" about git submodules.

In this project, dbj++ Nano is a [git submodule](https://gist.github.com/gitaarik/8735255). The text behind this link is also and actually, fully revealing why would anybody bother with git submodules.

GitHub Windows Desktop App is properly handling the git submodules, that means "transparently". If not already on you machine you will be asked to install it. Installation is quick and the app is pretty sleek and small.

After yuo have assigned a folder  somewhere on your machine, and github desktop app has done its quiet job, the sub folder of the project `dbj--nanolib` will contain the latest dbj++ Nanolib as a submodule.

It is a normal folder in which you can see the files and even open them in the VS IDE. Study them and put break codes in there.

## The code guide

dbj++ Nanolib is primarily for server side scenarios.  

- project is unicode, but wchar is not used
     - for non ascii [literals](https://en.cppreference.com/w/cpp/language/string_literal) we use `u8"... "`
- streams are not used
     - just while testing ;)
- `try/throw/catch` are not used
     - exceptions are not used
     - `<system_error>` just in a very limited and encapsulated fashion
- std::string strictly for what it is made for
     - that means: not for char buffering

And so on. 

**Assumption** (and the authors idea) is you will be following the code here through the excellent Visual Studio debugger. Good, but where to put the break points? What to look for?

`dbj++nanoplay.cpp` is where the main() is. It is actually pretty empty as such.

In here "dbj++ TU" is of course used. TU stands for "Test Unit". `dbj::tu` namespace resides in the Nanolib and contains a tiny (aka 'nano') but complete testing framework. It is in the `dbj++tu.h` file, inside the `dbj--nanolib` folder.

Basically wherever you see
```cpp
TU_REGISTER(
    [] {
        /* ... some code here ... */
    }
);
```
you can place a break point to follow that TU (aka Test Unit), through a debugger.

One such a place is the Nanolib in the folder `dbj--nanolib` in the file `dbj++valstat.h` where by the end of file we test the `valstat` default traits.

Idea and guiding principle is that standard C++ can be simple and useful. Thus the code you find here is not overly complex, neither the files are 10K lines monsters.

And yes, code in here tends to be well commented.

## Contact

In case you hotly dislike or hotly like, or anything in between, what you find in here please do write me:

[dbj@dbj.org](mailto:dbj@dbj.org)

## License

### CC BY-SA 4.0 -- https://creativecommons.org/licenses/by-sa/4.0/ 

---
[![dbjdbj github avatar](https://github.com/dbjdbj.png)
 &copy; 2019](https://dbj.netlify.com)
