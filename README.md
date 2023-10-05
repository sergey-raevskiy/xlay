# xlay
This repository contains some code that can read Sprint Layout (xlay) files.

# Overall LAY file structure
![image](https://github.com/sergey-raevskiy/xlay/assets/3956978/de532087-fa8e-4269-94bb-e229caaa2117)

# Overall board record structure
![image](https://github.com/sergey-raevskiy/xlay/assets/3956978/79cb89f3-ea6c-41bd-879c-d550b70e9ec2)

# LAY file specific data types
It seems that Sprint Layout is written in Delphi. This may be the reason that LAY files use some uncommon data types:

* `fixstr`: Length prefixed fixed size ANSI strings
* `varstr`: Length prefixed variable size ANSI strings
* `u8bool`: Short (one byte) boolean type
