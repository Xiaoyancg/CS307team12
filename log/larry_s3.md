# Changelog
## TODO
- Comment in core 
### Note
- why not use unordered_map<int, xxx>?
  - Because the action in editor is uncertain. RN we need to make a create
    object when user open the editor. But user may insert arbitrary id which may
    conflict with old id and insert fail. To use unordered_map with int, the
    logic in editor should be rewritten. Anyway, since the project is pretty
    small, it's feasible to traverse the vector every time we need.
## For logic
- Signal 
  - signal Type
  - System defined signal 
    - key signal 
      - constructor
  - user defined signal 
    - custom signal
      - constructor
  - Signal Union
    - operator =
    - constructors
      - self assign constructor
      - default constructor
      - constructors for different type
  - Signal class
    - parse
    - serialize
    - constructors 
    - setter and getter
    - update

- Script  
  - Signal Type 
- Logic class 
  - 
- Logic Manager class 
  - Logic Manager instance in Game 
    - pass currPage and pageList to Logic manager instance 
- id in entity and page 
### Added



### Changed


### Fixed


