# ParadoxGameParser
EU4, DHR, VIC2, etc.. savegame or some data file Parser, written by Modern? C++

# Usage 
first, Property page -> C/C++ -> Additional include Directories -> Add include folder!
and check main.cpp! 
 
# Syntax
program: | program 

list: val | var EQ val | var EQ left inner_list right | var EQ LEFT RIGHT | LEFT RIGHT

inner_list: val inner_list | val 
| var EQ left inner_list right | var EQ left inner_list right inner_list
| var EQ LEFT RIGHT | var EQ LEFT RIGHT inner_list
| var EQ val1 | var EQ val1 inner_list
| LEFT RIGHT | LEFT RIGHT inner_list
| left inner_list right | left inner_list right inner_list

var: string ( ex test, 222, 33.44, but "test test" is not alllowed! )
val: string ( ex "test test", test, 222, 33.44 )
left: LEFT
right: RIGHT



# check - ClauText is language(?)( which is based on my ParadoxGameParser, and has more functions? ).
https://github.com/vztpv/ClauText
