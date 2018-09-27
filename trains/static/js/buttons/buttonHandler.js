$(document).ready(function() {

	$('input:radio').change(function(){
		console.log("Radio Button clicked detected. Id;" + this.id);
	});
	
		$('button').click(function(){
		console.log("Button clicked detected. Id;" + this.id);
	});
});