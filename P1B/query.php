<?php
  $dbhost = "localhost";
  $dbuser = "cs143";
  $dbpass = "";
  $dbname = "CS143";
  $db = new mysqli($dbhost, $dbuser, $dbpass, $dbname);

  if($db->connect_errno > 0){
    die('Unable to connect to database [' . $db->connect_error . ']');
	}
?>

<html>
	<head>
		<title>Project 1 Part B</title>
	</head>
	<body>
		<p>Please do not run a complex query here. You may kill the server.</p>
		<p>Type an SQL query in the following box:</p>
		Example: <tt>SELECT * FROM Actor WHERE id=10;</tt><br/>
		<p>
			<form action="query.php" method="GET">
			<textarea name="query" cols="60" rows="8">SELECT * FROM Actor WHERE id < 20</textarea><br/>
			<input type="submit" value="Submit"/>
			</form>
		</p>
		<p>
			<small>Note: tables and fields are case sensitive. All tables in Project 1B are availale.</small>
		</p>

	<?php
		$query  = $_GET["query"];
		$result = $db->query($query);
		if (!$result) {
			die("Database query failed. " . $db->error($connection));
		}
	?>
		<h3>Results from MySQL:</h3>
		<table border=1 cellspacing=1 cellpadding=2>
		<?php
			$title_printed = FALSE;
			while($row = $result->fetch_assoc()) {
				echo "<tr align=center>";
				if(!$title_printed) {
					foreach(array_keys($row) as $name) {
						echo "<td><b>" . $name . "</b></td>";
					}
					echo "</tr>";
					echo "<tr align=center>";
					$title_printed = TRUE;
				}
				foreach($row as $val) {
					echo "<td>" . ($val ? $val : "N/A") . "</td>";
				}
				echo "</tr>";
			}
		?>
		</table>

	</body>
	</html>

	</body>
</html>

<?php
	mysql_free_result($rs);
  mysqli_close($connection);
?>