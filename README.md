# plasma5-plasmoid-buam

This KDE Plasma 5 widget displays the books one has borrowed from the Adam Mickiewicz University in Poznań
library together with the time left until one has to return them back.

The project consists of two parts:

 * a *plugin* -- written in C++/Qt, whose rôle is to communicate with the website and prepare a list of borrowed books;
 * a *plasmoid* -- a widget written in QML that can be put on the desktop that displays the downloaded list.
 
For most people this will not be very useful *per se* (unless you study or work at AMU),
although this project might be useful if you want to learn how to
write a plasmoid interacting with a module written in C++.

The working plasmoid looks like this:

![Screenshot](buam.png?raw=true "Screenshot")

The numbers on the right are the days left until the book has to be returned.
