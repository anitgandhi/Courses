Code used for IEEE Website. My work includes the PHP code, and some of the HTML.

Skillset Database
=================

Background - Employers and fellow students wanted a way to find students with certain skills. This skillset database allows for people to put in their information, and for others to search for it.

Information form for UIUC students to put in their information. Coded in HTML - http://ieee.illinois.edu/skillset-database/
The submit button links to the insert page, coded in PHP ("Skillset Database Insert.php") to insert into the MySQL database - http://ieee.illinois.edu/skillset-insert/

The retrieve form allows people to search for other people by querying the database, coded in HTML - http://ieee.illinois.edu/skillset-search/
This goes to the results page, which is coded in PHP ("Skillset Database Retrieve.php") - http://ieee.illinois.edu/skillset-results/

Announce
========

Background - Announce is IEEE UIUC's newsletter. Previously, we were creating it by manually copying and pasting the entry's information into HTML. To make it easier, I automated the process with PHP and MySQL.

To request an entry into the next Announce, an HTML form was created. Upon submission, it processes the PHP file ("Announce Request Insert.php"), putting it in the MySQL database. - http://ieee.illinois.edu/announce/announce-request/

To automatically make an announce when desired, a PHP file ("Announce Generate.php") can be executed, which finds all the "unread" entries and makes an HTML file that can be emailed. - http://ieee.illinois.edu/announce/announce-generation/
