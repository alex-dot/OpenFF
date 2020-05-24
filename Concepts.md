== Dynamic Key assignment ==
* Creates callable functions for all relevant classes
* InputHandler keeps map of callable functions
** maps between configuration setting <- enum -> callable function

== Conditional Key assignment through modular class delegation ==
* Main application creates default/base actions
* Different modules (Menu, World, etc.) re-assign callbacks in InputHandler on activation
