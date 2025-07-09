file(REMOVE_RECURSE
  "main"
  "main.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/main.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
