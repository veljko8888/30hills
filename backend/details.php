<html>
	<head>
	</head>
		<body>
			<center><h1>Details<h1></center>
			<center>
			
			<?php

				include 'connection.php';
			 
				if( isset($_GET['det']) ){
					// DIRECT FRIENDS
					$id = $_GET['det'];
					$result = mysqli_fetch_array(mysqli_query($conn, "SELECT * FROM user WHERE `id` = '$id'"));
					$firstname = $result['firstName'];
					$surname = $result['surname'];

					echo "<h2> Direct friends of " . $firstname . " " . $surname . " are: </h3>";
					
					$result2 = mysqli_query($conn, "SELECT * FROM friends WHERE `user1id` = '$id' OR `user2id` = '$id'");
					while($dfriends = mysqli_fetch_array($result2)){
						if($dfriends['user1id'] == $id){
							$frid = $dfriends['user2id'];
							$result3 = mysqli_fetch_array(mysqli_query($conn, "SELECT * FROM user WHERE `id` = '$frid'"));
							$fname = $result3['firstName'];
							$sname = $result3['surname'];
						    echo "(ID "; echo $frid; echo ") "; echo $fname; 
							echo " "; echo $sname; echo "<br>";
						}
						else {
							$frid = $dfriends['user1id'];
							$result3 = mysqli_fetch_array(mysqli_query($conn, "SELECT * FROM user WHERE `id` = '$frid'"));
							$fname = $result3['firstName'];
							$sname = $result3['surname'];
							echo "(ID "; echo $frid; echo ") "; echo $fname; 
							echo " "; echo $sname; echo "<br>";
						}
					}
					
					echo "<h2> Friends of " . $firstname . " " . $surname . "'s friends are: </h3>";
					
					//FRIENDS OF FRIENDS
					$i = 0;
					$v = 0;
					$result4 = mysqli_query($conn, "SELECT * FROM friends WHERE `user1id` = '$id' OR `user2id` = '$id'");
					while($dirf = mysqli_fetch_array($result4)){
						if($dirf['user1id'] == $id) $dirfx[$i] = $dirf['user2id'];
						else $dirfx[$i] = $dirf['user1id'];
						//echo $dirfx[$i]; echo " ";
						$i++;					
					}
					//echo "<br><br>"; 
					for($k = 0; $k < $i; $k++){
						$m = 0;
						$result5 = mysqli_query($conn, "SELECT * FROM friends WHERE `user1id` = '$dirfx[$k]' OR `user2id` = '$dirfx[$k]'");
						while($dirf = mysqli_fetch_array($result5)){
							if($dirf['user1id'] == $dirfx[$k]) $dirfxy[$m] = $dirf['user2id'];
							else $dirfxy[$m] = $dirf['user1id'];
							//echo $dirfxy[$m]; echo " ";
							$m++;					
						}
						for($q = 0; $q < $m; $q++){
							$ima = false;
							for($w = 0; $w < $i; $w++){
								if($dirfxy[$q] == $dirfx[$w]) $ima = true;
							}
							if($ima == false){ 
								$konacan[$v] = $dirfxy[$q];
								$v++;
							}
						}
						//echo "<br>";
					}

					//echo "<br><br>";
					$konacan = array_unique($konacan);
					$s = count($konacan);
					for($u = 0; $u < $v; $u++){
							if(isset($konacan[$u])){
								$result = mysqli_fetch_array(mysqli_query($conn, "SELECT * FROM user WHERE `id` = '$konacan[$u]'"));
								$firstname1 = $result['firstName'];
								$surname1 = $result['surname'];
								echo "(ID "; echo $konacan[$u]; echo ") ";
								echo $firstname1; echo " "; echo $surname1;
								echo "<br>";
						}
					}
					
					echo "<h2> Suggested friends for " . $firstname . " " . $surname . " are: </h3>";
					
					// SUGGESTED FRIENDS			
					$result6 = mysqli_query($conn, "SELECT * FROM user WHERE `id` != '$id'");
					while($dirf = mysqli_fetch_array($result6)){
						$z = 0;
						$p = 0;
						$id1 = $dirf['id'];
						$result7 = mysqli_query($conn, "SELECT * FROM friends WHERE `user1id` = '$id1' OR `user2id` = '$id1'");
						while($dirfr = mysqli_fetch_array($result7)){
							if($dirfr['user1id'] == $id1) $dirfxx[$p] = $dirfr['user2id'];
							else $dirfxx[$p] = $dirfr['user1id'];
							//echo $dirfxx[$p]; echo " ";
							$p++;	
						} //echo "<br>";
						$ok = true;
						for($b = 0; $b < $p; $b++){
							if($dirfxx[$b] == $id1) $ok = false;
						} 
						if($ok == true){
							for($t = 0; $t < $p; $t++){
								for($y = 0; $y < $i; $y++){
									if($dirfxx[$t] == $dirfx[$y]) $z++;
								} 
							}							
						}
						if($z > 1){
								$resultset = mysqli_fetch_array(mysqli_query($conn, "SELECT * FROM user WHERE `id` = '$id1'"));
								echo "(ID "; echo $id1; echo ") "; echo $resultset['firstName']; 
								echo " "; echo $resultset['surname']; echo "<br>";
							}
					}					
				}
?>
			
			</center>
		</body>
</html>






