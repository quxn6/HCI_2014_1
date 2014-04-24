var fs = require('fs');

var table = new Object();

function start(response, postData) {
  console.log("Request handler 'start' was called.");
  
 	var body = 
		'<html>'+
		'<head>'+
		'<script type="text/javascript" src="https://www.google.com/jsapi"></script>'+
		'<script type="text/javascript">'+
		"google.load('visualization', '1.0', {'packages':['corechart']});"+
		'google.setOnLoadCallback(drawChart);'+
		'function drawChart() {'+
		'var data = new google.visualization.DataTable();'+
		"data.addColumn('string', 'Time');"+
		"data.addColumn('number', 'Vibration');"+
		"data.addColumn('number', 'Loudness');"        +
		"var table = new Object();"+
		"table = " +JSON.stringify(table)+";"+
		"for ( var key in table ) {"+
		"data.addRows([[key, table[key][0], table[key][1]]]);"+
		"}"+
		"var options = {'title':'Estimate Noise','width':1000,'height':800};"        +
		"var chart = new google.visualization.AreaChart(document.getElementById('chart_div'));"+
		"chart.draw(data, options);"+
		"}"+
		"</script>"+
		"</head>"+
		"<body>"+
		"<div id='chart_div'></div>"+
		"</body>"+
		"</html>";

    response.writeHead(200, {"Content-Type": "text/html"});
    response.write(body);
    response.end();
}

function upload(response, postData) {
	console.log("Request handler 'upload' was called.");
	var today=new Date();
	var h=today.getHours();
	var m=today.getMinutes();
	var s=today.getSeconds();
	// add a zero in front of numbers<10
	var ctime = h+":"+m+":"+s;

	if ( Object.keys(table).length > 100 ) {
		delete table[Object.keys(table)[0]];
	}
    table[ctime] = JSON.parse(postData);
    console.log(table);
}

exports.start = start;
exports.upload = upload;