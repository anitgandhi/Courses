//Only do the rest if the user information inputs weren't blank
$temp = array_filter($_POST);
if (empty($temp))
    die('Error: No Data to Insert.');

$server = mysql_connect("127.0.0.1","username", "password");

// Check connection
if (!$server)
	echo "Failed to connect to MySQL: " . mysql_connect_error();
 
$User_Name = $_POST['User_Name'];
$NetID = $_POST['User_NetID'];
 
$Contact = $_POST['User_Name'] . " (" . $_POST['Contact'] . ")";
$Name = $_POST['Name'];
$Date_Time = $_POST['Date_Time'];
$Location = $_POST['Location'];
$Description = $_POST['Description'];
$Flyer = $_POST['Flyer'];

$Registration = $_POST['Registration'];
$Deadline = $_POST['Deadline'];
 
$Image = $_POST['Image'];
$Facebook = $_POST['Facebook'];
 
$columns = "User_Name, NetID, Contact, Name, Date_Time, Location, Description, Flyer, Registration, Deadline, Image, Facebook";
 
$values = "\"$User_Name\", \"$NetID\", \"$Contact\", \"$Name\", \"$Date_Time\", \"$Location\", \"$Description\", \"$Flyer\", \"$Registration\", \"$Deadline\", \"$Image\", \"$Facebook\"";
 
$query = "INSERT INTO eceieee_skillset.announce_request (" . $columns . ") " . "VALUES (" . $values . ")";
 
if (!mysql_query($query))
{
	die('Error: ' . mysql_error($server));
}
 
if (strcmp($Flyer, "Create") == 0)
{
	$mail = "User Name: $User_Name\n
			User NetID: $NetID\n
			Event Contact: $Contact\n
			Event Name: $Name\n
			Event Date and Time: $Date_Time\n
			Event Location: $Location\n
			Event Description: $Description\n
			Registration Link: $Registration\n
			Registration Deadline: $Deadline\n
			Image URL: $Image\n
			Facebook Event URL: $Facebook\n";
			
	if (!mail("publicity@ieee.illinois.edu", "Announce Request Submitted: " . $Name, $mail))
		echo "Email failed.";
}
 
echo "Announce Request Entry Submitted";
 
mysql_close($server);