echo getcwd();
echo "<ul>";
foreach(glob("announce/*html") as $file)
{
	$text = str_replace("announce/", "", $file);
	$text = str_replace(".html", "", $text);
	echo "<li><a href=\"/$file\" target=\"_blank\">$text</a></li>\n";
}

echo "\n</ul>";