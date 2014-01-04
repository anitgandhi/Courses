// if software is a "yes", return 2 3 and 4
function insert_sw($value, &$q)
{
	//global $query;
	if ($_POST[$value] == 2)
		$q .= " AND ($value=2 OR $value=3 OR $value=4)";
}

function insert_hw($value, &$q)
{
	//global $query;	
	if (!empty($_POST[$value]))
		$q .= " AND ($value=" . $_POST[$value] . ")";
}

function changeMajor($value)
{
	$search = array("EE", "CE", "AE", "MSE", "GE", "ME", "CEE", "BE", "ABE", "EM", "MATH", "CS", "PHYSICS", "OTHER ENG", "OTHER");
	$replace = array("Electrical Engineering", "Computer Engineering", "Aerospace Engineering", "Materials Science and Engineering", "General Engineering",
					 "Mechanical Engineering", "Civil Engineering", "Bioengineering", "Agricultural and Biological Engineering", "Engineering Mechanics",
					 "Math", "Computer Science", "Physics", "Other Engineering", "Other");
	
	return str_replace($search, $replace, $value);
}

//Only do the rest if user input was blank or you went straight to script
$temp = array_filter($_POST);
if (empty($temp))
    die('Error: No Data to Retrieve.');

$server = mysql_connect("127.0.0.1","username", "password");
// Check connection
if (!server)
{
	echo "Failed to connect to MySQL: " . mysql_connect_error();
}

$query = "SELECT * FROM eceieee_skillset.skillset_ieee WHERE major ";

if (!empty($_POST['major']))
	$query .= "='" . $_POST['major'] . "'";
else
	$query .= "LIKE '%'";

insert_hw('hw_beg_1', $query);
insert_hw('hw_beg_2', $query);
insert_hw('hw_beg_3', $query);
insert_hw('hw_beg_4', $query);

insert_hw('hw_int_1', $query);
insert_hw('hw_int_2', $query);
insert_hw('hw_int_3', $query);
insert_hw('hw_int_4', $query);
insert_hw('hw_int_5', $query);

insert_hw('hw_adv_1', $query);
insert_hw('hw_adv_2', $query);
insert_hw('hw_adv_3', $query);
insert_hw('hw_adv_4', $query);
insert_hw('hw_adv_5', $query);
insert_hw('hw_adv_6', $query);
insert_hw('hw_adv_7', $query);
insert_hw('hw_adv_8', $query);

insert_sw('sw_html', $query);
insert_sw('sw_sql', $query);
insert_sw('sw_js', $query);
insert_sw('sw_php', $query);
insert_sw('sw_c', $query);
insert_sw('sw_java', $query);
insert_sw('sw_ruby', $query);
insert_sw('sw_x86', $query);
insert_sw('sw_ios', $query);
insert_sw('sw_android', $query);
insert_sw('sw_matlab', $query);
insert_sw('sw_csharp', $query);
insert_sw('sw_django', $query);
insert_sw('sw_python', $query);
insert_sw('sw_perl', $query);

insert_sw('sw_opencv', $query);
insert_sw('sw_opengl', $query);
insert_sw('sw_opencl', $query);
insert_sw('sw_cuda', $query);
insert_sw('sw_osdev', $query);
insert_sw('sw_ds', $query);
insert_sw('sw_es', $query);
insert_sw('sw_sec', $query);

// Run query
if (! $data = mysql_query($query))
{
	die('Error: ' . mysql_error($server));
}

// If no results found
if (mysql_num_rows($data)==0)
	echo "<h4>No results found.</h4>";

