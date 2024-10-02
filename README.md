# KoolIntelligence
Intelligent ML assistant for KDE

## Building

See [Build instructions](docs/BuildInstructions.md)

## Formatting

Install clang-format.
Then configure cmake using
```
cmake -B build
```

Then run the following command to format the code
```
cd build
make clang-format
```

If you are using ninja(-GNinja), then run the following command
```
cd build
ninja clang-format
```

## Style guidelines

Follow KDE style guidelines which can be found at this [link](https://community.kde.org/Policies/Frameworks_Coding_Style)

As a summary of important things:


### Variable declaration
* Each variable should be declared on a new line
* Each new word in a variable name starts with a capital letter (so-called camelCase)
* Avoid abbreviations
* Use indicative/useful names. No short names
* Variables and functions start with a lowercase letter
