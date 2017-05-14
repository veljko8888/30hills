<html>
	<head>
	<style type="text/css">
	.list{
		width: 350px;
		height: 500px;
		display: block;
		margin: auto;
		overflow-y: scroll;
		overflow-x: hidden;
		padding-left: 30px;
		background: white;
	}
	.klasa{
		float: right;
		padding-right: 10px;
	}
	body{
	margin: 0;
	background: #D8ECF2;
	}
</style>
	</head>
		<body>

		<?php
			include 'connection.php';
			$check = mysqli_query($conn, "SELECT * FROM databasecheck");
			$check = mysqli_fetch_array($check);
			$check = $check[0];
		?>				
		<?php if($check == 0){ ?>
			 <center><a href="putdata.php"><h1>Put data into the database<h1></a></center>
		<?php } else{ ?>
			 <center><h1>Database ready!</h1></center>
		<?php } ?>

		<center><h1>Users:</h1></center>
		<div id="okolo">
			<div class="list">
		<?php
				include 'connection.php';

				$result = mysqli_query($conn, "SELECT * FROM user");
				
				while($user = mysqli_fetch_array($result)){
					$firstname = $user['firstName'];
					$surname = $user['surname'];
					echo "<h3>" . $user['id'] . ". ", $firstname, " ", $surname . "<a class=" . "klasa" . " href='details.php?det=$user[id]'>DETAILS</a>" .  "</h3>";
				}
			?>
			</div>
		</div>
			
		</body>
</html>






