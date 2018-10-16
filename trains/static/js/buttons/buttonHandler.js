$(document).ready(function() {
	
		//Radio Button capture for Automation Data
	$(".AutoButton").change(function(){
		console.log("Auto Radio Button clicked detected. Id;" + this.id);
	});
	
	//Radio Button capture for Lighting Data
	$(".LightingButton").change(function(){
		console.log("Light Radio Button clicked detected. Id;" + this.id);
		$.post("/commands/",
		{datatype:"command", command:this.id}
			);
	});
	
	//Pressed Button capture for Lighting Data
	$(".LightingButton").click(function(){
		console.log("Light Button clicked detected. Id;" + this.id);
		$.post("/commands/",
		{datatype:"command", command:this.id}
			);
	});
	
	//Slider Capture for lighting brightness data
	$(".slider").change(function(){
		console.log("Slider value changed Id;" + this.id + ", Value;" + this.value);
		data = this.id + "_" + this.value;
		$.post("/commands/",
			{datatype:"command", command:data}
		);
	});		
	
	//Radio Button capture for Train Data
	$(".TrainButton").change(function(){
		console.log("Train Radio Button change detected. Id;" + this.id);
		$.post("/command_trains/",
			{datatype:"command", command:this.id}
		);
	});
			
	//Pressed Button capture for Train Data
	$(".TrainButton").click(function(){
		console.log("Train Button clicked detected. Id;" + this.id);
		$.post("/command_trains/",
			{datatype:"command", command:this.id}
		);
	});			
});