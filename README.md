# KoolIntelligence
Intelligent ML assistant for KDE

## Building

See [Build instructions](docs/BuildInstructions.md)

## Formatting

Install clang-format and then from the root directory of project in linux, run

```bash
SOURCE_DIR="src" && find "$SOURCE_DIR" -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.tpp" \) | while read -r FILENAME; do clang-format -i "$FILENAME"; done
```