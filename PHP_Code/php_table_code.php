<!DOCTYPE>
<html>
<head>
	<meta charset = "UTF-8">
	<meta http-equiv="refresh" content = "30">
	<style type = "text/css">
		.spec{
			text-align:center;
		}
		.con{
			text-align:left;
		}
		</style>
</head>
 
<body>
	<hl align = "center">Cafe Manager Table</hl>
	<div class = "spec">
		# <b>Cafe Ingredient</b>
		<br>
		<br> 
	</div>

	<table border = '1' style = "width = 40%" align = "center"> 
	<tr align = "center">
		<th>AREA</th>
		<th>BEAN</th>
		<th>MILK</th>
		<th>CHOCO</th>
		<th>TEA</th>
		<th>ICE</th>
		<th>SYRUP</th>
	</tr>
	<?php
		$conn = mysqli_connect("localhost","root","kcci");
		mysqli_select_db($conn, "Cafe");
		$result = mysqli_query($conn, "select * from menu");

		while($row = mysqli_fetch_array($result)){
		echo "<tr align = center>";
		echo '<td>'.$row['AREA'].'</td>';
		echo '<td>'.$row['BEAN'].'</td>';
		echo '<td>'.$row['MILK'].'</td>';
		echo '<td>'.$row['CHOCO'].'</td>';
		echo '<td>'.$row['TEA'].'</td>';
		echo '<td>'.$row['ICE'].'</td>';
		echo '<td>'.$row['SYRUP'].'</td>';
		echo "</tr>";
		mysqli_close($conn);
		}
	?>
	</table>
</body>
</html>
