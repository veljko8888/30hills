<?php

	include 'connection.php';

	$json_string = 'http://dev.30hills.com/data.json';
	$jsondata = file_get_contents($json_string);
	$obj = json_decode($jsondata, false);
	
	$elementCount  = count($obj);
	$special_char = "'";
	
	for ($x = 0; $x < $elementCount; $x++) {
		
		// prepare and bind
		// $stmt = $conn->prepare("INSERT INTO user (`id`, `firstName`, `surname`, `age`, `gender`) 
					// VALUES(?,?,?,?,?)");
		// $stmt->bind_param("issis", $id, $firstname, $surname, $age, $gender);

		// set parameters and execute
		$id = $obj[$x]->id;
		$firstname = $obj[$x]->firstName;		
		$surname = $obj[$x]->surname;
		$age = $obj[$x]->age;
		$gender = $obj[$x]->gender;
		//$stmt->execute();
		
		if (preg_match('/'.$special_char.'/', $surname)){ 
			$surname = str_replace("'","''",$surname);
		}
		if (preg_match('/'.$special_char.'/', $firstname)){ 
			$surname = str_replace("'","''",$firstname);
		}
		
		if($age == null)
		mysqli_query($conn, "INSERT INTO user (`id`, `firstName`, `surname`, `age`, `gender`) 
					VALUES($id, '$firstname', '$surname', NULL, '$gender')") 
					or die(mysqli_error($conn));
		else 
			mysqli_query($conn, "INSERT INTO user (`id`, `firstName`, `surname`, `age`, `gender`) 
					VALUES($id, '$firstname', '$surname', $age, '$gender')") 
					or die(mysqli_error($conn));
	}
	
	for ($x = 0; $x < $elementCount; $x++) {
		$id = $obj[$x]->id;
		$friendsnum = count($obj[$x]->friends);
		for($i = 0; $i < $friendsnum; $i++){
			echo $user2 = $obj[$x]->friends[$i];
			echo " ";
			if($id < $user2) 
				mysqli_query($conn, "INSERT INTO friends (`user1id`, `user2id`) 
					VALUES($id, $user2)")
					or die(mysqli_error($conn));
		}
		echo "<br>";
	}
		
	mysqli_query($conn, "INSERT INTO databasecheck (`dbfull`) VALUES(1)") or die(mysqli_error($conn));

	header("Location: http://localhost/30hills/home.php");
	exit();

	// $stmt->close();
	// $conn->close();
?>