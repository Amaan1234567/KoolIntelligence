# Coding Guidelines

Ensure that you follow the below guidelines when contributing to the project.

## Creating a new branch
* When creating a new branch, ensure that the branch is created from the `main` branch.
* When creating a new branch, please prefix the branch name with `pr/`. This is to ensure that the intention of the branch is clear and that it is a pull request branch. 
* If you do not prefix the branch name with `pr/`, the CI/CD pipeline will fail and the pull request will not be merged.
* Before submitting a pull request, ensure that the branch is updated with the latest changes from the `main` branch. You can do this by rebasing the branch with the `main` branch. 

## Warnings(Important)
> [!IMPORTANT]
> Please read this section carefully. If you do not follow this, your pull request will not be merged.

* Ensure that there are no new warnings than before. The only reason new warnings may be allowed is if the code that produces the warnings are from third party code that we cannot modify.(Example: Qt, KDE Frameworks, git submodules etc.). If there are new warnings, your pull request will not be merged.

* Do not use print statements. Only logs are allowed.

## Formatting
> [!NOTE] If the code is not formatted correctly, it will be automatically rejected by the CI/CD pipeline.

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
* Classes always start with an upper-case letter. Public classes start with a 'Q' (QRgb) followed by an upper case letter. Public functions most often start with a 'q' (qRgb).
* Acronyms are camel-cased (e.g. QXmlStreamReader, not QXMLStreamReader).


Example:
```cpp
// wrong
KProgressBar *prbar;
QString prtxt, errstr;

// correct
KProgressBar *downloadProgressBar;
QString progressText;
QString errorString;
```