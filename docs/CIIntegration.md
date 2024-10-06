# How our CI/CD pipeline works

1. Ensure the branch name starts with `pr/`.
2. Verify the code is correctly formatted.
3. Build the code to check for any errors.

If any of the above steps fail, the pipeline will fail, and the pull request will not be merged even if you get approval.

# What to do if the pipeline fails due to a dependency issue?

If the pipeline fails due to a dependency issue, you can follow the below steps to fix it:
1) Check the error message to identify the missing dependency.
2) Update the ```.github/workflows/buildAndTest.yml``` file to include the missing dependency.
3) Depending on the type of dependency, follow the headings below to add the dependency.

### Adding a Qt6 module

Generally if the dependency is a qt module, you can add it to the ```Install Qt``` step. You can find the name of the qt module from this [link](https://ddalcino.github.io/aqt-list-server/).

### Adding a KDE Framework

If the dependency is a KDE Framework, you need to find the github repo for the KDE Framework and add a new step similar to the other steps where you compile the KDE Framework and install it.

### Other dependencies

Generally, this means that the dependency should be in apt. You can add the dependency to the ```Install Dependencies``` step in the apt install command.
