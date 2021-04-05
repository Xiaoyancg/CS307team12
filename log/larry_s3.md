# Changelog

### Note
- why not use unordered_map<int, xxx>?
  - Because the action in editor is uncertain. RN we need to make a create
    object when user open the editor. But user may insert arbitrary id which may
    conflict with old id and insert fail. To use unordered_map with int, the
    logic in editor should be rewritten. Anyway, since the project is pretty
    small, it's feasible to traverse the vector every time we need.
## For logic
1. change the project to use unordered map with string key
2. better communication between game and editor
3. make better structure on page and sprite
4. add script
5. add logic

### Added



### Changed


### Fixed