// output all the data from each entry in query results
while($row = mysql_fetch_array($data)) 
{
	// User information
	echo "<h4>Name: " . $row['name'] . "</h4>";
	echo "<h4>Major: " . changeMajor($row['major']) . "</h4>";
	echo "<h4>Email: " . $row['email'] . "</h4>";
	
	// Hardware information
	echo "<h3>Hardware Skills</h3>";
	
	// Remove single quotes at beginning and end, and format 
	//nicely to make sure empty entries don't come up
	
	/*$hw_beg = implode(", ", array_filter(explode(",", $row['hw_beg'])));
	$hw_int = implode(", ", array_filter(explode(",", $row['hw_int'])));
	$hw_adv = implode(", ", array_filter(explode(",", $row['hw_adv'])));*/
		
	echo "Beginner: " . implode(", ", array_filter(explode(",", $row['hw_beg']))) . "<br/>";
	echo "Intermediate: " . implode(", ", array_filter(explode(",", $row['hw_int']))) . "<br/>";
	echo "Advanced: " . implode(", ", array_filter(explode(",", $row['hw_adv']))) . "<br/>";
	echo "Other: " . implode(", ", array_filter(explode(",", $row['hw_other']))) . "<br/>";
	
	// Software information
	echo "<h3>Software Skills</h3>";
	
	// Sort languages by expertise level
	// Get 3 arrays for each level containing the languages they know at that level

	$sw_beg_arr = array();
	$sw_int_arr = array();
	$sw_adv_arr = array();
	$sw_con_arr = array();
	
	$languages = array('sw_html', 'sw_sql', 'sw_js', 'sw_php', 'sw_c', 'sw_java', 'sw_ruby', 'sw_x86', 'sw_ios', 'sw_android', 'sw_matlab', 'sw_csharp', 'sw_django', 						'sw_python', 'sw_perl');
	$languagesFull = array('HTML/CSS', 'SQL', 'JavaScript', 'PHP', 'C/C++', 'Java', 'Ruby', 'x86', 'iOS', 'Android', 'MATLAB', 'C#', 'Django', 'Python', 'Perl');
	
	$concepts = array('sw_opencv', 'sw_opengl', 'sw_opencl', 'sw_cuda', 'sw_osdev', 'sw_ds', 'sw_es', 'sw_sec');
	$conceptsFull = array('OpenCV', 'OpenGL', 'OpenCL', 'CUDA', 'Operating System Development', 'Data Structures', 'Embedded Systems', 'Computer Security');
	
	foreach($row as $key=>$value)
	{
		// if key starts with "sw_"
		if ( (strpos($key,'sw_') !== false) )
		{
			// If it's a language
			if (in_array($key, $languages))
			{
				if ($value == 2)
					array_push($sw_beg_arr, $key);
				if ($value == 3)
					array_push($sw_int_arr, $key);
				if ($value == 4)
					array_push($sw_adv_arr, $key);
			}
			
			// If it's a concept
			if (in_array($key, $concepts))
				if ($value == 2)
					array_push($sw_con_arr, $key);
		}
		
	}
	
	// Implode arrays to strings
	$sw_beg_str = implode(", ", $sw_beg_arr);
	$sw_int_str = implode(", ", $sw_int_arr);
	$sw_adv_str = implode(", ", $sw_adv_arr);
	
	$sw_con_str = implode(", ", $sw_con_arr);
	
	// Put in proper names
	$sw_beg_str = str_replace($languages, $languagesFull, $sw_beg_str);
	$sw_int_str = str_replace($languages, $languagesFull, $sw_int_str);
	$sw_adv_str = str_replace($languages, $languagesFull, $sw_adv_str);
	
	$sw_con_str = str_replace($concepts, $conceptsFull, $sw_con_str);
	
	// Languages
	echo "Beginner: " . $sw_beg_str . "<br/>";
	echo "Intermediate: " . $sw_int_str . "<br/>";
	echo "Advanced: " . $sw_adv_str . "<br/>";
	echo "Concepts: " . $sw_con_str . "<br/>";
	echo "Other: " . implode(", ", array_filter(explode(",", $row['sw_other'])));
	
	// Concepts
	//, );
	
	echo "<hr>";
}

mysql_close($server);