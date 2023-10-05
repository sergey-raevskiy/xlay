# xlay
This repository contains some code that can read Sprint Layout (xlay) files.

# LAY file specific data types
It seems that Sprint Layout is written in Delphi. This may be the reason that LAY files use some uncommon data types:

* `fixstr`: Length prefixed fixed size ANSI strings
* `varstr`: Length prefixed variable size ANSI strings
* `u8bool`: Short (one byte) boolean type
