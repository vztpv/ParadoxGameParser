# ParadoxGameParser
    EU4, DHR, VIC2, etc.. savegame or some data file Parser, written by Modern? C++

# Usage 
    first, Property page -> C/C++ -> Additional include Directories -> Add include folder!
    and check main.cpp! 
 
# Syntax
    program: | list program 

    list: val | var EQ val | var EQ left inner_list right | var EQ LEFT RIGHT | LEFT RIGHT

    inner_list: val inner_list | val 
    | var EQ left inner_list right | var EQ left inner_list right inner_list
    | var EQ LEFT RIGHT | var EQ LEFT RIGHT inner_list
    | var EQ val1 | var EQ val1 inner_list
    | LEFT RIGHT | LEFT RIGHT inner_list
    | left inner_list right | left inner_list right inner_list

    var: string ( ex test, 222, 33.44, "test test" is alllowed! )
    val: string ( ex "test test", test, 222, 33.44 )
    left: LEFT
    right: RIGHT
    EQ : =
    LEFT : {
    RIGHT : }
    
# Notices!
    val  :  it is itemtype, its name is empty("")!
    var = val : it is itemtype, its name is var, value is val.
    var = { } : it is usertype, its name is var.
    {    } : it is usertype, its name is empty("")!
    all var, val is string! if val is integer type, and you want to use val as int then you have to change string to int!
    
# check - ClauText is language(?)( which is based on my ParadoxGameParser, and has more functions? ).
https://github.com/vztpv/ClauText
