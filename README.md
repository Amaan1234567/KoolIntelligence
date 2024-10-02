# KoolIntelligence
Intelligent ML assistant for KDE

> [!NOTE]
> Make sure to clone the repository with the `--recursive` flag to get all the submodules.
> ```bash
> git clone --recursive https://github.com/Amaan1234567/KoolIntelligence
> ```  

## Building

See [Build instructions](docs/BuildInstructions.md)

## CI/CD pipeline
See [CI/CD pipeline](docs/CIIntegration.md) to understand how the CI/CD pipeline works.

## Coding Guidelines

See [Coding Guidelines](docs/codingGuidelines.md) to understand the coding guidelines.

## Note regarding logging

Logs are print statements that we use. We do not use print statements. Only logs are allowed. 
This is so that we can easily turn off logs when we are done debugging and only have certain logs depending on the severity. This helps in debugging and also in the performance of the application.

There are 5 levels of logs:
1. DEBUG
2. INFO
3. WARNING
4. ERROR
5. CRITICAL

The levels are in increasing order of severity. So, if we set the log level to INFO, then all logs of level INFO and above will be printed. If we set the log level to WARNING, then all logs of level WARNING and above will be printed. And so on.

To set the log level, when configuring cmake, use the appropriate flag. For example, to set the log level to INFO, use the following flag:
```
cmake -DLOG_LEVEL_INFO=ON -B build
```

similarly, for other log levels the flags are:


* DEBUG: -DLOG_LEVEL_DEBUG=ON
* INFO: -DLOG_LEVEL_INFO=ON
* WARNING: -DLOG_LEVEL_WARNING=ON
* ERROR: -DLOG_LEVEL_ERROR=ON
* CRITICAL: -DLOG_LEVEL_CRITICAL=ON

*

