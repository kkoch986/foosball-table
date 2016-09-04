

var numPixels = 24;

var Simulator = function(callback) {

	var pixelState = {};
	for(var x = 0 ; x < numPixels ; x++) {
		pixelState[x] = false;
	}

	function setPixel(x, val) {
		if(x > numPixels) return;
		pixelState[x] = val;
	}

	function render() {
		var line1 = "";
		var line2 = "";
		for(var x = 0 ; x < numPixels ; x++) {
			if(x < numPixels / 2) {
				line1 += "[" + (pixelState[x] ? pixelState[x] : " ") + "]";
			} else {
				line2 += "[" + (pixelState[x] ? pixelState[x] : " ") + "]";
			}
		}

		console.log(" 0  1  2  3  4  5  6  7  8  9  10 11");
		console.log(line1);
		console.log(line2);
		console.log(" 12 13 14 15 16 17 18 19 20 21 22 23");
		console.log();
	}

	function loop() {
		process.stdout.write('\033c');
		render();
		callback(setPixel);
	}

	setInterval(loop, 15);
};

function map(x, in_min, in_max, out_min, out_max)
{
  return parseInt((parseInt(x) - parseInt(in_min)) * (parseInt(out_max) - parseInt(out_min)) / (parseInt(in_max) - parseInt(in_min)) + parseInt(out_min));
}


var current_frame = 0;
Simulator(function(setPixel) {

	var sweepSpeed = 10;
	var iterations = 1;
	var iterationLength = numPixels / 2;

	var pos = parseInt(current_frame / sweepSpeed);
	var prev_pos = parseInt((current_frame - 1) / sweepSpeed);
	pos = pos % iterationLength;
	console.log(pos);
	if(current_frame <= sweepSpeed * iterations * iterationLength) {
		for(var x = 0 ; x < numPixels ; x++) {
			if(pos <= numPixels / 4) {
				if(x < pos) {
					setPixel(x, "R");
					setPixel(map(
						x, 
						0, 
						(numPixels / 4), 
						(numPixels / 2) - 1, 
						(numPixels / 4)
					), "R");
					setPixel(map(x, 0, (numPixels / 4), (numPixels / 2), (numPixels / 4) * 3), "R");
					setPixel(map(x, 0, (numPixels / 4), numPixels, (numPixels / 4) * 3) - 1, "R");
				}
			} else {
				var a = (numPixels / 4) - (x % (numPixels / 4));
				var b = pos % (numPixels / 4);
				if(x < (numPixels / 4) || (x < 3 * (numPixels / 4) && x > 2 * (numPixels / 4))) {
					if(a <= b) {
						setPixel(x, false);
					}
				} else {
					console.log(x, pos, a, b);
				}
			}
		}
	} else {
		process.exit();
	}
if(pos === 8) process.exit();
	current_frame++;
});
