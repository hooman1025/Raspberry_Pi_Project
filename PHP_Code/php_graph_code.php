<?php
$conn = mysqli_connect("localhost","root","kcci");
mysqli_set_charset($conn, "utf8");
mysqli_select_db($conn, "Cafe");
$result = mysqli_query($conn, "select AREA, BEAN, MILK, CHOCO, TEA, ICE, SYRUP from menu");
$data = array(array('adsasde', '원두(단위: g)', '우유(단위: mL)', '초코(단위: g)', '허브(단위: g)', '얼음(단위: g)', '시럽(단위: mL)'));
if($result){
	while($row = mysqli_fetch_array($result)){
		array_push($data, array($row['AREA'], intval($row[1]), intval($row[2]), intval($row[3]), intval($row[4]), intval($row[5]), intval($row[6])));
	}
}
$options = array(
	'title' => 'Cafe Ingredient',
	'width' => 700, 'height' => 500
);
?>
 
<script src="//www.google.com/jsapi"></script>
<script>
    let data = <?= json_encode($data) ?>;
    let options = <?= json_encode($options) ?>;
    google.load('visualization', '1.0', {'packages':['corechart']});
    google.setOnLoadCallback(function() {
        let chart = new google.visualization.ColumnChart(document.querySelector('#chart_div'));
        chart.draw(google.visualization.arrayToDataTable(data), options);
    });
</script>
<div id="chart_div"></div>
