$server = mysql_connect("127.0.0.1","username", "password");

// Check connection
if (!$server)
	echo "Failed to connect to MySQL: " . mysql_connect_error();

// Main content
$query="SELECT * FROM eceieee_skillset.announce_request WHERE Added=0";

if (! $data = mysql_query($query))
{
	die('Error: ' . mysql_error($server));
}

if (mysql_num_rows($data) == 0)
{
	die("No unread entries to add to a new Announce.");
}

// Set the html output up
$today_date = date('F j, Y');
$announce_file = fopen("./announce/" . $today_date . ".html", "w");
$announce_start = file_get_contents("./wordpress/wp-content/uploads/2013/11/announce_start.html");
$announce_start = str_replace("insert_date", $today_date, $announce_start);
fwrite($announce_file, $announce_start);
fwrite($announce_file, "\n\n");
	
while($info = mysql_fetch_array($data)) 
{
	// Reset the out variables
	$Name = "<p align=\"left\" class=\"article-title\" style=\"color: red\">insert_name</p><div align=\"left\" class=\"article-content\">";
	$Image = "<img style=\"float: right;width: 200px; padding: 10px;\" src=\"insert_image\" alt=\"\" />";
	$Date_Time = "<b>Date & Time: </b>insert_date_time<br />";
	$Location = "<b>Location: </b>insert_location<br />";
	$Contact = "<b>Contact: </b>insert_contact<br />";
	$Registration = "<b>Registration:</b> <a href=\"insert_registration\" target=\"_blank\">Link Provided</a></br />";
	$Deadline = "<b>Deadline: </b>insert_deadline<br />";
	$Facebook = "<a target=\"_blank\" href=\"insert_facebook\"><img src=\"http://www.nwpclawestside.org/images/FacebookRSVP.jpg\" alt=\"Facebook Event\" /></a>";
	$Description = "<p>insert_description</p>";

	// Replace html parts, write to file only if you should and wr
	if (!empty($info['Name']))
	{
		$Name = str_replace("insert_name", $info['Name'], $Name);
		fwrite($announce_file, $Name);
		fwrite($announce_file, "\n");
	}
		
	if (!empty($info['Image']))
	{
		$Image = str_replace("insert_image", $info['Image'], $Image);
		fwrite($announce_file, $Image);
		fwrite($announce_file, "\n");
	}

	if (!empty($info['Date_Time']))
	{
		$Date_Time = str_replace("insert_date_time", $info['Date_Time'], $Date_Time);
		fwrite($announce_file, $Date_Time);
		fwrite($announce_file, "\n");
	}
		
	if (!empty($info['Location']))
	{
		$Location = str_replace("insert_location", $info['Location'], $Location);
		fwrite($announce_file, $Location);
		fwrite($announce_file, "\n");
	}
		
	if (!empty($info['Contact']))
	{
		$Contact = str_replace("insert_contact", $info['Contact'], $Contact);
		fwrite($announce_file, $Contact);
		fwrite($announce_file, "\n");
	}
	
	if (!empty($info['Registration']))
	{
		$Registration = str_replace("insert_deadline", $info['Registration'], $Registration);
		fwrite($announce_file, $Registration);
		fwrite($announce_file, "\n");
	}

	if (!empty($info['Deadline']))
	{
		$Deadline = str_replace("insert_deadline", $info['Deadline'], $Deadline);
		fwrite($announce_file, $Deadline);
		fwrite($announce_file, "\n");
	}
		
	if (!empty($info['Facebook']))
	{
		$Facebook = str_replace("insert_facebook", $info['Facebook'], $Facebook);
		fwrite($announce_file, $Facebook);
		fwrite($announce_file, "\n");
	}
		
	if (!empty($info['Description']))
	{
		$Description = str_replace("insert_description", $info['Description'], $Description);
		fwrite($announce_file, nl2br($Description));
		fwrite($announce_file, "\n");
	}

	fwrite($announce_file, "<br>\n<hr>\n\n");

	// Update the Added variable
	$temp = $info['Name'];
	$set_announce = "UPDATE eceieee_skillset.announce_request SET Added=1 WHERE Added=0 AND Name='$temp'";
	mysql_query($set_announce);
}

// Finish it and close
$announce_end = file_get_contents("./wordpress/wp-content/uploads/2013/11/announce_end.html");
fwrite($announce_file, $announce_end);
fclose($announce_file);

mysql_close($server);

echo "<a href=\"../$today_date.html\" target=\"_blank\">Click Me!</a>";