function check_input($value)
{
    // Stripslashes
    if (get_magic_quotes_gpc())
        $value = stripslashes($value);
    // Quote if not a number
    //if (!is_numeric($value))
    //    $value = "'" . mysql_real_escape_string($value) . "'";
    return $value;
}

//Only do the rest if the user information inputs weren't blank
$temp = array_filter($_POST);
if (empty($temp))
    die('Error: No Data to Insert.');

$server = mysql_connect("127.0.0.1","username", "password");
// Check connection
if (!server)
{
	echo "Failed to connect to MySQL: " . mysql_connect_error();
}

// Set up variables from POST data
// User's information
$user_name = $_POST['user_name'];
$email = $_POST['email'];
$uin = $_POST['uin'];
$netID = $_POST['netID'];
$major = $_POST['major'];
$member_id = $_POST['member_id'];

// Hardware skills
$hw_beg = 
$_POST['hw_beg_1'] . "," . 
$_POST['hw_beg_2'] . "," . 
$_POST['hw_beg_3'] . "," . 
$_POST['hw_beg_4'];
$hw_int = 
$_POST['hw_int_1'] . "," . 
$_POST['hw_int_2'] . "," . 
$_POST['hw_int_3'] . "," . 
$_POST['hw_int_4'] . "," .
$_POST['hw_int_5'];
$hw_adv = 
$_POST['hw_adv_1'] . "," . 
$_POST['hw_adv_2'] . "," . 
$_POST['hw_adv_3'] . "," . 
$_POST['hw_adv_4'] . "," .
$_POST['hw_adv_5'] . "," .
$_POST['hw_adv_6'] . "," .
$_POST['hw_adv_7'] . "," .
$_POST['hw_adv_8'];
$hw_other = nl2br($_POST['hw_other']);

// Software skills
$sw_html = $_POST['sw_html'];
$sw_sql = $_POST['sw_sql'];
$sw_js = $_POST['sw_js'];
$sw_php = $_POST['sw_php'];
$sw_c = $_POST['sw_c'];
$sw_java = $_POST['sw_java'];
$sw_ruby = $_POST['sw_ruby'];
$sw_x86 = $_POST['sw_x86'];
$sw_ios = $_POST['sw_ios'];
$sw_android = $_POST['sw_android'];
$sw_matlab = $_POST['sw_matlab'];
$sw_csharp = $_POST['sw_csharp'];
$sw_django = $_POST['sw_django'];
$sw_python= $_POST['sw_python'];
$sw_perl = $_POST['sw_perl'];
$sw_opencv = $_POST['sw_opencv'];
$sw_opengl = $_POST['sw_opengl'];
$sw_opencl = $_POST['sw_opencl'];
$sw_cuda = $_POST['sw_cuda'];
$sw_osdev = $_POST['sw_osdev'];
$sw_ds = $_POST['sw_ds'];
$sw_es = $_POST['sw_es'];
$sw_sec = $_POST['sw_sec'];
$sw_other = nl2br($_POST['sw_other']);

// Clean inputs
$user_name = check_input($user_name);
$email = check_input($email);
$uin = check_input($uin);
$netid = check_input($netID);
$major = check_input($major);
$member_id = check_input($member_id);

$hw_beg = check_input($hw_beg);
$hw_int = check_input($hw_int);
$hw_adv = check_input($hw_adv);
$hw_other = check_input($hw_other);

$sw_html = check_input($sw_html);
$sw_sql = check_input($sw_sql);
$sw_js = check_input($sw_js);
$sw_php = check_input($sw_php);
$sw_c = check_input($sw_c);
$sw_java = check_input($sw_java);
$sw_ruby = check_input($sw_ruby);
$sw_x86 = check_input($sw_x86);
$sw_ios = check_input($sw_ios);
$sw_android = check_input($sw_android);
$sw_matlab = check_input($sw_matlab);
$sw_csharp = check_input($sw_csharp);
$sw_django = check_input($sw_django);
$sw_python= check_input($sw_python);
$sw_perl = check_input($sw_perl);
$sw_opencv = check_input($sw_opencv);
$sw_opengl = check_input($sw_opengl);
$sw_opencl = check_input($sw_opencl);
$sw_cuda = check_input($sw_cuda);
$sw_osdev = check_input($sw_osdev);
$sw_ds = check_input($sw_ds);
$sw_es = check_input($sw_es);
$sw_sec = check_input($sw_sec);
$sw_other = check_input($sw_other);

// Main insert
$columns = "name, email, uin, netid, major, member_id, hw_beg, hw_int, hw_adv, hw_other, sw_html, sw_sql, sw_js, sw_php, sw_c, sw_java, sw_ruby, sw_x86, sw_ios, sw_android, sw_matlab, sw_csharp, sw_django, sw_python, sw_perl, sw_opencv, sw_opengl, sw_opencl, sw_cuda, sw_osdev, sw_ds, sw_es, sw_sec, sw_other";

$values = "'$user_name', '$email', '$uin', '$netid', '$major', '$member_id', '$hw_beg', '$hw_int', '$hw_adv', '$hw_other', '$sw_html', '$sw_sql', '$sw_js', '$sw_php', '$sw_c', '$sw_java', '$sw_ruby', '$sw_x86', '$sw_ios', '$sw_android', '$sw_matlab', '$sw_csharp', '$sw_django', '$sw_python', '$sw_perl', '$sw_opencv', '$sw_opengl', '$sw_opencl', '$sw_cuda', '$sw_osdev', '$sw_ds', '$sw_es', '$sw_sec', '$sw_other'";

$query = "INSERT INTO eceieee_skillset.skillset_ieee  (" . $columns . ") " . "VALUES (" . $values . ")";

if (!mysql_query($query))
	die('Error: ' . mysql_error($server));

mysql_close($server);

echo "<h3>Your submission has been recorded. Thank you!</h3>";