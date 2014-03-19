Background - Announce is IEEE UIUC's newsletter. Previously, we were creating it by manually copying and pasting the entry's information into HTML. To make it easier, I automated the process with PHP and MySQL.

To request an entry into the next Announce, an HTML form was created. Upon submission, it processes the PHP file ("Announce Request Insert.php"), putting it in the MySQL database. - http://ieee.illinois.edu/announce/announce-request/

To automatically make an announce when desired, a PHP file ("Announce Generate.php") can be executed, which finds all the "unread" entries and makes an HTML file that can be emailed. - http://ieee.illinois.edu/announce/announce-generation/
